#pragma once
#include <string>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>

namespace lfc
{
    struct FootballClub
    {
        int id;
        std::string name;
        std::string en_name;
        std::vector<std::shared_ptr<Player>> players;

        bool HasPlayer(std::shared_ptr<Player> player)
        {
            for (auto p : players) {
                if (p == player) {
                    return true;
                }
            }

            return false;
        }

        template<class Archive>
        void save(Archive &archive) const
        {
            archive(CEREAL_NVP(id), CEREAL_NVP(name), CEREAL_NVP(en_name), CEREAL_NVP(players));
        }

        template<class Archive>
        void load(Archive &archive)
        {
            archive(CEREAL_NVP(id), CEREAL_NVP(name), CEREAL_NVP(en_name), CEREAL_NVP(players));
        }
    };
}