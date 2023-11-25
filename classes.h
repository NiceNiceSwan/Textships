#ifndef CLASSES_H
#define CLASSES_H
#include <string>
#include <iostream>

class ship
{
    public:
        std::string ship_class;
        int hp;
        int armor;
        int firepower;
        int armor_penetration;
        int gun_range;
        int reload_time;
        int time_to_reload;
        int accuracy;
        int dodging_target_hit_chance;
        int speed;
        int moves_left_in_turn;
        int dodge_chance;
        int certain_detection_range;
        int uncertain_detection_range;
        int position_x;
        int position_y;
        int destination_x;
        int destination_y;
        int cost;

        //ship type functions
        void battleship(int num);
        void cruiser(int num);
        void destroyer(int num);
        // void carrier();
        void print_detailed_ship_info();
        void print_short_ship_info();

        // ship constructor
        ship(int type, int num)
        {
            switch (type)
            {
            case 0:
                destroyer(num);
                break;
            case 1:
                cruiser(num);
                break;
            case 2:
                battleship(num);
                break;
            // case 3:
            //     carrier();
            //     break;
            default:
                break;
            }
        }
};



#endif