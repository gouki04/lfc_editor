#pragma once
#include "imgui.h"
#include "data\database.hpp"
#include "utility.hpp"

static void ShowEditPlayerWindow(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Edit Player", p_open))
    {
        // left
        ImGui::BeginChild("player pane", ImVec2(300, 0), true);
        ImGui::Columns(4);
        // header
        ImGui::Separator();
        ImGui::Text("id"); ImGui::NextColumn();
        ImGui::Text("name"); ImGui::NextColumn();
        ImGui::Text("no"); ImGui::NextColumn();
        ImGui::Text("position"); ImGui::NextColumn();
        ImGui::Separator();

        for (size_t i = 0; i < global_db.players.size(); i++) {
            auto player = global_db.players[i];

            char label[32];
            sprintf(label, "%04d", player->id);
            if (ImGui::Selectable(label, global_db.selected_player == player, ImGuiSelectableFlags_SpanAllColumns)) {
                global_db.selected_player = player;
            }
            ImGui::NextColumn();

            ImGui::Text(player->name.c_str()); ImGui::NextColumn();
            if (player->detail) {
                ImGui::Text("%d", player->detail->no); ImGui::NextColumn();
                ImGui::Text(utility::ToString(player->detail->position)); ImGui::NextColumn();
            }
            else {
                ImGui::NextColumn();
                ImGui::NextColumn();
            }
        }
        ImGui::Columns(1);
        ImGui::PushItemWidth(-1);
        if (ImGui::Button("+ player", ImVec2(-1, 0))) {
            auto player = std::make_shared<Player>();
            player->id = global_db.MakePlayerId();
            global_db.players.push_back(player);

            global_db.selected_player = player;
        }
        ImGui::PopItemWidth();
        ImGui::EndChild();
        ImGui::SameLine();

        // right
        auto player = global_db.selected_player;
        
        ImGui::BeginChild("player view", ImVec2(300, 0)); // Leave room for 1 line below us

        if (player) {
            ImGui::LabelText("id", "%d", player->id);

            char buf[128];
            sprintf(buf, player->name.c_str());
            if (ImGui::InputText("name", buf, 128)) {
                player->name = std::string(buf);
            }

            sprintf(buf, player->en_name.c_str());
            if (ImGui::InputText("en name", buf, 128)) {
                player->en_name = std::string(buf);
            }

            ImGui::Separator();

            ImGui::Text("detail:");
            if (player->detail) {
                ImGui::InputInt("no", &player->detail->no);
                ImGui::EPositionEdit("position", &player->detail->position);
                ImGui::DateEdit("born", &player->detail->born_time);
                ImGui::DateEdit("join", &player->detail->join_time);
                ImGui::DateEdit("leave", &player->detail->leave_time);
            }
            else {
                if (ImGui::Button("+")) {
                    player->detail = std::make_shared<PlayerDetailInfo>();
                }
            }

            int goal_cnt = 0;
            int assist_cnt = 0;
            for (size_t i = 0; i < global_db.goals.size(); ++i) {
                auto goal = global_db.goals[i];
                if (goal) {
                    if (goal->score_player == player) {
                        ++goal_cnt;
                    }
                    if (goal->assist_player == player) {
                        ++assist_cnt;
                    }
                }
            }

            ImGui::LabelText("goals", "%d", goal_cnt);
            ImGui::LabelText("assists", "%d", assist_cnt);
        }

        ImGui::EndChild();
    }
    ImGui::End();
}