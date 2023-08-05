#pragma once

#pragma warning(push)
#include "F4SE/F4SE.h"
#include "RE/Fallout.h"
#include "xbyak/xbyak.h"

#ifdef NDEBUG
#	include <spdlog/sinks/basic_file_sink.h>
#else
#	include <spdlog/sinks/msvc_sink.h>
#endif
#pragma warning(pop)

#define DLLEXPORT __declspec(dllexport)

namespace logger = F4SE::log;

using namespace std::literals;

#include "Version.h"

#include <SimpleIni.h>

namespace stl {

	template<typename T, std::size_t N>
	inline void write_call(REL::ID id)
	{
		F4SE::AllocTrampoline(16);

		auto& trampoline = F4SE::GetTrampoline();

		T::fn = trampoline.write_call<N>(id.address() + T::offset, T::thunk);
	}

	template<typename T, std::size_t N>
	inline void write_call(REL::ID id, std::ptrdiff_t offset)
	{
		F4SE::AllocTrampoline(16);

		auto& trampoline = F4SE::GetTrampoline();

		T::fn = trampoline.write_call<N>(id.address() + offset, T::thunk);
	}

	template<typename T>
	inline void write_vfunc()
	{
		REL::Relocation<std::uintptr_t> fn{ T::vtbl[0] };

		T::func = fn.write_vfunc(T::index, T::thunk);
	}

	template<typename T>
	inline void write_value(REL::ID id, std::ptrdiff_t offset, T value, std::uint8_t data[], std::size_t size, std::uint32_t index)
	{
		REL::Relocation<std::uintptr_t> fn(id, offset);

		std::memcpy(&data[index], &value, sizeof value);

		REL::safe_write(fn.address(), data, size);
	}

	inline void write_value(REL::ID id, std::ptrdiff_t offset, std::uint8_t data[], std::size_t size)
	{
		REL::Relocation<std::uintptr_t> fn(id, offset);

		REL::safe_write(fn.address(), data, size);
	}
}