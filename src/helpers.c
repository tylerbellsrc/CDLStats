#include "match.h"

// Function to check if a player is in a match
int isPlayerInMatch(const Match *match, const char *playerName) {
    for (int i = 0; i < match->totalMapsPlayed; i++) {
        for (int j = 0; j < MAX_PLAYERS; j++) {
            if (strcmp(match->maps[i].team1Players[j].name, playerName) == 0) {
                return 1; // Player found in team 1
            }
            if (strcmp(match->maps[i].team2Players[j].name, playerName) == 0) {
                return 1; // Player found in team 2
            }
        }
    }
    return 0; // Player not found
}

// Will return all matches that have the given event name
Match* getMatchesByEvent(const Match *matches, int matchCount, const char *eventName, int *resultCount) {
    *resultCount = 0;

    // Allocate memory for the result array
    Match *result = malloc(matchCount * sizeof(Match));
    if (!result) {
        return NULL; // Memory allocation failed
    }

    for (int i = 0; i < matchCount; i++) {
        if (strcmp(matches[i].event, eventName) == 0) {
            result[*resultCount] = matches[i];
            (*resultCount)++;
        }
    }

    // Reallocate memory to fit the exact number of matches found
    result = realloc(result, (*resultCount) * sizeof(Match));
    if (!result && *resultCount > 0) {
        return NULL; // Memory reallocation failed
    }

    return result;
}

// Will return all matches that have the given player name
Match* getMatchesByPlayer(const Match *matches, int matchCount, const char *playerName, int *resultCount) {
    *resultCount = 0;

    // Allocate memory for the result array
    Match *result = malloc(matchCount * sizeof(Match));
    if (!result) {
        return NULL; // Memory allocation failed
    }

    for (int i = 0; i < matchCount; i++) {
        if (isPlayerInMatch(&matches[i], playerName)) {
            result[*resultCount] = matches[i];
            (*resultCount)++;
        }
    }

    // Reallocate memory to fit the exact number of matches found
    result = realloc(result, (*resultCount) * sizeof(Match));
    if (!result && *resultCount > 0) {
        return NULL; // Memory reallocation failed
    }

    return result;
}

// Return aggregate stats for a player across all matches
PlayerStats getAggregateStats(const Match *matches, int matchCount, const char *playerName) {
    PlayerStats aggregate = {0};

    for (int i = 0; i < matchCount; i++) {
        for (int j = 0; j < matches[i].totalMapsPlayed; j++) {
            for (int k = 0; k < MAX_PLAYERS; k++) {
                if (strcmp(matches[i].maps[j].team1Players[k].name, playerName) == 0) {
                    aggregate.kills += matches[i].maps[j].team1Players[k].kills;
                    aggregate.deaths += matches[i].maps[j].team1Players[k].deaths;
                    aggregate.damage += matches[i].maps[j].team1Players[k].damage;
                } else if (strcmp(matches[i].maps[j].team2Players[k].name, playerName) == 0) {
                    aggregate.kills += matches[i].maps[j].team2Players[k].kills;
                    aggregate.deaths += matches[i].maps[j].team2Players[k].deaths;
                    aggregate.damage += matches[i].maps[j].team2Players[k].damage;
                }
            }
        }
    }
    return aggregate;
}

// Return aggregate stats for a player based on date. 0 for before inclusive, 1 for after inclusive
PlayerStats getAggregateStatsByDate(const Match *matches, int matchCount, const char *playerName, const char *date, int after) {
    PlayerStats aggregate = {0};

    for (int i = 0; i < matchCount; i++) {
        if ((after && strcmp(matches[i].date, date) >= 0) || (!after && strcmp(matches[i].date, date) <= 0)) {
            for (int j = 0; j < matches[i].totalMapsPlayed; j++) {
                for (int k = 0; k < MAX_PLAYERS; k++) {
                    if (strcmp(matches[i].maps[j].team1Players[k].name, playerName) == 0) {
                        aggregate.kills += matches[i].maps[j].team1Players[k].kills;
                        aggregate.deaths += matches[i].maps[j].team1Players[k].deaths;
                        aggregate.damage += matches[i].maps[j].team1Players[k].damage;
                    } else if (strcmp(matches[i].maps[j].team2Players[k].name, playerName) == 0) {
                        aggregate.kills += matches[i].maps[j].team2Players[k].kills;
                        aggregate.deaths += matches[i].maps[j].team2Players[k].deaths;
                        aggregate.damage += matches[i].maps[j].team2Players[k].damage;
                    }
                }
            }
        }
    }

    return aggregate;
}

// Calculate the total kills for a given player across all matches
int getTotalKills(const Match *matches, int matchCount, const char *playerName) {
    int totalKills = 0;

    for (int i = 0; i < matchCount; i++) {
        for (int j = 0; j < matches[i].totalMapsPlayed; j++) {
            for (int k = 0; k < MAX_PLAYERS; k++) {
                if (strcmp(matches[i].maps[j].team1Players[k].name, playerName) == 0) {
                    totalKills += matches[i].maps[j].team1Players[k].kills;
                } else if (strcmp(matches[i].maps[j].team2Players[k].name, playerName) == 0) {
                    totalKills += matches[i].maps[j].team2Players[k].kills;
                }
            }
        }
    }

    return totalKills;
}

// Calculate the total damage for a given player across all matches
int getTotalDamage(const Match *matches, int matchCount, const char *playerName) {
    int totalDamage = 0;

    for (int i = 0; i < matchCount; i++) {
        for (int j = 0; j < matches[i].totalMapsPlayed; j++) {
            for (int k = 0; k < MAX_PLAYERS; k++) {
                if (strcmp(matches[i].maps[j].team1Players[k].name, playerName) == 0) {
                    totalDamage += matches[i].maps[j].team1Players[k].damage;
                } else if (strcmp(matches[i].maps[j].team2Players[k].name, playerName) == 0) {
                    totalDamage += matches[i].maps[j].team2Players[k].damage;
                }
            }
        }
    }

    return totalDamage;
}

// Calculate the average kills per map for a given player
float getAverageKills(const Match *matches, int matchCount, const char *playerName) {
    int totalKills = 0;
    int totalMaps = 0;

    for (int i = 0; i < matchCount; i++) {
        for (int j = 0; j < matches[i].totalMapsPlayed; j++) {
            for (int k = 0; k < MAX_PLAYERS; k++) {
                if (strcmp(matches[i].maps[j].team1Players[k].name, playerName) == 0 ||
                    strcmp(matches[i].maps[j].team2Players[k].name, playerName) == 0) {
                    totalKills += matches[i].maps[j].team1Players[k].kills;
                    totalMaps++;
                    break; // Count the map only once for the player
                }
            }
        }
    }

    return totalMaps > 0 ? (float)totalKills / totalMaps : 0.0f;
}

// Calculate the kill/death ratio (K/D) for a given player across all matches
float getKillDeathRatio(const Match *matches, int matchCount, const char *playerName) {
    int totalKills = 0;
    int totalDeaths = 0;

    for (int i = 0; i < matchCount; i++) {
        for (int j = 0; j < matches[i].totalMapsPlayed; j++) {
            for (int k = 0; k < MAX_PLAYERS; k++) {
                if (strcmp(matches[i].maps[j].team1Players[k].name, playerName) == 0) {
                    totalKills += matches[i].maps[j].team1Players[k].kills;
                    totalDeaths += matches[i].maps[j].team1Players[k].deaths;
                } else if (strcmp(matches[i].maps[j].team2Players[k].name, playerName) == 0) {
                    totalKills += matches[i].maps[j].team2Players[k].kills;
                    totalDeaths += matches[i].maps[j].team2Players[k].deaths;
                }
            }
        }
    }

    return totalDeaths > 0 ? (float)totalKills / totalDeaths : (float)totalKills;
}

// Get the total number of maps won by a specific team
int getTotalMapsWonByTeam(const Match *matches, int matchCount, const char *teamName) {
    int totalMapsWon = 0;

    for (int i = 0; i < matchCount; i++) {
        for (int j = 0; j < matches[i].totalMapsPlayed; j++) {
            if (strcmp(matches[i].team1, teamName) == 0 && matches[i].maps[j].team1Score > matches[i].maps[j].team2Score) {
                totalMapsWon++;
            } else if (strcmp(matches[i].team2, teamName) == 0 && matches[i].maps[j].team2Score > matches[i].maps[j].team1Score) {
                totalMapsWon++;
            }
        }
    }

    return totalMapsWon;
}

// Get the total number of maps lost by a specific team
int getTotalMapsLostByTeam(const Match *matches, int matchCount, const char *teamName) {
    int totalMapsLost = 0;

    for (int i = 0; i < matchCount; i++) {
        for (int j = 0; j < matches[i].totalMapsPlayed; j++) {
            if (strcmp(matches[i].team1, teamName) == 0 && matches[i].maps[j].team1Score < matches[i].maps[j].team2Score) {
                totalMapsLost++;
            } else if (strcmp(matches[i].team2, teamName) == 0 && matches[i].maps[j].team2Score < matches[i].maps[j].team1Score) {
                totalMapsLost++;
            }
        }
    }

    return totalMapsLost;
}