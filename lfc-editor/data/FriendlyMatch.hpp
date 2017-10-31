#pragma once
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <string>
#include "Event.hpp"
#include "Match.hpp"

namespace lfc
{
    struct FriendlyMatch : public Event
    {
		//std::shared_ptr<Match> match;
		std::vector<std::shared_ptr<Match>> matchs;

        virtual ~FriendlyMatch() {}

        virtual void FillMatchs(std::vector<std::shared_ptr<Match>> &to_fill)
        {
            FillMatch(to_fill, matchs);
        }

        //template<class Archive>
        //void serialize(Archive &archive)
        //{
        //    archive(cereal::base_class<Event>(this), CEREAL_NVP(match));
        //}

		template<class Archive>
		void save(Archive &archive) const
		{
			archive(cereal::base_class<Event>(this), CEREAL_NVP(matchs));
		}

		template<class Archive>
		void load(Archive &archive)
		{
			archive(cereal::base_class<Event>(this), CEREAL_NVP(matchs));
		}
    };
}

#include <cereal/archives/json.hpp>

CEREAL_REGISTER_TYPE(lfc::FriendlyMatch);
CEREAL_REGISTER_POLYMORPHIC_RELATION(lfc::Event, lfc::FriendlyMatch)