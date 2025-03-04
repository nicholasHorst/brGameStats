#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <filesystem>
#include <json/json.h> 

namespace fs = std::filesystem;

struct PlayerStats {
    int wins = 0;
    int games = 0;
    std::map<std::string, int> teammateWins;  // Track wins with teammates
    std::set<std::string> bestChampions;
};

// Map to store player statistics
std::map<std::string, PlayerStats> playerData;

// Function to process a single CSV game file
void processGameFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open: " << filename << "\n";
        return;
    }

    std::string line;
    getline(file, line); // Skip header line

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string date, winner1, winner2, winner3, loser1, loser2, loser3, bans, champ1, champ2, champ3;

        getline(ss, date, ',');
        getline(ss, winner1, ',');
        getline(ss, winner2, ',');
        getline(ss, winner3, ',');
        getline(ss, loser1, ',');
        getline(ss, loser2, ',');
        getline(ss, loser3, ',');
        getline(ss, bans, ',');
        getline(ss, champ1, ',');
        getline(ss, champ2, ',');
        getline(ss, champ3, ',');

        std::vector<std::string> winningTeam = {winner1, winner2, winner3};
        std::map<std::string, std::string> playerChamps = {{winner1, champ1}, {winner2, champ2}, {winner3, champ3}};

        // Update stats for each winning player
        for (const auto &player : winningTeam) {
            playerData[player].wins++;
            playerData[player].games++;

            // Track the champions they played
            playerData[player].bestChampions.insert(playerChamps[player]);

            // Update teammate stats
            for (const auto &teammate : winningTeam) {
                if (teammate != player) {
                    playerData[player].teammateWins[teammate]++;
                }
            }
        }
    }
    file.close();
}

// Function to process all CSV files in the `data/` folder
void processAllGames() {
    for (const auto &entry : fs::directory_iterator("data")) {
        if (entry.path().extension() == ".csv") {
            processGameFile(entry.path().string());
        }
    }
}

// Function to find best teammate
std::string getBestTeammate(const std::map<std::string, int> &teammates) {
    if (teammates.empty()) return "None";

    return std::max_element(teammates.begin(), teammates.end(),
                            [](const auto &a, const auto &b) { return a.second < b.second; })
        ->first;
}

// Function to export leaderboard data as JSON
void exportLeaderboard() {
    Json::Value root;
    for (const auto &[player, stats] : playerData) {
        Json::Value playerJson;
        playerJson["win_percentage"] = (stats.games > 0) ? (stats.wins * 100 / stats.games) : 0;
        playerJson["best_teammate"] = getBestTeammate(stats.teammateWins);

        Json::Value champs(Json::arrayValue);
        for (const auto &champ : stats.bestChampions) champs.append(champ);
        playerJson["best_champions"] = champs;

        root[player] = playerJson;
    }

    std::ofstream file("leaderboard.json");
    file << root.toStyledString();
    file.close();
}

int main() {
    processAllGames();
    exportLeaderboard();
    std::cout << "Leaderboard data saved to leaderboard.json!\n";
    return 0;
}
