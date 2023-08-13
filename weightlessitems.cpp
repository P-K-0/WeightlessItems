
#include "weightlessitems.h"

namespace WeightlessItems {

	template<typename TForm>
	[[nodiscard]] bool HasKeyword(TForm& item, const Settings::Manager::Keywords& keywords) noexcept
	{
		if (keywords.empty())
			return false;

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

	template<typename TForm>
	[[nodiscard]] bool HasKeyword(TForm& item, std::initializer_list<const char*> list) noexcept
	{
		if (!item.keywords || item.numKeywords == 0)
			return false;

		for (std::uint32_t idx{}; idx < item.numKeywords; idx++) {

			auto frmKey = item.keywords[idx];

			if (!frmKey)
				continue;

			for (auto l : list)
				if (_strcmpi(item.keywords[idx]->formEditorID.c_str(), l) == 0)
					return true;
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

	template<typename Fn = std::function<void(RE::BGSMod::Attachment::Mod&)>>
	void ForEachMod(Fn fn) noexcept
	{
		auto dataHandler = RE::TESDataHandler::GetSingleton();

		if (!dataHandler)
			return;

		for (auto& item : dataHandler->GetFormArray<RE::BGSMod::Attachment::Mod>()) {

			if (!item)
				continue;

			fn(*item);
		}
	}

	void Init() noexcept
	{
		auto& settings = Settings::Manager::GetSingleton();

		if (settings.EnableAlchemy())
			ForEachForm<RE::AlchemyItem>([](RE::AlchemyItem& item) { item.weight = 0.0f; }, settings.AlchemyKeywords());

		ForEachForm<RE::TESObjectWEAP>([&](RE::TESObjectWEAP& item) {

			auto isExplosive = HasKeyword(item, std::initializer_list{ "WeaponTypeGrenade", "WeaponTypeExplosive"});

			if (isExplosive && settings.EnableExplosive())
				item.weaponData.weight = 0.0f;

			if (!isExplosive && settings.EnableWeapon())
				item.weaponData.weight = 0.0f;

		}, settings.WeaponKeywords());

		if (settings.EnableArmor())
			ForEachForm<RE::TESObjectARMO>([](RE::TESObjectARMO& item) { item.data.weight = 0.0f; }, settings.ArmorKeywords());

		if (settings.EnableMisc())
			ForEachForm<RE::TESObjectMISC>([](RE::TESObjectMISC& item) { item.weight = 0.0f; }, settings.MiscKeywords());

		if (settings.EnableIngredient())
			ForEachForm<RE::IngredientItem>([](RE::IngredientItem& item) { item.weight = 0.0f; }, settings.IngredientKeywords());

		if (settings.EnableAmmo())
			ForEachForm<RE::TESAmmo>([](RE::TESAmmo& item) { item.weight = 0.0f; }, settings.AmmoKeywords());

		if (settings.EnableBook())
			ForEachForm<RE::TESObjectBOOK>([](RE::TESObjectBOOK& item) { item.weight = 0.0f; }, settings.BookKeywords());

		ForEachMod([&](RE::BGSMod::Attachment::Mod& item) {

			auto isWeap{ item.targetFormType.get() == RE::ENUM_FORM_ID::kWEAP };
			auto isArmor{ item.targetFormType.get() == RE::ENUM_FORM_ID::kARMO };

			auto mods = item.GetBuffer<RE::BGSMod::Property::Mod>(1);

			for (auto& mod : mods) {

				if ((mod.target == 30 && isWeap && settings.EnableWeaponMod()) ||
					(mod.target == 4 && isArmor && settings.EnableArmorMod())) {

					mod.data.mm.min.f = 0.0f;
				}
			}
		});
	}
}
