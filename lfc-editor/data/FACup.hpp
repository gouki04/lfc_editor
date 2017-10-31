#pragma once
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>
#include <string>
#include "Event.hpp"
#include "Match.hpp"

namespace lfc
{
	// 重赛规则的比赛
	struct ReplayMatch
	{
		std::shared_ptr<Match> match;
		std::shared_ptr<Match> replay;

		EMatchResult GetResult()
		{
			if (replay) {
				return replay->GetResult();
			}

			if (match) {
				auto result = match->GetResult();
				if (result == EMatchResult::Draw) {
					return EMatchResult::Any;
				}

				return result;
			}

			return EMatchResult::Any;
		}

		template<class Archive>
		void serialize(Archive &archive)
		{
			archive(CEREAL_NVP(match), CEREAL_NVP(replay));
		}
	};

	// 英足总杯
	struct FACup : public Event
	{
		// 64强
		ReplayMatch top_of_64;

		// 32强
		ReplayMatch top_of_32;

		// 16强
		ReplayMatch top_of_16;

		// 8强
		ReplayMatch quarter_final;

		// 半决赛
		std::shared_ptr<Match> semi_final;

		// 决赛
		std::shared_ptr<Match> final;

		virtual ~FACup() {}

        virtual void FillMatchs(std::vector<std::shared_ptr<Match>> &to_fill)
        {
            FillMatch(to_fill, top_of_64.match);
            FillMatch(to_fill, top_of_64.replay);
            FillMatch(to_fill, top_of_32.match);
            FillMatch(to_fill, top_of_32.replay);
            FillMatch(to_fill, top_of_16.match);
            FillMatch(to_fill, top_of_16.replay);
            FillMatch(to_fill, quarter_final.match);
            FillMatch(to_fill, quarter_final.replay);
            FillMatch(to_fill, semi_final);
            FillMatch(to_fill, final);
        }

		template<class Archive>
		void save(Archive &archive) const
		{
			archive(cereal::base_class<Event>(this), CEREAL_NVP(top_of_64), CEREAL_NVP(top_of_32), CEREAL_NVP(top_of_16),
				CEREAL_NVP(quarter_final), CEREAL_NVP(semi_final), CEREAL_NVP(final));
		}

		template<class Archive>
		void load(Archive &archive)
		{
			archive(cereal::base_class<Event>(this), CEREAL_NVP(top_of_64), CEREAL_NVP(top_of_32), CEREAL_NVP(top_of_16),
				CEREAL_NVP(quarter_final), CEREAL_NVP(semi_final), CEREAL_NVP(final));
		}
	};
}

#include <cereal/archives/json.hpp>

CEREAL_REGISTER_TYPE(lfc::FACup);
CEREAL_REGISTER_POLYMORPHIC_RELATION(lfc::Event, lfc::FACup)