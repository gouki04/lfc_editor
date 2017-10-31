#pragma once
#include <cereal/cereal.hpp>
#include <string>
#include "Match.hpp"

namespace lfc
{
	// 两回合比赛
	struct TwoMatch
	{
		// 首回合
		std::shared_ptr<Match> first;

		// 次回合
		std::shared_ptr<Match> second;

		// 两回合比赛最终结果
		EMatchResult GetResult()
		{
			if (!first || !second) {
				return EMatchResult::Any;
			}

			// 有点球大战，直接看点球大战的结果
			if (second->HasPenaltyShootout()) {
				return second->GetResult();
			}

			// 总进球数多的赢
			auto lfc_goals_cnt = first->lfc_goals.size() + second->lfc_goals.size();
			auto oppenont_goals_cnt = first->opponent_goals.size() + second->opponent_goals.size();
			if (lfc_goals_cnt != oppenont_goals_cnt) {
				return lfc_goals_cnt > oppenont_goals_cnt ? EMatchResult::Win : EMatchResult::Lose;
			}

			// 客场进球数多的赢（欧冠加时赛也算客场进球，所以下面不需要区分加时赛进球）
			auto lfc_away_goals_cnt = 0;
			auto oppenont_away_goals_cnt = 0;

			if (first->side == EMatchSide::Home) {
				lfc_away_goals_cnt = second->lfc_goals.size();
				oppenont_away_goals_cnt = first->opponent_goals.size();
			}
			else {
				lfc_away_goals_cnt = first->lfc_goals.size();
				oppenont_away_goals_cnt = second->opponent_goals.size();
			}

			return lfc_away_goals_cnt > oppenont_away_goals_cnt ? EMatchResult::Win : EMatchResult::Lose;
		}

		template<class Archive>
		void serialize(Archive &archive)
		{
			archive(CEREAL_NVP(first), CEREAL_NVP(second));
		}
	};
}