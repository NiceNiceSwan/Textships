#include "functions.h"

void clear_screen_func()
{
    HANDLE                     hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD                      count;
    DWORD                      cellCount;
    COORD                      homeCoords = { 0, 0 };

    hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
    if (hStdOut == INVALID_HANDLE_VALUE) return;

    /* Get the number of cells in the current buffer */
    if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
    cellCount = csbi.dwSize.X *csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &count
    )) return;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &count
    )) return;

    /* Move the cursor home */
    SetConsoleCursorPosition( hStdOut, homeCoords );
}
// this is a test
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

bool is_starting_position_valid(int ship_id, std::vector<ship> fleet, int min_x, int max_x, int min_y, int max_y)
{
    if (fleet[ship_id].position_x < min_x || fleet[ship_id].position_x > max_x || fleet[ship_id].position_y < min_y || fleet[ship_id].position_y > max_y)
    {
        std::cout << "Invalid ship position, try again\n";
        return false;
    }
    for (size_t i = 0; i < fleet.size(); i++)
    {
        if (i == ship_id)
        {
            continue;
        }
        if (fleet[ship_id].position_x == fleet[i].position_x && fleet[ship_id].position_y == fleet[i].position_y)
        {
            std::cout << "Invalid ship position, try again\n";
            return false;
        }
    }
    return true;
}

void starting_setup(std::vector<ship> &player_fleet, int player_num, int low_x_boundary, int high_x_boundary, int low_y_boundary, int high_y_boundary)
{
    clear_screen_func();
    std::cout << "Player " << player_num << " configuration\n";
    player_configuration(player_fleet, low_x_boundary, high_x_boundary, low_y_boundary, high_y_boundary);
    clear_screen_func();
}

void player_configuration(std::vector<ship> &fleet, int min_x, int max_x, int min_y, int max_y)
{
    std::cout << "Your spawning boundaries are from " << min_x << "x to " << max_x << "x and from " << min_y << "y to " << max_y << "y\n";
    std::cout << "Configure the starting positions of your ships:\n";
    for (size_t i = 0; i < fleet.size(); i++)
    {
        std::cout << fleet[i].ship_class << "\n";
        do
        {
            std::cout << "Starting position x: ";
            fleet[i].position_x = get_int_input();
            std::cout << "Starting position y: ";
            fleet[i].position_y = get_int_input();
        } while (!is_starting_position_valid(i, fleet, min_x, max_x, min_y, max_y));
    }
}

void ship_spoting_handler(std::vector<ship> fleet_that_spots, std::vector<ship> fleet_being_spotted, std::vector<bool> &spotted_ship_table)
{
    for (size_t i = 0; i < fleet_that_spots.size(); i++)
    {
        for (size_t j = 0; j < fleet_being_spotted.size(); j++)
        {
            if (spotted_ship_table[j])
            {
                continue;
            }
            ship_spotter(fleet_that_spots[i], fleet_being_spotted[j], j, spotted_ship_table);
        }   
    }
}

void ship_spotter(ship ship_that_spots, ship ship_being_spotted, int id_of_spotted_ship, std::vector<bool> &spotted_ship_table)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    int distance_x_between_ships = std::abs(ship_that_spots.position_x - ship_being_spotted.position_x);
    int distance_y_between_ships = std::abs(ship_that_spots.position_y - ship_being_spotted.position_y);
    if (distance_x_between_ships <= ship_being_spotted.certain_detection_range && distance_y_between_ships <= ship_being_spotted.certain_detection_range)
    {
        // "This is terrible, but VGUI has forced my hand"
        spotted_ship_table[id_of_spotted_ship] = true;
        SetConsoleTextAttribute(h, 10);
        std::cout << "\n" << ship_that_spots.ship_class;
        SetConsoleTextAttribute(h,7);
        std::cout << " is spotting ";
        SetConsoleTextAttribute(h,12);
        std::cout << ship_being_spotted.ship_class;
        SetConsoleTextAttribute(h,7);
        std::cout << ". ";
        SetConsoleTextAttribute(h,12);
        std::cout << ship_being_spotted.ship_class;
        SetConsoleTextAttribute(h,7);
        std::cout << " position is:\n"
        << ship_being_spotted.position_x << "x\n"
        << ship_being_spotted.position_y << "y\n";
        if (distance_x_between_ships <= ship_that_spots.certain_detection_range && distance_y_between_ships <= ship_that_spots.certain_detection_range)
        {
            std::cout << "Because of how close we are, ";
            SetConsoleTextAttribute(h,10);
            std::cout << ship_that_spots.ship_class;
            SetConsoleTextAttribute(h,7);
            std::cout << " was spotted as well\n";
        }
        else if (distance_x_between_ships <= ship_that_spots.uncertain_detection_range && distance_y_between_ships <= ship_that_spots.uncertain_detection_range)
        {
            std::cout << "Because of how close we are, ";
            SetConsoleTextAttribute(h,10);
            std::cout << ship_that_spots.ship_class;
            SetConsoleTextAttribute(h,7);
            std::cout << " may have been spotted as well\n";
        }   
    }
    else if (distance_x_between_ships <= ship_being_spotted.uncertain_detection_range && distance_y_between_ships <= ship_being_spotted.uncertain_detection_range && rand()%100 + 1 > 65)
    {
        spotted_ship_table[id_of_spotted_ship] = true;
        SetConsoleTextAttribute(h, 10);
        std::cout << "\n" << ship_that_spots.ship_class;
        SetConsoleTextAttribute(h, 7);
        std::cout << " has managed to spot ";
        SetConsoleTextAttribute(h,12);
        std::cout << ship_being_spotted.ship_class;
        SetConsoleTextAttribute(h,7);
        std::cout << ". ";
        SetConsoleTextAttribute(h,12);
        std::cout << ship_being_spotted.ship_class;
        SetConsoleTextAttribute(h,7);
        std::cout << " position is:\n"
        << ship_being_spotted.position_x << "x\n"
        << ship_being_spotted.position_y << "y\n";
        if (distance_x_between_ships <= ship_that_spots.certain_detection_range && distance_y_between_ships <= ship_that_spots.certain_detection_range)
        {
            std::cout << "Because of how close we are, "
            << ship_that_spots.ship_class << " was spotted as well\n";
        }
        else if (distance_x_between_ships <= ship_that_spots.uncertain_detection_range && distance_y_between_ships <= ship_that_spots.uncertain_detection_range)
        {
            std::cout << "Because of how close we are, "
            << ship_that_spots.ship_class << " may have been spotted as well\n";
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
    int damage_dealt;
    if (attacking_ship.armor_penetration == 0)
    {
        damage_dealt = attacking_ship.firepower - targeted_ship.armor;
        if (damage_dealt < 0)
        {
            damage_dealt = 0;
            return damage_dealt;
        }
        
        targeted_ship.hp -= damage_dealt;
        return damage_dealt;
    }
    else
    {
        damage_dealt = attacking_ship.firepower - ((float)targeted_ship.armor / attacking_ship.armor_penetration);
        if (damage_dealt < 0)
        {
            damage_dealt = 0;
            return damage_dealt;
        }
        
        targeted_ship.hp -= damage_dealt;
        return damage_dealt;
    }
}

void fire_at_coordinates(std::vector<ship> &attacking_fleet, std::vector<ship> &target_fleet, std::vector<bool> spotted_ships_id)
{
    int x;
    int y;
    int attacking_ship_id;
    bool ready_to_fire;
    // maybe I could put it in a separate function if I feel like this block of code is getting too long and unwieldy
    // TODO: ability to call ships by their name and number rather than ID
    // std::cout << "Choose the ship you want to attack with: ";
    std::cout << "\nInput the ID of the ship you want to fire with: ";
    attacking_ship_id = get_int_input();
    if (attacking_fleet[attacking_ship_id].time_to_reload > 0)
    {
        std::cout << "Ship has not reloaded it's guns yet, aborting\n";
        return;
    }
    if (attacking_ship_id > attacking_fleet.size())
    {
        std::cout << "No ship with such id, aborting\n";
        return;
    }
    

    std::cout << "Input target coordinates:\n"
    << "x: ";
    x = get_int_input();
    std::cout << "y: ";
    y = get_int_input();
    int distance_x = std::abs(x - attacking_fleet[attacking_ship_id].position_x);
    int distance_y = std::abs(y - attacking_fleet[attacking_ship_id].position_y);
    if (distance_x > attacking_fleet[attacking_ship_id].gun_range || distance_y > attacking_fleet[attacking_ship_id].gun_range)
    {
        std::cout << "Target out of range, aborting\n";
        return;
    }

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

void short_info_print(std::vector<ship> fleet)
{
    std::cout << "\nShip statistics: \n";
    for (size_t i = 0; i < fleet.size(); i++)
    {
        std::cout << "\nShip ID: " << i;
        fleet[i].print_short_ship_info();
    }
    
}

void player_interface(int player, std::vector<ship> &current_player_fleet, std::vector<ship> &other_player_fleet)
{
    std::cout << "Player " << player << " turn\n";
    // std::cout << "Ship statistics: \n";
    // for (size_t i = 0; i < current_player_fleet.size(); i++)
    // {
    //     std::cout << "\nShip ID: " << i;
    //     current_player_fleet[i].print_short_ship_info();
    // }
    short_info_print(current_player_fleet);
    std::cout << "\n\nDetected enemy ships:\n"; 
    std::vector<bool> spotted_ships_id(other_player_fleet.size(), false);
    ship_spoting_handler(current_player_fleet, other_player_fleet, spotted_ships_id);
    while (main_action_selector(current_player_fleet, other_player_fleet, spotted_ships_id))
    {
    }
    
}

bool main_action_selector(std::vector<ship> &current_player_fleet, std::vector<ship> &other_player_fleet, std::vector<bool> spotted_ships_id)
{
    std::cout << "\nChoose option: "
    << "\n(0) End turn"
    << "\n(1) Clear screen"
    << "\n(2) Re-print short ship info"
    << "\n(3) Show detailed ship info"
    // << "\n(2) Show detailes spotting info"
    << "\n(4) Fire at coordinates"
    << "\n(5) Order ships to move\n";
    int ship_id_to_get_detailed_info;
    bool end_turn_prompt;
    switch (get_int_input())
        {
        case 0:
            std::cout << "\nAre you sure you want to end your turn?\n"
            << "(0) - No\n(1) - Yes\n";
            std::cin >> end_turn_prompt;
            if (end_turn_prompt)
            {
                return false;
            }
            return true;
            break;
        case 1:
            clear_screen_func();
            short_info_print(current_player_fleet);
            ship_spoting_handler(current_player_fleet, other_player_fleet, spotted_ships_id);
            return true;
            break;
        case 2:
            short_info_print(current_player_fleet);
            return true;
            break;
        case 3:
            std::cout << "\nInput the ID of the ship you want to see\n";
            ship_id_to_get_detailed_info = get_int_input();
            std:: cout << "\n";
            if (ship_id_to_get_detailed_info > current_player_fleet.size())
            {
                std::cout << "No ship with such ID\n";
                return true;
            }
            
            current_player_fleet[ship_id_to_get_detailed_info].print_detailed_ship_info();
            return true;
            break;
        // case 2:
        //     // TODO: detailed spotting info
        //     std::cout << "Detailed spotting info placeholder\n";
        //     return true;
        //     break;
        case 4:
            fire_at_coordinates(current_player_fleet, other_player_fleet, spotted_ships_id);
            return true;
            break;
        case 5:
            order_ship_to_move(current_player_fleet, other_player_fleet);
            ship_spoting_handler(current_player_fleet, other_player_fleet, spotted_ships_id);
            return true;
            break;
        default:
            std::cout << "Wrong input\n";
            return true;
            break;
        }
}

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
    std::cout << "\nInput the ID of the ship you want to move:\n";
    int id_of_ship_being_moved = get_int_input();
    if (player_fleet[id_of_ship_being_moved].moves_left_in_turn <= 0)
    {
        // std::cout << "This shit cannot move anymore in this turn\n"; -- this was a missclick, but it's so funny
        // that i'll let it be
        std::cout << "This ship cannot move anymore in this turn\n";
        return;
    }
    if (id_of_ship_being_moved > player_fleet.size())
    {
        std::cout << "No ship with such ID, aborting\n";
        return;
    }
    

    std::cout << "Input destination coordinates:\n"
    << "x: ";
    player_fleet[id_of_ship_being_moved].destination_x = get_int_input();
    if (player_fleet[id_of_ship_being_moved].destination_x > map_max_size_x || player_fleet[id_of_ship_being_moved].destination_x < 0)
    {
        std::cout << "Wrong destination, aborting\n";
        return;
    }
    
    std::cout << "y: ";
    player_fleet[id_of_ship_being_moved].destination_y = get_int_input();
    if (player_fleet[id_of_ship_being_moved].destination_y > map_max_size_y || player_fleet[id_of_ship_being_moved].destination_y < 0)
    {
        std::cout << "Wrong destination, aborting\n";
        return;
    }

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

// void print_detailed_spotting_info(std::vector<bool> spotted_ships_id, std::vector<ship> fleet)
// {
//     for (size_t i = 0; i < fleet.size(); i++)
//     {
//         if (spotted_ships_id[i])
//         {
            
//         }
        
//     }
    
// }
