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
    
    double x;
    double y;
    double mass; //kg
    double initial_v_x;
    double initial_v_y; //velocity magnitude in m/s
    std::string name; // Just for visualisation, can be removed later
    // std::mutex lock; commented because was giving me bugs

    Body(){
        this->x = 0.0;
        this->y = 0.0;
        this->mass = 0.0;
        this->initial_v_x = 0.0;
        this->initial_v_y = 0.0;
        this->name = "B";
    }

    Body(double x, double y, double mass, double v_x, double v_y) {
        this->x = x;
        this->y = y;
        this->mass = mass;
        this->initial_v_x = v_x;
        this->initial_v_y = v_y;
        this->name = "B";
    }
    ~Body(); 
    // I think it makes more sense to have the get_force function
    //    a standalone function. - Siggi
    // double get_force(Body* b_1, Body* b_2);
    void print();
    void apply_force(double x_force, double y_force, double dt);
};

Body::~Body() {
}

void Body::apply_force(double x_force, double y_force, double dt){
    // Takes two force vectors and updates both position
    // and velocity of the Body.

    // first we need the acceleration in each direction
    double x_acc = x_force/this->mass;
    double y_acc = y_force/this->mass;

    // displacement = v0*t + 0.5*acc*t^2
    this->x += this->initial_v_x*dt + 0.5*x_acc*pow(dt,2);
    this->y += this->initial_v_y*dt + 0.5*y_acc*pow(dt,2);

    // velocity = v0 + t*acc
    this->initial_v_x += dt*x_acc;
    this->initial_v_y += dt*y_acc; 

}

void Body::print(){
    // For debugging purposes we print the characteristics of a body
    std::cout<<"---Body---"<<std::endl;
    std::cout<<"  mass: "<<this->mass<<std::endl;
    std::cout<<"  x   : "<<this->x<<std::endl;
    std::cout<<"  y   : "<<this->y<<std::endl;
    std::cout<<"  v_x : "<<this->initial_v_x<<std::endl;
    std::cout<<"  v_y : "<<this->initial_v_y<<std::endl;
    std::cout<<"  norm: "<<sqrt(pow(this->initial_v_x,2) + pow(this->initial_v_y,2))<<std::endl;
    std::cout<<"----------"<<std::endl;
}

std::vector<Body> generate_random_bodies(int num_bodies){
    // For the moment, these are the boundaries:
    //      mass is between 0 and 100
    //      x,y coordinates are between -1 and 1
    //      velocities are between -10 and 10 
    std::vector<Body> bodies;
    double m,x,y,v_x, v_y;
    // Body rand_body; 

    for (int i=0; i<num_bodies; i++){
        m = 100 * (rand() / double(RAND_MAX)) ;
        x =2* (rand() / double(RAND_MAX)) -1 ;
        y =2* (rand() / double(RAND_MAX)) -1;
        v_x = 20 * (rand() / double(RAND_MAX)) -10;
        v_y = 20 * (rand() / double(RAND_MAX)) -10;
        Body rand_body(x,y,m, v_x,v_y); 
        bodies.push_back(rand_body); 
    }
    return bodies; 
}

std::vector<Body> generate_earth_moon(){
    
    Body earth(0, 0, 5.972e24, 0, 0); // Earth is not moving and in the center. Its mass is 80 times that of the moon
    Body moon(3.844e8, 0, 7.348e22, 0, -1000.); // Moon's starting position is at (1,0), i.e. to the right of the earth. It has 0 x velocity and -12.8 y velocity
    earth.name = "E";
    moon.name = "M";
    std::vector<Body> bodies{earth, moon};
    return bodies; 
}

std::vector<Body> generate_earth_moon_sun(){ 
    // WARNING this function does not really work.
    //      It is not representative of the sun-earth-moon system because,
    //      well there is no sun as you can see.
    //      I removed the sun because with the resolution of the graph made
    //      by visualise_bodies function the earth and the moon collide, so
    //      it was not interesting. 
    Body earth(0, 0, 5.972e24, 0, 0); // Earth is not moving and in the center. Its mass is 80 times that of the moon
    Body moon(3.844e8, 0, 7.348e22, 0, -1000.); // Moon's starting position is at (1,0), i.e. to the right of the earth. It has 0 x velocity and -12.8 y velocity
    Body small_moon(3.844e8/1.87, 0, 7.348e15, 0, -800.);
    small_moon.name = "S";
    earth.name = "E";
    moon.name = "M";
    std::vector<Body> bodies{earth, small_moon, moon};
    return bodies; 
}

// double Body::get_force(Body* b_1, Body* b_2){
//     double dist = pow( b_1->x - b_2->x , 2) + pow(b_1->y - b_2->y , 2);
//     return (GRAVITY * b_1->mass * b_2->mass)/dist;
// }

double get_force(Body* b_1, Body* b_2){
    // IDEA: could it be good to let this function return the x and y components
    //          of the force instead of just the total force? We need to compute
    //          those at some point anyway. 
    double dist = pow( b_1->x - b_2->x , 2) + pow(b_1->y - b_2->y , 2);
    return (GRAVITY * b_1->mass * b_2->mass)/dist;
}

void get_force_components(Body* b_1, Body* b_2, double &F_x, double &F_y){ 
    double dist = pow( b_1->x - b_2->x , 2) + pow(b_1->y - b_2->y , 2);
    double magnitude = ((GRAVITY * b_1->mass * b_2->mass)/dist);
    double adj, opp, hyp;
    adj = b_2->x - b_1->x;
    opp = b_2->y - b_1->y; 
    hyp = sqrt(pow(adj,2)+pow(opp,2));
    // recall that cos(theta) = adj/hyp
    // recall that sin(theta) = opp/hyp
    F_x = magnitude * adj/hyp;
    F_y = magnitude * opp/hyp;
}


// maybe
// double get_force_x(Body* b_1, Body* b_2){
    

// }

// double get_force_y(Body* b_1, Body* b_2){


// }


// printing
void visualise_bodies(std::vector<Body> bodies, double normalise_val){
    std::vector<int> xcoords, ycoords; 
    for (size_t i=0; i<bodies.size(); i++){
        // bodies[i].x is in [-X_max,X_max], we want it rounded to an integer in [-10, 10]
        double new_x, new_y;
        new_x = bodies[i].x/normalise_val;
        new_y = bodies[i].y/normalise_val;
        xcoords.push_back((int) round(10*new_x));
        ycoords.push_back((int) round(10*new_y));

    }
    // The top border
    std::cout<<"   "; 
    for (int j = 0;j<22;j++){
        std::cout<<"+ "; 
    }
    std::cout<<"+"<<std::endl;

    // The grid itself and side borders, y coords
    for (int i = 0;i<21;i++){
        // The y coordinates and left border 
        if (i%10 == 0){
            if (i<18) {
                std::cout<<1-i/10<<"  +";
            } else {
                std::cout<<1-i/10<<" +";
            }
        } else {
            std::cout<<"   +";
        }
        // The grid itself
        for (int j = 0;j<21;j++){
            // we need to check if the point j,i is in xcoords,ycoords
            bool point = false;
            std::string name;
            for (size_t k=0; k<bodies.size(); k++){
                if (xcoords[k] == j-10 && ycoords[k] == 10-i){
                    point = true;
                    name = bodies[k].name;
                    break;
                }
            }
            if (point){
                std::cout<<" "<<name; 
            } else{
                std::cout<<"  "; 
            }
        }
        // The right border
        std::cout<<" +"<<std::endl;
    }

    // The bottom border
    std::cout<<"   "; 
    for (int j = 0;j<22;j++){
        std::cout<<"+ "; 
    }
    std::cout<<"+"<<std::endl;
    std::cout<<"    "; 

    // The x-coordinates
    for (int j = 0;j<21;j++){
        if (j%10 == 0){
            if (j<5) {
                std::cout<<-1+j/10<<"";
            } else {
                std::cout<<" "<<-1+j/10<<"";
            }
        } else {
            std::cout<<"  ";
        }
    }
    std::cout<<std::endl; 
}