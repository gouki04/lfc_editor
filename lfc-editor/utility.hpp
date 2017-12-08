#pragma once
#include <iosfwd>
#include "data\Player.hpp"
#include "Localization.hpp"
#include "data\MatchTime.hpp"
#include "data\FACup.hpp"
#include "data\TwoMatch.hpp"
#include "TopNTable.hpp"

using namespace lfc;

namespace utility
{
    const char *ToString(EPosition v)
    {
        switch (v)
        {
        case EPosition::Any:
            return "Any";
        case EPosition::Forward:
            return "Forward";
        case EPosition::Midfielder:
            return "Midfielder";
        case EPosition::Defender:
            return "Defender";
        case EPosition::Goalkeeper:
            return "Goalkeeper";
        default:
            break;
        }

        return nullptr;
    }

    const char *ToString(EEventType v)
    {
        switch (v)
        {
        case EEventType::Any:
            return "Any";
        case EEventType::FriendlyMatch:
            return "FriendlyMatch";
        case EEventType::PremierLeague:
            return "PremierLeague";
        case EEventType::UEFAChampionsLeague:
            return "UEFAChampionsLeague";
        case EEventType::PremierLeagueAsiaTrophy:
            return "PremierLeagueAsiaTrophy";
        case EEventType::AudiCup:
            return "AudiCup";
        case EEventType::EFLCup:
            return "EFLCup";
        case EEventType::FACup:
            return "FACup";
        case EEventType::InternationalChampionsCup:
            return "InternationalChampionsCup";
        case EEventType::UEFAEuropaLeague:
            return "UEFAEuropaLeague";
        default:
            break;
        }

        return nullptr;
    }

    const char *ToString(EMatchSide v)
    {
        switch (v)
        {
        case EMatchSide::Any:
            return "Any";
        case EMatchSide::Home:
            return "Home";
        case EMatchSide::Away:
            return "Away";
        default:
            break;
        }

        return nullptr;
    }

    const char *ToString(EGoalType v)
    {
        switch (v)
        {
        case EGoalType::Any:
            return "Any";
        case EGoalType::Normal:
            return "Normal";
        case EGoalType::Penalty:
            return "Penalty";
        case EGoalType::PenaltyShootOut:
            return "PenaltyShootOut";
        case EGoalType::FreeKick:
            return "FreeKick";
        case EGoalType::OwnGoal:
            return "OwnGoal";
        default:
            break;
        }

        return nullptr;
    }

    const char *ToString(EMatchResult v)
    {
        switch (v)
        {
        case EMatchResult::Any:
            return "Any";
        case EMatchResult::Win:
            return "Win";
        case EMatchResult::Draw:
            return "Draw";
        case EMatchResult::Lose:
            return "Lose";
        default:
            break;
        }

        return nullptr;
    }

    std::string ToString(Date &date)
    {
        if (!date.IsValid()) {
            return "unkonw";
        }

        std::stringstream ss;
        ss << date.year << '-' << date.month << '-' << date.day;

        return ss.str();
    }

    std::string ToString(std::shared_ptr<Match> match)
    {
        if (!match) {
            return "------";
        }

        std::stringstream ss;
        if (match->side == EMatchSide::Home) {
            if (match->HasPenaltyShootout()) {
                ss << global_db.LFC->name << ' ' << match->lfc_goals.size() << " (" << match->lfc_penalty_shootout_goals.size() << ')'
                   << " v " 
                   << '(' << match->opponent_penalty_shootout_goals.size() << ") " << match->opponent_goals.size() << ' ' << match->opponent_team->name;
            }
            else {
                ss << global_db.LFC->name << ' ' << match->lfc_goals.size() << " v " << match->opponent_goals.size() << ' ' << match->opponent_team->name;
            }
        }
        else {
            if (match->HasPenaltyShootout()) {
                ss << match->opponent_team->name << ' ' << match->opponent_goals.size() << " (" << match->opponent_penalty_shootout_goals.size() << ')'
                    << " v "
                    << '(' << match->lfc_penalty_shootout_goals.size() << ") " << match->lfc_goals.size() << ' ' << global_db.LFC->name;
            }
            else {
                ss << match->opponent_team->name << ' ' << match->opponent_goals.size() << " v " << match->lfc_goals.size() << ' ' << global_db.LFC->name;
            }
        }

        return ss.str();
    }

    std::string ToString(MatchTime &match_time)
    {
        if (match_time.is_penalty_shootout) {
            return utf8("(penalty)");
        }

        std::stringstream ss;
        if (match_time.injury_time > 0) {
            ss << '(' << match_time.local_time << '+' << match_time.injury_time << ')';
        }
        else {
            ss << '(' << match_time.local_time << ')';
        }

        return ss.str();
    }

    const char *ToString(std::shared_ptr<Player> player)
    {
        if (player) {
            return player->name.c_str();
        }
        else {
            return "null";
        }
    }

    std::string ToString(std::shared_ptr<Goal> goal)
    {
        std::stringstream ss;
        ss << ToString(goal->time);

        if (goal->type == EGoalType::Penalty) {
            ss << utf8("(penalty)");
        }
        else if (goal->type == EGoalType::OwnGoal) {
            ss << utf8("(own goal)");
        }

        ss << ToString(goal->score_player);

        if (goal->assist_player) {
            ss << " (" << ToString(goal->assist_player) << ")";
        }

        return ss.str();
    }

    const char *ToString(TwoMatch &two_match)
    {
        if (two_match.second) {
            auto result = two_match.GetResult();
            if (result == EMatchResult::Win) {
                return utf8("pass");
            }
            else {
                return utf8("out");
            }
        }
        else if (two_match.first) {
            return utf8("...");
        }
        else {
            return utf8("none");
        }
    }

    const char *ToString(bool v)
    {
        return v ? "true" : "false";
    }
}

namespace ImGui
{
    static void DateEdit(const char *label, Date *date)
    {
        ImGui::DateEdit(label, &date->year, &date->month, &date->day);
    }

    static void EPositionEdit(const char *label, EPosition *val)
    {
        ImGui::Combo(label, (int*)(val), "Any\0Forward\0Midfielder\0Defender\0Goalkeeper\0\0");
    }

    static void EEventEdit(const char *label, EEventType *val)
    {
        ImGui::Combo(label, (int*)(val), "Any\0FriendlyMatch\0PremierLeague\0UEFAChampionsLeague\0PremierLeagueAsiaTrophy\0AudiCup\0EFLCup\0FACup\0InternationalChampionsCup\0UEFAEuropaLeague\0\0");
    }

    static void ESideEdit(const char *label, EMatchSide *val)
    {
        ImGui::Combo(label, (int*)(val), "Any\0Home\0Away\0\0");
    }

    static void EMatchResultEdit(const char *label, EMatchResult *val)
    {
        ImGui::Combo(label, (int*)(val), "Any\0Win\0Draw\0Lose\0\0");
    }

    static void EGoalTypeEdit(const char *label, EGoalType *val)
    {
        ImGui::Combo(label, (int*)(val), "Normal\0Penalty\0PenaltyShootOut\0FreeKick\0OwnGoal\0\0");
    }

    static bool club_getter(void*, int idx, const char** out_text)
    {
        if (idx == 0) {
            *out_text = "null";
        }
        else {
            *out_text = global_db.clubs[idx -1]->name.c_str();
        }
        return true;
    }

    static int get_club_index(std::vector<std::shared_ptr<FootballClub>> &clubs, std::shared_ptr<FootballClub> club)
    {
        if (club == nullptr) {
            return 0;
        }

        auto itr = std::find(clubs.begin(), clubs.end(), club);
        if (itr == clubs.end()) {
            return 0;
        }

        return itr - clubs.begin() + 1;
    }

    static void ClubCombo(const char *label, std::shared_ptr<FootballClub> *p_club)
    {
        auto clubs = &global_db.clubs;
        auto club = *p_club;
        auto index = get_club_index(*clubs, club);
        if (ImGui::Combo(label, &index, club_getter, nullptr, global_db.clubs.size() + 1)) {
            if (index == 0) {
                *p_club = nullptr;
            }
            else {
                *p_club = (*clubs)[index - 1];
            }
        }
    }

    static bool player_getter(void *data, int idx, const char** out_text)
    {
        auto players = (std::vector<std::shared_ptr<Player>>*)data;

        if (idx == 0) {
            *out_text = "null";
        }
        else {
            *out_text = (*players)[idx - 1]->name.c_str();
        }
        return true;
    }

    static int get_player_index(std::vector<std::shared_ptr<Player>> &players, std::shared_ptr<Player> player)
    {
        if (player == nullptr) {
            return 0;
        }

        auto itr = std::find(players.begin(), players.end(), player);
        if (itr == players.end()) {
            return 0;
        }

        return itr - players.begin() + 1;
    }

    static void PlayerCombo(const char *label, std::shared_ptr<Player> *p_player, std::vector<std::shared_ptr<Player>> *players = nullptr)
    {
        if (players == nullptr) {
            players = &global_db.players;
        }

        auto player = *p_player;
        auto index = get_player_index(*players, player);
        if (ImGui::Combo(label, &index, player_getter, players, players->size() + 1)) {
            if (index == 0) {
                *p_player = nullptr;
            }
            else {
                *p_player = (*players)[index - 1];
            }
        }
    }

    // date range
    static bool date_range_getter(void *data, int idx, const char** out_text)
    {
        auto date_ranges = (std::vector<std::shared_ptr<DateRange>>*)data;

        if (idx == 0) {
            *out_text = "null";
        }
        else {
            *out_text = (*date_ranges)[idx - 1]->name.c_str();
        }
        return true;
    }

    static int get_date_range_index(std::vector<std::shared_ptr<DateRange>> &date_ranges, std::shared_ptr<DateRange> date_range)
    {
        if (date_range == nullptr) {
            return 0;
        }

        auto itr = std::find(date_ranges.begin(), date_ranges.end(), date_range);
        if (itr == date_ranges.end()) {
            return 0;
        }

        return itr - date_ranges.begin() + 1;
    }

    static void DateRangeCombo(const char *label, std::shared_ptr<DateRange> *p_date_range, std::vector<std::shared_ptr<DateRange>> *date_ranges = nullptr)
    {
        if (date_ranges == nullptr) {
            date_ranges = &global_db.date_ranges;
        }

        auto date_range = *p_date_range;
        auto index = get_date_range_index(*date_ranges, date_range);
        if (ImGui::Combo(label, &index, date_range_getter, date_ranges, date_ranges->size() + 1)) {
            if (index == 0) {
                *p_date_range = nullptr;
            }
            else {
                *p_date_range = (*date_ranges)[index - 1];
            }
        }
    }

    static void Text(const std::string &txt)
    {
        ImGui::Text(txt.c_str());
    }

    static void LabelText(const char *label, const std::string &txt)
    {
        ImGui::LabelText(label, txt.c_str());
    }

    static void GoalButton(const char *label, std::shared_ptr<Goal> *p_goal)
    {
        auto goal = *p_goal;
        ImGui::LabelText(label, utility::ToString(goal));
        ImGui::SameLine();
        if (goal) {
            if (goal == global_db.selected_goal) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImColor(255, 255, 0));
                ImGui::Button("E", ImVec2(-1, 0));
                ImGui::PopStyleColor(1);
            }
            else {
                if (ImGui::Button("E", ImVec2(-1, 0))) {
                    global_db.selected_goal = goal;
                }
            }
        }
    }

    static bool MatchButton(const char *label, std::shared_ptr<Match> *p_match)
    {
        auto match = *p_match;
        if (!match) {
            ImGui::LabelText(label, "------");
        }
        else if (match->GetResult() == EMatchResult::Win) {
            ImGui::LabelTextColored(ImColor(255, 0, 0), label, utility::ToString(match).c_str());
        }
        else if (match->GetResult() == EMatchResult::Draw) {
            ImGui::LabelTextColored(ImColor(0, 255, 0), label, utility::ToString(match).c_str());
        }
        else if (match->GetResult() == EMatchResult::Lose) {
            ImGui::LabelTextColored(ImColor(0, 0, 255), label, utility::ToString(match).c_str());
        }
        else {
            ImGui::LabelText(label, utility::ToString(match).c_str());
        }

        ImGui::SameLine();
        if (match) {
            if (match == global_db.selected_match) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImColor(255, 255, 0));
                ImGui::Button("E", ImVec2(-1, 0));
                ImGui::PopStyleColor(1);
            }
            else {
                if (ImGui::Button("E", ImVec2(-1, 0))) {
                    global_db.selected_match = match;
                }
            }
        }
        else {
            if (ImGui::Button("+", ImVec2(-1, 0))) {
                match = std::make_shared<Match>();
                match->id = global_db.MakeMatchId();
                match->opponent_team = global_db.LFC;
                match->side = EMatchSide::Home;

                *p_match = match;

                global_db.matchs.push_back(match);
                global_db.selected_match = match;

                return true;
            }
        }

        return false;
    }

    static void ReplayMatchButton(const char *label, ReplayMatch *p_replay_match)
    {
        ImGui::PushID(1); MatchButton(label, &p_replay_match->match); ImGui::PopID();

        if (p_replay_match->match) {
            auto result = p_replay_match->match->GetResult();
            if (result == EMatchResult::Draw) {
                ImGui::PushID(2); MatchButton(utf8("replay"), &p_replay_match->replay); ImGui::PopID();
            }
        }
    }

    static void TwoMatchTitle(const char *label, TwoMatch &two_match)
    {
        ImGui::Text(label);
        ImGui::SameLine();
        ImGui::Text(utility::ToString(two_match));
    }

    static void ColumnHeaders(int count, ...)
    {
        if (ImGui::IsKeyDown(GLFW_KEY_LEFT_SHIFT)) {
            ImGui::Columns(count);
            ImGui::ShowColumnsNorm();
        }
        else {
            va_list args;
            va_start(args, count);
            ImGui::FixedColumnsV(count, args);
            va_end(args);
        }
    }

    static void TopNPlayerTable(const char *label, TopNTable<Player> *p_table, ImVec2 size = ImVec2(200, 0))
    {
        TopNTable<Player> &table = *p_table;

        PushID(p_table);
        ImGui::BeginChild(label, size);
        ImGui::Columns(1);
        // header
        ImGui::Separator();
        ImGui::Text(label); ImGui::NextColumn();
        ImGui::Separator();

        ImGui::ColumnHeaders(3, 0.1, 0.7, 0.2);
        for (int i = 0; i < table.Keys.size(); ++i) {
            auto player = table.Keys[i];
            if (!player) {
                break;
            }

            ImGui::Text("%d", i + 1); ImGui::NextColumn();
            ImGui::Text(player->name); ImGui::NextColumn();
            ImGui::Text("%d", table.Values[i]); ImGui::NextColumn();
        }
        ImGui::Separator();
        ImGui::Columns(1);
        ImGui::EndChild();
        PopID();
    }
}