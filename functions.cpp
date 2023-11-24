#include "functions.h"

int get_choice()
{
    int choice = 0;
    std::cin >> choice;
    while (std::cin.fail())
    {
        std::cout << "Wrong input\n";
        std::cin.clear();
        std::cin.ignore(1000,'\n');
        std::cin>>choice;
    }
    std::cout << "\n";
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
        std::cin >> ship[i].position_x;
        std::cout << "Starting position y: ";
        std::cin >> ship[i].position_y;
        while (!is_starting_position_valid(i, ship, max_x, max_y))
        {
            std::cin.clear();
            std::cin.ignore(1000,'\n');
            std::cout << "Invalid starting position, please try again\n";
            std::cout << "Starting position x: ";
            std::cin >> ship[i].position_x;
            std::cout << "Starting position y: ";
            std::cin >> ship[i].position_y;
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
    do
    {
        // TODO: change this so that the ship ID input goes first, we could do a reload check first before moving
        // on to inputing target coordinates
        std::cout << "Input target coordinates:\n"
        << "x: ";
        std::cin >> x;
        std::cout << "y: ";
        std::cin >> y;
        std::cout << "Choose the ship you want to attack with: ";
        std::cin >> attacking_ship_id;
        distance_x = std::abs(x - attacking_fleet[attacking_ship_id].position_x);
        distance_y = std::abs(x - attacking_fleet[attacking_ship_id].position_y);
        if (distance_x > attacking_fleet[attacking_ship_id].gun_range || distance_y > attacking_fleet[attacking_ship_id].gun_range)
        {
            std::cout << "Target out of range, try again\n";
            ready_to_fire = false;
        }
        else if (attacking_fleet[attacking_ship_id].time_to_reload > 0)
        {
            std::cout << "Ship has not reloaded it's guns yet, try again\n";
            ready_to_fire = false;
        }
        else
        {
            ready_to_fire = true;
        }
    } while (!ready_to_fire);
    // FIXME: we have no way of leaving the loop if no target is in range or no ship has it's guns reloaded
    std::cout << "The ship is ready to fire, are you sure you want to fire? (0 - No, 1 - Yes): ";
    std::cin >> ready_to_fire;
    // I could put this in a separate function too
    int targeted_ship_id;
    if (ready_to_fire)
    {
        if (ship_finder_at_coordinates(target_fleet, x, y, targeted_ship_id))
        {
            shot_handler(attacking_fleet[attacking_ship_id], target_fleet[targeted_ship_id], spotted_ships_id[targeted_ship_id]);
            // TODO: all the stuff that happens when a ship's HP reaches 0
            // also some way to notify the other player that his ship has been damaged / sunk
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

bool player_interface(int player, std::vector<ship> &current_player_fleet, std::vector<ship> &other_player_fleet)
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
    std::cout << "\nChoose option: "
    << "\n(0) End turn"
    << "\n(1) Show detailed ship info"
    << "\n(2) Fire at coordinates"
    << "\n(3) Order ships to move\n";
    int ship_id_to_get_detailed_info;
    bool end_turn_prompt;
    switch (get_choice())
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
        std::cin >> ship_id_to_get_detailed_info;
        current_player_fleet[ship_id_to_get_detailed_info].print_detailed_ship_info();
        return true;
        break;
    case 2:
        fire_at_coordinates(current_player_fleet, other_player_fleet, spotted_ships_id);
        return true;
        break;
    case 3:
        // TODO: a function to have the ship move to destination coordinates, probably using the bresenham's line algorithm
        // although we may ignore the algorithm if the destination is within the ship's move range
        std::cout << "Order ships to move placeholder\n"; 
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
void ship_movement_handler(std::vector<ship> &current_player_fleet, std::vector<ship> other_player_fleet)
{
    // I will have to check not only for other ships of the same player, but also ships of the other player. 
    // This makes me depressed
    std::cout << "Choose the ship with which you want to move:\n";
    int id_of_ship_being_moved;
    std::cin >> id_of_ship_being_moved;
    std::cout << "Input destination coordinates:\n"
    << "x: ";
    std::cin >> current_player_fleet[id_of_ship_being_moved].destination_x;
    std::cout << "y: ";
    std::cin >> current_player_fleet[id_of_ship_being_moved].destination_y;
}

void ship_mover(ship &ship_being_moved, std::vector<ship> other_player_fleet)
{
    int distance_x_to_destination = std::abs(ship_being_moved.position_x - ship_being_moved.destination_x);
    int distance_y_to_destination = std::abs(ship_being_moved.position_y - ship_being_moved.destination_y);
    if (distance_x_to_destination <= ship_being_moved.speed)
    {
        for (size_t i = 0; i < other_player_fleet.size(); i++)
        {
            if (destination_same_as_another_ships_position(ship_being_moved, other_player_fleet))
            {
                /* code */
            }
            
        }
        
    }
    
}

bool destination_same_as_another_ships_position(ship ship_being_moved, std::vector<ship> fleet_to_check)
{
    for (size_t i = 0; i < fleet_to_check.size(); i++)
    {
        if (ship_being_moved.destination_x == fleet_to_check[i].position_x && ship_being_moved.destination_y == fleet_to_check[i].position_y)
        {
            return true;
        }
    }
    return false;
    
}
