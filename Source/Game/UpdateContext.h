#pragma once
namespace CommonUtilities
{
	class Input;
}
class InputInterface;

struct UpdateContext
{
	CommonUtilities::Input* myInput;
	InputInterface* myInputInterface;
};
