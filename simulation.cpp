#include "n_body.cpp"
#define dt 1. // Let's say that dt, our time step, is 1 day
#define MAX_TIME 28. // Let's say that we want to simulate one month, or approx. 28 days
// #define PRINT_FORCE_MATRIX
// #define PRINT_POSITIONS

int main(){
    int N = 2;
    double force_matrix[N][N]; 
    Body *B_i, *B_j; 
    double force_ij; 
    
    // std::vector<Body> bodies = generate_random_bodies(N);
    std::vector<Body> bodies = generate_earth_moon();
    #ifdef PRINT_POSITIONS
    std::cout<<"Initial positions:"<<std::endl; 
    for (int i = 0; i<N; i++){
        bodies[i].print();
    }
    #endif
    double time = 0;

    visualise_bodies(bodies); 
    return 0; 

    while (time<MAX_TIME){
        // First we compute the forces between all of the bodies 
        for (int i = 0; i<N; i++){
            B_i = &bodies[i]; 
            for (int j =0; j<N; j++){
                // we want to compute the forces between bodies i and j
                if (i==j){
                    force_matrix[i][j] = 0.; 
                    continue;
                } // don't compute force from i to i 
                B_j = &bodies[j];
                force_ij = get_force(B_i, B_j); 

                // this matrix is symmetric, we are calling get force twice for each body - MUST FIX
                force_matrix[i][j] = force_ij; 
            }
        }
        #ifdef PRINT_FORCE_MATRIX
        std::cout<<"---force matrix:---"<<std::endl; 
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                std::cout << force_matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout<<"------"<<std::endl; 
        #endif
        // Now we have computed the forces between all the bodies and stored them in a matrix
        // Time to update the positions and velocities.
        // For simplicities sake, I have this in two for loops, obviously we are looping over
        // the same stuff twice so we should move this into a single loop later. 
        for (int i = 0; i<N; i++){
            B_i = &bodies[i]; 
            double total_x_force = 0.;
            double total_y_force = 0.;
            for (int j = 0; j<N; j++){
                // Here we compute the x and y components of the force
                // on each body. This could potentially be done inside the
                // get force function
                if (i==j) continue ; // no force between a body and itself
                B_j = &bodies[j]; 
                double x_force,y_force; 
                double adj, opp, hyp; // trigonometry beurk
                adj = B_i->x - B_j->x;
                opp = B_i->y - B_j->y; 
                hyp = sqrt(pow(adj,2)+pow(opp,2));
                force_ij = force_matrix[i][j]; 
                #ifdef PRINT_FORCE_MATRIX
                std::cout<<"adj_"<<i<<j<<"= "<<adj<<std::endl;
                std::cout<<"opp_"<<i<<j<<"= "<<opp<<std::endl;
                std::cout<<"hyp_"<<i<<j<<"= "<<hyp<<std::endl;
                #endif
                // cos(angle) = adjacent / hypotenuse
                x_force = force_ij * adj/hyp; 
                y_force = force_ij * opp/hyp;
                total_x_force += x_force;
                total_y_force += y_force; 
            }
            #ifdef PRINT_FORCE_MATRIX
            std::cout<<"total x force on body "<<i<<": "<<total_x_force<<std::endl;
            std::cout<<"total y force on body "<<i<<": "<<total_y_force<<std::endl;
            #endif

            // Here, we have the total x force and y force, time to update the position and velocity.
            double x_acc, y_acc; 
            // acceleration = Force / mass
            x_acc = total_x_force/B_i->mass; 
            y_acc = total_y_force/B_i->mass; 
            // displacement = v0*t + 0.5*acc*t^2
            B_i->x += B_i->initial_v_x*dt + 0.5*x_acc*pow(dt,2);
            B_i->y += B_i->initial_v_y*dt + 0.5*y_acc*pow(dt,2);
            // velocity = v0 + t*acc
            B_i->initial_v_x += dt*x_acc;
            B_i->initial_v_y += dt*y_acc; 

            // now we have sucessfully updated the position of B_i
        }
        time += dt; // dt is defined as a macro on line 2
    }
    #ifdef PRINT_POSITIONS
    std::cout<<"Final positions:"<<std::endl; 
    for (int i = 0; i<N; i++){
        bodies[i].print();
    }
    #endif 


    return 0; 
}