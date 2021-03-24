#pragma once
#include "Scene.h"

class MenuButton;
class AudioManager;

class Options :
    public Scene
{
public:
	Options();
	virtual ~Options() override;

	virtual void Init() override;

	virtual void Update(const float aDeltaTime, UpdateContext& anUpdateContext) override;
	virtual void Render(RenderQueue* const aRenderQueue, RenderContext& aRenderContext) override;

private:
	void InitButtons();
	void MuteSound();

	bool myIsMuted;

	std::unique_ptr<MenuButton> myMuteSound;
	std::unique_ptr<MenuButton> myBackButton;
	std::unique_ptr<AudioManager> myAudioManager;

	std::vector<std::unique_ptr<MenuButton>> myButtons;
};

