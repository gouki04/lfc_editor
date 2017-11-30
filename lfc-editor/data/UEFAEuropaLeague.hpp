#pragma once
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>
#include <string>
#include "Event.hpp"
#include "Match.hpp"
#include "TwoMatch.hpp"
#include "UEFAChampionsLeague.hpp"

namespace lfc
{
    // 欧联最终成绩
    enum class EUEFAEuropaLeagueResult
    {
        // 没参加
        None = 0,

        // 资格赛出局
        PlayOff = 1,

        // 小组第四
        Group4th = 2,

        // 小组第三
        Group3th = 3,

        // 32强出局
        Top32 = 4,

        // 16强出局
        Top16 = 5,

        // 四分之一决赛出局
        Top8 = 6,

        // 半决赛出局
        Top4 = 7,

        // 亚军
        SecondPlace = 8,

        // 冠军
        Champion = 9,
    };

    // 欧联
    struct UEFAEuropaLeague : public Event
    {
        // 资格赛
        TwoMatch qualifying;

        // 附加赛
        TwoMatch play_off;

        // 小组赛
        GroupState group_state;

        // 32强
        TwoMatch round_of_32;

        // 16强
        TwoMatch round_of_16;

        // 四分之一决赛
        TwoMatch quarter_final;

        // 半决赛
        TwoMatch semi_final;

        // 决赛
        std::shared_ptr<Match> final;

        EUEFAEuropaLeagueResult GetResult()
        {
            if (final) {
                auto result = final->GetResult();
                return result == EMatchResult::Win ? EUEFAEuropaLeagueResult::Champion : EUEFAEuropaLeagueResult::SecondPlace;
            }

            if (semi_final.second) {
                auto result = semi_final.GetResult();
                return result == EMatchResult::Win ? EUEFAEuropaLeagueResult::SecondPlace : EUEFAEuropaLeagueResult::Top4;
            }

            if (quarter_final.second) {
                auto result = quarter_final.GetResult();
                return result == EMatchResult::Win ? EUEFAEuropaLeagueResult::Top4 : EUEFAEuropaLeagueResult::Top8;
            }

            if (round_of_16.second) {
                auto result = round_of_16.GetResult();
                return result == EMatchResult::Win ? EUEFAEuropaLeagueResult::Top8 : EUEFAEuropaLeagueResult::Top16;
            }

            if (round_of_32.second) {
                auto result = round_of_32.GetResult();
                return result == EMatchResult::Win ? EUEFAEuropaLeagueResult::Top16 : EUEFAEuropaLeagueResult::Top32;
            }

            if (group_state.matches[5]) {
                if (group_state.rank == 1 || group_state.rank == 2) {
                    return EUEFAEuropaLeagueResult::Top32;
                }
                else if (group_state.rank == 3) {
                    return EUEFAEuropaLeagueResult::Group3th;
                }
                else {
                    return EUEFAEuropaLeagueResult::Group4th;
                }
            }

            if (qualifying.second) {
                auto result = qualifying.GetResult();
                return result == EMatchResult::Win ? EUEFAEuropaLeagueResult::Group4th : EUEFAEuropaLeagueResult::PlayOff;
            }

            if (qualifying.first) {
                return EUEFAEuropaLeagueResult::PlayOff;
            }

            return EUEFAEuropaLeagueResult::None;
        }

        UEFAEuropaLeague()
        {

        }

        virtual ~UEFAEuropaLeague() {}

        virtual void FillMatchs(std::vector<std::shared_ptr<Match>> &to_fill)
        {
            FillMatch(to_fill, qualifying.first);
            FillMatch(to_fill, qualifying.second);
            FillMatch(to_fill, group_state.matches);
            FillMatch(to_fill, round_of_32.first);
            FillMatch(to_fill, round_of_32.second);
            FillMatch(to_fill, round_of_16.first);
            FillMatch(to_fill, round_of_16.second);
            FillMatch(to_fill, quarter_final.first);
            FillMatch(to_fill, quarter_final.second);
            FillMatch(to_fill, semi_final.first);
            FillMatch(to_fill, semi_final.second);
            FillMatch(to_fill, final);
        }

        template<class Archive>
        void save(Archive &archive) const
        {
            archive(cereal::base_class<Event>(this), CEREAL_NVP(qualifying), CEREAL_NVP(play_off), CEREAL_NVP(group_state),
                CEREAL_NVP(round_of_32), CEREAL_NVP(round_of_16), CEREAL_NVP(quarter_final), CEREAL_NVP(semi_final),
                CEREAL_NVP(final));
        }

        template<class Archive>
        void load(Archive &archive)
        {
            archive(cereal::base_class<Event>(this), CEREAL_NVP(qualifying), CEREAL_NVP(play_off), CEREAL_NVP(group_state),
                CEREAL_NVP(round_of_32), CEREAL_NVP(round_of_16), CEREAL_NVP(quarter_final), CEREAL_NVP(semi_final),
                CEREAL_NVP(final));
        }
    };
}

#include <cereal/archives/json.hpp>

CEREAL_REGISTER_TYPE(lfc::UEFAEuropaLeague);
CEREAL_REGISTER_POLYMORPHIC_RELATION(lfc::Event, lfc::UEFAEuropaLeague)