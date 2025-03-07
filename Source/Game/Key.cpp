#include "stdafx.h"
#include "Key.h"
#include "GlobalServiceProvider.h"
#include "JsonManager.h"
#include "LevelManagerProxy.h"
#include "Player.h"

#include "DialogueBox.h"
#include "SpriteSheetAnimation.h"
#include "UpdateContext.h"

#include "SpriteWrapper.h"


Key::Key(Scene* aScene, PowerUpType aPowerupType, std::string aDialogID)
	: Collectable(aScene, aPowerupType)
{
	myDialogID = aDialogID;
	InitWithJson(GetScene()->GetGlobalServiceProvider()->GetJsonManager()->GetData("JSON/Entities.json").at("Key"));
}

void Key::InitWithJson(const JsonData& someProperties)
{
	Init();

	myDialogBox = std::make_unique<DialogueBox>(GetScene(), true);
	myDialogBox->Init(myDialogID);

	mySprite = std::make_shared<SpriteWrapper>();
	mySprite->SetLayer(GameLayer::HUD);

	myAnimation = std::make_unique<SpriteSheetAnimation>(myScene->GetGlobalServiceProvider()->GetJsonManager(), "Animations/BouncingArrow.json");

	myAnimation->SetState("idle");
	myAnimation->SetIsLooping(true);

	myAnimation->ApplyToSprite(mySprite);
	SetTriggerSize(mySprite->GetSize());
}

void Key::OnCollect(Player* aPlayer)
{
	myDialogBox->OnInteract(aPlayer);
}

void Key::Update(const float aDeltaTime, UpdateContext& anUpdateContext)
{
	TriggerVolume::Update(aDeltaTime, anUpdateContext);

	myDialogBox->Update(aDeltaTime, anUpdateContext);

	if (anUpdateContext.myInputInterface->IsPressingUse())
	{
		myIsPressingUse = true;
	}

	myAnimation->Update(aDeltaTime);
	myAnimation->ApplyToSprite(mySprite);

	if (myDialogBox->GetDeleteThisFrame() == true) 
	{
		GetScene()->GetLevelManagerProxy()->TransitionNextLevel();
	}
}

void Key::Render(RenderQueue* const aRenderQueue, RenderContext& aRenderContext)
{
	GameObject::Render(aRenderQueue, aRenderContext);
	myDialogBox->Render(aRenderQueue, aRenderContext);
}

void Key::TriggerStay(GameObject* aGameObject)
{
	Player* player = static_cast<Player*>(aGameObject);

	if (CanCollect(player) && myIsPressingUse == true)
	{
		OnCollect(player);
		myIsPressingUse = false;

		//myIsCollected = true;

		// TODO: Mark GameObject for destruction
		//SetDeleteThisFrame();
	}
}
