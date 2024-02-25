#pragma once
#include "struct.h"
#include "figure.h"
#include<vector>
#include<iostream>

using namespace std;
struct class_using_figures
{
    vector<vector<e>> using_figures_e = vector<vector<e>>(2);
    vector<vector<b>> using_figures_b = vector<vector<b>>(2);
    vector<vector<n>> using_figures_n = vector<vector<n>>(2);
    vector<vector<g>> using_figures_g = vector<vector<g>>(2);
    vector<vector<k>> using_figures_k = vector<vector<k>>(2);
};

class board
{
    vector<vector<figure*> > play_board = vector<vector<figure*>>(8, vector<figure*>(8, nullptr));
    vector<vector<figure*> > using_figures = vector<vector<figure*>>(2);
    class_using_figures class_figures = class_using_figures();
public:
    board(){}
private:
    void update_pointer() {
        coordinate place;
        play_board.clear();
        play_board = vector<vector<figure*>>(8, vector<figure*>(8, nullptr));
        for (int i = 0; i < 2; ++i) {
            using_figures[i].clear();
            for (int j = 0; j < class_figures.using_figures_e[i].size(); ++j) {
                place = class_figures.using_figures_e[i][j].get_place();
                using_figures[i].push_back(&class_figures.using_figures_e[i][j]);
                play_board[place.x][place.y] = &class_figures.using_figures_e[i][j];
            }
            for (int j = 0; j < class_figures.using_figures_b[i].size(); ++j) {
                place = class_figures.using_figures_b[i][j].get_place();
                using_figures[i].push_back(&class_figures.using_figures_b[i][j]);
                play_board[place.x][place.y] = &class_figures.using_figures_b[i][j];

            }
            for (int j = 0; j < class_figures.using_figures_n[i].size(); ++j) {
                place = class_figures.using_figures_n[i][j].get_place();
                using_figures[i].push_back(&class_figures.using_figures_n[i][j]);
                play_board[place.x][place.y] = &class_figures.using_figures_n[i][j];
            }
            for (int j = 0; j < class_figures.using_figures_g[i].size(); ++j) {
                place = class_figures.using_figures_g[i][j].get_place();
                using_figures[i].push_back(&class_figures.using_figures_g[i][j]);
                play_board[place.x][place.y] = &class_figures.using_figures_g[i][j];
            }
            for (int j = 0; j < class_figures.using_figures_k[i].size(); ++j) {
                place = class_figures.using_figures_k[i][j].get_place();
                using_figures[i].push_back(&class_figures.using_figures_k[i][j]);
                play_board[place.x][place.y] = &class_figures.using_figures_k[i][j];
            }
        }
    }

    bool steps_is_true(figure* this_figure, coordinate new_place, bool delete_last)
    {
        vector<coordinate> steps = this_figure->generation_eat_direction(new_place);
        if (delete_last) {
            steps.pop_back();
        }
        for (int i = 0; i < steps.size(); ++i) {
            if (play_board[steps[i].x][steps[i].y] != nullptr) {
                return 0;
            }
        }
        return 1;
    }
    bool compare_figure(figure* first, figure* second) {
        bool type = first->get_type() == second->get_type();
        bool color = first->get_color() == second->get_color();
        bool price = first->get_price() == second->get_price();
        bool place = compare_coordinate(first->get_place(), second->get_place());
        return type * color * price * place;
    }
public:
    vector<vector<figure*>> get_board()
    {
        return play_board;
    }

    vector<vector<figure*>> get_using_figures()
    {
        return using_figures;
    }

    class_using_figures get_using_class_figures()
    {
        return class_figures;
    }

    figure* get_figure_board(coordinate coor)
    {
        return play_board[coor.x][coor.y];
    }

    figure* get_king(bool color) {
        coordinate place_king = class_figures.using_figures_k[color][0].get_place();
        return get_figure_board(place_king);
    }

    void print()
    {
        map<int, char> lat = {
            {0, 'a'}, {1, 'b'}, {2, 'c'}, {3, 'd'}, {4, 'e'}, {5, 'f'}, {6 , 'g'}, {7, 'h'}
        };

        update_pointer();
        cout << "X ";
        for (int i = 0; i < 8; ++i)
        {
            cout << " " << lat[i] << " ";
        }
        cout << endl;
        for (int i = 0; i < 8; ++i)
        {
            cout << i << "|";
            for (int j = 0; j < 8; ++j)
            {
                if (play_board[j][i] == nullptr) {
                    cout << ' ' << ' ' << "|";
                }
                else {
                    cout << play_board[j][i]->get_type() + to_string(play_board[j][i]->get_color()) << "|";
                }
            }
            cout << endl;
        }
    }

    void copy_board(board &new_board)
    {
        class_figures = new_board.get_using_class_figures();
        update_pointer();
    }

    void add_figure(figure* new_figure) {
        if (new_figure->get_type() == 'e') {
            class_figures.using_figures_e[new_figure->get_color()].push_back(e(new_figure->get_color(), new_figure->get_place()));
        }
        else if (new_figure->get_type() == 'b') {
            class_figures.using_figures_b[new_figure->get_color()].push_back(b(new_figure->get_color(), new_figure->get_place()));
        }
        else if (new_figure->get_type() == 'n') {
            class_figures.using_figures_n[new_figure->get_color()].push_back(n(new_figure->get_color(), new_figure->get_place()));
        }
        else if (new_figure->get_type() == 'g') {
            class_figures.using_figures_g[new_figure->get_color()].push_back(g(new_figure->get_color(), new_figure->get_place()));
        }
        else if (new_figure->get_type() == 'k') {
            class_figures.using_figures_k[new_figure->get_color()].push_back(k(new_figure->get_color(), new_figure->get_place()));
        }
        update_pointer();

    }
    void delete_figure(figure* delete_figure) {
        if (delete_figure->get_type() == 'e') {
            for (int i = 0; i < class_figures.using_figures_e[delete_figure->get_color()].size(); ++i) {
                figure* may_be_figure = &class_figures.using_figures_e[delete_figure->get_color()][i];
                if (compare_figure(may_be_figure, delete_figure)) {
                    class_figures.using_figures_e[delete_figure->get_color()].erase(class_figures.using_figures_e[delete_figure->get_color()].begin() + i);
                }
            }
        }
        else if (delete_figure->get_type() == 'b') {
            for (int i = 0; i < class_figures.using_figures_b[delete_figure->get_color()].size(); ++i) {
                figure* may_be_figure = &class_figures.using_figures_b[delete_figure->get_color()][i];
                if (compare_figure(may_be_figure, delete_figure)) {
                    class_figures.using_figures_b[delete_figure->get_color()].erase(class_figures.using_figures_b[delete_figure->get_color()].begin() + i);
                }
            }
        }
        else if (delete_figure->get_type() == 'n') {
            for (int i = 0; i < class_figures.using_figures_n[delete_figure->get_color()].size(); ++i) {
                figure* may_be_figure = &class_figures.using_figures_n[delete_figure->get_color()][i];
                if (compare_figure(may_be_figure, delete_figure)) {
                    class_figures.using_figures_n[delete_figure->get_color()].erase(class_figures.using_figures_n[delete_figure->get_color()].begin() + i);
                }
            }
        }
        else if (delete_figure->get_type() == 'g') {
            for (int i = 0; i < class_figures.using_figures_g[delete_figure->get_color()].size(); ++i) {
                figure* may_be_figure = &class_figures.using_figures_g[delete_figure->get_color()][i];
                if (compare_figure(may_be_figure, delete_figure)) {
                    class_figures.using_figures_g[delete_figure->get_color()].erase(class_figures.using_figures_g[delete_figure->get_color()].begin() + i);
                }
            }
        }
        update_pointer();
    }

    bool move_is_true(figure* this_figure, coordinate new_place) {
        if (this_figure->move_is_true(new_place)) {
            if (steps_is_true(this_figure, new_place, 0)) {
                return 1;
            }
        }
        return 0;
    }

    void move_figure(figure* this_figure, coordinate new_place) {
        this_figure->move(new_place);
        update_pointer();
    }

    bool eat_is_true(figure* this_figure, figure* eat_figure) {
        if (this_figure->eat_figure_is_true(eat_figure->get_place()) && this_figure->get_color() != eat_figure->get_color() && eat_figure != nullptr) {
            if (steps_is_true(this_figure, eat_figure->get_place(), 1)) {
                return 1;
            }
        }
        return 0;
    }

    void eat_figure(figure* this_figure, figure* eat_figure) {
        this_figure->move(eat_figure->get_place());
        delete_figure(eat_figure);
        update_pointer();
    }

    bool identify_check(bool color) {
        figure* king = get_king(color);
        for (int i = 0; i < using_figures[!color].size(); ++i) {
            figure* check_figure = using_figures[!color][i];
            if (eat_is_true(check_figure, king)) {
                return 1;
            }
        }
        return 0;
    }
    void clear() {
        play_board = vector<vector<figure*>>(8, vector<figure*>(8, nullptr));
        using_figures = vector<vector<figure*>>(2);
        class_figures = class_using_figures();
    }

};
