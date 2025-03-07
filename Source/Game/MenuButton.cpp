#include "stdafx.h"
#include "MenuButton.h"

#include "SpriteWrapper.h"

#include "RenderQueue.h"
#include "RenderCommand.h"

#include "Collider.h"
#include "CollisionManager.h"
#include "CollisionInfo.h"

MenuButton::MenuButton(Scene* aScene, const char* aSpritePath, const char* aSpritePath2, GameObjectTag aTag)
	: GameObject(aScene, aSpritePath)
{
	SetTag(aTag);
	myHover = false;

	mySprite->SetSamplerFilter(RenderSamplerFilter::Bilinear);

	myHoverSprite = std::make_shared<SpriteWrapper>(aSpritePath2);
	myHoverSprite->SetSamplerFilter(RenderSamplerFilter::Bilinear);
	
	myCollider = std::make_shared<Collider>();
	//myCollider->Init(this, myPosition, 40.f);
	myCollider->SetBoxSize(CU::Vector2(mySprite->GetSize().x, mySprite->GetSize().y * 0.18f));
	myCollider->Init(myPosition, CU::Vector2(mySprite->GetSize().x * 0.9f, mySprite->GetSize().y * 0.17f));
	myCollider->SetCollisionListener(this);
	myCollider->SetGameObject(this);
	
	myScene->GetCollisionManager()->AddCollider(myCollider);
}

MenuButton::~MenuButton() = default;

void MenuButton::Init()
{
}

void MenuButton::Update()
{
	myCollider->SetPosition(myPosition);
	myPositionLastFrame = myPosition;
}

void MenuButton::Render(RenderQueue* const aRenderQueue, RenderContext& /*aRenderContext*/)
{
	std::shared_ptr<SpriteWrapper>& spriteToRender = myHover ? myHoverSprite : mySprite;

	aRenderQueue->Queue(RenderCommand(spriteToRender));
}

void MenuButton::SetPosition(const CU::Vector2<float> aPosition)
{
	myPosition = aPosition;
	mySprite->SetPosition(myPosition);
	myHoverSprite->SetPosition(myPosition);

	if (myCollider.get() != nullptr)
	{
		myCollider->SetPosition(myPosition);
	}
}

void MenuButton::SetLayer(GameLayer::Layer aLayer)
{
	mySprite->SetLayer(aLayer);
	myHoverSprite->SetLayer(aLayer);
}

void MenuButton::SetPanStrengthFactor(float aPanStrengthFactor)
{
	mySprite->SetPanStrengthFactor(aPanStrengthFactor);
	myHoverSprite->SetPanStrengthFactor(aPanStrengthFactor);
}

void MenuButton::SetColliderSize(const CU::Vector2<float> aSize)
{
	float x = mySprite->GetSize().x;
	float y = mySprite->GetSize().y;
	myCollider->SetBoxSize(CU::Vector2(x * aSize.x, y * aSize.y));
}

void MenuButton::OnEnter(const CollisionInfo& /*someCollisionInfo*/)
{
	myHover = true;
}

void MenuButton::OnExit(const CollisionInfo& /*someCollisionInfo*/)
{
	myHover = false;
}
