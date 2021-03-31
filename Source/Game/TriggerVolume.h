#pragma once

#include "GameObject.h"

#include <optional>

class TriggerVolume : public GameObject
{
public:

	TriggerVolume(Scene* aScene, std::optional<GameObjectTag> aFilter = std::nullopt);

	virtual void OnCollision(GameObject* aGameObject) override;

	void SetTriggerRadius(const float aRadius);
	const float GetTriggerRadius() const;

protected:

	virtual void OnEnter(GameObject* aGameObject);
	virtual void OnStay(GameObject* aGameObject);
	virtual void OnExit(GameObject* aGameObject);

private:

	std::optional<GameObjectTag> myFilter;

};
