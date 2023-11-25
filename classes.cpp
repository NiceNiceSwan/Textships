#include "classes.h"

// "This seems like a bad idea but it's fine for now"
void ship::battleship(int num)
{
    ship_class = "Battleship " + std::to_string(num);
    hp = 40;
    armor = 20;
    certain_detection_range = 5;
    uncertain_detection_range = certain_detection_range + 1;
    accuracy = 80;
    dodging_target_hit_chance = 35;
    reload_time = 5;
    time_to_reload = 0;
    firepower = 20;
    armor_penetration = 20;
    gun_range = 7;
    speed = 2;
    moves_left_in_turn = speed;
    dodge_chance = 0;
    cost = 100;
}

void ship::cruiser(int num)
{
    ship_class = "Cruiser " + std::to_string(num);
    hp = 28;
    armor = 10;
    certain_detection_range = 4;
    uncertain_detection_range = certain_detection_range + 1;
    accuracy = 90;
    dodging_target_hit_chance = 80;
    reload_time = 2;
    time_to_reload = 0;
    firepower = 15;
    armor_penetration = 8;
    gun_range = 5;
    speed = 3;
    moves_left_in_turn = speed;
    dodge_chance = 25;
    cost = 70;
}

void ship::destroyer(int num)
{
    ship_class = "Destroyer " + std::to_string(num);
    hp = 15;
    armor = 0;
    certain_detection_range = 2;
    uncertain_detection_range = certain_detection_range + 1;
    accuracy = 80;
    dodging_target_hit_chance = 90;
    reload_time = 2;
    time_to_reload = 0;
    firepower = 6;
    armor_penetration = 2;
    gun_range = 3;
    speed = 4;
    moves_left_in_turn = speed;
    dodge_chance = 50;
    cost = 50;
}

void ship::print_detailed_ship_info()
{
    std::cout << "HP remaining: " << hp 
    << "\nArmor: " << armor
    << "\nFirepower: " << firepower
    << "\nArmor penetration: " << armor_penetration
    << "\nGun range: " << gun_range
    << "\nFires every " << reload_time << " turns"
    << "\nGuns reloaded in " << time_to_reload << " turns"
    << "\nAccuracy:" << accuracy << "%"
    << "\nChance to hit maneuvering target: " << dodging_target_hit_chance
    << "\nCan move " << speed << " tiles in a single turn"
    << "\nCan still move " << moves_left_in_turn << " tiles in this turn"
    << "\nChance to dodge: " << dodge_chance
    << "\nCurrent coordinates: " << position_x << "x " << position_y << "y\n";
}

void ship::print_short_ship_info()
{
    std::cout << "\n"
    << ship_class
    << "\nHP: " << hp
    << ", Time to reload: " << time_to_reload
    << ", Current coordinates: " << position_x << "x " << position_y << "y"
    << ", Moves remaining: " << moves_left_in_turn << "\n";
}

