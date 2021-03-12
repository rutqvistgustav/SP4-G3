#include "stdafx.h"
#include "Player.h"
#include "Collider.h"

#include "UpdateContext.h"
#include "RenderContext.h"
#include "RenderQueue.h"
#include "RenderCommand.h"

#include "PlayerWeaponController.h"
#include "Scene.h"

// Tools
#include "SpriteWrapper.h"
#include <Vector2.hpp>
//#include <InputManager.h>
#include "InputInterface.h"
#include <iostream>

// json
#include <json.hpp>
#include <fstream>
#include <string>

Player::Player(Scene* aScene)
	: GameObject(aScene)
{
	// json
	nlohmann::json data;
	std::ifstream file("JSON/Player.json");
	data = nlohmann::json::parse(file);
	file.close();


	mySpeed = data.at("MovementSpeed");

	myOnGround = false;

	myPosition.x = 0.5f;
	myPosition.y = 0.5f;

	myIsPlayer = true;//temporary

	InitVariables(data);

	// Init Sprite
	mySprite = std::make_shared<SpriteWrapper>("Sprites/Grump.dds");
	CU::Vector2<float> startPosition(950.0f, 540.0f);
	mySprite->SetPosition(startPosition);

	// Init weapon controller
	myWeaponController = std::make_unique<PlayerWeaponController>(GetScene()->GetWeaponFactory(), this);

}

Player::~Player() = default;

void Player::Update(const float aDeltaTime, UpdateContext& anUpdateContext)
{
	GameObject::Update(aDeltaTime, anUpdateContext);
	Controller(aDeltaTime, anUpdateContext.myInputInterface);

	myWeaponController->Update(aDeltaTime, anUpdateContext);
}

void Player::Render(RenderQueue* const aRenderQueue, RenderContext& aRenderContext)
{
	aRenderQueue->Queue(RenderCommand(mySprite));

	myWeaponController->Render(aRenderQueue, aRenderContext);
}

void Player::Controller(const float aDeltaTime, InputInterface* anInput)
{
	Movement(aDeltaTime, anInput);
}

void Player::BrakeMovement(const float aDeltaTime)
{


	//MouseInput(anInput);
	//myPositionLastFrame = myPosition;
	if (myIsMovingLeft == false && myIsMovingRight == false)
	{
		if (myVel.x > myStopAtVelocity || myVel.x < -myStopAtVelocity)
		{
			myVel.x *= pow(myReduceMovementSpeed, aDeltaTime);
		}
		else
		{
			myVel.x = 0;
		}
	}
}

void Player::ApplyForce(const CU::Vector2<float>& aForce)
{
	myVel += aForce;
}

void Player::PlayerInput(InputInterface* anInput)
{
	if (anInput->IsMovingLeft_Pressed())
	{
		myIsMovingLeft = true;
		//std::cout << "Left Pressed" << std::endl;
	}
	if (anInput->IsMovingRight_Pressed())
	{
		myIsMovingRight = true;
		//std::cout << "Right Pressed" << std::endl;
	}
	if (anInput->IsMovingLeft_Released())
	{
		myIsMovingLeft = false;
		//std::cout << "Left Released" << std::endl;
	}
	if (anInput->IsMovingRight_Released())
	{
		myIsMovingRight = false;
		//std::cout << "Right Released" << std::endl;
	}

	if (anInput->IsJumping())
	{
		myIsJumping = true;
		myHasRemovedNegativeVel = false;
	}

	if (anInput->Is_G_Pressed())
	{
		if (myGravityActive == false)
		{
			myGravityActive = true;
		}
		else
		{
			myGravityActive = false;
		}
	}
}

void Player::InitVariables(nlohmann::json someData)
{
	// Movement
	mySpeed = someData.at("MovementSpeed");
	myMaxSpeed = someData.at("MaxSpeedCap");
	myReduceMovementSpeed = someData.at("BrakeStrength");
	myStopAtVelocity = someData.at("StopAtVelocity");
	myGravity = someData.at("GravityStrength");

	// Jump
	myJumpCharges = someData.at("JumpCharges");
	myJumpChargeReset = myJumpCharges;
	myJumpStrength = someData.at("JumpStrength");
	myJumpDuration = someData.at("JumpDuration");
	myJumpDurationReset = myJumpDuration;
}

//void Player::OnCollision(const GameObject* aGameObject)
//{
//	myOnGround = true;
//}


void Player::StopMovement()
{
	myVel = CU::Vector2<float>();
}

void Player::Movement(const float aDeltaTime, InputInterface* anInput)
{
	PlayerInput(anInput);
	CU::Vector2<float> movement = GetDirection(anInput);

	if (myIsMovingLeft == true || myIsMovingRight == true)
	{
		if (myVel.x <= myMaxSpeed && -myMaxSpeed <= myVel.x)
		{


			myVel += movement * mySpeed * aDeltaTime;
		}
	}
	BrakeMovement(aDeltaTime);

	Jump(aDeltaTime);

	myPosition += myVel * aDeltaTime;
	mySprite->SetPosition(myPosition);

	//std::cout << "x " << myPosition.x << " y " << myPosition.y << std::endl;
	//std::cout << "Velocity " << myVel.x << std::endl;

}

void Player::Jump(const float aDeltaTime)
{
	if (myIsJumping == true /*&& myJumpCharges > 0*/)
	{
		myJumpDuration -= aDeltaTime;
		if (myJumpDuration > 0)
		{
			if (myHasRemovedNegativeVel == false)
			{
				if (myVel.y >= 0)
				{
					myVel.y = 0;
				}
				myHasRemovedNegativeVel = true;
			}

			myVel.y -= myJumpStrength * aDeltaTime;
		}
		else
		{
			myIsJumping = false;
			myJumpDuration = myJumpDurationReset;
			//--myJumpCharges;
		}
		// if landed set myVel.y = 0
		// Reset myJumpCharges
	}
	else
	{
		if (myGravityActive == true)
		{
			myVel.y += myGravity * aDeltaTime;
		}
	}
}

CU::Vector2<float> Player::GetDirection(InputInterface* anInput)
{
	CU::Vector2<float> direction(0.0f, 0.0f);
	if (anInput->IsMovingLeft_Down() && myIsMovingRight == false)
	{
		--direction.x;
		//std::cout << "Left" << std::endl;
	}
	if (anInput->IsMovingRight_Down() && myIsMovingLeft == false)
	{
		++direction.x;
		//std::cout << "Right direction" << direction.x << std::endl;
	}

	return direction;
}
