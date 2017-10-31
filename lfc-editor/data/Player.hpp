#pragma once

#include <string>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include "Date.hpp"

namespace lfc
{
    enum class EPosition
    {
        Any = 0,
        Forward = 1,
        Midfielder = 2,
        Defender = 3,
        Goalkeeper = 4,
    };

    struct Goal;

    struct PlayerDetailInfo
    {
        int no;
        EPosition position;
        Date born_time;
        Date join_time;
        Date leave_time;

        PlayerDetailInfo()
        {
            no = 0;
            position = EPosition::Any;
        }

        int GetAge()
        {
            return (Date::Now() - born_time).TotalYears();
        }

		template<class Archive>
		void save(Archive &archive) const
		{
			archive(CEREAL_NVP(no), CEREAL_NVP(position), CEREAL_NVP(born_time), CEREAL_NVP(join_time), CEREAL_NVP(leave_time));
		}

        template<class Archive>
        void load(Archive &archive)
        {
            archive(CEREAL_NVP(no), CEREAL_NVP(position), CEREAL_NVP(born_time), CEREAL_NVP(join_time), CEREAL_NVP(leave_time));
        }
    };

    struct Player
    {
        int id;
		std::string name;
		std::string en_name;
        std::shared_ptr<PlayerDetailInfo> detail;
        //std::vector<std::shared_ptr<Goal>> goals;

		template<class Archive>
		void save(Archive &archive) const
		{
			archive(CEREAL_NVP(id), CEREAL_NVP(name), CEREAL_NVP(detail), CEREAL_NVP(en_name));
		}

        template<class Archive>
        void load(Archive &archive)
        {
            archive(CEREAL_NVP(id), CEREAL_NVP(name), CEREAL_NVP(detail), CEREAL_NVP(en_name));
        }
    };
}