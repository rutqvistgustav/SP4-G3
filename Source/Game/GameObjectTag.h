#pragma once

enum class GameObjectTag
{
	Default,

	Player,
	Enemy,

	DamageVolume,

	Trigger,
	PowerUp,

	MousePointer,

	//Menu buttons
	StartButton,
	RestartButton,
	SettingsButton,
	BackButton,
	TutorialButton,
	CreditsButton,
	LevelSelectButton,
	MuteButton,
	Level1Button,
	Level2Button,
	Level3Button,
	ArrowLeftButton,
	ArrowRightButton,
	QuitButton,

	//Sliders
	MasterSlider,
	SfxSlider,
	MusicSlider,

	// Level tags
	Level_1,
	Level_2,
	Level_3,
	Level_4
};
