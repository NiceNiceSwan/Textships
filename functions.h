#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "classes.h"
#include <vector>
#include <cmath>
#include <vector>
#include <stdlib.h>
#include <chrono>

int get_int_input();

bool get_bool_input();

bool is_starting_position_valid(int ship_id, std::vector<ship> ship, int max_x, int max_y);

void player_configuration(std::vector<ship> &ship, int max_x, int max_y);

void ship_spoting_handler(std::vector<ship> fleet_that_spots, std::vector<ship> fleet_being_spotted, std::vector<bool> &spotted_ship_table);

void ship_spotter(ship ship_that_spots, ship ship_being_spotted, int id_of_spotted_ship, std::vector<bool> &spotted_ship_table);

bool ship_finder_at_coordinates(std::vector<ship> fleet_to_search, int x, int y, int &ship_id);

int hit_calculator(ship &attacking_ship, ship &targeted_ship);

void fire_at_coordinates(std::vector<ship> &attacking_fleet, std::vector<ship> &target_fleet, std::vector<bool> spotted_ships_id);

void shot_handler(ship &attacking_ship, ship &targeted_ship, bool target_is_spotted);

void player_interface(int player, std::vector<ship> &current_player_fleet, std::vector<ship> &other_player_fleet);

bool main_action_selector(std::vector<ship> &current_player_fleet, std::vector<ship> &other_player_fleet, std::vector<bool> spotted_ships_id);

void order_ship_to_move(std::vector<ship> &player_fleet, std::vector<ship> other_fleet);

void ship_next_position_handler(std::vector<ship> &current_fleet, int id_selected_ship, std::vector<ship> other_fleet);

void ship_destination_clear(std::vector<ship> &current_fleet, int id_selected_ship, std::vector<ship> other_fleet, int &distance_x_to_destination, int &distance_y_to_destination);

void end_turn_handler(std::vector<ship> &fleet1, std::vector<ship> &fleet2);

void ship_killer(std::vector<ship> &fleet);

#endif