#include "stdafx.h"
#include "Collectable.h"

#include "GameObjectTag.h"

#include "Player.h"
#include "Scene.h"

#include "GlobalServiceProvider.h"

#include "SpriteSheetAnimation.h"
#include "SpriteWrapper.h"

Collectable::Collectable(Scene* aScene, PowerUpType aPowerupType) :
	TriggerVolume(aScene, GameObjectTag::Player)
	,myPowerupType(aPowerupType)
{}

Collectable::~Collectable() = default;

void Collectable::InitWithJson(const JsonData& /*someProperties*/)
{ 
	Init();

	// TODO: Find sprite from and configure animation from json

	myAnimation = std::make_unique<SpriteSheetAnimation>(myScene->GetGlobalServiceProvider()->GetJsonManager(), "Animation/TestAnimation.json");

	myAnimation->SetState("idle");
	myAnimation->SetIsLooping(true);

	myAnimation->ApplyToSprite(mySprite);
	SetTriggerSize(mySprite->GetSize());
}

void Collectable::Update(const float aDeltaTime, UpdateContext& anUpdateContext)
{
	TriggerVolume::Update(aDeltaTime, anUpdateContext);

	myAnimation->Update(aDeltaTime);
	myAnimation->ApplyToSprite(mySprite);
}

void Collectable::Render(RenderQueue* const aRenderQueue, RenderContext& aRenderContext)
{
	GameObject::Render(aRenderQueue, aRenderContext);
}

const PowerUpType Collectable::GetType()
{
	return myPowerupType;
}

const std::string Collectable::GetDialogID()
{
	return myDialogID;
}

void Collectable::TriggerStay(GameObject* aGameObject)
{
	Player* player = static_cast<Player*>(aGameObject);

	if (CanCollect(player))
	{
		OnCollect(player);

		myIsCollected = true;

		// TODO: Mark GameObject for destruction
		SetDeleteThisFrame();
	}
}

bool Collectable::CanCollect(Player* /*aPlayer*/)
{
	return !myIsCollected;
}
