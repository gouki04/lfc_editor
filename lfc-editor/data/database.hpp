#pragma once
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <algorithm>
#include "Player.hpp"
#include "FootballClub.hpp"
#include "Match.hpp"
#include "Goal.hpp"
#include "Event.hpp"

namespace lfc
{
    bool sort_event_by_begin_date(std::shared_ptr<Event> a, std::shared_ptr<Event> b)
    {
        return a->begin_time > b->begin_time;
    }

    bool sort_match_by_begin_date(std::shared_ptr<Match> a, std::shared_ptr<Match> b)
    {
        return a->time > b->time;
    }

    bool sort_player_by_goal_and_assist(std::shared_ptr<Player> a, std::shared_ptr<Player> b);
    bool sort_club_by_match_count(std::shared_ptr<FootballClub> a, std::shared_ptr<FootballClub> b);

    struct datebase
    {
        int MakePlayerId() { return players.size(); }
        int MakeClubId() { return clubs.size(); }
        int MakeGoalId() { return goals.size(); }
        int MakeMatchId() { return matchs.size(); }
        int MakeEventId() { return events.size(); }

        std::shared_ptr<FootballClub> LFC;
        std::vector<std::shared_ptr<Player>> players;
        std::vector<std::shared_ptr<FootballClub>> clubs;
        std::vector<std::shared_ptr<Goal>> goals;
        std::vector<std::shared_ptr<Match>> matchs;
        std::vector<std::shared_ptr<Event>> events;
        std::vector<std::shared_ptr<DateRange>> dates;

        std::shared_ptr<Match> selected_match;
        std::shared_ptr<Goal> selected_goal;
        std::shared_ptr<Player> selected_player;

        void SortEvents()
        {
            sort(events.begin(), events.end(), sort_event_by_begin_date);
        }

        void SortMatchs()
        {
            sort(matchs.begin(), matchs.end(), sort_match_by_begin_date);
        }

        void SortMatchs(std::vector<std::shared_ptr<Match>> &_matchs)
        {
            sort(_matchs.begin(), _matchs.end(), sort_match_by_begin_date);
        }

        void SortPlayers()
        {
            sort(players.begin(), players.end(), sort_player_by_goal_and_assist);
        }

        void SortPlayersInClubs()
        {
            for (auto club : clubs) {
                sort(club->players.begin(), club->players.end(), sort_player_by_goal_and_assist);
            }
        }

        void SortClubs()
        {
            sort(clubs.begin(), clubs.end(), sort_club_by_match_count);
        }

        int GetPlayerCreateGoalsCount(std::shared_ptr<Player> player)
        {
            int sum = 0;
            for (auto goal : goals) {
                if (goal->score_player == player || goal->assist_player == player) {
                    ++sum;
                }
            }

            return sum;
        }

        int GetMatchCount(std::shared_ptr<FootballClub> club)
        {
            if (club == LFC) {
                return (int)matchs.size();
            }

            int sum = 0;
            for (auto match : matchs) {
                if (match->opponent_team == club) {
                    ++sum;
                }
            }

            return sum;
        }

        template<class Archive>
        void serialize(Archive &archive)
        {
            archive(CEREAL_NVP(players), CEREAL_NVP(clubs), CEREAL_NVP(goals), CEREAL_NVP(matchs), CEREAL_NVP(events), CEREAL_NVP(LFC));
        }
    };
}

lfc::datebase global_db;

namespace lfc
{
    bool sort_player_by_goal_and_assist(std::shared_ptr<Player> a, std::shared_ptr<Player> b)
    {
        return global_db.GetPlayerCreateGoalsCount(a) > global_db.GetPlayerCreateGoalsCount(b);
    }

    bool sort_club_by_match_count(std::shared_ptr<FootballClub> a, std::shared_ptr<FootballClub> b)
    {
        return global_db.GetMatchCount(a) > global_db.GetMatchCount(b);
    }

}