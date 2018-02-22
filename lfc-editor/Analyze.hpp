#pragma once
#include "utility.hpp"

// ����������
struct AnalyzeMatchFilter
{
	// ָ������
	std::shared_ptr<FootballClub> oppenent_team;

	// ���ͳ�
	EMatchSide side;

	// ָ�����£���ϣ�
	long event_flag;

	std::shared_ptr<DateRange> date_range;

	// ָ��ʱ���
	Date min_date;
	Date max_date;

	AnalyzeMatchFilter()
	{
		event_flag = 0;
		side = EMatchSide::Any;
	}
};