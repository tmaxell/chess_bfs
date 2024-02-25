#pragma once
#include<string>
#include<vector>
#include<map>

std::string name_figure[10] = { "e1", "e0", "g1", "g0", "b1", "b0", "n1", "n0", "k1", "k0" };
std::map<int, char> lat = {
    {0, 'a'}, {1, 'b'}, {2, 'c'}, {3, 'd'}, {4, 'e'}, {5, 'f'}, {6 , 'g'}, {7, 'h'}
};


struct coordinate
{
    int x = 0;
    int y = 0;
    coordinate() {}
    coordinate(int new_x, int new_y) : x(new_x), y(new_y) {}
};
bool compare_coordinate(coordinate first, coordinate second) {
    bool coor_x = first.x == second.x;
    bool coor_y = first.y == second.y;
    return coor_x * coor_y;
}

struct step
{
    char step_figure;
    coordinate from;
    coordinate to;
    step(){}
    step(char this_figure, coordinate from_place): step_figure(this_figure), from(from_place){}
    step(char this_figure, coordinate from_place, coordinate to_place): step_figure(this_figure), from(from_place), to(to_place){}
    void print() {
        std::cout << step_figure << ' ' << "(" << from.x << ", " << lat[from.y] << ") - (" << to.x << ", " << lat[to.y] << ")" << std::endl;
    }
};

template <typename T>
struct history_game
{
    T now_board;
    int num_step = 0;
    int price = 0;
    std::vector<T> history_board;
    std::vector<step> history_steps;
    history_game(){}
    history_game(T start_board){}
};
