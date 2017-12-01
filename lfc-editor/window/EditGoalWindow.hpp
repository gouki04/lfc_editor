#pragma once
#include "imgui.h"
#include <cereal\types\vector.hpp>
#include "data\database.hpp"
#include "utility.hpp"

static void ShowEditGoalWindow()
{
    if (!global_db.selected_goal) {
        return;
    }

    auto goal = global_db.selected_goal;
    ImGui::SetNextWindowSize(ImVec2(650, 440), ImGuiCond_FirstUseEver);
    bool open = true;
    if (ImGui::Begin("Show Goal", &open)) {
        ImGui::Text(utf8("id"), goal->id);
        ImGui::LabelText(utf8("match"), utility::ToString(goal->match));
        ImGui::EGoalTypeEdit(utf8("goal type"), &goal->type);

        auto filter_players = &global_db.players;
        if (global_db.selected_goal && global_db.selected_match) {
            auto is_lfc_goal = global_db.selected_match->IsLfcGoal(goal);
            if (is_lfc_goal) {
                if (goal->type == EGoalType::OwnGoal) {
                    filter_players = &global_db.selected_match->opponent_team->players;
                }
                else {
                    filter_players = &global_db.LFC->players;
                }
            }
            else {
                if (goal->type == EGoalType::OwnGoal) {
                    filter_players = &global_db.LFC->players;
                }
                else {
                    filter_players = &global_db.selected_match->opponent_team->players;
                }
            }
        }

        ImGui::PlayerCombo(utf8("score player"), &goal->score_player, filter_players);
        ImGui::PlayerCombo(utf8("assist player"), &goal->assist_player, filter_players);

        ImGui::MatchTimeEdit(utf8("match time"), goal->time);
        ImGui::Checkbox(utf8("for lfc"), &goal->for_lfc);
    }

    if (!open) {
        global_db.selected_goal = nullptr;
    }
    ImGui::End();
}
