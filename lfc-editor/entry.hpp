#pragma once
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/binary.hpp>
#include <fstream>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include "data/database.hpp"
#include "utility.hpp"
#include "window/EditPlayerWindow.hpp"
#include "window/EditEventWindow.hpp"
#include "window/AnalyzeMatchWindow.hpp"
#include "Localization.hpp"

namespace cereal
{
    //void load(JSONInputArchive &ar, std::shared_ptr<Goal> goal)
    //{
    //    ar(make_nvp("id", goal->id), make_nvp("match", goal->match), make_nvp("score_player", goal->score_player), 
    //        make_nvp("assist_player", goal->assist_player), make_nvp("type", goal->type), make_nvp("time", goal->time));
    //}

    //void save(JSONOutputArchive &ar, std::shared_ptr<Goal> goal)
    //{
    //    ar(make_nvp("id", goal->id), make_nvp("match", goal->match), make_nvp("score_player", goal->score_player),
    //        make_nvp("assist_player", goal->assist_player), make_nvp("type", goal->type), make_nvp("time", goal->time),
    //        make_nvp("for_lfc", goal->for_lfc));
    //}
}

using namespace lfc;

void LoadDBJson()
{
    std::ifstream is("data.json");
    cereal::JSONInputArchive archive(is);

    archive(global_db);
}

void SaveDBJson()
{
    std::ofstream os("data.json");
    cereal::JSONOutputArchive archive(os);

    archive(global_db);
}

void LoadDBBinary()
{
    std::ifstream is("data.bin");
    cereal::BinaryInputArchive archive(is);

    archive(global_db);
}

void SaveDBBinary()
{
    std::ofstream os("data.bin");
    cereal::BinaryOutputArchive archive(os);

    archive(global_db);
}

void LoadDBXml()
{
    std::ifstream is("data.xml");
    cereal::XMLInputArchive archive(is);

    archive(global_db);
}

void SaveDBXml()
{
    std::ofstream os("data.xml");
    cereal::XMLOutputArchive archive(os);

    archive(global_db);
}

void TestDB()
{
    //std::ofstream ss("data.json");
    //cereal::JSONOutputArchive oarchive(ss);

    //auto p1 = std::make_shared<Player>();
    //p1->id = 0;
    //p1->name = "gouki";
    //p1->detail = nullptr;

    //auto p2 = std::make_shared<Player>();
    //p2->id = 1;
    //p2->name = "dfjakd";
    //p2->detail = std::make_shared<PlayerDetailInfo>();
    //p2->detail->no = 5;
    //p2->detail->position = EPosition::Forward;
    //p2->detail->born_time.Set(2004, 12, 8);
    //p2->detail->join_time = Date::Now();

    //global_db.players.push_back(p1);
    //global_db.players.push_back(p2);

    //auto club = std::make_shared<FootballClub>();
    //club->id = 0;
    //club->name = "liverpool";

    //auto club2 = std::make_shared<FootballClub>();
    //club2->id = 1;
    //club2->name = "kjjfdkk";

    //global_db.LFC = club;
    //global_db.clubs.push_back(club);
    //global_db.clubs.push_back(club2);

    //auto goal1 = std::make_shared<Goal>();
    //goal1->id = 0;
    //goal1->score_player = p1;
    //goal1->time.local_time = 44;

    //auto goal2 = std::make_shared<Goal>();
    //goal2->id = 1;
    //goal2->score_player = p1;
    //goal2->time.local_time = 90;
    //goal2->time.injury_time = 3;

    //global_db.goals.push_back(goal1);
    //global_db.goals.push_back(goal2);

    //auto match = std::make_shared<Match>();
    //match->id = 0;
    //match->opponent_team = club2;
    //match->time = Date::Now();
    //match->side = EMatchSide::Away;
    //match->lfc_goals.push_back(goal1);
    //match->lfc_goals.push_back(goal2);

    //goal1->match = match;
    //goal2->match = match;

    //global_db.matchs.push_back(match);

    //auto evt = std::make_shared<FriendlyMatch>();
    //evt->id = 0;
    //evt->name = "FriendlyMatch";
    //evt->event_type = EEventType::FriendlyMatch;
    //evt->begin_time.Set(1977, 9, 7);
    //evt->match = match;

    //auto pl = std::make_shared<PremierLeague>();
    //pl->id = 1;
    //pl->name = "PremierLeague";
    //pl->event_type = EEventType::PremierLeague;
    //pl->rounds[4] = match;

    //auto cl = std::make_shared<UEFAChampionsLeague>();
    //cl->id = 2;
    //cl->name = "UEFAChampionsLeague";
    //cl->event_type = EEventType::UEFAChampionsLeague;
    //cl->end_time.Set(1997, 9, 27);
    //cl->play_off.first = match;
    //cl->group_state.matches[2] = match;
    //cl->round_of_16.second = match;

    //global_db.events.push_back(evt);
    //global_db.events.push_back(pl);
    //global_db.events.push_back(cl);

    //oarchive(global_db);
}

void Entry()
{
    {
        std::ifstream is("localization.json");
        cereal::JSONInputArchive archive(is);

        archive(global_localization);
    }

    {
        //TestDB();
        LoadDBJson();

        //auto friendly_17 = std::make_shared<FriendlyMatch>();
        //auto friendly_16 = std::make_shared<FriendlyMatch>();

        //friendly_17->event_type = EEventType::FriendlyMatch;
        //friendly_16->event_type = EEventType::FriendlyMatch;

        //std::vector<std::shared_ptr<Event>> new_event_list;
        //new_event_list.push_back(friendly_17);
        //new_event_list.push_back(friendly_16);

  //      for (auto evt : global_db.events) {
        //	if (evt->event_type == EEventType::FriendlyMatch) {
        //		auto fm = std::static_pointer_cast<FriendlyMatch>(evt);
        //		if (fm->begin_time.year >= 2017) {
        //			friendly_17->matchs.push_back(fm->matchs[0]);
        //		}
        //		else {
        //			friendly_16->matchs.push_back(fm->matchs[0]);
        //		}
        //	}
        //	else {
        //		new_event_list.push_back(evt);
        //	}
  //      }

        //global_db.events = new_event_list;
        //for (int i = 0; i < global_db.events.size(); ++i) {
        //	auto evt = global_db.events[i];
        //	evt->id = i;
        //}
    }
}

bool draw_gui(bool *show_test_window)
{
    static bool s_show_edit_player = false;
    static bool s_show_edit_event = false;
    static bool s_show_edit_club = false;
    static bool s_show_edit_goal = false;
    static bool s_show_edit_match = false;
    static bool s_show_analyze_match = false;

    bool ret = false;
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::BeginMenu("Open")) {
                if (ImGui::MenuItem("Json")) {
                    LoadDBJson();
                }
                if (ImGui::MenuItem("Xml")) {
                    LoadDBXml();
                }
                if (ImGui::MenuItem("Binary")) {
                    LoadDBBinary();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Save")) {
                if (ImGui::MenuItem("Json")) {
                    SaveDBJson();
                }
                if (ImGui::MenuItem("Xml")) {
                    SaveDBXml();
                }
                if (ImGui::MenuItem("Binary")) {
                    SaveDBBinary();
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Clear")) {
                // TODO clear global_db
            }
            if (ImGui::MenuItem("Quit")) {
                ret = true;
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Player")) {
                s_show_edit_player = true;
            }
            if (ImGui::MenuItem("Club")) {
                s_show_edit_club = true;
            }
            if (ImGui::MenuItem("Event")) {
                s_show_edit_event = true;
            }
            if (ImGui::MenuItem("Goal")) {
                s_show_edit_goal = true;
            }
            if (ImGui::MenuItem("Match")) {
                s_show_edit_match = true;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Sort"))
        {
            if (ImGui::MenuItem("Player")) {
                global_db.SortPlayers();
            }
            if (ImGui::MenuItem("Club")) {
                global_db.SortClubs();
            }
            if (ImGui::MenuItem("Event")) {
                global_db.SortEvents();
            }
            if (ImGui::MenuItem("Player In Club")) {
                global_db.SortPlayersInClubs();
            }
            if (ImGui::MenuItem("Match")) {
                global_db.SortMatchs();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Analyze"))
        {
            if (ImGui::MenuItem("Match")) {
                s_show_analyze_match = true;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Demo")) {
                *show_test_window = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (s_show_edit_player) {
        ShowEditPlayerWindow(&s_show_edit_player);
    }

    if (s_show_edit_event) {
        ShowEventWindow(&s_show_edit_event);
    }

    if (s_show_edit_club) {
        ShowClubWindow(&s_show_edit_club);
    }

    if (s_show_edit_goal) {
        ShowGoalWindow(&s_show_edit_goal);
    }

    if (s_show_edit_match) {
        ShowMatchWindow(&s_show_edit_match);
    }

    if (s_show_analyze_match) {
        ShowAnalyzeMatchWindow(&s_show_analyze_match);
    }

    ShowEditMatchWindow();
    ShowEditGoalWindow();

    return ret;
}