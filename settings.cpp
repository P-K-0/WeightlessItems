
#include "settings.h"

namespace Settings {

	enum class Section {
		Main = 0,
		Keywords
	};

	static const std::vector<std::string> vSection{ "Main", "Keywords" };

	inline const char* GetValue(CSimpleIni& ini, const Section section, const char* key, const char* def) noexcept
	{
		auto s = vSection[static_cast<std::uint32_t>(section)].c_str();

		auto ret = ini.KeyExists(s, key);

		if (ret)
			return ini.GetValue(s, key, def);

		ini.SetValue(s, key, def);
		ini.SaveFile(IniFile);

		return def;
	}

	[[nodiscard]] inline int GetValue(CSimpleIni& ini, const Section section, const char* key, int def) noexcept
	{
		return std::stoi(GetValue(ini, section, key, std::to_string(def).c_str()));
	}

	void ParseKeywords(CSimpleIni& ini, const Section section, const char* key, const char* def, Settings::Manager::Keywords& keywords) noexcept
	{
		std::string str = GetValue(ini, section, key, def);

		keywords.clear();

		while (str != "") {

			auto found = str.find(",");

			if (found == std::string::npos) {

				auto frm = RE::TESForm::GetFormByEditorID(RE::BSFixedString(str));

				if (frm)
					keywords.push_back(frm->GetFormID());

				str = "";

				break;
			}
			else {

				auto frm = RE::TESForm::GetFormByEditorID(RE::BSFixedString(str.substr(0, found)));

				if (frm)
					keywords.push_back(frm->GetFormID());

				str = str.substr(found + 1);
			}
		}

		if (!keywords.empty()) {

			std::sort(keywords.begin(), keywords.end());

			keywords.erase(std::unique(keywords.begin(), keywords.end()), keywords.end());
		}
	}

	void Manager::Read() noexcept
	{

		CSimpleIni ini;

		ini.SetUnicode();
		auto error = ini.LoadFile(IniFile);

		if (error != SI_OK) {

			ini.LoadData(
				"[Main]\n\n"
				"bEnableAlchemy = 1\n"
				"bEnableWeapon = 0\n"
				"bEnableExplosive = 0\n"
				"bEnableArmor = 0\n"
				"bEnableMisc = 1\n"
				"bEnableIngredient = 1\n"
				"bEnableAmmo = 1\n"
				"bEnableBook = 1\n"
				"bEnableMods = 1\n"
				"[Keywords]\n"
				"; Enter only the editor id of the keyword of the object that you want to exclude from the weight zeroing process, if there are more keywords separate them with a comma.\n"
				"; example : sMiscBlacklist=ObjectTypeLooseMod,CA_ObjType_PrewarItem\n"
				"sAlchemyBlacklist=\n"
				"sWeaponBlacklist=\n"
				"sArmorBlacklist=\n"
				"sMiscBlacklist=\n"
				"sIngredientBlacklist=\n"
				"sAmmoBlacklist=\n"
				"sBookBlacklist=\n"
				"sModsBlacklist=\n"
			);

			ini.SaveFile(IniFile);
		}

#define INI_GETVALUE(v, s, d) \
		v = GetValue(ini, s, # v, d);

		INI_GETVALUE(bEnableAlchemy, Section::Main, 1);
		INI_GETVALUE(bEnableWeapon, Section::Main, 0);
		INI_GETVALUE(bEnableExplosive, Section::Main, 0);
		INI_GETVALUE(bEnableArmor, Section::Main, 0);
		INI_GETVALUE(bEnableMisc, Section::Main, 1);
		INI_GETVALUE(bEnableIngredient, Section::Main, 1);
		INI_GETVALUE(bEnableAmmo, Section::Main, 1);
		INI_GETVALUE(bEnableBook, Section::Main, 1);
		INI_GETVALUE(bEnableArmorMod, Section::Main, 1);
		INI_GETVALUE(bEnableWeaponMod, Section::Main, 1);

		ParseKeywords(ini, Section::Keywords, "sAlchemyBlacklist", "", vAlchemyKeywords);
		ParseKeywords(ini, Section::Keywords, "sWeaponBlacklist", "", vWeaponKeywords);
		ParseKeywords(ini, Section::Keywords, "sArmorBlacklist", "", vArmorKeywords);
		ParseKeywords(ini, Section::Keywords, "sMiscBlacklist", "", vMiscKeywords);
		ParseKeywords(ini, Section::Keywords, "sIngredientBlacklist", "", vIngredientKeywords);
		ParseKeywords(ini, Section::Keywords, "sAmmoBlacklist", "", vAmmoKeywords);
		ParseKeywords(ini, Section::Keywords, "sBookBlacklist", "", vBookKeywords);

		logger::info("Settings read!");
	}
}
