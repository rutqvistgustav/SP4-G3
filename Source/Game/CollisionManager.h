#pragma once
#include <map>
#include <memory>

#include "Collider.h"

namespace Tga2D
{
	class CColor;
}

class TiledCollision;
class RenderQueue;
class RenderContext;

class CollisionManager
{
public:
	/*static std::shared_ptr<CollisionManager> GetInstance()
	{
		static std::shared_ptr<CollisionManager> ourCollisionManager(new CollisionManager);
		return ourCollisionManager;
	}*/
	CollisionManager(TiledCollision* aTiledCollision);
	CollisionManager(CollisionManager const&) = delete;
	void operator=(CollisionManager const&) = delete;
	~CollisionManager();

	void Update();

	void AddCollider(std::shared_ptr<Collider> aCollider);
	void RemoveCollider(std::shared_ptr<Collider> aCollider);
	

#ifdef _DEBUG
	void InitDebug();
	void RenderDebug(RenderQueue* const aRenderQueue, RenderContext& aRenderContext);
	bool myDoRender = false;

#endif // _DEBUG
private:

	void CheckTileCollision(const int& anIndex, const CU::Vector2<float> anOffset);

private:
	std::vector<std::shared_ptr<Collider>> myColliders;
	std::map<int, int> myCollisionIndexes;//TODO:change to support collision with multiple GameObjects
	std::map<int, CU::Vector2<float>> myTileCollisionIndexes;//TODO:change to support collision with multiple GameObjects
	TiledCollision* myTiledCollision;
};


//TODO:make colliders with fall-through