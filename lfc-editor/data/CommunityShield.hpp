#pragma once
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>
#include <string>
#include "Event.hpp"
#include "Match.hpp"

namespace lfc
{
    // ÉçÇø¶Ü
    struct CommunityShield : public Event
    {
        std::shared_ptr<Match> final;

        virtual ~CommunityShield() {}

        virtual void FillMatchs(std::vector<std::shared_ptr<Match>> &to_fill)
        {
            FillMatch(to_fill, final);
        }

        template<class Archive>
        void save(Archive &archive) const
        {
            archive(cereal::base_class<Event>(this), CEREAL_NVP(final));
        }

        template<class Archive>
        void load(Archive &archive)
        {
            archive(cereal::base_class<Event>(this), CEREAL_NVP(final));
        }
    };
}

#include <cereal/archives/json.hpp>

CEREAL_REGISTER_TYPE(lfc::CommunityShield);
CEREAL_REGISTER_POLYMORPHIC_RELATION(lfc::Event, lfc::CommunityShield)