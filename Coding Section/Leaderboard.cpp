#include "Leaderboard.h"
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

// Read the stored leaderboard status from the given file
void Leaderboard::read_from_file(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) return;

    string player_name;
    unsigned long score;
    time_t last_played;

    while (file >> player_name >> score >> last_played) {
        LeaderboardEntry *entry = new LeaderboardEntry(score, last_played, player_name);
        insert(entry);
    }
    file.close();
}

// Write the latest leaderboard status to the given file
void Leaderboard::write_to_file(const string &filename) {
    ofstream file(filename, ios::trunc);
    if (!file.is_open()) return;

    LeaderboardEntry *current = head_leaderboard_entry;
    while (current) {
        file << current->player_name << " " << current->score << " " << current->last_played << endl;
        current = current->next;
    }
    file.close();
}

// Print the current leaderboard status to the standard output
void Leaderboard::print_leaderboard() {
    LeaderboardEntry *current = head_leaderboard_entry;
    int rank = 1;

    while (current) {
        cout << rank << ". " << current->player_name << " - " << current->score
             << " (" << std::put_time(std::localtime(&current->last_played), "%Y-%m-%d %H:%M:%S") << ")" << endl;
        current = current->next;
        rank++;
    }
}

// Insert a new LeaderboardEntry instance into the leaderboard
void Leaderboard::insert(LeaderboardEntry *new_entry) {
    if (!head_leaderboard_entry || new_entry->score > head_leaderboard_entry->score) {
        // Yeni skor en yüksekse başa eklenir
        new_entry->next = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } else {
        // Uygun konumu bul ve ekle
        LeaderboardEntry *current = head_leaderboard_entry;
        while (current->next && current->next->score >= new_entry->score) {
            current = current->next;
        }
        new_entry->next = current->next;
        current->next = new_entry;
    }


    LeaderboardEntry *current = head_leaderboard_entry;
    int count = 1;
    while (current && current->next) {
        if (++count > MAX_LEADERBOARD_SIZE) {
            delete current->next; // 11. skoru sil
            current->next = nullptr;
        } else {
            current = current->next;
        }
    }
}

// Free dynamically allocated memory used for storing leaderboard entries
Leaderboard::~Leaderboard() {
    LeaderboardEntry *current = head_leaderboard_entry;
    while (current) {
        LeaderboardEntry *next = current->next;
        delete current;
        current = next;
    }
}
