#include "functions.h"

int get_int_input()
{
    int choice = 0;
    std::cin >> choice;
    while (std::cin.fail())
    {
        std::cout << "Wrong input, please input a number\n";
        std::cin.clear();
        std::cin.ignore(1000,'\n');
        std::cin>>choice;
    }
    return choice;
}

bool get_bool_input()
{
    bool choice = 0;
    std::cin >> choice;
    while (std::cin.fail())
    {
        std::cout << "Wrong input, please input a number\n";
        std::cin.clear();
        std::cin.ignore(1000,'\n');
        std::cin>>choice;
    }
    return choice;
}

bool is_starting_position_valid(int ship_id, std::vector<ship> ship, int max_x, int max_y)
{
    if (ship[ship_id].position_x < 0 || ship[ship_id].position_x > max_x || ship[ship_id].position_y < 0 || ship[ship_id].position_y > max_y)
    {
        return false;
    }
    for (size_t i = 0; i < ship.size(); i++)
    {
        if (i==ship_id)
        {
            continue;
        }
        if (ship[i].position_x == ship[ship_id].position_x && ship[i].position_y == ship[ship_id].position_y)
        {
            return false;
        }
        
    }

    return true;
}

void player_configuration(std::vector<ship> &ship, int max_x, int max_y)
{
    std::cout << "Map size: " << max_x << "x by " << max_y << "y\n";
    std::cout << "Configure the starting positions of your ships:\n";
    for (size_t i = 0; i < ship.size(); i++)
    {
        std::cout << ship[i].ship_class << "\n";

        std::cout << "Starting position x: ";
        ship[i].position_x = get_int_input();
        std::cout << "Starting position y: ";
        ship[i].position_y = get_int_input();
        while (!is_starting_position_valid(i, ship, max_x, max_y))
        {
            std::cin.clear();
            std::cin.ignore(1000,'\n');
            std::cout << "Invalid starting position, please try again\n";
            std::cout << "Starting position x: ";
            ship[i].position_x = get_int_input();
            std::cout << "Starting position y: ";
            ship[i].position_y = get_int_input();
        }
        ship[i].destination_x = ship[i].position_x;
        ship[i].destination_y = ship[i].position_y;
    }
}

void ship_spotter(std::vector<ship> fleet_that_spots, std::vector<ship> fleet_being_spotted, std::vector<bool> &spotted_ship_table)
{
    int distance_x_between_ships;
    int distance_y_between_ships;
    for (size_t i = 0; i < fleet_that_spots.size(); i++)
    {
        for (size_t j = 0; j < fleet_being_spotted.size(); j++)
        {
            // TODO: implement a detection probability when the ship is outside the certain detection range, but inside the uncertain detection range
            // a simple rand() generated number, if the number is above/below x the ship is spotted/not spotted
            distance_x_between_ships = std::abs(fleet_that_spots[i].position_x - fleet_being_spotted[j].position_x);
            distance_y_between_ships = std::abs(fleet_that_spots[i].position_y - fleet_being_spotted[j].position_y);
            if (distance_x_between_ships <= fleet_being_spotted[j].certain_detection_range && distance_y_between_ships <= fleet_being_spotted[j].certain_detection_range)
            {
                // FIXME: we have no way of stopping multiple ships from spotting the same target and printing that info out, meaning
                // that right now 2 ships can spot one target and both can relay the same message. I personally think this is bad, but there
                // are pluses coming from this, like a player knowing that multiple ships are spotting the same target, so he can move one away
                // either to keep it safe or to spot over a larger area
                // TODO: think about which solution is better, and if both are good enough,
                // maybe even add a new entry in the player interface to display detailed spotting data
                spotted_ship_table[j] = true;
                std::cout << "\n" <<fleet_that_spots[i].ship_class
                << " is spotting " << fleet_being_spotted[j].ship_class
                << ". " << fleet_being_spotted[j].ship_class << " position is:\n"
                << fleet_being_spotted[j].position_x << "x\n"
                << fleet_being_spotted[j].position_y << "y\n";
                if (distance_x_between_ships <= fleet_that_spots[i].certain_detection_range && distance_y_between_ships <= fleet_that_spots[i].certain_detection_range)
                {
                    std::cout << "Because of how close we are, "
                    << fleet_that_spots[i].ship_class << " was spotted as well\n";
                }
                else if (distance_x_between_ships <= fleet_that_spots[i].uncertain_detection_range && distance_y_between_ships <= fleet_that_spots[i].uncertain_detection_range)
                {
                    std::cout << "Because of how close we are, "
                    << fleet_that_spots[i].ship_class << " may have been spotted as well\n";
                }
                
                
            }
            
        }
        
    }
    
}

bool ship_finder_at_coordinates(std::vector<ship> fleet_to_search, int x, int y, int &ship_id)
{
    for (size_t i = 0; i < fleet_to_search.size(); i++)
    {
        if (fleet_to_search[i].position_x == x && fleet_to_search[i].position_y == y)
        {
            ship_id = i;
            return true;
        }
    }
    return false;
}

// same function as above, but doesn't take ship_id as a parameter, and therefore has no way of informing us what ship
// is occupying this coordinates
bool ship_finder_at_coordinates(std::vector<ship> fleet_to_search, int x, int y)
{
    for (size_t i = 0; i < fleet_to_search.size(); i++)
    {
        if (fleet_to_search[i].position_x == x && fleet_to_search[i].position_y == y)
        {
            return true;
        }
    }
    return false;
}

int hit_calculator(ship &attacking_ship, ship &targeted_ship)
{
    if (targeted_ship.armor == 0)
    {
        targeted_ship.hp -= attacking_ship.firepower;
        return attacking_ship.firepower;
    }
    else
    {
        targeted_ship.hp -= attacking_ship.firepower * ((float)attacking_ship.armor_penetration / targeted_ship.armor);
        return attacking_ship.firepower * ((float)attacking_ship.armor_penetration / targeted_ship.armor);
    }
}

void fire_at_coordinates(std::vector<ship> &attacking_fleet, std::vector<ship> &target_fleet, std::vector<bool> spotted_ships_id)
{
    int x;
    int y;
    int attacking_ship_id;
    int distance_x;
    int distance_y;
    bool ready_to_fire;
    // maybe I could put it in a separate function if I feel like this block of code is getting too long and unwieldy
    // TODO: change this so that the ship ID input goes first, we could do a reload check first before moving
    // on to inputing target coordinates
    std::cout << "Input target coordinates:\n"
    << "x: ";
    x = get_int_input();
    std::cout << "y: ";
    y = get_int_input();
    // std::cout << "Choose the ship you want to attack with: ";
    std::cout << "Input the ID of the ship you want to fire with: ";
    attacking_ship_id = get_int_input();
    distance_x = std::abs(x - attacking_fleet[attacking_ship_id].position_x);
    distance_y = std::abs(x - attacking_fleet[attacking_ship_id].position_y);
    if (distance_x > attacking_fleet[attacking_ship_id].gun_range || distance_y > attacking_fleet[attacking_ship_id].gun_range)
    {
        std::cout << "Target out of range, aborting\n";
        return;
    }
    else if (attacking_fleet[attacking_ship_id].time_to_reload > 0)
    {
        std::cout << "Ship has not reloaded it's guns yet, aborting\n";
        return;
    }
    // FIXME: we have no way of leaving the loop if no target is in range or no ship has it's guns reloaded
    std::cout << "The ship is ready to fire, are you sure you want to fire? (0 - No, 1 - Yes): ";
    ready_to_fire = get_bool_input();
    // I could put this in a separate function too
    int targeted_ship_id;
    if (ready_to_fire)
    {
        attacking_fleet[attacking_ship_id].time_to_reload = attacking_fleet[attacking_ship_id].reload_time;
        if (ship_finder_at_coordinates(target_fleet, x, y, targeted_ship_id))
        {
            shot_handler(attacking_fleet[attacking_ship_id], target_fleet[targeted_ship_id], spotted_ships_id[targeted_ship_id]);
            // TODO: some way to notify the other player that his ship has been damaged / sunk
        }
        else
        {
            std::cout << "We haven't managed to hit anything\n";
        }
    }
}

void shot_handler(ship &attacking_ship, ship &targeted_ship, bool target_is_spotted)
{
    attacking_ship.time_to_reload = attacking_ship.reload_time;

    if (rand()%100 + 1 > targeted_ship.dodge_chance - attacking_ship.dodging_target_hit_chance)
    {
        int damage_dealt = hit_calculator(attacking_ship, targeted_ship);
        if (target_is_spotted)
        {
            std::cout << "We have hit " << targeted_ship.ship_class << " resulting in "
            << damage_dealt << " DMG dealt\n";
        }
        else
        {
            std::cout << "We haven't managed to hit anything\n";
        }
        
    }
    else
    {
        std::cout << "We haven't managed to hit anything\n";
    }
    
}

void player_interface(int player, std::vector<ship> &current_player_fleet, std::vector<ship> &other_player_fleet)
{
    std::cout << "Player " << player << " turn\n";
    std::cout << "Ship statistics: \n";
    for (size_t i = 0; i < current_player_fleet.size(); i++)
    {
        std::cout << "\nShip ID: " << i;
        current_player_fleet[i].print_short_ship_info();
    }
    std::cout << "\n\nDetected enemy ships:\n"; 
    std::vector<bool> spotted_ships_id(other_player_fleet.size(), false);
    ship_spotter(current_player_fleet, other_player_fleet, spotted_ships_id);
    while (main_action_selector(current_player_fleet, other_player_fleet, spotted_ships_id))
    {
    }
    
}

bool main_action_selector(std::vector<ship> &current_player_fleet, std::vector<ship> &other_player_fleet, std::vector<bool> spotted_ships_id)
{
    std::cout << "\nChoose option: "
    << "\n(0) End turn"
    << "\n(1) Show detailed ship info"
    << "\n(2) Fire at coordinates"
    << "\n(3) Order ships to move\n";
    int ship_id_to_get_detailed_info;
    bool end_turn_prompt;
    switch (get_int_input())
        {
        case 0:
            std::cout << "Are you sure you want to end your turn?\n"
            << "(0) - No\n(1) - Yes\n";
            std::cin >> end_turn_prompt;
            if (end_turn_prompt)
            {
                return false;
            }
            return true;
            break;
        case 1:
            std::cout << "Input the ID of the ship you want to see\n";
            ship_id_to_get_detailed_info = get_int_input();
            current_player_fleet[ship_id_to_get_detailed_info].print_detailed_ship_info();
            return true;
            break;
        case 2:
            fire_at_coordinates(current_player_fleet, other_player_fleet, spotted_ships_id);
            return true;
            break;
        case 3:
            order_ship_to_move(current_player_fleet, other_player_fleet);
            return true;
            break;
        default:
            return true;
            break;
        }
}

// TODO: all of this stuff below
// Ten kod ma za zadanie sprawdzać pozycję wszystkich statków na mapie, brać od gracza ID statku który chce przemieścić,
// koordynaty do których chce go przemieścić, sprawdzić czy te koordynaty są w zasięgu ruchu tego statku w jednej turze (prędkości)
// i zależnie od tego przemieścić statek do jakiegoś pola
// w wypadku gdy gracz chce przemieścić statek do pozycji w której znajduje się inny jego statek, to nie powinno mu pozwolić przenieść go wogóle
// jeśli chce przemieścić go na pozycję na której znajduje się statek wroga, powinno go postawić zaraz obok tego statku, najlepiej w kierunku z którego
// ten statek wyruszył
// przykład: 
// gracz 1 ma statek na pozycji 0x 0y, a gracz 2 ma statek na pozycji 2x 2y
// jeśli gracz 1 spróbuje przmieścić swój statek na pozycję 2x 2y, to statek zostanie cofnięty na pozycję 1x 1y, najlepiej refundując
// jeden punkt ruchu statku (czyli zamiast zabrać 2, zabierze 1)
// I never want to have anything to do with coordinates ever again
void order_ship_to_move(std::vector<ship> &player_fleet, std::vector<ship> other_fleet)
{
    // std::cout << "Choose the ship with which you want to move:\n";
    std::cout << "Input the ID of the ship you want to move:\n";
    int id_of_ship_being_moved = get_int_input();
    if (player_fleet[id_of_ship_being_moved].moves_left_in_turn <= 0)
    {
        // std::cout << "This shit cannot move anymore in this turn\n"; -- this was a missclick, but it's so funny
        // that i'll let it be
        std::cout << "This ship cannot move anymore in this turn\n";
        return;
    }
    std::cout << "Input destination coordinates:\n"
    << "x: ";
    player_fleet[id_of_ship_being_moved].destination_x = get_int_input();
    std::cout << "y: ";
    player_fleet[id_of_ship_being_moved].destination_y = get_int_input();
    if (player_fleet[id_of_ship_being_moved].destination_x == player_fleet[id_of_ship_being_moved].position_x && player_fleet[id_of_ship_being_moved].destination_y == player_fleet[id_of_ship_being_moved].position_y)
    {
        return;
    }
    
    // look ok i'm sorry I don't think there is a better way of doing this because of the insane amount
    // of checks we need to make
    ship_next_position_handler(player_fleet, id_of_ship_being_moved, other_fleet);
}

void ship_next_position_handler(std::vector<ship> &current_fleet, int id_selected_ship, std::vector<ship> other_fleet)
{
    int distance_x_to_destination = current_fleet[id_selected_ship].destination_x - current_fleet[id_selected_ship].position_x;
    int distance_y_to_destination = current_fleet[id_selected_ship].destination_y - current_fleet[id_selected_ship].position_y;
    if (std::max(std::abs(distance_x_to_destination), std::abs(distance_y_to_destination)) <= current_fleet[id_selected_ship].moves_left_in_turn)
    {
        ship_destination_clear(current_fleet, id_selected_ship, other_fleet, distance_x_to_destination, distance_y_to_destination);
        current_fleet[id_selected_ship].position_x += distance_x_to_destination;
        current_fleet[id_selected_ship].position_y += distance_y_to_destination;
        current_fleet[id_selected_ship].moves_left_in_turn -= std::max(std::abs(distance_x_to_destination), std::abs(distance_y_to_destination));
        return;
    }
    if (std::abs(distance_x_to_destination) > current_fleet[id_selected_ship].moves_left_in_turn)
    {
        if (distance_x_to_destination > 0)
        {
            current_fleet[id_selected_ship].destination_x = current_fleet[id_selected_ship].position_x + current_fleet[id_selected_ship].moves_left_in_turn;
        }
        else if (distance_x_to_destination < 0)
        {
            current_fleet[id_selected_ship].destination_x = current_fleet[id_selected_ship].position_x - current_fleet[id_selected_ship].moves_left_in_turn;
        }
    }
    if (std::abs(distance_y_to_destination) > current_fleet[id_selected_ship].moves_left_in_turn)
    {
        if (distance_y_to_destination > 0)
        {
            current_fleet[id_selected_ship].destination_y = current_fleet[id_selected_ship].position_y + current_fleet[id_selected_ship].moves_left_in_turn;
        }
        else if (distance_y_to_destination < 0)
        {
            current_fleet[id_selected_ship].destination_y = current_fleet[id_selected_ship].position_y - current_fleet[id_selected_ship].moves_left_in_turn;
        }      
    }  
    ship_destination_clear(current_fleet, id_selected_ship, other_fleet, distance_x_to_destination, distance_y_to_destination);
    // current_fleet[id_selected_ship].position_x += distance_x_to_destination;
    // current_fleet[id_selected_ship].position_y += distance_y_to_destination;
    distance_x_to_destination = current_fleet[id_selected_ship].destination_x - current_fleet[id_selected_ship].position_x;
    distance_y_to_destination = current_fleet[id_selected_ship].destination_y - current_fleet[id_selected_ship].position_y;
    current_fleet[id_selected_ship].position_x = current_fleet[id_selected_ship].destination_x;
    current_fleet[id_selected_ship].position_y = current_fleet[id_selected_ship].destination_y;
    current_fleet[id_selected_ship].moves_left_in_turn -= std::max(std::abs(distance_x_to_destination), std::abs(distance_y_to_destination));
    return;
    
}

void ship_destination_clear(std::vector<ship> &current_fleet, int id_selected_ship, std::vector<ship> other_fleet, int &distance_x_to_destination, int &distance_y_to_destination)
{
    while (ship_finder_at_coordinates(current_fleet, current_fleet[id_selected_ship].destination_x, current_fleet[id_selected_ship].destination_y) || ship_finder_at_coordinates(other_fleet, current_fleet[id_selected_ship].destination_x, current_fleet[id_selected_ship].destination_y))
    {
        if (distance_x_to_destination > 0)
        {
            distance_x_to_destination--;
            current_fleet[id_selected_ship].destination_x = current_fleet[id_selected_ship].position_x + distance_x_to_destination;
        }
        else if (distance_x_to_destination < 0)
        {
            distance_x_to_destination++;
            current_fleet[id_selected_ship].destination_x = current_fleet[id_selected_ship].position_x + distance_x_to_destination;
        }
        if (distance_y_to_destination > 0)
        {
            distance_y_to_destination--;
            current_fleet[id_selected_ship].destination_y = current_fleet[id_selected_ship].position_y + distance_y_to_destination;
        }
        else if (distance_y_to_destination)
        {
            distance_y_to_destination++;
            current_fleet[id_selected_ship].destination_y = current_fleet[id_selected_ship].position_y + distance_y_to_destination;
        }
    }
}

void end_turn_handler(std::vector<ship> &fleet1, std::vector<ship> &fleet2)
{
    for (size_t i = 0; i < fleet1.size(); i++)
    {
        if (fleet1[i].time_to_reload > 0)
        {
            fleet1[i].time_to_reload -= 1;
        }
        fleet1[i].moves_left_in_turn = fleet1[i].speed;
    }
    for (size_t i = 0; i < fleet2.size(); i++)
    {
        if (fleet2[i].time_to_reload > 0)
        {
            fleet2[i].time_to_reload -= 1;
        }
        fleet2[i].moves_left_in_turn = fleet2[i].speed;     
    }
    
}

void ship_killer(std::vector<ship> &fleet)
{
    if (fleet.size() == 1)
    {
        if (fleet[0].hp <= 0)
        {
            fleet.clear();
        }
        
    }
    
    for (size_t i = 0; i < fleet.size(); i++)
    {
        if (fleet[i].hp <= 0)
        {
            fleet.erase(fleet.begin()+i);
        }
        
    }
    
}