#pragma once
#include <cereal/types/map.hpp>

struct Localization
{
	std::map<std::string, std::string> translation;

	const char *translate(const char *str)
	{
		auto itr = translation.find(str);
		if (itr != translation.end()) {
			return itr->second.c_str();
		}

		return str;
	}

	template<class Archive>
	void serialize(Archive &archive)
	{
		archive(CEREAL_NVP(translation));
	}
};

Localization global_localization;

#define utf8(str) global_localization.translate(str)