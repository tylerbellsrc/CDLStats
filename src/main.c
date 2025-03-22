#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "match.h"

// Function to trim newline characters from strings
void trimNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Function to read a match from a JSON file
void readMatchFromFile(const char *filename, Match *match) {
    // Open the JSON file
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    // Read the entire file into a buffer
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char *buffer = malloc(fileSize + 1);
    if (!buffer) {
        perror("Error allocating memory");
        fclose(file);
        return;
    }

    fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0';
    fclose(file);

    // Parse the JSON data
    cJSON *json = cJSON_Parse(buffer);
    free(buffer);

    if (!json) {
        printf("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }

    // Extract match data
    strcpy(match->date, cJSON_GetObjectItem(json, "date")->valuestring);
    strcpy(match->event, cJSON_GetObjectItem(json, "event")->valuestring);
    strcpy(match->team1, cJSON_GetObjectItem(json, "team1")->valuestring);
    strcpy(match->team2, cJSON_GetObjectItem(json, "team2")->valuestring);
    match->totalMapsPlayed = cJSON_GetObjectItem(json, "totalMapsPlayed")->valueint;
    match->team1OverallScore = cJSON_GetObjectItem(json, "team1OverallScore")->valueint;
    match->team2OverallScore = cJSON_GetObjectItem(json, "team2OverallScore")->valueint;

    // Extract maps array
    cJSON *maps = cJSON_GetObjectItem(json, "maps");
    for (int i = 0; i < match->totalMapsPlayed; i++) {
        cJSON *map = cJSON_GetArrayItem(maps, i);
        strcpy(match->maps[i].mapName, cJSON_GetObjectItem(map, "mapName")->valuestring);
        match->maps[i].gameMode = cJSON_GetObjectItem(map, "gameMode")->valueint;
        match->maps[i].team1Score = cJSON_GetObjectItem(map, "team1Score")->valueint;
        match->maps[i].team2Score = cJSON_GetObjectItem(map, "team2Score")->valueint;

        // Extract team1Players array
        cJSON *team1Players = cJSON_GetObjectItem(map, "team1Players");
        for (int j = 0; j < MAX_PLAYERS; j++) {
            cJSON *player = cJSON_GetArrayItem(team1Players, j);
            if (player) {
                strcpy(match->maps[i].team1Players[j].name, cJSON_GetObjectItem(player, "name")->valuestring);
                match->maps[i].team1Players[j].kills = cJSON_GetObjectItem(player, "kills")->valueint;
                match->maps[i].team1Players[j].deaths = cJSON_GetObjectItem(player, "deaths")->valueint;
                match->maps[i].team1Players[j].damage = cJSON_GetObjectItem(player, "damage")->valueint;
            }
        }

        // Extract team2Players array
        cJSON *team2Players = cJSON_GetObjectItem(map, "team2Players");
        for (int j = 0; j < MAX_PLAYERS; j++) {
            cJSON *player = cJSON_GetArrayItem(team2Players, j);
            if (player) {
                strcpy(match->maps[i].team2Players[j].name, cJSON_GetObjectItem(player, "name")->valuestring);
                match->maps[i].team2Players[j].kills = cJSON_GetObjectItem(player, "kills")->valueint;
                match->maps[i].team2Players[j].deaths = cJSON_GetObjectItem(player, "deaths")->valueint;
                match->maps[i].team2Players[j].damage = cJSON_GetObjectItem(player, "damage")->valueint;
            }
        }
    }

    // Free the JSON object
    cJSON_Delete(json);
}

// Helper function to get the game mode name
const char* getGameModeName(int gameMode) {
    switch (gameMode) {
        case 0: return "Hardpoint";
        case 1: return "Search and Destroy";
        case 2: return "Control";
        default: return "Unknown";
    }
}

// Function to print match data for verification
void printMatch(const Match *match) {
    printf("Date: %s\n", match->date);
    printf("Event: %s\n", match->event);
    printf("Teams: %s vs %s\n", match->team1, match->team2);
    printf("Total Maps Played: %d\n\n", match->totalMapsPlayed);

    for (int i = 0; i < match->totalMapsPlayed; i++) {
        printf("Map %d: %s (Mode: %s) Score: %d-%d\n",
               i + 1, match->maps[i].mapName, getGameModeName(match->maps[i].gameMode),
               match->maps[i].team1Score, match->maps[i].team2Score);

        printf("Players:\n");
        for (int j = 0; j < MAX_PLAYERS; j++) {
            if (strlen(match->maps[i].team1Players[j].name) > 0) {
                printf("  %s - K: %d D: %d DMG: %d\n",
                       match->maps[i].team1Players[j].name,
                       match->maps[i].team1Players[j].kills,
                       match->maps[i].team1Players[j].deaths,
                       match->maps[i].team1Players[j].damage);
            }
        }
        for (int j = 0; j < MAX_PLAYERS; j++) {
            if (strlen(match->maps[i].team2Players[j].name) > 0) {
                printf("  %s - K: %d D: %d DMG: %d\n",
                       match->maps[i].team2Players[j].name,
                       match->maps[i].team2Players[j].kills,
                       match->maps[i].team2Players[j].deaths,
                       match->maps[i].team2Players[j].damage);
            }
        }
    }
}

// Function to write matches to a binary file
void writeMatchesToFile(const char *filename, const Match *matches, int matchCount) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file for writing");
        return;
    }

    // Write the number of matches first
    if (fwrite(&matchCount, sizeof(int), 1, file) != 1) {
        perror("Error writing match count");
        fclose(file);
        return;
    }

    // Write each match to the file
    for (int i = 0; i < matchCount; i++) {
        if (fwrite(&matches[i], sizeof(Match), 1, file) != 1) {
            perror("Error writing match data");
            fclose(file);
            return;
        }
    }

    fclose(file);
    printf("Successfully wrote %d match(es) to %s\n", matchCount, filename);
}

// Main function to test reading a match
int main() {
    Match match;
    readMatchFromFile("matches.json", &match);
    printMatch(&match);
    writeMatchesToFile("matches.bin", &match, sizeof(match) / sizeof(Match));
    return 0;
}
