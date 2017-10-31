#pragma once
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>
#include <string>
#include "Event.hpp"
#include "Match.hpp"

namespace lfc
{
    struct PremierLeagueTable
    {
        int position;
        int played;
        int won;
        int drawn;
        int lost;
        int goals_for;
        int goals_against;
        int goal_difference;
        int points;

        PremierLeagueTable()
        {
            position = 0;
            played = 0;
            won = 0;
            drawn = 0;
            lost = 0;
            goals_for = 0;
            goals_against = 0;
            goal_difference = 0;
            points = 0;
        }
    };

    // Ó¢³¬
    struct PremierLeague : public Event
    {
        // 38ÂÖ
        std::vector<std::shared_ptr<Match>> rounds;

        // ÅÅÃû
        int position;

        PremierLeague()
        {
            rounds.resize(38, nullptr);
        }

        virtual ~PremierLeague() {}

        std::shared_ptr<Match> GetMatch(int round)
        {
            return rounds[round - 1];
        }

        int GetPoint()
        {
            int points = 0;
            for (size_t i = 0; i < rounds.size(); ++i) {
                auto match = rounds[i];
                if (match) {
                    auto result = match->GetResult();
                    if (result == EMatchResult::Win) {
                        points += 3;
                    }
                    else if (result == EMatchResult::Draw) {
                        points += 1;
                    }
                }
            }

            return points;
        }

        PremierLeagueTable GetTable(int rounds_cnt = 0)
        {
            PremierLeagueTable table;
            table.position = position;

            if (rounds_cnt == 0 || rounds_cnt > rounds.size()) {
                rounds_cnt = rounds.size();
            }
            
            for (size_t i = 0; i < rounds_cnt; ++i) {
                auto match = rounds[i];
                if (match) {
                    ++table.played;

                    auto result = match->GetResult();
                    if (result == EMatchResult::Win) {
                        ++table.won;
                        table.points += 3;
                    }
                    else if (result == EMatchResult::Draw) {
                        ++table.drawn;
                        table.points += 1;
                    }
                    else {
                        ++table.lost;
                    }

                    table.goals_for += match->lfc_goals.size();
                    table.goals_against += match->opponent_goals.size();
                }
            }

            table.goal_difference = table.goals_for - table.goals_against;

            return table;
        }

        virtual void FillMatchs(std::vector<std::shared_ptr<Match>> &to_fill)
        {
            FillMatch(to_fill, rounds);
        }

		template<class Archive>
		void save(Archive &archive) const
		{
			archive(cereal::base_class<Event>(this), CEREAL_NVP(rounds), CEREAL_NVP(position));
		}

        template<class Archive>
        void load(Archive &archive)
        {
            archive(cereal::base_class<Event>(this), CEREAL_NVP(rounds), CEREAL_NVP(position));
        }
    };
}

#include <cereal/archives/json.hpp>

CEREAL_REGISTER_TYPE(lfc::PremierLeague);
CEREAL_REGISTER_POLYMORPHIC_RELATION(lfc::Event, lfc::PremierLeague)