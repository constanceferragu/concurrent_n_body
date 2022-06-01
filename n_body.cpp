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
#include <math.h>

const double GRAVITY = 0.000000000066742;

class Body {
public:
    double mass;
    double x;
    double y;
    double initial_v_x;
    double initial_v_y;
    std::mutex lock;

    Body(){
        this->x = 0.0;
        this->y = 0.0;
        this->mass = 0.0;
        this->initial_v_x = 0.0;
        this->initial_v_y = 0.0;
    }

    Body(double x, double y, double mass, double v_x, double v_y) {
        this->x = x;
        this->y = y;
        this->mass = mass;
        this->initial_v_x = v_x;
        this->initial_v_y = v_y;
    }
    
    double get_force(Body* b_1, Body* b_2);
};

Body::~Body() {
}

std::vector<Body> generate_random_bodies(int num_bodies){
    // For the moment, these are the boundaries:
    //      mass is between 0 and 100
    //      x,y coordinates are between 0 and 1
    //      velocities are between 0 and 10 
    std::vector<Body> bodies;
    double m,x,y,v_x, v_y;
    Body rand_body; 

    for (int i=0; i<num_bodies; i++){
        m = 100 * (rand() / double(RAND_MAX)) ;
        x = (rand() / double(RAND_MAX)) ;
        y = (rand() / double(RAND_MAX)) ;
        v_x = 10 * (rand() / double(RAND_MAX)) ;
        v_y = 10 * (rand() / double(RAND_MAX)) ;
        rand_body = Body(x,y,m, v_x,v_y); 
        bodies.push_back(rand_body); 
    }
    return bodies; 
}

double Body::get_force(Body* b_1, Body* b_2){
    double dist = pow( b_1->x - b_2->x , 2) + pow(b_1->y - b_2->y , 2);
    return (GRAVITY * b_1->mass * b_2->mass)/dist;
}