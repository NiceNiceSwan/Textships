#include <iostream>
#include <vector>
#include <conio.h>
#include "classes.h"
#include "functions.h"

const int map_max_size_x = 50;
const int map_max_size_y = 50;

int main()
{
    std::cout << "Textships v0.2.2\nMade by Swan\n";
    std::cout << "Press any key to continue\n";
    getch();

    srand(time(NULL));
    std::vector<ship> fleet1 = {ship(0,1), ship(0,2), ship(1,1), ship(1,2), ship(2,1)};
    std::vector<ship> fleet2 = {ship(0,1), ship(0,2), ship(1,1), ship(1,2), ship(2,1)};

    /* Testing code
    std::vector<ship> fleet1 = {ship(0,1), ship(0,2), ship(1,1), ship(1,2), ship(2,1)};
    std::vector<ship> fleet2 = {ship(0,1)};
    */

    starting_setup(fleet1, 1, 0, 10, 0, 10);
    std::cout << "Waiting for 2nd player, press any key when ready";
    getch();
    starting_setup(fleet2, 2, 40, 50, 40, 50);

    /* Testing code
    starting_setup(fleet1, 1, 0, 50, 0, 50);
    std::cout << "Waiting for 2nd player, press any key when ready";
    getch();
    starting_setup(fleet2, 2, 0, 50, 0, 50);
    */

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
    
    clear_screen_func();
    if (fleet1.size() == 0)
    {
        std::cout << "Player 2 Won!\nPress any key to exit the game\n";
    }
    else if (fleet2.size() == 0)
    {
        std::cout << "Player 1 Won!\nPress any key to exit the game\n";
    }
    getch();
}