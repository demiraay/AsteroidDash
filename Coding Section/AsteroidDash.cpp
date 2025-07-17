#include "AsteroidDash.h"

#include <fstream>
#include <sstream>



// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

        : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {

    read_player(player_file_name, player_name);  // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name);  // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name);  // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name);
}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file) {
    // TODO: Your code here
    std::ifstream file(input_file);



    std ::string line;
    std::vector<std::vector<int>> temp_grid;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::vector<int> row;
        int cell;


        while (ss >> cell) {
            row.push_back(cell);
        }


        temp_grid.push_back(row);
    }


    space_grid = temp_grid;

    file.close();

}

// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name) {
    // TODO: Your code here

    int start_row, start_col;
    std::ifstream file(player_file_name);
    std::vector<std::vector<bool>> player_shape;
    std::string line;


    file >> start_row >> start_col;

    if(std::getline(file, line)) {
        std::istringstream ss(line);
        ss >> start_row >> start_col;
    }


    while (std::getline(file, line)) {


        std::vector<bool> row;
        for (char ch : line) {
            if (ch == '1') row.push_back(true);
            else if (ch == '0') row.push_back(false);
        }
        player_shape.push_back(row);
    }

    player = new Player(player_shape, start_row, start_col, player_name);
    file.close();

}

// Function to read celestial objects from a file
void AsteroidDash::read_celestial_objects(const string &input_file) {
    // TODO: Your code here
    std::ifstream file(input_file);
    std::string line;

    CelestialObject *previous_object ;

    celestial_objects_list_head = nullptr;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::vector<std::vector<bool>> shape;
        ObjectType type ;
        int start_row ;
        int time_of_appearance;
        bool is_powerup ;


        if (line[0] == '[') {
            type = ASTEROID;
            is_powerup = false;
        } else if (line[0] == '{') {
            is_powerup = true;
        }


        do {
            std::vector<bool> row;
            for (char ch : line) {
                if (ch == '1') row.push_back(true);
                else if (ch == '0') row.push_back(false);
            }
            shape.push_back(row);
        } while (std::getline(file, line) && !line.empty() && line[0] != 's' && line[0] != 't' && line[0] != 'e');


        if (line[0] == 's') {
            start_row = std::stoi(line.substr(2));
            std::getline(file, line);
        }


        if (line[0] == 't') {
            time_of_appearance = std::stoi(line.substr(2));
            std::getline(file, line); // Bir sonraki satıra geç
        }


        if (is_powerup && line[0] == 'e') {
            if (line.find("life") != std::string::npos) {
                type = LIFE_UP;
            } else if (line.find("ammo") != std::string::npos) {
                type = AMMO;
            }
            std::getline(file, line); // Bir sonraki satıra geç
        }


        CelestialObject *new_object = new CelestialObject(shape, type, start_row, time_of_appearance);

        new_object->compute_rotations();





        if (celestial_objects_list_head == nullptr) {

            celestial_objects_list_head = new_object;
            previous_object = new_object;



        } else {

            previous_object->next_celestial_object = new_object;

            previous_object = new_object;


        }




    }

    file.close();

    // Tüm rotasyonların next_celestial_object alanlarını bağlama
    CelestialObject *current = celestial_objects_list_head;
    while (current != nullptr) {
        CelestialObject *rotation = current;
        do {
            rotation->next_celestial_object = current->next_celestial_object; // Her rotasyonu bağlıyor
            rotation = rotation->right_rotation; // Bir sonraki rotasyona geç
        } while (rotation != current); // Tüm rotasyonları güncelle
        current = current->next_celestial_object;
    }



}


// Print the entire space grid
void AsteroidDash::print_space_grid() {
    // TODO: Your code here



    cout << "Tick: " << game_time  << endl;
    cout << "Lives: " << player->lives << endl;
    cout << "Ammo: " << player->current_ammo << endl;
    cout << "Score: " << current_score << endl;



    cout << "High Score: " << current_score << endl;



    auto temp_grid = space_grid;

    for(int i = 0 ; i < player->spacecraft_shape.size() ; i++) {
        for(int j = 0 ; j < player->spacecraft_shape[i].size() ; j++) {
            if(player->spacecraft_shape[i][j]) {
                if (player->position_row + i >= 0 && player->position_row + i < temp_grid.size() &&
                    player->position_col + j >= 0 && player->position_col + j < temp_grid[0].size()) {
                    temp_grid[player->position_row + i][player->position_col + j] = 1;
                    }
            }
        }
    }

    CelestialObject* current = celestial_objects_list_head;
    while (current != nullptr) {
        int time_diff = game_time - current->time_of_appearance;


        if (time_diff >= 0 && time_diff < space_grid[0].size() + current->shape[0].size()) {
            int obj_row = current->starting_row;
            int obj_col = static_cast<int>(space_grid[0].size()) - 1 - time_diff;


            for (int i = 0; i < current->shape.size(); ++i) {
                for (int j = 0; j < current->shape[i].size(); ++j) {
                    int col_position = obj_col + j;


                    if (obj_row + i >= 0 && obj_row + i < temp_grid.size() &&
                        col_position >= 0 && col_position < temp_grid[0].size()) {

                        if (current->shape[i][j]) {
                            temp_grid[obj_row + i][col_position] = 1;
                        }
                        }
                }
            }
        }
        current = current->next_celestial_object;
    }

    for (const auto& row : temp_grid) {
        for (int cell : row) {
            if (cell == 0) {
                cout << unoccupiedCellChar;
            } else {
                cout << occupiedCellChar;
            }
        }
        cout << endl;
    }


}


// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid() {
    // Grid'i sıfırla
    for (auto &row : space_grid) {
        std::fill(row.begin(), row.end(), 0);
    }



    for (auto it = projectiles.begin(); it != projectiles.end();) {
        int row = it->first;
        int col = it->second;


        int new_col = col + 1;


        if (new_col < space_grid[0].size()) {
            space_grid[row][col] = 0;
            space_grid[row][new_col] = 2;
            it->second = new_col;
            ++it;
        } else {

            it = projectiles.erase(it);
        }
    }


    for (int i = 0; i < player->spacecraft_shape.size(); ++i) {
        for (int j = 0; j < player->spacecraft_shape[i].size(); ++j) {
            if (player->spacecraft_shape[i][j]) {
                int row = player->position_row + i;
                int col = player->position_col + j;
                if (row >= 0 && row < space_grid.size() && col >= 0 && col < space_grid[0].size()) {
                    space_grid[row][col] = 1;
                }
            }
        }
    }


}

// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
void AsteroidDash::shoot() {
    // Cephane kontrolü
    if (player->current_ammo <= 0) {
        return;
    }


    player->current_ammo -= 1;


    int start_row = player->position_row + player->spacecraft_shape.size() / 2;
    int start_col = player->position_col + player->spacecraft_shape[0].size()-1;


    if (start_col < space_grid[0].size()) {
        projectiles.push_back({start_row, start_col});
        space_grid[start_row][start_col] = 2;
    }
}

// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash() {
    delete player;

    CelestialObject* current = celestial_objects_list_head;
    while (current != nullptr) {
        CelestialObject* next = current->next_celestial_object;
        current->delete_rotations(current);
        delete current;
        current = next;
    }
}
