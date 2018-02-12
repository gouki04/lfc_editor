#pragma once
#include "imgui.h"
#include "data\database.hpp"
#include "utility.hpp"
#include "data\PremierLeague.hpp"

enum class ESortType
{
    Point = 1,
    Goals = 2,
    GoalsAgainst = 3,
    Win = 4,
    Draw = 5,
    Lose = 6,
};

static void HeaderButton(const char *title, ESortType type, ESortType *out_sort_type, bool *is_ascending)
{
    auto is_active = false;
    if (type == *out_sort_type) {
        is_active = true;

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.77f, 0.50f, 0.50f, 0.70f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.77f, 0.50f, 0.50f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.90f, 0.60f, 0.60f, 1.00f));
    }

    bool is_click_btn;
    if (is_active) {
        auto ascending_symbol = *is_ascending ? "+" : "-";
        auto new_title = std::string(title) + ascending_symbol;

        is_click_btn = ImGui::Button(new_title.c_str());
    }
    else {
        is_click_btn = ImGui::Button(title);
    }

    if (is_click_btn) {
        if (is_active) {
            *is_ascending = !*is_ascending;
        }
        else {
            *out_sort_type = type;
            *is_ascending = false;
        }
    }

    if (is_active) {
        ImGui::PopStyleColor(3);
    }
}

static void ShowAnalyzePrimierLeagueWindow(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Analyze PL", p_open)) {
        static int s_round = 1;
        static ESortType s_sort_type = ESortType::Point;
        static bool s_is_ascending = false;

        ImGui::SliderInt("round", &s_round, 1, 38);

        ImGui::ColumnHeaders(7, 0.4, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1);

        // header
        ImGui::Separator();
        ImGui::Text("name"); ImGui::NextColumn();
        HeaderButton("point", ESortType::Point, &s_sort_type, &s_is_ascending); ImGui::NextColumn();
        HeaderButton("won", ESortType::Win, &s_sort_type, &s_is_ascending); ImGui::NextColumn();
        HeaderButton("drawn", ESortType::Draw, &s_sort_type, &s_is_ascending); ImGui::NextColumn();
        HeaderButton("lost", ESortType::Lose, &s_sort_type, &s_is_ascending); ImGui::NextColumn();
        HeaderButton("goals", ESortType::Goals, &s_sort_type, &s_is_ascending); ImGui::NextColumn();
        HeaderButton("against", ESortType::GoalsAgainst, &s_sort_type, &s_is_ascending); ImGui::NextColumn();
        ImGui::Separator();

        TopNTable<PremierLeague> top_pl_table(10);

        for (auto evt : global_db.events) {
            if (evt->event_type != EEventType::PremierLeague) {
                continue;
            }

            auto pl = std::static_pointer_cast<PremierLeague>(evt);
            auto pl_table = pl->GetTable(s_round);

            int value = 0;
            switch (s_sort_type)
            {
            case ESortType::Point:
                value = pl_table.points;
                break;
            case ESortType::Goals:
                value = pl_table.goals_for;
                break;
            case ESortType::GoalsAgainst:
                value = pl_table.goals_against;
                break;
            case ESortType::Win:
                value = pl_table.won;
                break;
            case ESortType::Draw:
                value = pl_table.drawn;
                break;
            case ESortType::Lose:
                value = pl_table.lost;
                break;
            default:
                break;
            }

            top_pl_table.Push(pl, s_is_ascending ? -value : value);
        }

        for (int i = 0; i < top_pl_table.Keys.size(); ++i) {
            auto pl = top_pl_table.Keys[i];
            if (!pl) {
                break;
            }

            ImGui::Text(pl->name); ImGui::NextColumn();

            auto pl_table = pl->GetTable(s_round);
            ImGui::Text("%d", pl_table.points); ImGui::NextColumn();
            ImGui::Text("%d", pl_table.won); ImGui::NextColumn();
            ImGui::Text("%d", pl_table.drawn); ImGui::NextColumn();
            ImGui::Text("%d", pl_table.lost); ImGui::NextColumn();
            ImGui::Text("%d", pl_table.goals_for); ImGui::NextColumn();
            ImGui::Text("%d", pl_table.goals_against); ImGui::NextColumn();
        }
        
        ImGui::Columns(1);
    }
    ImGui::End();
}