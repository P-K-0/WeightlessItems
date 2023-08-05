
#include "PCH.h"

#include "weightlessitems.h"

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Query(const F4SE::QueryInterface * a_f4se, F4SE::PluginInfo * a_info)
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		return false;
	}

	*path /= fmt::format(FMT_STRING("{}.log"), Version::Project);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef NDEBUG
	log->set_level(spdlog::level::trace);
#else
	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::info);
#endif

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%s(%#): [%^%l%$] %v"s);

	logger::info(FMT_STRING("{} v{}"), Version::Project, Version::Name);

	a_info->infoVersion = F4SE::PluginInfo::kVersion;
	a_info->name = Version::Project.data();
	a_info->version = Version::Major;

	if (a_f4se->IsEditor()) {

		logger::critical("loaded in editor"sv);

		return false;
	}

	const auto ver = a_f4se->RuntimeVersion();
	if (ver < F4SE::RUNTIME_1_10_130) {

		logger::critical(FMT_STRING("unsupported runtime v{}"), ver.string());

		return false;
	}

	return true;
}

void Listener(F4SE::MessagingInterface::Message* msg)
{
	switch (msg->type) {

	case F4SE::MessagingInterface::kGameDataReady:

		WeightlessItems::Init();

		break;

	default:

		break;
	}
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface * a_f4se)
{
	F4SE::Init(a_f4se);

	auto messaging = static_cast<F4SE::MessagingInterface*>(a_f4se->QueryInterface(F4SE::LoadInterface::kMessaging));

	if (!messaging)
		return false;

	messaging->RegisterListener(Listener);

	return true;
}
