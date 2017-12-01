#pragma once
#include "imgui.h"
#include <cereal\types\vector.hpp>
#include "data\database.hpp"
#include "utility.hpp"

static void ShowGoalListWindow(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(700, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Edit Goal", p_open)) {
        // left
        ImGui::BeginChild("goal filter", ImVec2(200, 0), true);

        static bool s_filter_for_lfc = true;
        static std::shared_ptr<Player> s_filter_score_player = nullptr;
        static std::shared_ptr<Player> s_filter_assist_player = nullptr;
        static EGoalType s_filter_goal_type = EGoalType::Any;
        static MatchTime s_filter_min_time;
        static MatchTime s_filter_max_time;

        ImGui::Checkbox(utf8("for lfc"), &s_filter_for_lfc);
        ImGui::PlayerCombo(utf8("score player"), &s_filter_score_player);
        ImGui::PlayerCombo(utf8("assist player"), &s_filter_assist_player);
        ImGui::EGoalTypeEdit(utf8("goal type"), &s_filter_goal_type);
        ImGui::MatchTimeEdit(utf8("min time"), s_filter_min_time);
        ImGui::MatchTimeEdit(utf8("max time"), s_filter_max_time);

        ImGui::EndChild();
        ImGui::SameLine();

        static int selected = 0;
        ImGui::BeginChild("goal pane", ImVec2(500, 0), true);
        ImGui::Columns(6);

        // header
        ImGui::Separator();
        ImGui::Text("id"); ImGui::NextColumn();
        ImGui::Text("score"); ImGui::NextColumn();
        ImGui::Text("assist"); ImGui::NextColumn();
        ImGui::Text("type"); ImGui::NextColumn();
        ImGui::Text("time"); ImGui::NextColumn();
        ImGui::Text("for lfc"); ImGui::NextColumn();
        ImGui::Separator();

        for (size_t i = 0; i < global_db.goals.size(); i++) {
            auto goal = global_db.goals[i];
            if (goal->for_lfc != s_filter_for_lfc) {
                continue;
            }
            if (s_filter_score_player && goal->score_player != s_filter_score_player) {
                continue;
            }
            if (s_filter_assist_player && goal->assist_player != s_filter_assist_player) {
                continue;
            }
            if (s_filter_goal_type != EGoalType::Any && goal->type != s_filter_goal_type) {
                continue;
            }
            if (s_filter_min_time.IsValid() && goal->time < s_filter_min_time) {
                continue;
            }
            if (s_filter_max_time.IsValid() && goal->time > s_filter_max_time) {
                continue;
            }

            char label[32];
            sprintf(label, "%04d", goal->id);
            if (ImGui::Selectable(label, selected == (int)i, ImGuiSelectableFlags_SpanAllColumns)) {
                selected = i;
            }
            ImGui::NextColumn();

            ImGui::Text(utility::ToString(goal->score_player)); ImGui::NextColumn();
            ImGui::Text(utility::ToString(goal->assist_player)); ImGui::NextColumn();
            ImGui::Text(utility::ToString(goal->type)); ImGui::NextColumn();
            ImGui::Text(utility::ToString(goal->time)); ImGui::NextColumn();
            ImGui::Text(utility::ToString(goal->for_lfc)); ImGui::NextColumn();
        }
        ImGui::Columns(1);
        ImGui::EndChild();
        ImGui::SameLine();

        // right
        auto goal = global_db.goals[selected];
        ImGui::BeginChild("goal view", ImVec2(300, 0)); // Leave room for 1 line below us
        ImGui::LabelText("id", "%d", goal->id);

        ImGui::LabelText(utf8("match"), utility::ToString(goal->match));
        ImGui::EGoalTypeEdit(utf8("goal type"), &goal->type);
        ImGui::PlayerCombo(utf8("score player"), &goal->score_player);
        ImGui::PlayerCombo(utf8("assist player"), &goal->assist_player);
        ImGui::MatchTimeEdit(utf8("match time"), goal->time);
        ImGui::Checkbox(utf8("for lfc"), &goal->for_lfc);

        ImGui::Separator();

        ImGui::EndChild();
    }
    ImGui::End();
}
