#include <iostream>

// added this for now but we'll see what is needed and what isn't 
#include <cmath> 
#include <string>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <list>

// simulate the dynamic of several bodies

class Body {
    // describes one body 
    public:
    double mass;
    double x, y; // make up the coordinates 
    double u, v; // for velocity vector
    // will probably have to go back to thee types later.

    // constructor: will have to figure out if we define a body with all of these in the beginning 
    // or if we should have an "empty constructor" and compute all attributes after 
    Body(double mass, double x, double y, double u, double v) : mass(mass), x(x), y(y), u(u), v(v) {}; 

    double compute_force(Body b1, Body b2) {
        double G = 6.67e-11;
        return G * (b1.mass * b2.mass)/((b1.x-b2.x)*(b1.x-b2.x)+(b1.y-b2.y)*(b1.y-b2.y));
    } 

    double simulation_alorithm(std::vector<Body> &bodies) {
        // how i see it, we have a vector that contains all bodies and we want to compute the force between all of them 
        // by using a loop and then updating the positions. 
        // for (int i=0; i<bodies.size()-1;i++) {
        //     // force 0 and 1
        //     // force 1 and 2
        //     // ...
        //     // force n-1 and n
        //     compute_force(bodies[i], bodies[i+1]);
        //     // but we can't only compute the force between two "adjacent" bodies in the vector, it has to be all of them 
        //     // so double loop 
        // }

        for (int i=0; i<bodies.size()-1;i++) {
            for (int j=0; j<bodies.size(); j++) {
                if (i!=j) {
                    double force = compute_force(bodies[i], bodies[j]);
                    // update the positions : do something with xi xj
                }
                
            }
            
        }
    }
};