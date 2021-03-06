#pragma once
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>
#include <string>
#include "Event.hpp"
#include "Match.hpp"

namespace lfc
{
    struct PremierLeagueAsiaTrophy : public Event
    {
        std::shared_ptr<Match> first;
        std::shared_ptr<Match> second;

        virtual ~PremierLeagueAsiaTrophy() {}

        virtual void FillMatchs(std::vector<std::shared_ptr<Match>> &to_fill)
        {
            FillMatch(to_fill, first);
            FillMatch(to_fill, second);
        }

		template<class Archive>
		void save(Archive &archive) const
		{
			archive(cereal::base_class<Event>(this), CEREAL_NVP(first), CEREAL_NVP(second));
		}

        template<class Archive>
        void load(Archive &archive)
        {
            archive(cereal::base_class<Event>(this), CEREAL_NVP(first), CEREAL_NVP(second));
        }
    };
}

#include <cereal/archives/json.hpp>

CEREAL_REGISTER_TYPE(lfc::PremierLeagueAsiaTrophy);
CEREAL_REGISTER_POLYMORPHIC_RELATION(lfc::Event, lfc::PremierLeagueAsiaTrophy)