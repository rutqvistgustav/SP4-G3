#include "stdafx.h"
#include "PlayerWeaponController.h"

#include "UpdateContext.h"
#include "InputManager.h"
#include "InputInterface.h"

#include "MathHelper.h"
#include "Metrics.h"

#include "GlobalServiceProvider.h"
#include "Player.h"
#include "Scene.h"

#include "Grapple.h"
#include "GrappleHookProjectile.h"
#include "Weapon.h"
#include "WeaponFactory.h"

PlayerWeaponController::PlayerWeaponController(Scene* aScene, Player* aPlayer) :
	myScene(aScene),
	myPlayer(aPlayer)
{
}

PlayerWeaponController::~PlayerWeaponController() = default;

void PlayerWeaponController::Init()
{
	// NOTE: For now it seems we are only going to ever have 2 weapons so no need to get fancy
	myGrapple = std::static_pointer_cast<Grapple> (myScene->GetGlobalServiceProvider()->GetWeaponFactory()->CreateWeapon("grapple", myScene, this));
	myShotgun = myScene->GetGlobalServiceProvider()->GetWeaponFactory()->CreateWeapon("shotgun", myScene, this);
}

void PlayerWeaponController::Update(const float aDeltaTime, UpdateContext & anUpdateContext)
{
	const CU::Vector2<float> aimDirection = ComputeAimDirection(anUpdateContext);

	myGrapple->SetPosition(myPlayer->GetPosition());
	myShotgun->SetPosition(myPlayer->GetPosition());

	myGrapple->SetDirection(aimDirection);
	myShotgun->SetDirection(aimDirection);

	myGrapple->Update(aDeltaTime, anUpdateContext);
	myShotgun->Update(aDeltaTime, anUpdateContext);

	if (anUpdateContext.myInputInterface->IsGrappling())
	{
		myGrapple->Shoot();
	}
	else if (anUpdateContext.myInputInterface->IsShooting())
	{
		myShotgun->Shoot();
	}
}

void PlayerWeaponController::Render(RenderQueue* const aRenderQueue, RenderContext& aRenderContext)
{
	myGrapple->Render(aRenderQueue, aRenderContext);
	myShotgun->Render(aRenderQueue, aRenderContext);
}

CU::Vector2<float> PlayerWeaponController::ComputeAimDirection(UpdateContext& anUpdateContext)
{
	// TODO: Figure out direction of weapons and update
	// NOTE: Will have to be changed, this is just to test with a mouse

	const CU::Vector2<float> mousePosition = {
		anUpdateContext.myInput->GetMousePosition().myMouseX * Metrics::GetReferenceSize().x / Metrics::GetRenderSize().x,
		anUpdateContext.myInput->GetMousePosition().myMouseY * Metrics::GetReferenceSize().y / Metrics::GetRenderSize().y
	};

	CU::Vector2<float> direction;

	if (anUpdateContext.myInputInterface->IsUsingController())
	{
		direction = { anUpdateContext.myInputInterface->GetRightStickX(), -anUpdateContext.myInputInterface->GetRightStickY() };
	}
	else
	{
		direction = mousePosition - myPlayer->GetPosition();
	}

	const float radians = std::atan2f(direction.y, direction.x);
	const float step = 2.0f * MathHelper::locPif / 8.0f;

	const float lockedRadians = std::roundf(radians / step) * step;

	return CU::Vector2<float>(std::cosf(lockedRadians), std::sinf(lockedRadians));
}

void PlayerWeaponController::ApplyRecoilKnockback(Weapon* aWeapon, float someStrength)
{
	myPlayer->ApplyForce(aWeapon->GetDirection() * someStrength * -1.0f);
}

void PlayerWeaponController::OnGrappleHit(const CU::Vector2<float>& aTargetPosition, const CU::Vector2<float>& aGrapplingDirection)
{
	myPlayer->StartGrappling(aTargetPosition, aGrapplingDirection);
}

void PlayerWeaponController::StopGrappling()
{
	myGrapple->GetProjectile()->ResetProjectile();
}
