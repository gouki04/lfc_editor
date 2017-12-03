#pragma once
#include "imgui.h"
#include <cereal\types\vector.hpp>
#include "data\database.hpp"
#include "data\FootballClub.hpp"

using namespace lfc;

static void ShowClubListWindow(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(400, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Edit Club", p_open)) {
        // left
        char buf[128];

        static std::shared_ptr<FootballClub> selected = global_db.LFC;
        ImGui::BeginChild("club pane", ImVec2(200, 0), true);

        static std::string filter_club_name;
        sprintf(buf, filter_club_name.c_str());
        if (ImGui::InputText("filter", buf, 128)) {
            filter_club_name = std::string(buf);
        }

        ImGui::ColumnHeaders(2, 0.25, 0.75);
        // header
        ImGui::Separator();
        ImGui::Text("id"); ImGui::NextColumn();
        ImGui::Text("name"); ImGui::NextColumn();
        ImGui::Separator();

        for (size_t i = 0; i < global_db.clubs.size(); i++) {
            auto club = global_db.clubs[i];

            if (filter_club_name.size() > 0 &&
                club->name.find(filter_club_name) == std::string::npos &&
                club->en_name.find(filter_club_name) == std::string::npos) {
                continue;
            }

            char label[32];
            sprintf(label, "%04d", club->id);
            if (ImGui::Selectable(label, selected == club, ImGuiSelectableFlags_SpanAllColumns)) {
                selected = club;
            }
            ImGui::NextColumn();

            ImGui::Text(club->name.c_str()); ImGui::NextColumn();
        }
        ImGui::Columns(1);
        ImGui::PushItemWidth(-1);
        if (ImGui::Button("+ club", ImVec2(-1, 0))) {
            auto club = std::make_shared<FootballClub>();
            club->id = global_db.MakeClubId();
            global_db.clubs.push_back(club);

            selected = club;
        }
        ImGui::PopItemWidth();
        ImGui::EndChild();
        ImGui::SameLine();

        // right
        auto club = selected;
        ImGui::BeginChild("club view", ImVec2(200, 0));
        ImGui::LabelText("id", "%d", club->id);

        sprintf(buf, club->name.c_str());
        if (ImGui::InputText("name", buf, 128)) {
            club->name = std::string(buf);
        }

        sprintf(buf, club->en_name.c_str());
        if (ImGui::InputText("en name", buf, 128)) {
            club->en_name = std::string(buf);
        }

        ImGui::ColumnHeaders(3, 0.6, 0.25, 0.15);
        // header
        ImGui::Separator();
        ImGui::Text("name"); ImGui::NextColumn();
        ImGui::Text("action"); ImGui::NextColumn();
        ImGui::Text("edit"); ImGui::NextColumn();
        ImGui::Separator();

        std::shared_ptr<Player> player_to_del = nullptr;
        for (auto player : club->players) {
            ImGui::PushID(player->id);

            ImGui::Text(player->name.c_str()); ImGui::NextColumn();
            if (ImGui::Button("Del", ImVec2(-1, 0))) {
                player_to_del = player;
            }
            ImGui::NextColumn();

            if (ImGui::Button("E", ImVec2(-1, 0))) {
                global_db.selected_player = player;
            }
            ImGui::NextColumn();
            ImGui::PopID();
        }
        ImGui::Columns(1);

        if (player_to_del) {
            club->RemovePlayer(player_to_del);
        }

        ImGui::EndChild();
        ImGui::SameLine();

        // player pane
        ImGui::BeginChild("club select player view", ImVec2(200, 0), true);

        ImGui::Separator();

        static std::string filter_player_name;
        sprintf(buf, filter_player_name.c_str());
        if (ImGui::InputText("filter", buf, 128)) {
            filter_player_name = std::string(buf);
        }

        ImGui::ColumnHeaders(3, 0.55, 0.25, 0.2);
        // header
        ImGui::Separator();
        ImGui::Text("name"); ImGui::NextColumn();
        ImGui::Text("action"); ImGui::NextColumn();
        ImGui::Text("edit"); ImGui::NextColumn();
        ImGui::Separator();

        for (size_t i = 0; i < global_db.players.size(); i++) {
            auto player = global_db.players[i];

            // filter by club players
            if (club->HasPlayer(player)) {
                continue;
            }

            if (filter_player_name.size() > 0 &&
                player->name.find(filter_player_name) == std::string::npos &&
                player->en_name.find(filter_player_name) == std::string::npos) {
                continue;
            }

            ImGui::PushID(player->id);

            ImGui::Text(player->name.c_str()); ImGui::NextColumn();
            if (ImGui::Button("Add", ImVec2(-1, 0))) {
                club->players.push_back(player);
            }
            ImGui::NextColumn();

            if (ImGui::Button("E", ImVec2(-1, 0))) {
                global_db.selected_player = player;
            }
            ImGui::NextColumn();

            ImGui::PopID();
        }
        ImGui::Columns(1);

        ImGui::EndChild();
    }
    ImGui::End();
}