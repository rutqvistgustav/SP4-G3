#include "stdafx.h"
#include "MainMenu.h"

#include "Metrics.h"
#include "SpriteWrapper.h"

#include "MousePointer.h"

#include "LevelManagerProxy.h"
#include "SceneManagerProxy.h"

#include "GameScene.h"
#include "LevelSelect.h"
#include "IntroScene.h"

#include "MenuButton.h"
#include "Settings.h"

#include "RenderQueue.h"
#include "RenderCommand.h"
#include "Credits.h"

#include "GlobalServiceProvider.h"
#include "AudioManager.h"

MainMenu::MainMenu() = default;
MainMenu::~MainMenu() = default;

void MainMenu::Init()
{
	MenuScene::Init();

	myBackground = std::make_shared<SpriteWrapper>("Sprites/Menue UI/menu background.dds");
	myBackground->SetPosition(Metrics::GetReferenceSize() * 0.5f);
	myBackground->SetLayer(-1);

	InitButtons();

	myMousePointer->SetClickCallback(std::bind(&MainMenu::MouseClicked, this, std::placeholders::_1));
}

void MainMenu::Render(RenderQueue* const aRenderQueue, RenderContext& aRenderContext)
{
	MenuScene::Render(aRenderQueue, aRenderContext);

	aRenderQueue->Queue(RenderCommand(myBackground));
}

void MainMenu::InitButtons()
{
	const float width = Metrics::GetReferenceSize().x;
	const float height = Metrics::GetReferenceSize().y;

	auto startButton = std::make_shared<MenuButton>(this, "Sprites/Menue UI/start.dds", "Sprites/Menue UI/start_hover.dds", 
		GameObjectTag::StartButton);
	startButton->SetPosition(CommonUtilities::Vector2(width * 0.5f, height * 0.33f));
	AddInterfaceElement(startButton);

	auto levelSelectButton = std::make_shared<MenuButton>(this, "Sprites/Menue UI/levels.dds", "Sprites/Menue UI/levels_hover.dds",
		GameObjectTag::LevelSelectButton);
	levelSelectButton->SetPosition(CommonUtilities::Vector2(width * 0.5f, height * 0.43f));
	AddInterfaceElement(levelSelectButton);

	auto settingsButton = std::make_shared<MenuButton>(this, "Sprites/Menue UI/settings.dds", "Sprites/Menue UI/settings_hover.dds",
		GameObjectTag::SettingsButton);
	settingsButton->SetPosition(CommonUtilities::Vector2(width * 0.5f, height * 0.53f));
	AddInterfaceElement(settingsButton);

	auto creditsButton = std::make_shared<MenuButton>(this, "Sprites/Menue UI/credits.dds", "Sprites/Menue UI/credits_hover.dds",
		GameObjectTag::CreditsButton);
	creditsButton->SetPosition(CommonUtilities::Vector2(width * 0.5f, height * 0.63f));
	AddInterfaceElement(creditsButton);

	auto quitButton = std::make_shared<MenuButton>(this, "Sprites/Menue UI/quit.dds", "Sprites/Menue UI/quit_hover.dds",
		GameObjectTag::QuitButton);
	quitButton->SetPosition(CommonUtilities::Vector2(width * 0.5f, height * 0.73f));
	AddInterfaceElement(quitButton);
}

void MainMenu::MouseClicked(GameObject* aTarget)
{
	if (aTarget == nullptr)
	{
		return;
	}

	const GameObjectTag targetTag = aTarget->GetTag();

	GetGlobalServiceProvider()->GetAudioManager()->PlaySfx("Sound/Misc/Menu_Button.wav");

	switch (targetTag)
	{
	case GameObjectTag::StartButton:
		GetSceneManagerProxy()->Transition(std::make_unique<IntroScene>(), true);
		break;
	case GameObjectTag::LevelSelectButton:
		GetSceneManagerProxy()->Transition(std::make_unique<LevelSelect>(), false);
		break;

	case GameObjectTag::SettingsButton:
		GetSceneManagerProxy()->Transition(std::make_unique<Settings>(), false);
		break;

	case GameObjectTag::CreditsButton:
		GetSceneManagerProxy()->Transition(std::make_unique<Credits>(), false);
		break;

	case GameObjectTag::QuitButton:
		PostQuitMessage(0);
		break;
	}
}
