#pragma once
#include "imgui.h"
#include "data\database.hpp"
#include "utility.hpp"

static void ShowDateRangeListWindow(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Edit Date Range", p_open))
    {
        // left
        ImGui::BeginChild("Date range pane", ImVec2(300, 0), true);

        ImGui::ColumnHeaders(3, 0.4, 0.3, 0.3);

        static std::shared_ptr<DateRange> selected = nullptr;

        // header
        ImGui::Separator();
        ImGui::Text("name"); ImGui::NextColumn();
        ImGui::Text("begin"); ImGui::NextColumn();
        ImGui::Text("end"); ImGui::NextColumn();
        ImGui::Separator();

        for (size_t i = 0; i < global_db.date_ranges.size(); i++) {
            auto date_range = global_db.date_ranges[i];

            if (ImGui::Selectable(date_range->name.c_str(), selected == date_range, ImGuiSelectableFlags_SpanAllColumns)) {
                selected = date_range;
            }
            ImGui::NextColumn();

            ImGui::Text(utility::ToString(date_range->begin)); ImGui::NextColumn();
            ImGui::Text(utility::ToString(date_range->end)); ImGui::NextColumn();
        }
        ImGui::Columns(1);
        ImGui::PushItemWidth(-1);
        if (ImGui::Button("+ date range", ImVec2(-1, 0))) {
            auto date_range = std::make_shared<DateRange>();
            global_db.date_ranges.push_back(date_range);
        }
        ImGui::PopItemWidth();
        ImGui::EndChild();
        ImGui::SameLine();

        ImGui::BeginChild("date range view", ImVec2(300, 0)); // Leave room for 1 line below us

        if (selected) {
            char buf[128];
            sprintf(buf, selected->name.c_str());
            if (ImGui::InputText("name", buf, 128)) {
                selected->name = std::string(buf);
            }

            ImGui::Separator();

            ImGui::DateEdit("begin", &selected->begin);
            ImGui::DateEdit("end", &selected->end);
        }

        ImGui::EndChild();
    }
    ImGui::End();
}