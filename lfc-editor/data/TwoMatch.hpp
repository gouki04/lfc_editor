#pragma once
#include <cereal/cereal.hpp>
#include <string>
#include "Match.hpp"

namespace lfc
{
	// ���غϱ���
	struct TwoMatch
	{
		// �׻غ�
		std::shared_ptr<Match> first;

		// �λغ�
		std::shared_ptr<Match> second;

		// ���غϱ������ս��
		EMatchResult GetResult()
		{
			if (!first || !second) {
				return EMatchResult::Any;
			}

			// �е����ս��ֱ�ӿ������ս�Ľ��
			if (second->HasPenaltyShootout()) {
				return second->GetResult();
			}

			// �ܽ��������Ӯ
			auto lfc_goals_cnt = first->lfc_goals.size() + second->lfc_goals.size();
			auto oppenont_goals_cnt = first->opponent_goals.size() + second->opponent_goals.size();
			if (lfc_goals_cnt != oppenont_goals_cnt) {
				return lfc_goals_cnt > oppenont_goals_cnt ? EMatchResult::Win : EMatchResult::Lose;
			}

			// �ͳ����������Ӯ��ŷ�ڼ�ʱ��Ҳ��ͳ������������治��Ҫ���ּ�ʱ������
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