#pragma once
#include "imgui.h"
#include "data\database.hpp"
#include "utility.hpp"
#include "data\Player.hpp"
#include "TopNTable.hpp"

// 比赛过滤器
struct AnalyzeMatchFilter
{
    // 指定对手
    std::shared_ptr<FootballClub> oppenent_team;

    // 指定赛事（组合）
    long event_flag;

    // 指定时间段
    Date min_date;
    Date max_date;

    AnalyzeMatchFilter()
    {
        event_flag = 0;
    }
};

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

    // 单场最多进球
    int max_goal_at_one_match;

    // 单场最多丢球
    int max_against_goal_at_one_match;

    // 单场最多净胜球
    int max_goal_difference_at_one_match;

    // 单场最少净胜球（最大比分差落败）
    int min_goal_difference_at_one_match;

    AnalyzeMatchResult()
    {
        memset(this, 0, sizeof(AnalyzeMatchResult));
    }
};

struct AnalyzePlayerData
{
    int total_goal_for_lfc;
    int total_assist_for_lfc;
    int total_goal_against_lfc;
    int total_assist_against_lfc;

    AnalyzePlayerData()
    {
        memset(this, 0, sizeof(AnalyzePlayerData));
    }
};

static void ShowAnalyzeMatchWindow(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Analyze Match", p_open))
    {
        // left
        ImGui::BeginChild("analyze match filter", ImVec2(200, 0), true);

        static AnalyzeMatchFilter s_filter;

        ImGui::ClubCombo(utf8("oppenent team"), &s_filter.oppenent_team);
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

        ImGui::BeginChild("match pane", ImVec2(500, 0), true);
        ImGui::Columns(3);

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
        std::map<std::shared_ptr<Player>, AnalyzePlayerData> player_datas;
        for (auto match : all_matchs) {
            if (s_filter.oppenent_team && s_filter.oppenent_team != match->opponent_team) {
                continue;
            }
            if (s_filter.min_date.IsValid() && match->time < s_filter.min_date) {
                continue;
            }
            if (s_filter.max_date.IsValid() && match->time > s_filter.max_date) {
                continue;
            }

            char label[32];
            sprintf(label, "%04d", match->id);
            ImGui::Text(label);
            ImGui::NextColumn();

            ImGui::Text(utility::ToString(match)); ImGui::NextColumn();
            ImGui::Text(utility::ToString(match->time)); ImGui::NextColumn();

            ++result.total_played;
            
            auto match_result = match->GetResult();
            if (match_result == EMatchResult::Win) {
                ++result.total_win;
            }
            else if (match_result == EMatchResult::Draw) {
                ++result.total_draw;
            }
            else if (match_result == EMatchResult::Lose) {
                ++result.total_lose;
            }

            result.total_goal += match->lfc_goals.size();
            result.total_against += match->opponent_goals.size();

            for (auto goal : match->lfc_goals) {
                if (goal->score_player) {
                    ++player_datas[goal->score_player].total_goal_for_lfc;
                }
                if (goal->assist_player) {
                    ++player_datas[goal->assist_player].total_assist_for_lfc;
                }
            }

            for (auto goal : match->opponent_goals) {
                if (goal->score_player) {
                    ++player_datas[goal->score_player].total_goal_against_lfc;
                }
                if (goal->assist_player) {
                    ++player_datas[goal->assist_player].total_assist_against_lfc;
                }
            }
        }
        ImGui::Columns(1);
        ImGui::EndChild();
        ImGui::SameLine();

        // right
        ImGui::BeginChild("result view", ImVec2(600, 0));

        ImGui::Columns(6);
        // header
        ImGui::Separator();
        ImGui::Text(utf8("pl table played")); ImGui::NextColumn();
        ImGui::Text(utf8("pl table won/drawn/lost")); ImGui::NextColumn();
        ImGui::Text(utf8("pl table goals for/against")); ImGui::NextColumn();
        ImGui::Text(utf8("pl table win rate")); ImGui::NextColumn();
        ImGui::Text(utf8("pl table goal per match")); ImGui::NextColumn();
        ImGui::Text(utf8("pl table goal against per match")); ImGui::NextColumn();
        ImGui::Separator();

        ImGui::Text("%d", result.total_played); ImGui::NextColumn();
        ImGui::Text("%d/%d/%d", result.total_win, result.total_draw, result.total_lose); ImGui::NextColumn();
        ImGui::Text("%d/%d", result.total_goal, result.total_against); ImGui::NextColumn();
        ImGui::Text("%.2f", result.total_played == 0.f ? 0.f : (float)result.total_win / result.total_played); ImGui::NextColumn();
        ImGui::Text("%.2f", result.total_played == 0.f ? 0.f : (float)result.total_goal / result.total_played); ImGui::NextColumn();
        ImGui::Text("%.2f", result.total_played == 0.f ? 0.f : (float)result.total_against / result.total_played); ImGui::NextColumn();
        ImGui::Separator();
        ImGui::Columns(1);

        TopNTable<Player> top_score_player_for_lfc(10);
        TopNTable<Player> top_assist_player_for_lfc(10);
        TopNTable<Player> top_create_goal_player_for_lfc(10);
        TopNTable<Player> top_score_player_against_lfc(5);
        TopNTable<Player> top_assist_player_against_lfc(5);
        TopNTable<Player> top_create_goal_player_against_lfc(5);

        for (auto kvp : player_datas) {
            top_score_player_for_lfc.Push(kvp.first, kvp.second.total_goal_for_lfc);
            top_assist_player_for_lfc.Push(kvp.first, kvp.second.total_assist_for_lfc);
            top_create_goal_player_for_lfc.Push(kvp.first, kvp.second.total_goal_for_lfc + kvp.second.total_assist_for_lfc);

            top_score_player_against_lfc.Push(kvp.first, kvp.second.total_goal_against_lfc);
            top_assist_player_against_lfc.Push(kvp.first, kvp.second.total_assist_against_lfc);
            top_create_goal_player_against_lfc.Push(kvp.first, kvp.second.total_goal_against_lfc + kvp.second.total_assist_against_lfc);
        }

        ImGui::TopNPlayerTable(utf8("goal table"), &top_score_player_for_lfc, ImVec2(200, 250));
        ImGui::SameLine();

        ImGui::TopNPlayerTable(utf8("assist table"), &top_assist_player_for_lfc, ImVec2(200, 250));
        ImGui::SameLine();

        ImGui::TopNPlayerTable(utf8("create goal table"), &top_create_goal_player_for_lfc, ImVec2(200, 250));

        ImGui::TopNPlayerTable(utf8("goal table"), &top_score_player_against_lfc);
        ImGui::SameLine();

        ImGui::TopNPlayerTable(utf8("assist table"), &top_assist_player_against_lfc);
        ImGui::SameLine();

        ImGui::TopNPlayerTable(utf8("create goal table"), &top_create_goal_player_against_lfc);

        ImGui::Separator();

        ImGui::EndChild();
    }
    ImGui::End();
}