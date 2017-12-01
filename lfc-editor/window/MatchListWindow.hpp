#pragma once
#include "imgui.h"
#include <cereal\types\vector.hpp>
#include "data\database.hpp"
#include "utility.hpp"

static void ShowMatchListWindow(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(700, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Show Match", p_open)) {
        // left
        ImGui::BeginChild("match filter", ImVec2(200, 0), true);

        static std::shared_ptr<FootballClub> s_filter_opponent = nullptr;
        static EMatchSide s_filter_match_side = EMatchSide::Any;
        static EMatchResult s_filter_match_result = EMatchResult::Any;
        static Date s_filter_min_time;
        static Date s_filter_max_time;

        ImGui::ClubCombo(utf8("oppenent team"), &s_filter_opponent);
        ImGui::ESideEdit(utf8("side"), &s_filter_match_side);
        ImGui::EMatchResultEdit(utf8("result"), &s_filter_match_result);
        ImGui::DateEdit(utf8("min time"), &s_filter_min_time);
        ImGui::DateEdit(utf8("max time"), &s_filter_max_time);

        ImGui::EndChild();
        ImGui::SameLine();

        static int selected = 0;
        ImGui::BeginChild("goal pane", ImVec2(500, 0), true);
        ImGui::Columns(5);

        // header
        ImGui::Separator();
        ImGui::Text("id"); ImGui::NextColumn();
        ImGui::Text("time"); ImGui::NextColumn();
        ImGui::Text("side"); ImGui::NextColumn();
        ImGui::Text("fixture"); ImGui::NextColumn();
        ImGui::Text("result"); ImGui::NextColumn();
        ImGui::Separator();

        for (size_t i = 0; i < global_db.matchs.size(); i++) {
            auto match = global_db.matchs[i];
            if (s_filter_opponent && match->opponent_team != s_filter_opponent) {
                continue;
            }
            if (s_filter_match_side != EMatchSide::Any && match->side != s_filter_match_side) {
                continue;
            }
            if (s_filter_match_result != EMatchResult::Any && match->GetResult() != s_filter_match_result) {
                continue;
            }
            if (s_filter_min_time.IsValid() && match->time < s_filter_min_time) {
                continue;
            }
            if (s_filter_max_time.IsValid() && match->time > s_filter_max_time) {
                continue;
            }

            char label[32];
            sprintf(label, "%04d", match->id);
            if (ImGui::Selectable(label, selected == (int)i, ImGuiSelectableFlags_SpanAllColumns)) {
                selected = i;
            }
            ImGui::NextColumn();

            ImGui::Text(utility::ToString(match->time)); ImGui::NextColumn();
            ImGui::Text(utility::ToString(match->side)); ImGui::NextColumn();
            ImGui::Text(utility::ToString(match)); ImGui::NextColumn();
            ImGui::Text(utility::ToString(match->GetResult())); ImGui::NextColumn();
        }
        ImGui::Columns(1);
        ImGui::EndChild();
        ImGui::SameLine();

        // right
        auto match = global_db.matchs[selected];
        ImGui::BeginChild("goal view", ImVec2(300, 0)); // Leave room for 1 line below us
        ImGui::LabelText("id", "%d", match->id);

        ImGui::LabelText(utf8("match"), utility::ToString(match));

        ImGui::Separator();

        ImGui::EndChild();
    }
    ImGui::End();
}