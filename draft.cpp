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

    double simulation_alorithm() {
        
    }
};