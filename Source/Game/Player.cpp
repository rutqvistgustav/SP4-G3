#include "stdafx.h"
#include "Player.h"
#include "Collider.h"

#include "UpdateContext.h"
#include "RenderContext.h"
#include "RenderQueue.h"
#include "RenderCommand.h"

#include "PlayerWeaponController.h"
#include "Scene.h"
#include "GlobalServiceProvider.h"
#include "AudioManager.h"
#include "HUD.h"

#include "CheckpointMessage.h"
#include "CheckpointContext.h"

#include "GameMessenger.h"
#include "Scene.h"
#include "GameScene.h" // temp

#include "Camera.h"
#include "MathHelper.h"
#include "Health.h"

#include "CollisionInfo.h"

#include "CheckpointMessage.h"

#include "Shotgun.h"

// Tools
#include "SpriteSheetAnimation.h"
#include "SpriteWrapper.h"
#include <Vector2.hpp>
#include "InputInterface.h"
#include <iostream>
#include <imgui.h>
#include "JsonManager.h"
#include "HealthBar.h"
#include "LevelManagerProxy.h"

// json
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>


#ifdef _DEBUG
#include "CollisionManager.h"
#endif // _DEBUG

Player::Player(Scene* aScene)
	: GameObject(aScene, GameObjectTag::Player),
	myCamera(aScene->GetCamera()),
	myCharacterAnimator(aScene, "Animations/Player.json")
{
	// Init weapon controller
	myWeaponController = std::make_unique<PlayerWeaponController>(GetScene(), this);
}

Player::~Player()
{
	GetGlobalServiceProvider()->GetGameMessenger()->Unsubscribe(GameMessage::CheckpointLoad, this);
	GetGlobalServiceProvider()->GetGameMessenger()->Unsubscribe(GameMessage::CheckpointSave, this);
}

void Player::Init()
{
	GameObject::Init();

	nlohmann::json data = GetScene()->GetGlobalServiceProvider()->GetJsonManager()->GetData("JSON/Player.json");

	InitVariables(data);

	// Init Sprite
	mySprite = std::make_shared<SpriteWrapper>();
	mySprite->SetLayer(GameLayer::Player);

	myWeaponController->Init();

	// Init HUD
	myHUD = std::make_unique<HUD>(GetScene(), myHealth.get(), myWeaponController->GetShotgun());

	myHUD->Init();


	SetPosition(GetPosition());
	myCharacterAnimator.SetState(CharacterAnimator::State::Idle);
	myCharacterAnimator.ApplyToSprite(mySprite);

	CU::Vector2<float> colliderSize = mySprite->GetSize();
	if (myColliderWidth > 0.0f) colliderSize.x = myColliderWidth;
	if (myColliderHeight > 0.0f) colliderSize.y = myColliderHeight;

	myCollider->SetLayer(CollisionLayer::Player);
	myCollider->SetBoxSize(colliderSize);

	myPhysicsController.Init(GetScene(), colliderSize);
	myPhysicsController.SetPosition(GetPosition());
	myPhysicsController.SetGravity({ 0.0f, myGravity });

	// Subscribe to events
	GetGlobalServiceProvider()->GetGameMessenger()->Subscribe(GameMessage::CheckpointSave, this);
	GetGlobalServiceProvider()->GetGameMessenger()->Subscribe(GameMessage::CheckpointLoad, this);
}

void Player::Update(const float aDeltaTime, UpdateContext& anUpdateContext)
{
	GameObject::Update(aDeltaTime, anUpdateContext);

	Move(aDeltaTime, anUpdateContext.myInputInterface);

#ifdef _DEBUG

	if (anUpdateContext.myInputInterface->Is_C_Pressed())
	{
		myScene->GetCollisionManager()->myDoRender = !myScene->GetCollisionManager()->myDoRender;
	}

#endif // _DEBUG

	myPhysicsController.Update(aDeltaTime);
	SetPosition(myPhysicsController.GetPosition());

#ifdef _DEBUG

	// ImGui();

#endif

	const CU::Vector2<float> newCameraPosition = MathHelper::MoveTowards(myCamera->GetPosition(), myPosition, myCameraFollowSpeed * aDeltaTime);
	myCamera->SetPosition(newCameraPosition);

	myHealth->Update(aDeltaTime);
	myHUD->Update(aDeltaTime, myPosition);

	myWeaponController->Update(aDeltaTime, anUpdateContext);

	// NOTE: TODO:
	// Very simple test version for now, when more complex animations are added this might
	// need to be split into a separate state machine.

	if (std::abs(myMovementVelocity.x) >= 1.0f && myPhysicsController.IsGrounded())
	{
		myCharacterAnimator.SetState(CharacterAnimator::State::Run);
	}
	else if (std::abs(myMovementVelocity.x) < 1.0f && myPhysicsController.IsGrounded())
	{
		myCharacterAnimator.SetState(CharacterAnimator::State::Idle);
	}

	if (myCharacterAnimator.GetState() == CharacterAnimator::State::Boost)
	{
		myCharacterAnimator.EnableStateSwitch();
	}

	if (myHealth->IsDead() && myCharacterAnimator.HasEnded())
	{
		GetScene()->GetLevelManagerProxy()->RestartCurrentLevel();
	}
	

	myCharacterAnimator.Update(aDeltaTime);
	myCharacterAnimator.ApplyToSprite(mySprite);
	
}

void Player::Render(RenderQueue* const aRenderQueue, RenderContext& aRenderContext)
{
	aRenderQueue->Queue(RenderCommand(mySprite));

	myHUD->Render(aRenderQueue, aRenderContext);
	myWeaponController->Render(aRenderQueue, aRenderContext);
}

void Player::ApplyForce(const CU::Vector2<float>& aForce)
{
	myPhysicsController.ApplyForce(aForce);
}

void Player::InitVariables(nlohmann::json someData)
{
	myCameraFollowSpeed = someData.value("CameraFollowSpeed", 2000.0f);

	// Movement
	mySpeed = someData.at("MovementSpeed");
	myMaxSpeed = someData.at("MaxSpeedCap");
	myBerserkSpeed = someData.at("BerserkSpeed");
	myReduceMovementSpeed = someData.at("BrakeStrength");
	myStopAtVelocity = someData.at("StopAtVelocity");
	myGravity = someData.at("GravityStrength");

	// Jump
	myJumpCharges = someData.at("JumpCharges");
	myJumpChargeReset = myJumpCharges;
	myJumpStrength = someData.at("JumpStrength");
	myCoyoteTime = someData.at("CoyoteTime");
	myCoyoteTimeReset = myCoyoteTime;

	//Health
	myHealth = std::make_unique<Health>(someData.at("Health"));
	myHealth->SetInvincibilityTimer(someData.at("Invincibility"));

	myColliderWidth = someData.value("ColliderWidth", -1.0f);
	myColliderHeight = someData.value("ColliderHeight", -1.0f);

	// TODO: NOTE: Old name for different purpose, remove when not in use
	mySpriteShift.x = someData.value("ColliderShiftX", mySpriteShift.x);
	mySpriteShift.y = someData.value("ColliderShiftY", mySpriteShift.y);

	mySpriteShift.x = someData.value("SpriteShiftX", mySpriteShift.x);
	mySpriteShift.y = someData.value("SpriteShiftY", mySpriteShift.y);
}

void Player::StopMovement()
{
	myPhysicsController.SetVelocity({});
}

void Player::SetPosition(const CU::Vector2<float> aPosition)
{
	GameObject::SetPosition(aPosition);

	mySprite->SetPosition(aPosition + CU::Vector2<float>(mySpriteShift.x * myCharacterAnimator.GetDirection(), mySpriteShift.y));
	myPhysicsController.SetPosition(aPosition);
}

void Player::SetCanControl(const bool aState)
{
	myCanControl = aState;
}

bool Player::CanControl() const
{
	return myCanControl;
}

void Player::ActivatePowerUp(PowerUpType aPowerUpType)
{
	myWeaponController->ActivatePowerUp(aPowerUpType);
	myHUD->GetHealthBar()->ActivatePowerUp(aPowerUpType);
	if (aPowerUpType == PowerUpType::Berserk)
	{
		mySpeed += myBerserkSpeed;
	}
}

void Player::DisablePowerUp()
{
	if (myHUD->GetHealthBar()->HasActivePowerup())
	{
		const PowerUpType powerUpType = myHUD->GetHealthBar()->GetActivePowerupType();

		myWeaponController->DeactivatePowerUp();
		myHUD->GetHealthBar()->DeactivatePowerUp();

		if (powerUpType == PowerUpType::Berserk)
		{
			mySpeed -= myBerserkSpeed;
		}
	}
}

void Player::TakeDamage(const int aDamage)
{
	if (!myHealth->IsDead())
	{
		if (myHealth->IsPlayerInvincible() == false)
		{
			myHealth->TakeDamage(aDamage);
			//myHUD->GetHealthBar()->RemoveHP(aDamage);

			if (myHealth->IsDead())
			{
				myCharacterAnimator.SetState(CharacterAnimator::State::Death);
				GetScene()->GetGlobalServiceProvider()->GetAudioManager()->PlaySfx("Sound/Player/Player death.wav");
				SetCanControl(false);
				myCharacterAnimator.DisableStateSwitch();
			}
			else
			{
				GetScene()->GetGlobalServiceProvider()->GetAudioManager()->PlaySfx("Sound/Player/Player damage 05.wav");
			}
		}
	}
	
}

void Player::AddHealth(const int aHealthAmount)
{
	myHealth->AddHealth(aHealthAmount);
}

void Player::SetSaveCheckpointPosition(const CU::Vector2<float>& aPosition)
{
	// NOTE: aPositon is middle bottom of checkpoint, we'll offset it so the position will make bottom of our collision box touch this point
	mySaveCheckpointPosition = aPosition + CU::Vector2<float>(0.0f, myCollider->GetBoxSize().y * -0.5f);
}

GameMessageAction Player::OnMessage(const GameMessage aMessage, const CheckpointMessageData* someMessageData)
{
	switch (aMessage)
	{
	case GameMessage::CheckpointSave:
	{
		PlayerCheckpointData* saveData = someMessageData->myCheckpointContext->NewData<PlayerCheckpointData>("Player");

		if (mySaveCheckpointPosition.has_value())
		{
			saveData->myPosition = mySaveCheckpointPosition.value();
			mySaveCheckpointPosition.reset();
		}
		else
		{
			saveData->myPosition = GetPosition();
		}

		// TODO: Save more data as needed
	}

	break;

	case GameMessage::CheckpointLoad:
	{
		PlayerCheckpointData* saveData = someMessageData->myCheckpointContext->GetData<PlayerCheckpointData>("Player");

		// TODO: Reset all variables to a correct state
		DisablePowerUp();

		myMovementVelocity = {};
		myPhysicsController.SetVelocity({});

		// TODO: NOTE: Hack to get full health
		myHealth->SetFullHealth();
		myWeaponController->GetShotgun()->SetFullAmmo();

		SetPosition(saveData->myPosition);
		myCamera->SetPosition(GetPosition());

		myCharacterAnimator.EnableStateSwitch();
		myCharacterAnimator.SetState(CharacterAnimator::State::Idle);
		SetCanControl(true);
	}

	break;

	default:
		assert(false);
		break;
	}

	return GameMessageAction::Keep;
}

void Player::ImGui()
{
	ImGui::Begin("Player movement");

	ImGui::Text("");
	ImGui::SliderFloat("MovementSpeed", &mySpeed, 0, 100000);
	ImGui::SliderFloat("MaxSpeedCap", &myMaxSpeed, 0, 100000);

	ImGui::Text("");

	ImGui::Text("To make BrakeStrength stronger then use more zeroes after decimal. Ex. 0.0000001.");
	ImGui::Text("To make BrakeStrength weaker then use less zeroes after decimal. Ex. 0.0001.");
	ImGui::InputFloat("BrakeStrength", &myReduceMovementSpeed, 0, 1.0);

	ImGui::Text("");

	ImGui::Text("Will stop player entirely when movement slows down and approaches the StopAtVelocity amount.");
	ImGui::DragFloat("StopAtVelocity", &myStopAtVelocity, 0, 10000.0f);

	ImGui::Text("");

	ImGui::SliderFloat("GravityStrength", &myGravity, 0, 100000.0f);

	ImGui::SliderInt("JumpCharges", &myJumpCharges, 0, 100);

	ImGui::SliderFloat("JumpStrength", &myJumpStrength, 0, 100000);

	ImGui::Text("");
	ImGui::Text("Note!");
	ImGui::Text("ImGui will not change values in Player.json.");
	ImGui::Text("You are gonna have to do that yourself :D.");

	ImGui::End();
}

void Player::Move(const float aDeltaTime, InputInterface* anInput)
{
	CU::Vector2<float> physicsVelocity = myPhysicsController.GetVelocity();

	const CU::Vector2<float> direction = GetDirection(anInput);
	myMovementVelocity.x *= std::powf(myReduceMovementSpeed, aDeltaTime);
	myMovementVelocity.x += direction.x * mySpeed * aDeltaTime;
	myMovementVelocity.x = MathHelper::Clamp(myMovementVelocity.x, -myMaxSpeed, myMaxSpeed);

	if (std::fabsf(myMovementVelocity.x) <= myStopAtVelocity)
	{
		myMovementVelocity.x = 0.0f;
	}

	if (myPhysicsController.IsGrounded())
	{
		myJumpCharges = myJumpChargeReset;
		myCoyoteTime = myCoyoteTimeReset;
	}

	if (CanControl() && anInput->IsJumping() && myJumpCharges > 0)
	{
		if (myJumpCharges == 1)
		{
			GetScene()->GetGlobalServiceProvider()->GetAudioManager()->PlaySfx("Sound/Player/Jump 01.mp3");
		}
		physicsVelocity.y = -myJumpStrength;
		--myJumpCharges;
		myCharacterAnimator.SetState(CharacterAnimator::State::Jump);
	}

	if (!myPhysicsController.IsGrounded() && myJumpCharges == myJumpChargeReset)
	{
		myCoyoteTime -= aDeltaTime;
		if (myCoyoteTime <= 0)
		{
			--myJumpCharges;
		}
	}

	if (myMovementVelocity.x > 0.0f)
	{
		myCharacterAnimator.SetDirection(1.0f);
	}
	else if (myMovementVelocity.x < 0.0f)
	{
		myCharacterAnimator.SetDirection(-1.0f);
	}

	myPhysicsController.ApplyFrameImpulse(myMovementVelocity * aDeltaTime);

	// NOTE: Damp horizontal velocity
	physicsVelocity.x *= std::powf(myReduceMovementSpeed, aDeltaTime);
	myPhysicsController.SetVelocity(physicsVelocity);
}

CU::Vector2<float> Player::GetDirection(InputInterface* anInput)
{
	CU::Vector2<float> direction(0.0f, 0.0f);

	if (CanControl())
	{
		if (anInput->IsMovingLeft_Down()) --direction.x;
		if (anInput->IsMovingRight_Down()) ++direction.x;
	}

	return direction;
}
