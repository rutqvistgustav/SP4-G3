#include "stdafx.h"
#include "WeaponFactory.h"

#include "JsonManager.h"

#include "Weapon.h"

#include "Shotgun.h"

#include <cassert>

WeaponFactory::WeaponFactory(const JsonManager* aJsonManager)
	: myJsonManager(aJsonManager)
{
	RegisterBehaviour("shotgun", WeaponFactory::ConstructBehaviour<Shotgun>);

	const JsonData& weaponData = myJsonManager->GetData("JSON/Weapons.json");
	for (const auto& weapon : weaponData.items())
	{
		const std::string key = weapon.key();
		const std::string type = weapon.value().value("type", "shotgun");

		RegisterType(key, type, key);
	}
}

WeaponFactory::~WeaponFactory() = default;

std::shared_ptr<Weapon> WeaponFactory::CreateWeapon(const std::string& aKey, Scene* aScene, IWeaponHolder* aWeaponHolder) const
{
	return CreateType(aKey, aScene, aWeaponHolder);
}

std::shared_ptr<Weapon> WeaponFactory::ConstructType(std::shared_ptr<Weapon> aBehaviour, const std::string& someTypeData) const
{
	aBehaviour->Init(myJsonManager->GetData("JSON/Weapons.json")[someTypeData]);
	return aBehaviour;
}
