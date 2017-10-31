#pragma once
#include <cereal/types/memory.hpp>
#include "MatchTime.hpp"

namespace lfc
{
    // 进球类型
    enum class EGoalType
    {
		Any = -1,

        // 普通进球
        Normal = 0,

        // 点球
        Penalty = 1,

        // 点球大战
        PenaltyShootOut = 2,

        // 任意球
        FreeKick = 3,

        // 乌龙
        OwnGoal = 4,
    };

    struct Match;
    struct Player;

    struct Goal
    {
        int id;
        std::shared_ptr<Match> match;
        std::shared_ptr<Player> score_player;
        std::shared_ptr<Player> assist_player;
        EGoalType type;
        MatchTime time;
        bool for_lfc;

        Goal()
        {
            for_lfc = false;
            type = EGoalType::Normal;
        }

        template<class Archive>
        void serialize(Archive &archive)
        {
            archive(CEREAL_NVP(id), CEREAL_NVP(match), 
                CEREAL_NVP(score_player), CEREAL_NVP(assist_player), 
                CEREAL_NVP(type), CEREAL_NVP(time), CEREAL_NVP(for_lfc));
        }
    };
}