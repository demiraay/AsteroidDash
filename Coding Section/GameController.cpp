#include "GameController.h"

#include <fstream>

// Simply instantiates the game
GameController::GameController(
        const string &space_grid_file_name,
        const string &celestial_objects_file_name,
        const string &leaderboard_file_name,
        const string &player_file_name,
        const string &player_name

) {
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name,
                            player_name);
    // TODO: Your code here, if you want to perform extra initializations


}

// Reads commands from the given input file, executes each command in a game tick
void GameController::play(const string &commands_file) {
    // TODO: Your code here

    std::ifstream file(commands_file);
    std::string command;



    while (std::getline(file, command)) {
        if (command == "MOVE_UP") {
            game->player->move_up();
            game->game_time += 1;
            game->update_space_grid();
            game->print_space_grid();
        } else if (command == "MOVE_DOWN") {
            game->player->move_down(game->space_grid.size());
            game->game_time += 1;
            game->update_space_grid();
            game->print_space_grid();
        } else if (command == "MOVE_LEFT") {
            game->player->move_left();
            game->game_time += 1;
            game->update_space_grid();
            game->print_space_grid();
        } else if (command == "MOVE_RIGHT") {
            game->player->move_right(game->space_grid[0].size());
            game->game_time += 1;
            game->update_space_grid();
            game->print_space_grid();
        } else if (command == "SHOOT") {
            game->shoot();
            game->game_time += 1;
            game->update_space_grid();
            game->print_space_grid();

        }else if (command == "PRINT_GRID") {
            game->print_space_grid();
            game->game_time += 1;
        }
        else if (command == "NOP") {
            // NOP: No operation, sadece zamanı ilerlet
            game->game_time += 1;
            game->update_space_grid();
            game->print_space_grid();

        }


    }

    file.close();
}

// Destructor to delete dynamically allocated member variables here
GameController::~GameController() {
    // TODO: Your code here
    delete game;
}
