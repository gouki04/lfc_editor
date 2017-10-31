#pragma once
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>
#include <string>
#include "Event.hpp"
#include "Match.hpp"
#include "TwoMatch.hpp"

namespace lfc
{
    // Ӣ������
    struct EFLCup : public Event
    {
        // ��2��
        std::shared_ptr<Match> round_2;

        // 32ǿ
        std::shared_ptr<Match> top_of_32;

        // 16ǿ
        std::shared_ptr<Match> top_of_16;

        // 8ǿ
        std::shared_ptr<Match> quarter_final;

        // �����
        TwoMatch semi_final;

        // ����
        std::shared_ptr<Match> final;

        virtual ~EFLCup() {}

        virtual void FillMatchs(std::vector<std::shared_ptr<Match>> &to_fill)
        {
            FillMatch(to_fill, round_2);
            FillMatch(to_fill, top_of_32);
            FillMatch(to_fill, top_of_16);
            FillMatch(to_fill, quarter_final);
            FillMatch(to_fill, semi_final.first);
            FillMatch(to_fill, semi_final.second);
            FillMatch(to_fill, final);
        }

		template<class Archive>
		void save(Archive &archive) const
		{
			archive(cereal::base_class<Event>(this), CEREAL_NVP(round_2), CEREAL_NVP(top_of_32), CEREAL_NVP(top_of_16), CEREAL_NVP(quarter_final), CEREAL_NVP(semi_final), CEREAL_NVP(final));
		}

        template<class Archive>
        void load(Archive &archive)
        {
            archive(cereal::base_class<Event>(this), CEREAL_NVP(round_2), CEREAL_NVP(top_of_32), CEREAL_NVP(top_of_16), CEREAL_NVP(quarter_final), CEREAL_NVP(semi_final), CEREAL_NVP(final));
        }
    };
}

#include <cereal/archives/json.hpp>

CEREAL_REGISTER_TYPE(lfc::EFLCup);
CEREAL_REGISTER_POLYMORPHIC_RELATION(lfc::Event, lfc::EFLCup)