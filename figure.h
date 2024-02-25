#pragma once
#include<vector>
#include<iostream>
#include "struct.h"

using namespace std;

class figure
{
    char figure_type;
    int figure_price;
    bool figure_color;
    coordinate figure_place;

public:
    figure()
    {
        figure_type = ' ';
        figure_price = 0;
        figure_color = 0;
    }
    figure(bool color, coordinate coordinate) :figure_color(color), figure_place(coordinate) {}
    figure(bool color, coordinate coordinate, int price, char type) :figure_type(type), figure_price(price), figure_color(color), figure_place(coordinate){}
    int get_price()
    {
        return figure_price;
    }
    char get_type()
    {
        return figure_type;
    }

    bool get_color()
    {
        return figure_color;
    }

    coordinate get_place()
    {
        return figure_place;
    }
    void error()
    {
        cout << "Error: " << figure_type << ' ' << figure_color << ' ' << "(" << figure_place.x << "," << figure_place.y << ")" << endl;
    }
    void set_all(char type, int price)
    {
        figure_type = type;
        figure_price = price;
    }

    virtual void move(coordinate new_place)
    {
        figure_place = new_place;
    }
    virtual vector<coordinate> generation_direction(coordinate new_place)
    {
        vector<coordinate> steps;
        return steps;
    }
    virtual vector<coordinate> generation_eat_direction(coordinate new_place)
    {
        return figure::generation_direction(new_place);
    }
    virtual bool move_is_true(coordinate new_place) {
        return 0;
    }
    virtual vector<coordinate> move_generation() {
        vector<coordinate> may_be_move;
        return may_be_move;
    }
    virtual bool eat_figure_is_true(coordinate eat_figure) {
        return 0;
    }
    void print() {
        cout << get_type() << ' ' << get_color() << ' ' << get_price() << ' ' << "(" << get_place().x << ", " << get_place().y << ")" << endl;
    }
};

class e : public figure
{
public:
    e(bool color, coordinate coordinate) : figure(color, coordinate)
    {
        set_all('e', 1);
    }
    vector<coordinate> generation_direction(coordinate new_place) override
    {
        vector<coordinate> steps;
        if (!move_is_true(new_place)) {
            figure::generation_direction(new_place);
        }
        steps.push_back(new_place);
        return steps;
    }
    vector<coordinate> generation_eat_direction(coordinate new_place) override
    {
        vector<coordinate> steps;
        if (!eat_figure_is_true(new_place)) {
            figure::generation_direction(new_place);
        }
        steps.push_back(new_place);
        return steps;
    }
    bool move_is_true(coordinate new_place) override {
        return get_place().y - new_place.y == 0 && abs(get_place().x - new_place.x) == 1;
    }
    void move(coordinate new_place) override
    {
        if (move_is_true(new_place))
        {
            figure::move(new_place);
        }
    }
    vector<coordinate> move_generation() override {
        coordinate new_coor;
        if (get_color()) {
            new_coor = coordinate(get_place().x + 1, get_place().y);
        }
        else {
            new_coor = coordinate(get_place().x - 1, get_place().y);
        }
        vector<coordinate> new_coordinates;
        if ((new_coor.x < 7) && (new_coor.x > 0)) {
            new_coordinates.push_back(new_coor);
            if (new_coor.y < 7) {
                new_coordinates.push_back(coordinate(new_coor.x, new_coor.y + 1));
            }
            if (new_coor.y > 0) {
                new_coordinates.push_back(coordinate(new_coor.x, new_coor.y - 1));
            }
        }
        return new_coordinates;
    }
    bool eat_figure_is_true(coordinate eat_figure)
    {
        if (get_color()) {
            return eat_figure.x - get_place().x == 1 && abs(eat_figure.y - get_place().y) == 1;
        }
        else {
            return eat_figure.x - get_place().x == -1 && abs(eat_figure.y - get_place().y) == 1;
        }
    }
};
class b : public figure
{

public:
    b(bool color, coordinate coordinate) : figure(color, coordinate)
    {
        set_all('b', 3);
    }
private:
    void help_move_generation(vector<coordinate>& new_coordinates, int direction_travel_x, int direction_travel_y, coordinate limit) {
        coordinate new_coor = get_place();
        while (true) {
            new_coor.x += direction_travel_x;
            new_coor.y += direction_travel_y;
            if (new_coor.x == limit.x && new_coor.y == limit.y) {
                new_coordinates.push_back(new_coor);
                break;
            }
            if (new_coor.x <= 7 && new_coor.y <= 7 && new_coor.x >= 0 && new_coor.y >= 0) {
                new_coordinates.push_back(new_coor);
            }
            else {
                break;
            }
        }
    }
public:
    vector<coordinate> move_generation() override {
        vector<coordinate> steps;

        help_move_generation(steps, 1, 1, coordinate(-2, -2));
        help_move_generation(steps, 1, -1, coordinate(-2, -2));
        help_move_generation(steps, -1, 1, coordinate(-2, -2));
        help_move_generation(steps, -1, -1, coordinate(-2, -2));

        return steps;
    }
    void move(coordinate new_place) override
    {
        if (move_is_true(new_place))
        {
            figure::move(new_place);
        }
    }
    vector<coordinate> generation_direction(coordinate new_place) override
    {
        vector<coordinate> steps;
        if (!move_is_true(new_place)) {
            return figure::generation_direction(new_place);
        }
        coordinate delta(0, 0);
        if (new_place.x > get_place().x) {
            delta.x = 1;
        }
        else {
            delta.x = -1;
        }
        if (new_place.y > get_place().y) {
            delta.y = 1;
        }
        else {
            delta.y = -1;
        }
        help_move_generation(steps, delta.x, delta.y, new_place);
        return steps;
    }
    vector<coordinate> generation_eat_direction(coordinate new_place) override
    {
        return generation_direction(new_place);
    }
    bool move_is_true(coordinate new_place) override {
        return abs(get_place().y - new_place.y) == abs(get_place().x - new_place.x);
    }

    bool eat_figure_is_true(coordinate eat_figure)
    {
        return move_is_true(eat_figure);
    }
};
class n : public figure
{
    char figure_type = 'n';
    int figure_price = 3;

public:
    n(bool color, coordinate coordinate) : figure(color, coordinate)
    {
        set_all('n', 3);
    }
private:
    void help_move_generation(vector<coordinate>& new_coordinates, int direction_travel_x, int direction_travel_y) {
        coordinate new_coor = get_place();
        new_coor.x += direction_travel_x;
        new_coor.y += direction_travel_y;
        if (new_coor.x <= 7 && new_coor.x >= 0 && new_coor.y <= 7 && new_coor.y >= 0) {
            new_coordinates.push_back(new_coor);
        }
    }
public:
    vector<coordinate> move_generation() override {
        vector<coordinate> steps;
        help_move_generation(steps, 1, 1);
        help_move_generation(steps, 1, -1);
        help_move_generation(steps, -1, 1);
        help_move_generation(steps, -1, -1);

        return steps;
    }
    void move(coordinate new_place) override
    {
        if (move_is_true(new_place))
        {
            figure::move(new_place);
        }
    }
    vector<coordinate> generation_direction(coordinate new_place) override
    {
        vector<coordinate> steps;
        if (!move_is_true(new_place)) {
            figure::generation_direction(new_place);
        }
        coordinate delta(0, 0);
        if (new_place.x > get_place().x) {
            delta.x = 1;
        }
        else {
            delta.x = -1;
        }
        if (new_place.y > get_place().y) {
            delta.y = 1;
        }
        else {
            delta.y = -1;
        }
        help_move_generation(steps, delta.x, delta.y);
        return steps;
    }
    vector<coordinate> generation_eat_direction(coordinate new_place) override
    {
        return generation_direction(new_place);
    }
    bool move_is_true(coordinate new_place) override {
         return (abs(get_place().x - new_place.x) == 1 && abs(get_place().y - new_place.y) == 2) || (abs(get_place().y - new_place.y) == 1 && abs(get_place().x - new_place.x) == 2);
    }

    bool eat_figure_is_true(coordinate eat_figure)
    {
        return move_is_true(eat_figure);
    }
};
class g : public figure
{
public:
    g(bool color, coordinate coordinate) : figure(color, coordinate)
    {
        set_all('g', 5);
    }
private:
    void help_move_generation(vector<coordinate>& new_coordinates, int direction_travel_x, int direction_travel_y, coordinate limit) {
        coordinate new_coor = get_place();
        while (true) {
            new_coor.x += direction_travel_x;
            new_coor.y += direction_travel_y;
            if (new_coor.x == limit.x && new_coor.y == limit.y) {
                new_coordinates.push_back(new_coor);
                break;
            }
            if (new_coor.x <= 7 && new_coor.y <= 7 && new_coor.x >= 0 && new_coor.y >= 0) {
                new_coordinates.push_back(new_coor);
            }
            else {
                break;
            }
        }
    }
public:
    vector<coordinate> move_generation() override {
        vector<coordinate> steps;
        help_move_generation(steps, 1, 1, coordinate(-2, -2));
        help_move_generation(steps, 1, -1, coordinate(-2, -2));
        help_move_generation(steps, -1, 1, coordinate(-2, -2));
        help_move_generation(steps, -1, -1, coordinate(-2, -2));

        return steps;
    }
    void move(coordinate new_place) override
    {
        if (move_is_true(new_place))
        {
            figure::move(new_place);
        }
    }
    vector<coordinate> generation_direction(coordinate new_place) override
    {
        vector<coordinate> steps;
        if (!move_is_true(new_place)) {
            figure::generation_direction(new_place);
        }
        coordinate delta(0, 0);
        if (new_place.x > get_place().x) {
            delta.x = 1;
        }
        else if (new_place.x < get_place().x) {
            delta.x = -1;
        }
        if (new_place.y > get_place().y) {
            delta.y = 1;
        }
        else if (new_place.y < get_place().y) {
            delta.y = -1;
        }
        help_move_generation(steps, delta.x, delta.y, new_place);
        return steps;
    }
    vector<coordinate> generation_eat_direction(coordinate new_place) override
    {
        return generation_direction(new_place);
    }
    bool move_is_true(coordinate new_place) override {
        return (abs(get_place().x - new_place.x) > 0 && get_place().y - new_place.y == 0) || (abs(get_place().y - new_place.y) > 0 && get_place().x - new_place.x == 0);
    }

    bool eat_figure_is_true(coordinate eat_figure)
    {
        return move_is_true(eat_figure);
    }
};
class k : public figure
{

public:
    k(bool color, coordinate coordinate) : figure(color, coordinate)
    {
        set_all('k', 100);
    }
private:
    void help_move_generation(vector<coordinate>& new_coordinates, int direction_travel_x, int direction_travel_y) {
        if (direction_travel_x == 0 && direction_travel_y == 0) { return; }
        coordinate new_coor = get_place();
        new_coor.x += direction_travel_x;
        new_coor.y += direction_travel_y;
        if (new_coor.x <= 7 && new_coor.x >= 0 && new_coor.y <= 7 && new_coor.y >= 0) {
            new_coordinates.push_back(new_coor);
        }
    }
public:
    vector<coordinate> move_generation() override {
        vector<coordinate> new_coordinates;
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                help_move_generation(new_coordinates, i, j);
            }
        }
        return new_coordinates;
    }
    void move(coordinate new_place) override
    {
        if (move_is_true(new_place))
        {
            figure::move(new_place);
        }
    }
    vector<coordinate> generation_direction(coordinate new_place) override
    {
        vector<coordinate> steps;
        if (!move_is_true(new_place)) {
            figure::generation_direction(new_place);
        }
        coordinate delta(0, 0);
        if (new_place.x > get_place().x) {
            delta.x = 1;
        }
        else if (new_place.x < get_place().x) {
            delta.x = -1;
        }
        if (new_place.y > get_place().y) {
            delta.y = 1;
        }
        else if (new_place.y < get_place().y) {
            delta.y = -1;
        }
        help_move_generation(steps, delta.x, delta.y);
        return steps;
    }
    vector<coordinate> generation_eat_direction(coordinate new_place) override
    {
        return generation_direction(new_place);
    }
    bool move_is_true(coordinate new_place) override {
        return abs(get_place().x - new_place.x) + abs(get_place().y - new_place.y) > 0 && abs(get_place().x - new_place.x) + abs(get_place().y - new_place.y) < 3;
    }

    bool eat_figure_is_true(coordinate eat_figure)
    {
        return move_is_true(eat_figure);
    }
};
