#pragma once

enum class color
{
    red,
    green,
    blue
};

struct point2d
{
    point2d() {}
    point2d(int x_, int y_) : x(x_), y(y_) {}
    int x = 0;
    int y = 0;
};

struct shape
{
    shape() {}
    shape(std::string n) : name(n) {}
    virtual ~shape() {}

    void set_visible(bool v) { visible = v; }
    bool get_visible() const { return visible; }

    color color_ = color::blue;
    std::string name = "";
    point2d position;
    std::map<color, point2d> dictionary;

private:
    bool visible = false;

public:
    RTTR_ENABLE()
};

struct circle final: public shape
{
    circle() : shape() {}
    circle(std::string n) : shape(n) {}
    ~circle() {}

    double radius = 5.2;
    std::vector<point2d> points;

    int no_serialize = 100;

    RTTR_ENABLE(shape)
};