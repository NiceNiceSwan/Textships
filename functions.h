#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "classes.h"
#include <vector>
#include <cmath>
#include <vector>
#include <stdlib.h>
#include <chrono>

int get_choice();

bool is_starting_position_valid(int ship_id, std::vector<ship> ship, int max_x, int max_y);

void player_configuration(std::vector<ship> &ship, int max_x, int max_y);

void ship_spotter(std::vector<ship> fleet_that_spots, std::vector<ship> fleet_being_spotted, std::vector<bool> &spotted_ship_table);

bool ship_finder_at_coordinates(std::vector<ship> fleet_to_search, int x, int y, int &ship_id);

int hit_calculator(ship &attacking_ship, ship &targeted_ship);

void fire_at_coordinates(std::vector<ship> &attacking_fleet, std::vector<ship> &target_fleet, std::vector<bool> spotted_ships_id);

void shot_handler(ship &attacking_ship, ship &targeted_ship, bool target_is_spotted);

bool player_interface(int player, std::vector<ship> &current_player_fleet, std::vector<ship> &other_player_fleet);



#endif