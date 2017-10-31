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
    // ŷ�����ճɼ�
    enum class EUEFAEuropaLeagueResult
    {
        // û�μ�
        None = 0,

        // �ʸ�������
        PlayOff = 1,

        // С�����
        Group4th = 2,

        // С�����
        Group3th = 3,

        // 32ǿ����
        Top32 = 4,

        // 16ǿ����
        Top16 = 5,

        // �ķ�֮һ��������
        Top8 = 6,

        // ���������
        Top4 = 7,

        // �Ǿ�
        SecondPlace = 8,

        // �ھ�
        Champion = 9,
    };

    // ŷ��
    struct UEFAEuropaLeague : public Event
    {
        // �ʸ���
        TwoMatch play_off;

        // С����
        GroupState group_state;

        // 32ǿ
        TwoMatch round_of_32;

        // 16ǿ
        TwoMatch round_of_16;

        // �ķ�֮һ����
        TwoMatch quarter_final;

        // �����
        TwoMatch semi_final;

        // ����
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

            if (play_off.second) {
                auto result = play_off.GetResult();
                return result == EMatchResult::Win ? EUEFAEuropaLeagueResult::Group4th : EUEFAEuropaLeagueResult::PlayOff;
            }

            if (play_off.first) {
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
            FillMatch(to_fill, play_off.first);
            FillMatch(to_fill, play_off.second);
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
			archive(cereal::base_class<Event>(this), CEREAL_NVP(play_off), CEREAL_NVP(group_state),
				CEREAL_NVP(round_of_32), CEREAL_NVP(round_of_16), CEREAL_NVP(quarter_final), CEREAL_NVP(semi_final),
				CEREAL_NVP(final));
		}

        template<class Archive>
        void load(Archive &archive)
        {
            archive(cereal::base_class<Event>(this), CEREAL_NVP(play_off), CEREAL_NVP(group_state),
                CEREAL_NVP(round_of_32), CEREAL_NVP(round_of_16), CEREAL_NVP(quarter_final), CEREAL_NVP(semi_final),
                CEREAL_NVP(final));
        }
    };
}

#include <cereal/archives/json.hpp>

CEREAL_REGISTER_TYPE(lfc::UEFAEuropaLeague);
CEREAL_REGISTER_POLYMORPHIC_RELATION(lfc::Event, lfc::UEFAEuropaLeague)