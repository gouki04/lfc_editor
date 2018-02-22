#pragma once
#include "utility.hpp"

// 比赛过滤器
struct AnalyzeMatchFilter
{
	// 指定对手
	std::shared_ptr<FootballClub> oppenent_team;

	// 主客场
	EMatchSide side;

	// 指定赛事（组合）
	long event_flag;

	std::shared_ptr<DateRange> date_range;

	// 指定时间段
	Date min_date;
	Date max_date;

	AnalyzeMatchFilter()
	{
		event_flag = 0;
		side = EMatchSide::Any;
	}
};