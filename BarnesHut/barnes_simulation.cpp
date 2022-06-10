#include "barnes_hut_tree.cpp"
#define dt 1. // Let's say that dt, our time step, is 1 second
#define theta 0.5 // theta is 0.5 by default 
#define max_time 3. // how long we want the simulation to last 

int main(){
    Body *b_i, *b_j;
    // number of bodies 
    int N = 8; 

    // create tree 
    Body* b1 = new Body(0.1, 0.3, 1, 0, 0);
    Body* b2 = new Body(0.1, 0.4, 2, 0, 0);
    Body* b3 = new Body(0.6, 0.3, 3, 0, 0);
    Body* b4 = new Body(0.6, 0.9, 4, 0, 0);
    Body* b5 = new Body(0, 0.5, 5, 0, 0);
    Body* b6 = new Body(0.9, 0.9, 6, 0, 0);
    Body* b7 = new Body(0.55, 0.1, 7, 0, 0);
    Body* b8 = new Body(0.93, 0.44, 8, 0, 0);
    std::vector<Body*> bodies{b1, b2, b3, b4, b5, b6, b7, b8};
    double box_dim[4] = {0,0,1,1};
    BH_Tree* tree = new BH_Tree(box_dim, bodies);
    tree->print_tree();

    // force matrices 
    double force_matrix_x[N]; 
    double force_matrix_y[N]; 

    double time  = 0;

    while (time <= max_time){

        // compute forces 
        for (int i = 0; i<N; i++){
            b_i = bodies[i]; 
            // compute force on this body 
            double F_x, F_y;
            tree->compute_force_on_b(b_i, theta, F_x, F_y);

            force_matrix_x[i] = F_x;
            force_matrix_y[i] = F_y;
        }

        // update (x,y) coordinates 
        for (int i = 0; i<N; i++){
            bodies[i]->apply_force(force_matrix_x[i], force_matrix_y[i], dt);
        }

        // update time 
        time += dt; 
    }
    return 0;
}
