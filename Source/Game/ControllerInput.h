#pragma once
#include <Xinput.h>
#include <vector>
class ControllerInput
{
public:
	ControllerInput();
	ControllerInput(float aDeadZoneX, float aDeadZoneY);
	~ControllerInput() {};

	void UpdateControllerState(float aDeltaTime);

	void UpdateNormalizedStickValues();

	float GetLeftStickX();
	float GetLeftStickY();
	float GetRightStickX();
	float GetRightStickY();
	float GetLeftTrigger();
	float GetRightTrigger();


	bool IsPressed(WORD button);
	bool isReleased(WORD button);
	bool LeftStickReleased();
	bool LeftTriggerPressed();
	bool RightTriggerPressed();

	bool ControllerConnected();

private:
	float myElapsedTime = 3.0f;
	float myCheckCooldown = 5.0f;

#pragma region ControllerStickVariables
	float myXDeadZone;
	float myYDeadZone;
	float myLeftStickX = 0.0f;
	float myPreviousLeftStickX = 0.0f;
	float myLeftStickY = 0.0f;
	float myRightStickX = 0.0f;
	float myRightStickY = 0.0f;
#pragma endregion

	float myPreviousLeftTrigger = 0.0f;
	float myPreviousRightTrigger = 0.0f;
	float myLeftTrigger = 0.0f;
	float myRightTrigger = 0.0f;


	bool myHasController = false;
	int myControllerId;

	XINPUT_STATE myState;
	XINPUT_STATE myPreviousState;
};

