#include "stdafx.h"
#include "GameWorld.h"
#include "Player.h"
#include "Metrics.h"
#include "RenderCommand.h"
#include "RenderQueue.h"
#include "SpriteWrapper.h"
#include <tga2d/sprite/sprite.h>
#include <InputManager.h>
#include <memory>

CGameWorld::CGameWorld()
	: myPlayer(new Player())
{ }

CGameWorld::~CGameWorld() = default;

void CGameWorld::Init()  
{


	//myTga2dLogoSprite = new Tga2D::CSprite("sprites/tga_logo.dds");
	myTga2dLogoSprite = std::make_shared<SpriteWrapper>("Sprites/tga_logo.dds");
	myTga2dLogoSprite->SetPosition(Metrics::GetReferenceSize() * 0.5f);

	myTga2dLogoSprite->SetPivot({ 0.5f, 0.5f });
	myTga2dLogoSprite->SetPosition({ 0.5f, 0.5f });


	/*CollisionManager::GetInstance()->AddCollider(std::make_shared<Collider>(Collider(0.1f, 0.1f, 0.1f)));
	CollisionManager::GetInstance()->AddCollider(std::make_shared<Collider>(Collider(0.9f, 0.9f, 0.1f)));*/

	myGameObjects.push_back(GameObject(0.2f, 1.0f));
	myGameObjects.push_back(GameObject(0.8f, 1.0f));



	myPlayer->Init();

	for (size_t i = 0; i < myGameObjects.size(); ++i)
	{
		myGameObjects[i].Init();
	}
#ifdef _DEBUG
	CollisionManager::GetInstance()->InitDebug();
#endif // _DEBUG
}

void CGameWorld::Update(float aDeltaTime, CU::Input* anInput)
{ 	
	CollisionManager::GetInstance()->Update();
	myPlayer->Update(aDeltaTime, anInput);
}

void CGameWorld::Render(RenderQueue* aRenderQueue)
{
	//myTga2dLogoSprite->Render();
	aRenderQueue->Queue(RenderCommand(myTga2dLogoSprite));
#ifdef _DEBUG
	CollisionManager::GetInstance()->RenderDebug();
#endif // _DEBUG


}
