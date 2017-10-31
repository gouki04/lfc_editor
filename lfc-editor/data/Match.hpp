#pragma once
#include <string>
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include "Date.hpp"

namespace lfc
{
    enum class EMatchResult
    {
        Any = 0,
        Win = 1,
        Draw = 2,
        Lose = 3,
    };

    enum class EMatchSide
    {
        Any = 0,
        Home = 1,
        Away = 2,
    };

    struct FootballClub;
    struct Goal;

    struct Match
    {
        int id;
        Date time;
        EMatchSide side;
        std::shared_ptr<FootballClub> opponent_team;
        std::vector<std::shared_ptr<Goal>> lfc_goals;
        std::vector<std::shared_ptr<Goal>> opponent_goals;
        std::vector<std::shared_ptr<Goal>> lfc_penalty_shootout_goals;
        std::vector<std::shared_ptr<Goal>> opponent_penalty_shootout_goals;

		bool IsLfcGoal(std::shared_ptr<Goal> goal) 
		{
			for (auto g : lfc_goals) {
				if (g == goal) {
					return true;
				}
			}

			for (auto g : lfc_penalty_shootout_goals) {
				if (g == goal) {
					return true;
				}
			}

			return false;
		}

        EMatchResult GetResult()
        {
            if (lfc_penalty_shootout_goals.size() > 0 || opponent_penalty_shootout_goals.size() > 0) {
                if (lfc_penalty_shootout_goals.size() > opponent_penalty_shootout_goals.size()) {
                    return EMatchResult::Win;
                }
                else {
                    return EMatchResult::Lose;
                }
            }

            if (lfc_goals.size() > opponent_goals.size()) {
                return EMatchResult::Win;
            }
            else if (lfc_goals.size() == opponent_goals.size()) {
                return EMatchResult::Draw;
            }
            else {
                return EMatchResult::Lose;
            }
        }

		bool HasPenaltyShootout()
		{
			return lfc_penalty_shootout_goals.size() > 0 || opponent_penalty_shootout_goals.size();
		}

        template<class Archive>
        void serialize(Archive &archive)
        {
            archive(CEREAL_NVP(id), CEREAL_NVP(time), CEREAL_NVP(side), CEREAL_NVP(opponent_team),
                CEREAL_NVP(lfc_goals), CEREAL_NVP(opponent_goals),
                CEREAL_NVP(lfc_penalty_shootout_goals), CEREAL_NVP(opponent_penalty_shootout_goals));
        }
    };
}