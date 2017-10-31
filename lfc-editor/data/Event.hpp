#pragma once
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <string>
#include "Date.hpp"

namespace lfc 
{
	// ��������
	enum class EEventType
	{
		// ����
		Any = 0,

		// ������
		FriendlyMatch = 1,

		// Ӣ��
		PremierLeague = 2,

		// ŷ��
		UEFAChampionsLeague = 3,

		// ����������Ӣ�����ޱ�
		PremierLeagueAsiaTrophy = 4,

		// �����������µϱ�
		AudiCup = 5,

		// ������
		EFLCup = 6,

		// ���ܱ�
		FACup = 7,

		// �������������ʹھ���
		InternationalChampionsCup = 8,

		// ŷ��
		UEFAEuropaLeague = 9,

        EVENT_MAX,
	};

	// ����
    struct Event
    {
		// id
        int id;

		// ��������
        std::string name;

		// ��ʼʱ��
        Date begin_time;

		// ����ʱ��
        Date end_time;

		// ��������
		EEventType event_type;

        void FillMatch(std::vector<std::shared_ptr<Match>> &to_fill, std::vector<std::shared_ptr<Match>> &matchs)
        {
            for (auto match : matchs) {
                if (match) {
                    to_fill.push_back(match);
                }
            }
        }

        void FillMatch(std::vector<std::shared_ptr<Match>> &to_fill, std::shared_ptr<Match> match)
        {
            if (match) {
                to_fill.push_back(match);
            }
        }

        virtual void FillMatchs(std::vector<std::shared_ptr<Match>> &to_fill) = 0;

        virtual ~Event() {}

        //template<class Archive>
        //void serialize(Archive &archive)
        //{
        //    archive(CEREAL_NVP(id), CEREAL_NVP(name), CEREAL_NVP(begin_time), CEREAL_NVP(end_time), CEREAL_NVP(event_type));
        //}

		template<class Archive>
		void save(Archive &archive) const
		{
			archive(CEREAL_NVP(id), CEREAL_NVP(name), CEREAL_NVP(begin_time), CEREAL_NVP(end_time), CEREAL_NVP(event_type));
		}

		template<class Archive>
		void load(Archive &archive)
		{
			archive(CEREAL_NVP(id), CEREAL_NVP(name), CEREAL_NVP(begin_time), CEREAL_NVP(end_time), CEREAL_NVP(event_type));
		}
    };
}

#include <cereal/archives/json.hpp>

CEREAL_REGISTER_TYPE(lfc::Event);