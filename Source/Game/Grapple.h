#pragma once
#include <memory>
#include "Weapon.h"

class SpriteWrapper;
class GrappleHookProjectile;
class Scene;

class Grapple : public Weapon
{
public:

	Grapple(IWeaponHolder* aWeaponHolder);
	virtual ~Grapple() override;

	void InitGameObjects(Scene* aScene) override;
	virtual void Update(float aDeltaTime, UpdateContext& anUpdateContext, const CU::Vector2<float>& aPlayerPosition) override;
	virtual void Render(RenderQueue* const aRenderQueue, RenderContext& aRenderContext) override;

	virtual void Shoot(const CU::Vector2<float> aPlayerPosition) override;
	const bool& IsLoaded() const;
	void Reload(const float aDeltaTime);

	GrappleHookProjectile* GetProjectile();

protected:

	virtual void LoadJson(const JsonData& someJsonData) override;
	virtual void Setup() override;

private:
	bool myIsLoaded = true;

	float myMaxDistance{};
	float myHookSpeed{};
	float myContractSpeed{};
	float myCoolDown{};
	float myCoolDownReset{};

	std::unique_ptr<GrappleHookProjectile> myProjectile;

	//float myMaxDistance{};
	//float myHookSpeed{};
	//float myContractSpeed{};
};
