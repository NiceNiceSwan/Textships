#include <iostream>
#include <vector>
#include <conio.h>
#include "classes.h"
#include "functions.h"

int main()
{
    srand(time(NULL));
    int map_max_size_x = 50;
    int map_max_size_y = 50;
    std::vector<ship> fleet1 = {ship(0,1)};
    std::vector<ship> fleet2 = {ship(1,1),ship(2,1)};
    std::cout << "Player 1 configuration\n";
    //TODO: define a spawning box so that we won't have to worry about the players spawning ships on top of another player's ship.
    // int *player_left_x_spawning_boundary = new int;
    // int *player_left_y_spawning_boundary = new int;
    // int *player_right_x_spawning_boundary = new int;
    // int *player_right_y_spawning_boundary = new int;
    // *player_left_x_spawning_boundary = 10;
    // *player_left_y_spawning_boundary = 0;
    // *player_right_x_spawning_boundary = 40;
    // *player_right_y_spawning_boundary = 10;
    player_configuration(fleet1, map_max_size_x, map_max_size_y);
    system("CLS");
    std::cout << "Waiting for 2nd player, press any key when ready";
    getch();
    system("CLS");
    std::cout << "Player 2 configuration:\n";
    player_configuration(fleet2, map_max_size_x, map_max_size_y);
    // delete player_left_x_spawning_boundary;
    // delete player_left_y_spawning_boundary;
    // delete player_right_x_spawning_boundary;
    // delete player_right_y_spawning_boundary;
    // player_left_x_spawning_boundary = nullptr;
    // player_left_y_spawning_boundary = nullptr;
    // player_right_x_spawning_boundary = nullptr;
    // player_right_y_spawning_boundary = nullptr;
    system("CLS");
    while (!(fleet1.empty() || fleet2.empty()))
    {
        std::cout << "Player 1 turn, press any key to continue";
        getch();
        system("CLS");
        while (player_interface(1,fleet1,fleet2))
        {
        }
        std::cout << "Player 2 turn, press any key to continue";
        getch();
        system("CLS");
        while (player_interface(2,fleet2,fleet1))
        {
        }
        // TODO: some way to notify the players that their ships have been damaged/destroyed
    }
    
    // TODO: pętla tur, gdzie momentem końcowym będzie kompletne zniszczenie jednej z flot. Nie, nie będzie opcji wyjścia wcześniej :)
    player_interface(1,fleet1,fleet2);
}