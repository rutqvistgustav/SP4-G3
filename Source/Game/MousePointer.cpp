#include "stdafx.h"
#include "MousePointer.h"
#include "UpdateContext.h"
#include "InputManager.h"
#include "CoordinateHelper.h"
#include "CollisionManager.h"
#include "SpriteWrapper.h"
#include "RenderQueue.h"
#include "RenderCommand.h"
#include "Metrics.h"
#include "CollisionInfo.h"

MousePointer::MousePointer(Scene* aScene)
	: GameObject(aScene) 
{
	myButtonClicked = false;
	SetTag(GameObjectTag::MousePointer);
	myScene = aScene;
	mySprite = std::make_shared<SpriteWrapper>(SpriteWrapper("Sprites/Menue UI/ProgArt/Pointer.png"));
	mySprite->SetPanStrengthFactor(0);

	myCollider = std::make_shared<Collider>();
	myCollider->Init(myMousePointerPos, mySprite->GetSize());
	myScene->GetCollisionManager()->AddCollider(myCollider);
}

MousePointer::~MousePointer() = default;

void MousePointer::Update(float aDeltaTime, UpdateContext& anUpdateContext)
{	
	ReadingMouseCoordinates(aDeltaTime, anUpdateContext.myInput);
	ReadingLMBInput(anUpdateContext.myInputInterface);

	myCollider->SetPosition(myMousePointerPos);
	mySprite->SetPosition(myMousePointerPos);
}

void MousePointer::Render(RenderQueue* const aRenderQueue, RenderContext& aRenderContext)
{
	myCollider->RenderDebug(aRenderQueue, aRenderContext);
}

void MousePointer::OnEnter(const CollisionInfo& someCollisionInfo)
{
	if (GetLMBDown())
	{
		myButtonClicked = true;
		myClickedButton = someCollisionInfo.myOtherCollider->GetGameObject()->GetTag();
	}
}

bool MousePointer::GetLMBDown()
{
	return myClicked;
}

bool MousePointer::GetButtonClicked()
{
	return myButtonClicked;
}

void MousePointer::SetButtonClicked(bool aBool)
{
	myButtonClicked = aBool;
}

CU::Vector2<float> MousePointer::GetPointerPos()
{
	return myMousePointerPos;
}

GameObjectTag MousePointer::ClickedButton() const
{
	return myClickedButton;
}

void MousePointer::ReadingMouseCoordinates(float aDeltaTime, CommonUtilities::Input* aInput)
{
	auto mousePos = aInput->GetMousePosition();
	auto refSize = Metrics::GetReferenceSize();
	float mousX = static_cast<float>(mousePos.myMouseX);
	float mousY = static_cast<float>(mousePos.myMouseY);

	if (myLastPosCalculate == false)
	{
		myLastPosCalculate = true;
		myLastPos.x = mousX;
		myLastPos.y = mousY;
	}
	else
	{
		myTimer += aDeltaTime;
		if (myTimer >= 0.02f)
		{
			myTimer = {};
			myLastPosCalculate = false;			
			myDragPos = { myLastPos.x - mousX, myLastPos.y - mousY };
		}
	}
	//CoordinateHelper::GetClientPositionAsVirtual()
	myMousePointerPos = (CU::Vector2(mousX, mousY));
}

void MousePointer::ReadingLMBInput(InputInterface* aInputInterface)
{
	if (aInputInterface->IsShooting())
	{
		myClicked = true;
	}
	else
	{
		myClicked = false;
	}
}