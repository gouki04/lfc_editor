#pragma once
#include <cereal/cereal.hpp>

namespace lfc
{
	// 比赛时间点
    struct MatchTime
    {
		// 是否在点球大战时刻
        bool is_penalty_shootout;

		// 常规比赛时间
        int local_time;

		// 伤停补时
        int injury_time;

		MatchTime()
		{
			is_penalty_shootout = false;
			local_time = 0;
			injury_time = 0;
		}

		bool IsValid()
		{
			return is_penalty_shootout || local_time > 0;
		}

		bool operator==(const MatchTime &other) 
		{ 
			if (is_penalty_shootout) {
				return other.is_penalty_shootout;
			}
			else {
				return local_time == other.local_time && injury_time == other.injury_time;
			}
		}

		bool operator!=(const MatchTime &other)
		{
			return !(*this == other);
		}

		bool operator<(const MatchTime &other)
		{
			if (is_penalty_shootout) {
				return false;
			}
			else {
				if (local_time == other.local_time) {
					return injury_time < other.injury_time;
				}
				else {
					return local_time < other.local_time;
				}
			}
		}

		bool operator<=(const MatchTime &other)
		{
			if (is_penalty_shootout) {
				return other.is_penalty_shootout;
			}
			else {
				if (local_time == other.local_time) {
					return injury_time <= other.injury_time;
				}
				else {
					return local_time < other.local_time;
				}
			}
		}

		bool operator>(const MatchTime &other)
		{
			return !(*this <= other);
		}

		bool operator>=(const MatchTime &other)
		{
			return !(*this < other);
		}

        template<class Archive>
        void serialize(Archive &archive)
        {
            archive(CEREAL_NVP(is_penalty_shootout), CEREAL_NVP(local_time), CEREAL_NVP(injury_time));
        }
    };
}