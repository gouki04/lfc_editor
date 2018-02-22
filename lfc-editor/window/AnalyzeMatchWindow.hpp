#pragma once
#include "imgui.h"
#include "data\database.hpp"
#include "utility.hpp"
#include "Analyze.hpp"
#include "data\Player.hpp"
#include "TopNTable.hpp"

// 比赛分析的数据
struct AnalyzeMatchResult
{
	// 总场次
	int total_played;

	// 胜场
	int total_win;

	// 平局
	int total_draw;

	// 败场
	int total_lose;

	// 总进球
	int total_goal;

	// 总丢球
	int total_against;

	// 有进球的比赛数
	int match_count_with_goals;

	// 没进球的比赛数
	int match_count_with_no_goals;

	// 有丢球的比赛数
	int match_count_with_goal_againsted;

	// 无丢球（零封）的比赛数
	int match_count_with_no_goal_againsted;

	// 单场最多进球
	int max_goal_at_one_match;
	std::vector<std::shared_ptr<Match>> max_goal_at_one_match_vec;

	// 单场最多丢球
	int max_against_goal_at_one_match;
	std::vector<std::shared_ptr<Match>> max_against_goal_at_one_match_vec;

	// 单场最多净胜球
	int max_goal_difference_at_one_match;
	std::vector<std::shared_ptr<Match>> max_goal_difference_at_one_match_vec;

	// 单场最少净胜球（最大比分差落败）
	int min_goal_difference_at_one_match;
	std::vector<std::shared_ptr<Match>> min_goal_difference_at_one_match_vec;

	AnalyzeMatchResult()
	{
		total_played = 0;
		total_win = 0;
		total_draw = 0;
		total_lose = 0;
		total_goal = 0;
		total_against = 0;
		match_count_with_goals = 0;
		match_count_with_no_goals = 0;
		match_count_with_goal_againsted = 0;
		match_count_with_no_goal_againsted = 0;
		max_goal_at_one_match = 0;
		max_against_goal_at_one_match = 0;
		max_goal_difference_at_one_match = 0;
		min_goal_difference_at_one_match = 0;
	}
};

struct MatchRecord
{
	std::vector<std::shared_ptr<Match>> current;
	size_t max_count;
	std::vector<std::vector<std::shared_ptr<Match>>> max;

	MatchRecord()
	{
		max_count = 0;
	}

	void AddMatch(std::shared_ptr<Match> match)
	{
		current.push_back(match);
	}

	void Interrupt()
	{
		if (current.size() > 0) {
			if (current.size() > max_count) {
				max.clear();
				max.push_back(current);

				max_count = current.size();
			}
			else if (current.size() == max_count) {
				max.push_back(current);
			}

			current.clear();
		}
	}
};

struct MatchRecordResult
{
	// 连胜
	MatchRecord keep_win;

	// 连平
	MatchRecord keep_draw;

	// 连败
	MatchRecord keep_lose;

	// 不败
	MatchRecord keep_no_lose;

	// 不胜
	MatchRecord keep_no_win;
};

static void ShowAnalyzeMatchWindow(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Analyze Match", p_open)) {
		// left
		ImGui::BeginChild("analyze match filter", ImVec2(200, 0), true);

		char label[128];

		static AnalyzeMatchFilter s_filter;

		ImGui::ClubCombo(utf8("oppenent team"), &s_filter.oppenent_team);
		ImGui::ESideEdit(utf8("side"), &s_filter.side);

		static std::shared_ptr<DateRange> s_tmp_date_range;
		ImGui::DateRangeCombo(utf8("date"), &s_tmp_date_range);
		if (s_tmp_date_range != s_filter.date_range) {
			s_filter.date_range = s_tmp_date_range;
			if (s_filter.date_range) {
				s_filter.min_date = s_filter.date_range->begin;
				s_filter.max_date = s_filter.date_range->end;
			}
		}

		ImGui::DateEdit(utf8("min time"), &s_filter.min_date);
		ImGui::DateEdit(utf8("max time"), &s_filter.max_date);

		for (int i = (int)EEventType::FriendlyMatch; i < (int)EEventType::EVENT_MAX; ++i) {
			auto flag = 1 << i;
			auto is_checked = (s_filter.event_flag & flag) != 0;
			if (ImGui::Checkbox(utility::ToString((EEventType)i), &is_checked)) {
				if (is_checked) {
					s_filter.event_flag |= flag;
				}
				else {
					s_filter.event_flag &= ~flag;
				}
			}
		}

		ImGui::EndChild();
		ImGui::SameLine();

		ImGui::BeginChild("match pane", ImVec2(300, 0), true);
		ImGui::ColumnHeaders(3, 0.15, 0.55, 0.3);

		// header
		ImGui::Separator();
		ImGui::Text("id"); ImGui::NextColumn();
		ImGui::Text("match"); ImGui::NextColumn();
		ImGui::Text("time"); ImGui::NextColumn();
		ImGui::Separator();

		std::vector<std::shared_ptr<Match>> all_matchs;
		for (auto evt : global_db.events) {
			auto evt_flag = 1 << (int)evt->event_type;
			if ((s_filter.event_flag & evt_flag) != 0) {
				evt->FillMatchs(all_matchs);
			}
		}

		global_db.SortMatchs(all_matchs);

		AnalyzeMatchResult result;
		MatchRecordResult record;
		for (auto match : all_matchs) {
			if (s_filter.side != EMatchSide::Any && s_filter.side != match->side) {
				continue;
			}
			if (s_filter.oppenent_team && s_filter.oppenent_team != match->opponent_team) {
				continue;
			}
			if (s_filter.min_date.IsValid() && match->time < s_filter.min_date) {
				continue;
			}
			if (s_filter.max_date.IsValid() && match->time > s_filter.max_date) {
				continue;
			}

			sprintf(label, "%04d", match->id);
			ImGui::Text(label);
			ImGui::NextColumn();

			ImGui::Text(utility::ToString(match)); ImGui::NextColumn();
			ImGui::Text(utility::ToString(match->time)); ImGui::NextColumn();

			// 统计场次
			++result.total_played;

			// 统计胜平负
			auto match_result = match->GetResult();
			if (match_result == EMatchResult::Win) {
				++result.total_win;

				record.keep_win.AddMatch(match);
				record.keep_draw.Interrupt();
				record.keep_lose.Interrupt();
				record.keep_no_lose.AddMatch(match);
				record.keep_no_win.Interrupt();
			}
			else if (match_result == EMatchResult::Draw) {
				++result.total_draw;

				record.keep_win.Interrupt();
				record.keep_draw.AddMatch(match);
				record.keep_lose.Interrupt();
				record.keep_no_lose.AddMatch(match);
				record.keep_no_win.AddMatch(match);
			}
			else if (match_result == EMatchResult::Lose) {
				++result.total_lose;

				record.keep_win.Interrupt();
				record.keep_draw.Interrupt();
				record.keep_lose.AddMatch(match);
				record.keep_no_lose.Interrupt();
				record.keep_no_win.AddMatch(match);
			}

			auto goal_cnt = match->lfc_goals.size();
			auto against_goal_cnt = match->opponent_goals.size();
			auto goal_difference = (int)goal_cnt - (int)against_goal_cnt;

			// 统计进球场次、零封场次
			if (goal_cnt > 0) {
				++result.match_count_with_goals;
			}
			else {
				++result.match_count_with_no_goals;
			}

			if (against_goal_cnt > 0) {
				++result.match_count_with_goal_againsted;
			}
			else {
				++result.match_count_with_no_goal_againsted;
			}

			// 统计单场最多进球
			if (goal_cnt == result.max_goal_at_one_match) {
				result.max_goal_at_one_match_vec.push_back(match);
			}
			else if (goal_cnt > result.max_goal_at_one_match) {
				result.max_goal_at_one_match = goal_cnt;

				result.max_goal_at_one_match_vec.clear();
				result.max_goal_at_one_match_vec.push_back(match);
			}

			// 统计单场最多丢球
			if (against_goal_cnt == result.max_against_goal_at_one_match) {
				result.max_against_goal_at_one_match_vec.push_back(match);
			}
			else if (against_goal_cnt > result.max_against_goal_at_one_match) {
				result.max_against_goal_at_one_match = against_goal_cnt;

				result.max_against_goal_at_one_match_vec.clear();
				result.max_against_goal_at_one_match_vec.push_back(match);
			}

			// 统计单场最大比分差胜利
			if (goal_difference == result.max_goal_difference_at_one_match) {
				result.max_goal_difference_at_one_match_vec.push_back(match);
			}
			else if (goal_difference > result.max_goal_difference_at_one_match) {
				result.max_goal_difference_at_one_match = goal_difference;

				result.max_goal_difference_at_one_match_vec.clear();
				result.max_goal_difference_at_one_match_vec.push_back(match);
			}

			// 统计单场最大比分差失利
			if (goal_difference == result.min_goal_difference_at_one_match) {
				result.min_goal_difference_at_one_match_vec.push_back(match);
			}
			else if (goal_difference < result.min_goal_difference_at_one_match) {
				result.min_goal_difference_at_one_match = goal_difference;

				result.min_goal_difference_at_one_match_vec.clear();
				result.min_goal_difference_at_one_match_vec.push_back(match);
			}

			// 统计总进球数、总丢球数
			result.total_goal += goal_cnt;
			result.total_against += against_goal_cnt;
		}

		record.keep_win.Interrupt();
		record.keep_draw.Interrupt();
		record.keep_lose.Interrupt();
		record.keep_no_lose.Interrupt();
		record.keep_no_win.Interrupt();

		ImGui::Columns(1);
		ImGui::EndChild();
		ImGui::SameLine();

		// right
		ImGui::BeginChild("result view", ImVec2(600, 0));

		ImGui::Columns(8);
		// header
		ImGui::Separator();
		ImGui::Text(utf8("pl table played")); ImGui::NextColumn();
		ImGui::Text(utf8("pl table won/drawn/lost")); ImGui::NextColumn();
		ImGui::Text(utf8("pl table goals for/against")); ImGui::NextColumn();
		ImGui::Text(utf8("pl table win rate")); ImGui::NextColumn();
		ImGui::Text(utf8("pl table goal per match")); ImGui::NextColumn();
		ImGui::Text(utf8("pl table goal against per match")); ImGui::NextColumn();
		ImGui::Text(utf8("match with goal count")); ImGui::NextColumn();
		ImGui::Text(utf8("match with no goal against count")); ImGui::NextColumn();
		ImGui::Separator();

		ImGui::Text("%d", result.total_played); ImGui::NextColumn();
		ImGui::Text("%d/%d/%d", result.total_win, result.total_draw, result.total_lose); ImGui::NextColumn();
		ImGui::Text("%d/%d", result.total_goal, result.total_against); ImGui::NextColumn();
		ImGui::Text("%.2f", result.total_played == 0.f ? 0.f : (float)result.total_win / result.total_played); ImGui::NextColumn();
		ImGui::Text("%.2f", result.total_played == 0.f ? 0.f : (float)result.total_goal / result.total_played); ImGui::NextColumn();
		ImGui::Text("%.2f", result.total_played == 0.f ? 0.f : (float)result.total_against / result.total_played); ImGui::NextColumn();
		ImGui::Text("%d (%d%%)", result.match_count_with_goals, (int)(100.f * result.match_count_with_goals / result.total_played)); ImGui::NextColumn();
		ImGui::Text("%d (%d%%)", result.match_count_with_no_goal_againsted, (int)(100.f * result.match_count_with_no_goal_againsted / result.total_played)); ImGui::NextColumn();
		ImGui::Separator();
		ImGui::Columns(1);

		sprintf(label, utf8("max goal at one match"), result.max_goal_at_one_match);
		if (ImGui::TreeNode(label)) {
			for (auto match : result.max_goal_at_one_match_vec) {
				ImGui::Text(utility::ToString(match));
			}
			ImGui::TreePop();
		}

		sprintf(label, utf8("max goal against at one match"), result.max_against_goal_at_one_match);
		if (ImGui::TreeNode(label)) {
			for (auto match : result.max_against_goal_at_one_match_vec) {
				ImGui::Text(utility::ToString(match));
			}
			ImGui::TreePop();
		}

		sprintf(label, utf8("max goal difference at one match"), result.max_goal_difference_at_one_match);
		if (ImGui::TreeNode(label)) {
			for (auto match : result.max_goal_difference_at_one_match_vec) {
				ImGui::Text(utility::ToString(match));
			}
			ImGui::TreePop();
		}

		sprintf(label, utf8("min goal difference at one match"), result.min_goal_difference_at_one_match);
		if (ImGui::TreeNode(label)) {
			for (auto match : result.min_goal_difference_at_one_match_vec) {
				ImGui::Text(utility::ToString(match));
			}
			ImGui::TreePop();
		}

		sprintf(label, "keep win (%d)", record.keep_win.max_count);
		if (ImGui::TreeNode(label)) {
			ImGui::PushID("keep win");
			for (auto i = 0; i < record.keep_win.max.size(); ++i) {
				sprintf(label, "%d", i);
				if (ImGui::TreeNode(label)) {
					ImGui::Columns(2);
					for (auto match : record.keep_win.max[i]) {
						ImGui::Text(utility::ToString(match)); ImGui::NextColumn();
						ImGui::Text(utility::ToString(match->time)); ImGui::NextColumn();
					}
					ImGui::Columns(1);
					ImGui::TreePop();
				}
			}
			ImGui::PopID();
			ImGui::TreePop();
		}

		sprintf(label, "keep draw (%d)", record.keep_draw.max_count);
		if (ImGui::TreeNode(label)) {
			ImGui::PushID("keep draw");
			for (auto i = 0; i < record.keep_draw.max.size(); ++i) {
				sprintf(label, "%d", i);
				if (ImGui::TreeNode(label)) {
					ImGui::Columns(2);
					for (auto match : record.keep_draw.max[i]) {
						ImGui::Text(utility::ToString(match)); ImGui::NextColumn();
						ImGui::Text(utility::ToString(match->time)); ImGui::NextColumn();
					}
					ImGui::Columns(1);
					ImGui::TreePop();
				}
			}
			ImGui::PopID();
			ImGui::TreePop();
		}

		sprintf(label, "keep lose (%d)", record.keep_lose.max_count);
		if (ImGui::TreeNode(label)) {
			ImGui::PushID("keep lose");
			for (auto i = 0; i < record.keep_lose.max.size(); ++i) {
				sprintf(label, "%d", i);
				if (ImGui::TreeNode(label)) {
					ImGui::Columns(2);
					for (auto match : record.keep_lose.max[i]) {
						ImGui::Text(utility::ToString(match)); ImGui::NextColumn();
						ImGui::Text(utility::ToString(match->time)); ImGui::NextColumn();
					}
					ImGui::Columns(1);
					ImGui::TreePop();
				}
			}
			ImGui::PopID();
			ImGui::TreePop();
		}

		sprintf(label, "keep no lose (%d)", record.keep_no_lose.max_count);
		if (ImGui::TreeNode(label)) {
			ImGui::PushID("keep no lose");
			for (auto i = 0; i < record.keep_no_lose.max.size(); ++i) {
				sprintf(label, "%d", i);
				if (ImGui::TreeNode(label)) {
					for (auto match : record.keep_no_lose.max[i]) {
						ImGui::Text(utility::ToString(match));
					}
					ImGui::TreePop();
				}
			}
			ImGui::PopID();
			ImGui::TreePop();
		}

		sprintf(label, "keep no win (%d)", record.keep_no_win.max_count);
		if (ImGui::TreeNode(label)) {
			ImGui::PushID("keep no win");
			for (auto i = 0; i < record.keep_no_win.max.size(); ++i) {
				sprintf(label, "%d", i);
				if (ImGui::TreeNode(label)) {
					for (auto match : record.keep_no_win.max[i]) {
						ImGui::Text(utility::ToString(match));
					}
					ImGui::TreePop();
				}
			}
			ImGui::PopID();
			ImGui::TreePop();
		}

		ImGui::EndChild();
	}
	ImGui::End();
}