#pragma once
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <string>
#include "Date.hpp"

namespace lfc 
{
    // 赛事类型
    enum class EEventType
    {
        // 任意
        Any = 0,

        // 友谊赛
        FriendlyMatch = 1,

        // 英超
        PremierLeague = 2,

        // 欧冠
        UEFAChampionsLeague = 3,

        // 【友谊赛】英超亚洲杯
        PremierLeagueAsiaTrophy = 4,

        // 【友谊赛】奥迪杯
        AudiCup = 5,

        // 联赛杯
        EFLCup = 6,

        // 足总杯
        FACup = 7,

        // 【友谊赛】国际冠军杯
        InternationalChampionsCup = 8,

        // 欧联
        UEFAEuropaLeague = 9,

        EVENT_MAX,
    };

    // 赛事
    struct Event
    {
        // id
        int id;

        // 赛事名字
        std::string name;

        // 开始时间
        Date begin_time;

        // 结束时间
        Date end_time;

        // 赛事类型
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