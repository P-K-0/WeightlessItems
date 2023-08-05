
#include "weightlessitems.h"

namespace WeightlessItems {

	template<typename TForm>
	[[nodiscard]] bool HasKeyword(TForm& item, const Settings::Manager::Keywords& keywords) noexcept
	{
		if (!item.keywords || item.numKeywords == 0)
			return false;

		for (std::uint32_t idx{}; idx < item.numKeywords; idx++) {

			auto frmKey = item.keywords[idx];

			if (!frmKey)
				continue;

			return std::binary_search(keywords.begin(), keywords.end(), frmKey->GetFormID());
		}

		return false;
	}

	template<typename TForm, typename Fn = std::function<void(TForm&)>>
	void ForEachForm(Fn fn, const Settings::Manager::Keywords& keywords) noexcept
	{
		auto dataHandler = RE::TESDataHandler::GetSingleton();

		if (!dataHandler)
			return;

		for (auto& item : dataHandler->GetFormArray<TForm>()) {

			if (!item)
				continue;

			if (HasKeyword(*item, keywords)) {

				logger::info("Item excluded = Name : {} Form type : {}", item->GetFullName(), static_cast<std::uint32_t>(item->GetFormType()));

				continue;
			}

			fn(*item);
		}
	}

	void Init() noexcept
	{
		auto& settings = Settings::Manager::GetSingleton();

		if (settings.EnableAlchemy())
			ForEachForm<RE::AlchemyItem>([&](RE::AlchemyItem& item) { item.weight = 0.0f; }, settings.AlchemyKeywords());

		if (settings.EnableWeapon())
			ForEachForm<RE::TESObjectWEAP>([](RE::TESObjectWEAP& item) { item.weaponData.weight = 0.0f; }, settings.WeaponKeywords());

		if (settings.EnableArmor())
			ForEachForm<RE::TESObjectARMO>([](RE::TESObjectARMO& item) { item.data.weight = 0.0f; }, settings.ArmorKeywords());

		if (settings.EnableMisc())
			ForEachForm<RE::TESObjectMISC>([](RE::TESObjectMISC& item) { item.weight = 0.0f; }, settings.MiscKeywords());

		if (settings.EnableIngredient())
			ForEachForm<RE::IngredientItem>([](RE::IngredientItem& item) { item.weight = 0.0f; }, settings.IngredientKeywords());

		if (settings.EnableAmmo())
			ForEachForm<RE::TESAmmo>([](RE::TESAmmo& item) { item.weight = 0.0f; }, settings.AmmoKeywords());
	}
}
