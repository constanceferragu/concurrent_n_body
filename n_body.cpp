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
    // std::mutex lock; commented because was giving me bugs

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
    ~Body(); 
    // I think it makes more sense to have the get_force function
    //    a standalone function. - Siggi
    // double get_force(Body* b_1, Body* b_2);
    void print();
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
    // Body rand_body; 

    for (int i=0; i<num_bodies; i++){
        m = 100 * (rand() / double(RAND_MAX)) ;
        x = (rand() / double(RAND_MAX)) ;
        y = (rand() / double(RAND_MAX)) ;
        v_x = 10 * (rand() / double(RAND_MAX)) ;
        v_y = 10 * (rand() / double(RAND_MAX)) ;
        Body rand_body(x,y,m, v_x,v_y); 
        bodies.push_back(rand_body); 
    }
    return bodies; 
}

std::vector<Body> generate_earth_moon(){
    Body earth(0,0, 80, 0,0); // Earth is not moving and in the center. Its mass is 80 times that of the moon
    Body moon(1.,0, 1, 0,-12.8); // Moon's starting position is at (1,0), i.e. to the right of the earth. It has 0 x velocity and -12.8 y velocity
    std::vector<Body> bodies{earth, moon};
    return bodies; 
}

// double Body::get_force(Body* b_1, Body* b_2){
//     double dist = pow( b_1->x - b_2->x , 2) + pow(b_1->y - b_2->y , 2);
//     return (GRAVITY * b_1->mass * b_2->mass)/dist;
// }

void Body::print(){
    // For debugging purposes we print the characteristics of a body
    std::cout<<"---Body---"<<std::endl;
    std::cout<<"  mass: "<<this->mass<<std::endl;
    std::cout<<"  x   : "<<this->x<<std::endl;
    std::cout<<"  y   : "<<this->y<<std::endl;
    std::cout<<"  v_x : "<<this->initial_v_x<<std::endl;
    std::cout<<"  v_y : "<<this->initial_v_y<<std::endl;
    std::cout<<"----------"<<std::endl;
}

double get_force(Body* b_1, Body* b_2){
    // IDEA: could it be good to let this function return the x and y components
    //          of the force instead of just the total force? We need to compute
    //          those at some point anyway. 
    double dist = pow( b_1->x - b_2->x , 2) + pow(b_1->y - b_2->y , 2);
    return (GRAVITY * b_1->mass * b_2->mass)/dist;
}