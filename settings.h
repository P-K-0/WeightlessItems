#pragma once

#include "PCH.h"

#include "version.h"

namespace Settings {

	constexpr auto IniFile = "Data\\F4SE\\Plugins\\WeightlessItems.ini";

	class Manager {

	public:

		using Keyword = std::uint32_t;
		using Keywords = std::vector<Keyword>;

		[[nodiscard]] static Manager& GetSingleton() noexcept
		{
			static Manager instance;
			return instance;
		}

		[[nodiscard]] const auto& EnableAlchemy() const noexcept { return bEnableAlchemy; }
		[[nodiscard]] const auto& EnableWeapon() const noexcept { return bEnableWeapon; }
		[[nodiscard]] const auto& EnableExplosive() const noexcept { return bEnableExplosive; }
		[[nodiscard]] const auto& EnableArmor() const noexcept { return bEnableArmor; }
		[[nodiscard]] const auto& EnableMisc() const noexcept { return bEnableMisc; }
		[[nodiscard]] const auto& EnableIngredient() const noexcept { return bEnableIngredient; }
		[[nodiscard]] const auto& EnableAmmo() const noexcept { return bEnableAmmo; }
		[[nodiscard]] const auto& EnableBook() const noexcept { return bEnableBook; }
		[[nodiscard]] const auto& EnableArmorMod() const noexcept { return bEnableArmorMod; }
		[[nodiscard]] const auto& EnableWeaponMod() const noexcept { return bEnableWeaponMod; }

		[[nodiscard]] const auto& AlchemyKeywords() noexcept { return vAlchemyKeywords; }
		[[nodiscard]] const auto& WeaponKeywords() noexcept { return vWeaponKeywords; }
		[[nodiscard]] const auto& ArmorKeywords() noexcept { return vArmorKeywords; }
		[[nodiscard]] const auto& MiscKeywords() noexcept { return vMiscKeywords; }
		[[nodiscard]] const auto& IngredientKeywords() noexcept { return vIngredientKeywords; }
		[[nodiscard]] const auto& AmmoKeywords() noexcept { return vAmmoKeywords; }
		[[nodiscard]] const auto& BookKeywords() noexcept { return vBookKeywords; }

	private:

		Manager() noexcept { Read(); }
		~Manager() = default;

		Manager(const Manager&) = delete;
		Manager(Manager&&) = delete;

		Manager& operator=(const Manager&) = delete;
		Manager& operator=(Manager&&) = delete;

		void Read() noexcept;

		bool bEnableAlchemy{};
		Keywords vAlchemyKeywords;

		bool bEnableWeapon{};
		bool bEnableExplosive{};
		Keywords vWeaponKeywords;

		bool bEnableArmor{};
		Keywords vArmorKeywords;

		bool bEnableMisc{};
		Keywords vMiscKeywords;

		bool bEnableIngredient{};
		Keywords vIngredientKeywords;

		bool bEnableAmmo{};
		Keywords vAmmoKeywords;

		bool bEnableBook{};
		Keywords vBookKeywords;

		bool bEnableArmorMod{};
		bool bEnableWeaponMod{};
	};
}