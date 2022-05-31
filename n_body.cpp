#include <algorithm>
#include <atomic>
#include <climits>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>


class Body {
public:
    long mass;
    long x;
    long y;
    long initial_v_x;
    long initial_v_y;
    std::mutex lock;

    Body(long x, long y, long mass) {
        this->x = x;
        this->y = y;
        this->mass = mass;
        this->initial_v_x = 0.0;
        this->initial_v_y = 0.0;
    }
};