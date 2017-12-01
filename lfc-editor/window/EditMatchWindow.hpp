#pragma once
#include "imgui.h"
#include <cereal\types\vector.hpp>
#include "data\database.hpp"
#include "utility.hpp"

static bool ShowGoalList(const char *label, std::shared_ptr<Match> match, std::vector<std::shared_ptr<Goal>> &goal_vec, bool for_lfc)
{
    auto ret = false;
    ImGui::Text(label);
    ImGui::PushID(&goal_vec);
    for (size_t i = 0; i < goal_vec.size(); ++i) {
        ImGui::PushID(i);
        ImGui::GoalButton("", &goal_vec[i]);
        ImGui::PopID();
    }

    if (ImGui::Button(utf8("+ goal"))) {
        auto goal = std::make_shared<Goal>();
        goal->id = global_db.MakeGoalId();
        goal->match = match;
        goal->for_lfc = for_lfc;

        global_db.goals.push_back(goal);
        goal_vec.push_back(goal);

        global_db.selected_goal = goal;
        ret = true;
    }
    ImGui::PopID();

    return false;
}

static void ShowEditMatchWindow()
{
    if (!global_db.selected_match) {
        return;
    }

    auto match = global_db.selected_match;
    ImGui::SetNextWindowSize(ImVec2(600, 440), ImGuiCond_FirstUseEver);

    bool open = true;
    if (ImGui::Begin("Edit Match", &open)) {
        ImGui::Text(utf8("id"), match->id);

        ImGui::DateEdit(utf8("match time"), &match->time);
        ImGui::ESideEdit(utf8("side"), &match->side);
        ImGui::ClubCombo(utf8("opponent team"), &match->opponent_team);

        ImGui::Separator();
        ShowGoalList(utf8("lfc goals"), match, match->lfc_goals, true);

        ImGui::Separator();
        ShowGoalList(utf8("opponent goals"), match, match->opponent_goals, false);

        ImGui::Separator();
        ShowGoalList(utf8("lfc penalty shootout goals"), match, match->lfc_penalty_shootout_goals, true);

        ImGui::Separator();
        ShowGoalList(utf8("opponent penalty shootout goals"), match, match->opponent_penalty_shootout_goals, false);
    }

    if (!open) {
        global_db.selected_match = nullptr;
    }
    ImGui::End();
}
