#pragma once

#include "GameObject.h"
#include "CollisionListener.h"

#include <any>
#include <memory>

class Health;
class HealthBar;
class AmmoCounter;
class Shotgun;

class HUD :
    public GameObject
{
public:
    HUD(Scene* aScene, Health* aHealthInterface, Shotgun* aShotgun);
    ~HUD() = default;

    virtual void Init() override;

    virtual void Update(const float aDeltaTime, CU::Vector2<float> aPlayerPosition);
    virtual void Render(RenderQueue* const aRenderQueue, RenderContext& aRenderContext) override;

    HealthBar* GetHealthBar();
    AmmoCounter* GetAmmoCounter();

private:
    std::unique_ptr<HealthBar> myHealthBar;
    std::unique_ptr<AmmoCounter> myAmmoCounter;
};

