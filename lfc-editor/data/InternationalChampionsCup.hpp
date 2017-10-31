#pragma once
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>
#include <string>
#include "Event.hpp"
#include "Match.hpp"

namespace lfc
{
    struct InternationalChampionsCupTable
    {
        int played;
        int won;
        int drawn;
        int lost;
        int goals_for;
        int goals_against;
        int points;

        InternationalChampionsCupTable()
        {
            played = 0;
            won = 0;
            drawn = 0;
            lost = 0;
            goals_for = 0;
            goals_against = 0;
            points = 0;
        }
    };

    struct InternationalChampionsCup : public Event
    {
        bool is_one_goal_one_point;

        bool is_champion;
        std::vector<std::shared_ptr<Match>> rounds;

        std::shared_ptr<Match> final;

        InternationalChampionsCup()
        {
            rounds.resize(3, nullptr);
            is_champion = false;
            is_one_goal_one_point = true;
        }

        virtual ~InternationalChampionsCup() {}

        InternationalChampionsCupTable GetTable(int rounds_cnt = 0)
        {
            InternationalChampionsCupTable table;

            if (rounds_cnt == 0 || rounds_cnt > rounds.size()) {
                rounds_cnt = rounds.size();
            }

            for (size_t i = 0; i < rounds_cnt; ++i) {
                auto match = rounds[i];
                if (match) {
                    ++table.played;

                    auto result = match->GetResult();
                    if (match->HasPenaltyShootout()) {
                        if (result == EMatchResult::Win) {
                            table.points += 2;
                        }
                        else {
                            table.points += 1;
                        }
                    }
                    else {
                        if (result == EMatchResult::Win) {
                            table.points += 3;
                        }
                    }

                    if (result == EMatchResult::Win) {
                        ++table.won;
                    }
                    else if (result == EMatchResult::Draw) {
                        ++table.drawn;
                    }
                    else {
                        ++table.lost;
                    }

                    table.goals_for += match->lfc_goals.size();
                    table.goals_against += match->opponent_goals.size();

                    if (is_one_goal_one_point) {
                        table.points += match->lfc_goals.size();
                    }
                }
            }

            return table;
        }

        virtual void FillMatchs(std::vector<std::shared_ptr<Match>> &to_fill)
        {
            FillMatch(to_fill, rounds);
        }

		template<class Archive>
		void save(Archive &archive) const
		{
			archive(cereal::base_class<Event>(this), CEREAL_NVP(rounds), CEREAL_NVP(is_champion), CEREAL_NVP(is_one_goal_one_point), CEREAL_NVP(final));
		}

        template<class Archive>
        void load(Archive &archive)
        {
            archive(cereal::base_class<Event>(this), CEREAL_NVP(rounds), CEREAL_NVP(is_champion), CEREAL_NVP(is_one_goal_one_point), CEREAL_NVP(final));
        }
    };
}

#include <cereal/archives/json.hpp>

CEREAL_REGISTER_TYPE(lfc::InternationalChampionsCup);
CEREAL_REGISTER_POLYMORPHIC_RELATION(lfc::Event, lfc::InternationalChampionsCup)