#include <iostream>
#include <vector>
#include <conio.h>
#include "classes.h"
#include "functions.h"

const int map_max_size_x = 50;
const int map_max_size_y = 50;

int main()
{
    srand(time(NULL));
    std::vector<ship> fleet1 = {ship(0,1), ship(0,2), ship(1,1), ship(1,2), ship(2,1)};
    std::vector<ship> fleet2 = {ship(0,1), ship(0,2), ship(1,1), ship(1,2), ship(2,1)};
    std::cout << "Player 1 configuration\n";
    //TODO: define a spawning box so that we won't have to worry about the players spawning ships on top of another player's ship.
    // TODO: put this into a function so we could throw away the spawning boundaries after set up
    player_configuration(fleet1, map_max_size_x, map_max_size_y);
    clear_screen_func();
    std::cout << "Waiting for 2nd player, press any key when ready";
    getch();
    clear_screen_func();
    std::cout << "Player 2 configuration:\n";
    player_configuration(fleet2, map_max_size_x, map_max_size_y);
    while (!(fleet1.size() == 0 || fleet2.size() == 0))
    {
        clear_screen_func();
        std::cout << "Player 1 turn, press any key to continue";
        getch();
        clear_screen_func();
        player_interface(1,fleet1,fleet2);
        ship_killer(fleet2);
        if (fleet2.size() == 0)
        {
            break;
        }
        clear_screen_func();
        std::cout << "Player 2 turn, press any key to continue";
        getch();
        clear_screen_func();
        player_interface(2,fleet2,fleet1);
        ship_killer(fleet1);
        end_turn_handler(fleet1, fleet2);
        if (fleet1.size() == 0)
        {
            break;
        }
        
        // TODO: some way to notify the players that their ships have been damaged/destroyed
    }
}