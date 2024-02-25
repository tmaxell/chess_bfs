#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <random>
#include <ctime>
#include <climits>
#include <map>
#include <queue>
#include <list>
#include "struct.h"
#include "figure.h"
#include "board.h"

using namespace std;

bool comp(history_game<board> first, history_game<board> second) {
    if (first.price != second.price) {
        return first.price > second.price;
    }
    else {
        return first.num_step <= second.num_step;
    }
}

void insertionSort(vector<history_game<board>>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        history_game<board> key = arr[i];
        int j = i - 1;
        while (j >= 0 && comp(key, arr[j])) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

map<string, int> write_all_figures()
{
    cout << "Select the number of figures of certain types you want to place" << endl;
    cout << "First input for white, then for black. For each side, there will always be at least a king, knight, bishop" << endl;
    map<string, int> write_figure = { {"e1", -1}, {"e0", -1}, {"b1", -1}, {"b0", -1}, {"n1", -1}, {"n0", -1}, {"g1", -1}, {"g0", -1}, {"k1", 1}, {"k0", 1} };
    map<char, int> figure_limits = { {'e', 8}, {'b', 1}, {'n', 1}, {'g', 2} };
    bool data_correct = false;
    while (!data_correct) {
        cout << "Pawn (0-8): ";
        cin >> write_figure["e1"] >> write_figure["e0"];
        cout << "Bishop (0-1): ";
        cin >> write_figure["b1"] >> write_figure["b0"];
        cout << "Knight (0-1): ";
        cin >> write_figure["n1"] >> write_figure["n0"];
        cout << "Rook (0-2): ";
        cin >> write_figure["g1"] >> write_figure["g0"];
        data_correct = true;
        for (int i = 0; i < 8; ++i) {
            if ((write_figure[name_figure[i]] < 0) || (write_figure[name_figure[i]] > figure_limits[name_figure[i][0]])) {
                data_correct = false;
                cout << "You entered something incorrectly" << endl;
                break;
            }
        }
    }
    write_figure["b1"]++;
    write_figure["b0"]++;
    write_figure["n1"]++;
    write_figure["n0"]++;
    return write_figure;
}

void board_generation(board& playing_board, map<string, int> using_figures)
{
    srand(time(NULL));
    int m = -2;
    const int num_steps = 50;
    const int num_crash = 1000;

    for (int i = 0; i < 10; ++i) {
        bool color = name_figure[i][1] - '0';
        m += color;
        coordinate coor;
        for (int j = 0; j < using_figures[name_figure[i]]; ++j) {
            figure* add_this_figure = nullptr;
            if (color) {
                coor.x = 0;
            }
            else {
                coor.x = 7;
            }
            if (name_figure[i][0] == 'e') {
                if (color) { coor.x++; }
                else { coor.x--; }
                while (true) {
                    coor.y = rand() % 8;
                    if (playing_board.get_figure_board(coor) == nullptr) {
                        e figure_for_add(color, coor);
                        add_this_figure = &figure_for_add;
                        playing_board.add_figure(add_this_figure);
                        break;
                    }
                }
            }
            else {
                while (true) {
                    bool take_place = rand() % 2;
                    figure* type_cage;
                    if (take_place) {
                        coor.y = m;
                    }
                    else {
                        coor.y = 7 - m;
                    }
                    type_cage = playing_board.get_figure_board(coor);
                    if (type_cage == nullptr) {
                        if (m == 0) { g figure_for_add(color, coor);
                        add_this_figure = &figure_for_add;
                        }
                        else if (m == 1) { b figure_for_add(color, coor);
                        add_this_figure = &figure_for_add;
                        }
                        else if (m == 2) { n figure_for_add(color, coor);
                        add_this_figure = &figure_for_add;
                        }
                        else if (m == 3) { k figure_for_add(color, coor);
                        add_this_figure = &figure_for_add;
                        }
                        playing_board.add_figure(add_this_figure);
                        break;
                    }
                }
            }
        }
    }
    int n_step = 0;
    int k_crash = 0;
    while (n_step < num_steps && k_crash < num_crash) {
        bool random_color = rand() % 2;
        figure* use_figure = playing_board.get_using_figures()[random_color][rand() % playing_board.get_using_figures()[random_color].size()];
        vector<coordinate> may_be_move = use_figure->move_generation();
        while (may_be_move.size() > 0 && k_crash < num_crash) {
            k_crash++;
            if (n_step == 49 && may_be_move.size() == 8) {
                cout << " ";
            }
            int index_step = rand() % may_be_move.size();
            coordinate may_be_step = may_be_move[index_step];
            may_be_move.erase(may_be_move.begin() + index_step);
            if (playing_board.move_is_true(use_figure, may_be_step)) {
                playing_board.move_figure(use_figure, may_be_step);
                if (!playing_board.identify_check(1) && !playing_board.identify_check(0)) {
                    n_step++;
                    break;
                }
            }
        }

    }
}

void add_in_queue(queue<history_game<board>>& Queue, history_game<board> &now_position, board new_board, figure* use_figure, coordinate old_place) {
    history_game<board> new_step = now_position;
    new_step.num_step++;
    new_step.history_steps.push_back(step(use_figure->get_type(), old_place, use_figure->get_place()));
    new_step.now_board.copy_board(new_board);
    new_step.history_board.push_back(new_board);
    if (now_position.now_board.get_figure_board(use_figure->get_place()) != nullptr){
        new_step.price += now_position.now_board.get_figure_board(use_figure->get_place())->get_price();
    }
    if (new_board.identify_check(0)) {
        new_step.price += 100;
    }
    Queue.push(new_step);
}

void search_sucessfull_way()
{
    board playing_board;
    board_generation(playing_board, write_all_figures());
    playing_board.print();
    cout << "Starting to search for possible moves..." << endl;
    list<history_game<board> > history;
    queue<history_game<board> > Queue;
    history_game<board> now_position;
    now_position.now_board.copy_board(playing_board);
    Queue.push(history_game<board>(now_position));
    unsigned long long index_Queue = 0, index_Top = 0;
    while (!Queue.empty()) {
        index_Queue++;
        history_game <board> now_position = Queue.front();
        Queue.pop();
        if (now_position.num_step >= 3 || now_position.price >= 100) {
            index_Top++;
            history.push_back(now_position);
            continue;
        }
        for (int i = 0; i < now_position.now_board.get_using_figures()[1].size(); ++i) {
            board copy_playing_board;
            copy_playing_board.copy_board(now_position.now_board);
            figure* use_figure = copy_playing_board.get_using_figures()[1][i];
            vector<coordinate> may_be_move = use_figure->move_generation();
            while (may_be_move.size() > 0) {
                figure* use_figure = copy_playing_board.get_using_figures()[1][i];
                coordinate old_coordinate = use_figure->get_place();
                if (copy_playing_board.get_figure_board(may_be_move[0]) != nullptr) {
                    if (copy_playing_board.eat_is_true(use_figure, copy_playing_board.get_figure_board(may_be_move[0]))) {
                        copy_playing_board.eat_figure(use_figure, copy_playing_board.get_figure_board(may_be_move[0]));
                        if (copy_playing_board.identify_check(1)) {
                            may_be_move.erase(may_be_move.begin());
                            continue;
                        }
                        add_in_queue(Queue, now_position, copy_playing_board, use_figure, old_coordinate);
                    }
                }
                else {
                    if (copy_playing_board.move_is_true(use_figure, may_be_move[0])) {
                        copy_playing_board.move_figure(use_figure, may_be_move[0]);
                        if (copy_playing_board.identify_check(1)) {
                            may_be_move.erase(may_be_move.begin());
                            continue;
                        }
                        add_in_queue(Queue, now_position, copy_playing_board, use_figure, old_coordinate);
                    }

                }
                may_be_move.erase(may_be_move.begin());
                now_position.now_board.copy_board(now_position.now_board);
                copy_playing_board.copy_board(now_position.now_board);
            }
        }
    }
    cout << "Please wait, everything will be displayed soon" << endl;
    const int write_q = 3;
    vector<history_game<board>> my_top;
    auto iterator = history.begin();
    for (;my_top.size() < write_q; ++iterator) {
        my_top.push_back(*iterator);
    }
    for (; iterator != history.end(); ++iterator) {
        my_top.push_back(*iterator);
        insertionSort(my_top);
        my_top.pop_back();
    }
    for (int i = 0; i < write_q; ++i) {
        cout << "Game " << i + 1 << " price = " << my_top[i].price << endl;
        playing_board.print();
        for (int j = 0; j < my_top[i].history_board.size(); ++j) {
            my_top[i].history_steps[j].print();
            my_top[i].history_board[j].print();
        }
    }

    cout << "Success" << endl;
}

int main()
{
    search_sucessfull_way();
    return 0;
}
