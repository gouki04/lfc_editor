#pragma once
#include "imgui.h"
#include <cereal\types\vector.hpp>
#include "data\database.hpp"
#include "utility.hpp"
#include "Localization.hpp"
#include "data\PremierLeagueAsiaTrophy.hpp"
#include "data\AudiCup.hpp"
#include "data\EFLCup.hpp"
#include "data\FACup.hpp"
#include "data\InternationalChampionsCup.hpp"
#include "data\UEFAEuropaLeague.hpp"
#include "data\FriendlyMatch.hpp"
#include "data\PremierLeague.hpp"

static void ShowEventListWindow(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(700, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Edit Event", p_open))
    {
        // left
        static int selected = 0;
        ImGui::BeginChild("event pane", ImVec2(300, 0), true);
		ImGui::ColumnHeaders(4, 0.15f, 0.35f, 0.25f, 0.25f);

        // header
        ImGui::Separator();
        ImGui::Text(utf8("id")); ImGui::NextColumn();
        ImGui::Text(utf8("name")); ImGui::NextColumn();
        ImGui::Text(utf8("begin")); ImGui::NextColumn();
        ImGui::Text(utf8("end")); ImGui::NextColumn();
        ImGui::Separator();

        for (size_t i = 0; i < global_db.events.size(); i++) {
            auto event = global_db.events[i];

            char label[32];
            sprintf(label, "%04d", event->id);
            if (ImGui::Selectable(label, selected == (int)i, ImGuiSelectableFlags_SpanAllColumns)) {
                selected = i;
            }
            ImGui::NextColumn();

            ImGui::Text(event->name.c_str()); ImGui::NextColumn();
            ImGui::Text(utility::ToString(event->begin_time)); ImGui::NextColumn();
            ImGui::Text(utility::ToString(event->end_time)); ImGui::NextColumn();
        }
        ImGui::Columns(1);
        ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() * 0.5f);
        static EEventType evt_type = EEventType::FriendlyMatch;
        ImGui::EEventEdit("", &evt_type);
        ImGui::SameLine();
        if (ImGui::Button(utf8("+ event"), ImVec2(-1, 0))) {
            std::shared_ptr<Event> evt = nullptr;
            switch (evt_type)
            {
            case lfc::EEventType::FriendlyMatch:
                evt = std::make_shared<FriendlyMatch>();
                break;
            case lfc::EEventType::PremierLeague:
                evt = std::make_shared<PremierLeague>();
                break;
            case lfc::EEventType::UEFAChampionsLeague:
                evt = std::make_shared<UEFAChampionsLeague>();
                break;
            case lfc::EEventType::PremierLeagueAsiaTrophy:
                evt = std::make_shared<PremierLeagueAsiaTrophy>();
                break;
            case lfc::EEventType::AudiCup:
                evt = std::make_shared<AudiCup>();
                break;
            case lfc::EEventType::EFLCup:
                evt = std::make_shared<EFLCup>();
                break;
            case lfc::EEventType::FACup:
                evt = std::make_shared<FACup>();
                break;
            case lfc::EEventType::InternationalChampionsCup:
                evt = std::make_shared<InternationalChampionsCup>();
                break;
            case lfc::EEventType::UEFAEuropaLeague:
                evt = std::make_shared<UEFAEuropaLeague>();
                break;
            default:
                break;
            }

            if (evt != nullptr) {
                evt->id = global_db.MakeEventId();
                evt->event_type = evt_type;
                global_db.events.push_back(evt);
            }
        }
        ImGui::PopItemWidth();
        ImGui::EndChild();
        ImGui::SameLine();

        // right
        auto event = global_db.events[selected];
        ImGui::BeginChild("event view", ImVec2(300, 0)); // Leave room for 1 line below us
        ImGui::LabelText(utf8("id"), "%d", event->id);

        char buf[128];
        sprintf(buf, event->name.c_str());
        if (ImGui::InputText(utf8("name"), buf, 128)) {
            event->name = std::string(buf);
        }

        ImGui::Separator();

        ImGui::DateEdit(utf8("begin"), &event->begin_time);
        ImGui::DateEdit(utf8("end"), &event->end_time);

        switch (event->event_type)
        {
        case lfc::EEventType::FriendlyMatch: {
            auto fm = std::static_pointer_cast<FriendlyMatch>(event);
            ImGui::PushID(fm.get());
            for (int i = 0; i < fm->matchs.size(); ++i) {
                ImGui::PushID(i);
                ImGui::MatchButton(utf8("match"), &fm->matchs[i]);
                ImGui::PopID();
            }

            static std::shared_ptr<Match> s_add_friendly_match = nullptr;
            ImGui::PushID(fm->matchs.size());
            if (ImGui::MatchButton(utf8("match"), &s_add_friendly_match)) {
                fm->matchs.push_back(s_add_friendly_match);
                s_add_friendly_match = nullptr;
            }
            ImGui::PopID();

            ImGui::PopID();
            break;
        }
        case lfc::EEventType::PremierLeague: {
            auto pl = std::static_pointer_cast<PremierLeague>(event);
            ImGui::PushID(pl.get());

            ImGui::ColumnHeaders(5, 0.2, 0.2, 0.2, 0.2, 0.2);
            // header
            ImGui::Separator();
            ImGui::Text(utf8("pl table position")); ImGui::NextColumn();
            ImGui::Text(utf8("pl table played")); ImGui::NextColumn();
            ImGui::Text(utf8("pl table won/drawn/lost")); ImGui::NextColumn();
            ImGui::Text(utf8("pl table goals for/against")); ImGui::NextColumn();
            ImGui::Text(utf8("pl table points")); ImGui::NextColumn();
            ImGui::Separator();

            auto table = pl->GetTable();
            ImGui::DragInt("", &pl->position, 1, 1, 38); ImGui::NextColumn();
            ImGui::Text("%d", table.played); ImGui::NextColumn();
            ImGui::Text("%d/%d/%d", table.won, table.drawn, table.lost); ImGui::NextColumn();
            ImGui::Text("%d/%d", table.goals_for, table.goals_against); ImGui::NextColumn();
            ImGui::Text("%d", table.points); ImGui::NextColumn();
            ImGui::Separator();
            ImGui::Columns(1);

            char label[32];
            for (size_t i = 0; i < pl->rounds.size(); ++i) {
                ImGui::PushID(i);
                sprintf(label, utf8("round %d"), i + 1);
                ImGui::MatchButton(label, &pl->rounds[i]);
                ImGui::PopID();
            }
            ImGui::PopID();
            break;
        }
        case lfc::EEventType::UEFAChampionsLeague: {
            auto cl = std::static_pointer_cast<UEFAChampionsLeague>(event);
            ImGui::PushID(cl.get());
            
            {
                ImGui::TwoMatchTitle(utf8("play off"), cl->play_off);
                ImGui::PushID(&cl->play_off);
                ImGui::PushID(1); ImGui::MatchButton(utf8("first"), &(cl->play_off.first)); ImGui::PopID();
                ImGui::PushID(2); ImGui::MatchButton(utf8("second"), &(cl->play_off.second)); ImGui::PopID();
                ImGui::PopID();
            }

            ImGui::Separator();

            {
                ImGui::Text(utf8("group state"));
                ImGui::PushID(&cl->group_state);
                char label[32];
                for (size_t i = 0; i < cl->group_state.matches.size(); ++i) {
                    ImGui::PushID(i);
                    sprintf(label, utf8("round %d"), i + 1);
                    ImGui::MatchButton(label, &(cl->group_state.matches[i]));
                    ImGui::PopID();
                }
                ImGui::PopID();
            }

            ImGui::Separator();

            {
                ImGui::Text(utf8("round of 16"));
                ImGui::PushID(&cl->round_of_16);
                ImGui::PushID(1); ImGui::MatchButton(utf8("first"), &(cl->round_of_16.first)); ImGui::PopID();
                ImGui::PushID(2); ImGui::MatchButton(utf8("second"), &(cl->round_of_16.second)); ImGui::PopID();
                ImGui::PopID();
            }

            ImGui::Separator();

            {
                ImGui::Text(utf8("quarter final"));
                ImGui::PushID(&cl->quarter_final);
                ImGui::PushID(1); ImGui::MatchButton(utf8("first"), &(cl->quarter_final.first)); ImGui::PopID();
                ImGui::PushID(2); ImGui::MatchButton(utf8("second"), &(cl->quarter_final.second)); ImGui::PopID();
                ImGui::PopID();
            }

            ImGui::Separator();

            {
                ImGui::Text(utf8("semi final"));
                ImGui::PushID(&cl->semi_final);
                ImGui::PushID(1); ImGui::MatchButton(utf8("first"), &(cl->semi_final.first)); ImGui::PopID();
                ImGui::PushID(2); ImGui::MatchButton(utf8("second"), &(cl->semi_final.second)); ImGui::PopID();
                ImGui::PopID();
            }

            ImGui::Separator();

            {
                ImGui::Text(utf8("final"));
                ImGui::PushID(&cl->final);
                ImGui::MatchButton(utf8("match"), &(cl->final));
                ImGui::PopID();
            }

            ImGui::PopID();
            break;
        }
        case lfc::EEventType::UEFAEuropaLeague: {
            auto el = std::static_pointer_cast<UEFAEuropaLeague>(event);
            ImGui::PushID(el.get());

            {
                ImGui::TwoMatchTitle(utf8("qualifying"), el->qualifying);
                ImGui::PushID(&el->qualifying);
                ImGui::PushID(1); ImGui::MatchButton(utf8("first"), &(el->qualifying.first)); ImGui::PopID();
                ImGui::PushID(2); ImGui::MatchButton(utf8("second"), &(el->qualifying.second)); ImGui::PopID();
                ImGui::PopID();
            }

            ImGui::Separator();

            {
                ImGui::TwoMatchTitle(utf8("play off"), el->play_off);
                ImGui::PushID(&el->play_off);
                ImGui::PushID(1); ImGui::MatchButton(utf8("first"), &(el->play_off.first)); ImGui::PopID();
                ImGui::PushID(2); ImGui::MatchButton(utf8("second"), &(el->play_off.second)); ImGui::PopID();
                ImGui::PopID();
            }

            ImGui::Separator();

            {
                ImGui::Text(utf8("group state"));
                ImGui::PushID(&el->group_state);
                char label[32];
                for (size_t i = 0; i < el->group_state.matches.size(); ++i) {
                    ImGui::PushID(i);
                    sprintf(label, utf8("round %d"), i + 1);
                    ImGui::MatchButton(label, &(el->group_state.matches[i]));
                    ImGui::PopID();
                }
                ImGui::PopID();
            }

            ImGui::Separator();

            {
                ImGui::Text(utf8("round of 32"));
                ImGui::PushID(&el->round_of_32);
                ImGui::PushID(1); ImGui::MatchButton(utf8("first"), &(el->round_of_32.first)); ImGui::PopID();
                ImGui::PushID(2); ImGui::MatchButton(utf8("second"), &(el->round_of_32.second)); ImGui::PopID();
                ImGui::PopID();
            }

            ImGui::Separator();

            {
                ImGui::Text(utf8("round of 16"));
                ImGui::PushID(&el->round_of_16);
                ImGui::PushID(1); ImGui::MatchButton(utf8("first"), &(el->round_of_16.first)); ImGui::PopID();
                ImGui::PushID(2); ImGui::MatchButton(utf8("second"), &(el->round_of_16.second)); ImGui::PopID();
                ImGui::PopID();
            }

            ImGui::Separator();

            {
                ImGui::Text(utf8("quarter final"));
                ImGui::PushID(&el->quarter_final);
                ImGui::PushID(1); ImGui::MatchButton(utf8("first"), &(el->quarter_final.first)); ImGui::PopID();
                ImGui::PushID(2); ImGui::MatchButton(utf8("second"), &(el->quarter_final.second)); ImGui::PopID();
                ImGui::PopID();
            }

            ImGui::Separator();

            {
                ImGui::Text(utf8("semi final"));
                ImGui::PushID(&el->semi_final);
                ImGui::PushID(1); ImGui::MatchButton(utf8("first"), &(el->semi_final.first)); ImGui::PopID();
                ImGui::PushID(2); ImGui::MatchButton(utf8("second"), &(el->semi_final.second)); ImGui::PopID();
                ImGui::PopID();
            }

            ImGui::Separator();

            {
                ImGui::Text(utf8("final"));
                ImGui::PushID(&el->final);
                ImGui::MatchButton(utf8("match"), &(el->final));
                ImGui::PopID();
            }

            ImGui::PopID();
            break;
        }
        case lfc::EEventType::PremierLeagueAsiaTrophy: {
            auto plat = std::static_pointer_cast<PremierLeagueAsiaTrophy>(event);
            ImGui::PushID(plat.get());

            ImGui::PushID(1); ImGui::MatchButton(utf8("first"), &(plat->first)); ImGui::PopID();
            ImGui::PushID(2); ImGui::MatchButton(utf8("second"), &(plat->second)); ImGui::PopID();

            ImGui::PopID();
            break;
        }
        case lfc::EEventType::AudiCup: {
            auto audi = std::static_pointer_cast<AudiCup>(event);
            ImGui::PushID(audi.get());

            ImGui::PushID(1); ImGui::MatchButton(utf8("first"), &(audi->first)); ImGui::PopID();
            ImGui::PushID(2); ImGui::MatchButton(utf8("second"), &(audi->second)); ImGui::PopID();

            ImGui::PopID();
            break;
        }
        case lfc::EEventType::EFLCup: {
            auto efl = std::static_pointer_cast<EFLCup>(event);
            ImGui::PushID(efl.get());

            ImGui::PushID(1); ImGui::MatchButton(utf8("round 2"), &(efl->round_2)); ImGui::PopID();
            ImGui::PushID(2); ImGui::MatchButton(utf8("top of 32"), &(efl->top_of_32)); ImGui::PopID();
            ImGui::PushID(3); ImGui::MatchButton(utf8("top of 16"), &(efl->top_of_16)); ImGui::PopID();
            ImGui::PushID(4); ImGui::MatchButton(utf8("quarter final"), &(efl->quarter_final)); ImGui::PopID();

            ImGui::PushID(5); ImGui::MatchButton(utf8("semi final first"), &(efl->semi_final.first)); ImGui::PopID();
            ImGui::PushID(6); ImGui::MatchButton(utf8("semi final second"), &(efl->semi_final.second)); ImGui::PopID();

            ImGui::PushID(7); ImGui::MatchButton(utf8("final"), &(efl->final)); ImGui::PopID();

            ImGui::Separator();

            ImGui::PopID();
            break;
        }
        case lfc::EEventType::FACup: {
            auto fa = std::static_pointer_cast<FACup>(event);
            ImGui::PushID(fa.get());

            ImGui::PushID(1); ImGui::ReplayMatchButton(utf8("top of 64"), &(fa->top_of_64)); ImGui::PopID();
            ImGui::PushID(2); ImGui::ReplayMatchButton(utf8("top of 32"), &(fa->top_of_32)); ImGui::PopID();
            ImGui::PushID(3); ImGui::ReplayMatchButton(utf8("top of 16"), &(fa->top_of_16)); ImGui::PopID();
            ImGui::PushID(4); ImGui::ReplayMatchButton(utf8("quarter final"), &(fa->quarter_final)); ImGui::PopID();

            ImGui::PushID(5); ImGui::MatchButton(utf8("semi final"), &(fa->semi_final)); ImGui::PopID();
            ImGui::PushID(6); ImGui::MatchButton(utf8("final"), &(fa->final)); ImGui::PopID();

            ImGui::PopID();
            break;
        }
        case lfc::EEventType::InternationalChampionsCup: {
            auto icc = std::static_pointer_cast<InternationalChampionsCup>(event);
            ImGui::PushID(icc.get());

            ImGui::Checkbox(utf8("one goal one point"), &icc->is_one_goal_one_point);

            ImGui::Columns(5);
            // header
            ImGui::Separator();
            ImGui::Text(utf8("pl table position")); ImGui::NextColumn();
            ImGui::Text(utf8("pl table played")); ImGui::NextColumn();
            ImGui::Text(utf8("pl table won/drawn/lost")); ImGui::NextColumn();
            ImGui::Text(utf8("pl table goals for/against")); ImGui::NextColumn();
            ImGui::Text(utf8("pl table points")); ImGui::NextColumn();
            ImGui::Separator();

            auto table = icc->GetTable();
            ImGui::Checkbox("", &icc->is_champion); ImGui::NextColumn();
            ImGui::Text("%d", table.played); ImGui::NextColumn();
            ImGui::Text("%d/%d/%d", table.won, table.drawn, table.lost); ImGui::NextColumn();
            ImGui::Text("%d/%d", table.goals_for, table.goals_against); ImGui::NextColumn();
            ImGui::Text("%d", table.points); ImGui::NextColumn();
            ImGui::Separator();
            ImGui::Columns(1);

            char label[32];
            for (size_t i = 0; i < icc->rounds.size(); ++i) {
                ImGui::PushID(i);
                sprintf(label, utf8("round %d"), i + 1);
                ImGui::MatchButton(label, &icc->rounds[i]);
                ImGui::PopID();
            }

            ImGui::PushID("final");
            ImGui::MatchButton(utf8("fianl"), &icc->final);
            ImGui::PopID();

            ImGui::PopID();
            break;
        }
        default:
            break;
        }

        ImGui::EndChild();
    }
    ImGui::End();
}