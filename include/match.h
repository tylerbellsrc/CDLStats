#ifndef MATCH_H
#define MATCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLAYERS 4
#define MAX_MAPS 5
#define MAX_NAME_LEN 50

// Structure to hold player performance data for each map
typedef struct {
    char name[MAX_NAME_LEN];
    int kills;
    int deaths;
    int damage;
} PlayerStats;

// Structure to hold map-specific data
typedef struct {
    char mapName[MAX_NAME_LEN];
    // 0 for HP, 1 for SND, 2 for CTL, -1 for unknown
    int gameMode;
    PlayerStats team1Players[MAX_PLAYERS];
    PlayerStats team2Players[MAX_PLAYERS];
    int team1Score;
    int team2Score;
} MapStats;

// Structure to hold match data
typedef struct {
    char team1[MAX_NAME_LEN];
    char team2[MAX_NAME_LEN];
    MapStats maps[MAX_MAPS];
    int totalMapsPlayed;
    char date[11];
    char event[MAX_NAME_LEN];
    int team1OverallScore; // Overall map wins for team1
    int team2OverallScore; // Overall map wins for team2
} Match;

// Updated Team structure with a dynamic array of matches
typedef struct {
    PlayerStats players[MAX_PLAYERS];
    char name[MAX_NAME_LEN];
    int wins;
    int losses;
    int points;
    Match *matches; // Dynamic array of matches
    int matchCount; // Number of matches
} Team;

// Function prototype for checking if a player is in a match
int isPlayerInMatch(const Match *match, const char *playerName);

// Will return all matches that have the given event name
Match* getMatchesByEvent(const Match *matches, int matchCount, const char *eventName, int *resultCount);

// Will return all matches that have the given player name
Match* getMatchesByPlayer(const Match *matches, int matchCount, const char *playerName, int *resultCount);

// Return aggregate stats for a player across all matches
PlayerStats getAggregateStats(const Match *matches, int matchCount, const char *playerName);

// Return aggregate stats for a player based on date. 0 for before inclusive, 1 for after inclusive
PlayerStats getAggregateStatsByDate(const Match *matches, int matchCount, const char *playerName, const char *date, int after);

// Calculate the total kills for a given player across all matches
int getTotalKills(const Match *matches, int matchCount, const char *playerName);

// Calculate the total damage for a given player across all matches
int getTotalDamage(const Match *matches, int matchCount, const char *playerName);

// Calculate the average kills per map for a given player
float getAverageKills(const Match *matches, int matchCount, const char *playerName);

// Calculate the kill/death ratio (K/D) for a given player across all matches
float getKillDeathRatio(const Match *matches, int matchCount, const char *playerName);

// Get the total number of maps won by a specific team
int getTotalMapsWonByTeam(const Match *matches, int matchCount, const char *teamName);

// Get the total number of maps lost by a specific team
int getTotalMapsLostByTeam(const Match *matches, int matchCount, const char *teamName);

#endif