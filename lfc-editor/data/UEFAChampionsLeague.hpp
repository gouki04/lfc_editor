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
    // ŷ�����ճɼ�
    enum class EUEFAChampionsLeagueResult
    {
        // û�μ�
        None = 0,
        
        // �ʸ�������
        PlayOff = 1,

        // С�����
        Group4th = 2,

        // С�����
        Group3th = 3,

        // 16ǿ����
        Top16 = 4,

        // �ķ�֮һ��������
        Top8 = 5,

        // ���������
        Top4 = 6,

        // �Ǿ�
        SecondPlace = 7,

        // �ھ�
        Champion = 8,
    };

    // С����
    struct GroupState
    {
        // 6�ֱ���
        std::vector<std::shared_ptr<Match>> matches;

        // ��������
        int rank;

        GroupState()
        {
            rank = 0;
            matches.resize(6, nullptr);
        }

        template<class Archive>
        void serialize(Archive &archive)
        {
            archive(CEREAL_NVP(matches), CEREAL_NVP(rank));
        }
    };

    // ŷ��
    struct UEFAChampionsLeague : public Event
    {
        // �ʸ���
        TwoMatch play_off;

        // С����
        GroupState group_state;

        // 16ǿ
        TwoMatch round_of_16;

        // �ķ�֮һ����
        TwoMatch quarter_final;

        // �����
        TwoMatch semi_final;

        // ����
        std::shared_ptr<Match> final;

        EUEFAChampionsLeagueResult GetResult()
        {
            if (final) {
                auto result = final->GetResult();
                return result == EMatchResult::Win ? EUEFAChampionsLeagueResult::Champion : EUEFAChampionsLeagueResult::SecondPlace;
            }

            if (semi_final.second) {
                auto result = semi_final.GetResult();
                return result == EMatchResult::Win ? EUEFAChampionsLeagueResult::SecondPlace : EUEFAChampionsLeagueResult::Top4;
            }

            if (quarter_final.second) {
                auto result = quarter_final.GetResult();
                return result == EMatchResult::Win ? EUEFAChampionsLeagueResult::Top4 : EUEFAChampionsLeagueResult::Top8;
            }

            if (round_of_16.second) {
                auto result = round_of_16.GetResult();
                return result == EMatchResult::Win ? EUEFAChampionsLeagueResult::Top8 : EUEFAChampionsLeagueResult::Top16;
            }

            if (group_state.matches[5]) {
                if (group_state.rank == 1 || group_state.rank == 2) {
                    return EUEFAChampionsLeagueResult::Top16;
                }
                else if (group_state.rank == 3) {
                    return EUEFAChampionsLeagueResult::Group3th;
                }
                else {
                    return EUEFAChampionsLeagueResult::Group4th;
                }
            }

            if (play_off.second) {
                auto result = play_off.GetResult();
                return result == EMatchResult::Win ? EUEFAChampionsLeagueResult::Group4th : EUEFAChampionsLeagueResult::PlayOff;
            }

            if (play_off.first) {
                return EUEFAChampionsLeagueResult::PlayOff;
            }

            return EUEFAChampionsLeagueResult::None;
        }

        UEFAChampionsLeague()
        {
            
        }

        virtual ~UEFAChampionsLeague() {}

        virtual void FillMatchs(std::vector<std::shared_ptr<Match>> &to_fill)
        {
            FillMatch(to_fill, play_off.first);
            FillMatch(to_fill, play_off.second);
            FillMatch(to_fill, group_state.matches);
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
				CEREAL_NVP(round_of_16), CEREAL_NVP(quarter_final), CEREAL_NVP(semi_final),
				CEREAL_NVP(final));
		}

        template<class Archive>
        void load(Archive &archive)
        {
            archive(cereal::base_class<Event>(this), CEREAL_NVP(play_off), CEREAL_NVP(group_state), 
                CEREAL_NVP(round_of_16), CEREAL_NVP(quarter_final), CEREAL_NVP(semi_final),
                CEREAL_NVP(final));
        }
    };
}

#include <cereal/archives/json.hpp>

CEREAL_REGISTER_TYPE(lfc::UEFAChampionsLeague);
CEREAL_REGISTER_POLYMORPHIC_RELATION(lfc::Event, lfc::UEFAChampionsLeague)