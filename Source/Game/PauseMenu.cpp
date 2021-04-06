#include "stdafx.h"
#include "PauseMenu.h"
#include "MenuButton.h"
#include "SpriteWrapper.h"
#include "SceneManagerProxy.h"
#include "UpdateContext.h"

#include "MainMenu.h"

#include "RenderQueue.h"
#include "RenderCommand.h"
#include "MousePointer.h"
#include "Metrics.h"
#include "CollisionManager.h"

PauseMenu::PauseMenu() = default;
PauseMenu::~PauseMenu() = default;

void PauseMenu::Init()
{
	myBackGround = std::make_shared<SpriteWrapper>("Sprites/Menue UI/menu background.dds");
	myBackGround->SetPosition(Metrics::GetReferenceSize() * 0.5f);
	myBackGround->SetLayer(-2);

	InitButtons();

	myMousePointer->SetClickCallback(std::bind(&PauseMenu::MouseClicked, this, std::placeholders::_1)); // TODO What does this do?
}

void PauseMenu::Update(const float aDeltaTime, UpdateContext& anUpdateContext)
{
	if (anUpdateContext.myInputInterface->IsPressingPause())
	{
		if (!myPauseIsActive)
		{
			myPauseIsActive = true;
		}
		else
		{
			myPauseIsActive = false;
		}
	}
}

void PauseMenu::Render(RenderQueue* const aRenderQueue, RenderContext& aRenderContext)
{
	if (myPauseIsActive == true)
	{
		MenuScene::Render(aRenderQueue, aRenderContext);

		aRenderQueue->Queue(RenderCommand(myBackGround));
	}
}

const bool PauseMenu::IsGamePaused()
{
	return myPauseIsActive;
}

void PauseMenu::InitButtons()
{
	float width = Metrics::GetReferenceSize().x;
	float height = Metrics::GetReferenceSize().y;
	
	auto startButton = std::make_shared<MenuButton>(this, "Sprites/Menue UI/start.dds", "Sprites/Menue UI/start_hover.dds",
		GameObjectTag::StartButton);
	startButton->SetPosition(CU::Vector2(width * 0.5f, height * 0.43f));
	AddGameObject(startButton);

	auto restartButton = std::make_shared<MenuButton>(this, "Sprites/Menue UI/start.dds", "Sprites/Menue UI/start_hover.dds",
		GameObjectTag::RestartButton);
	restartButton->SetPosition(CU::Vector2(width * 0.5f, height * 0.53f));
	AddGameObject(restartButton);
	
	auto quitButton = std::make_shared<MenuButton>(this, "Sprites/Menue UI/quit.dds", "Sprites/Menue UI/quit_hover.dds",
		GameObjectTag::QuitButton);
	quitButton->SetPosition(CU::Vector2(width * 0.5f, height * 0.63f));
	AddGameObject(quitButton);
}

void PauseMenu::MouseClicked(GameObject* aTarget)
{
	if (aTarget == nullptr) return;

	const GameObjectTag targetTag = aTarget->GetTag();

	switch (targetTag)
	{
	case GameObjectTag::StartButton:
		myPauseIsActive = false;
		break;

	case GameObjectTag::RestartButton:
		myPauseIsActive = false;
		//GetSceneManagerProxy()->Transition(Latest checkpoint);
		break;

	case GameObjectTag::QuitButton:
		GetSceneManagerProxy()->Transition(std::make_unique<MainMenu>());
		break;
	}
	
}
