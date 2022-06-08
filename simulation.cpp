#include "n_body.cpp"
#include <numeric>
#define dt 1. // Let's say that dt, our time step, is 1 second
#define DAY 86400. // Seconds in a day 
#define WEEK 604800. // Seconds in a week
#define MAX_TIME 604800. // Let's say that we want to simulate one month, or approx. 28 days
// #define MAX_TIME 1 // Let's say that we want to simulate one month, or approx. 28 days
// #define PRINT_FORCE_MATRIX
#define PRINT_POSITIONS
// #define PRINT_BEFORE_AFTER_APPLY_FORCE

int main(){
    Body *B_i, *B_j; 
    // double force_ij; 
    
    int N = 2; // number of bodies
    // std::vector<Body> bodies = generate_random_bodies(N);
    std::vector<Body> bodies = generate_earth_moon();

    // =================
    // This part is only for the visualisation function, to fit all the bodies on the same screen
    // I COMMENTED IT BECAUSE FOR WHATEVER REASON, x_max and y_max always stay equal to 0. 
    // THE value normalise_val is hardcoded to work with the earth-moon system.
                                // double x_max = 0. ,y_max = 0. ; 
                                // for (size_t i=0; i<bodies.size(); i++){
                                //     // if (abs(bodies[i].x)>x_max){
                                //     if (bodies[i].x>x_max){
                                //         x_max = abs(bodies[i].x); 
                                //     }
                                //     // if (abs(bodies[i].y)>y_max) {
                                //     if (bodies[i].y>y_max) {
                                //         x_max = abs(bodies[i].x); 
                                //     }
                                // }
                                // double normalise_val = 1.1*std::max(x_max,y_max); // We normalise by a value that is slightly larger 
    // HARDCODED
    double normalise_val = 1.1* (3.844e8); //
    // =================    
    double force_matrix_x[N][N]; 
    double force_matrix_y[N][N]; 

    #ifdef PRINT_POSITIONS
    std::cout<<"Initial positions:"<<std::endl; 
    for (int i = 0; i<N; i++){
        bodies[i].print();
    }
    visualise_bodies(bodies, normalise_val); 
    #endif
    double time = 0;


    while (time<WEEK){
        // First we compute the forces between all of the bodies 
        for (int i = 0; i<N; i++){
            B_i = &bodies[i]; 
            for (int j =i; j<N; j++){
                // we want to compute the forces between bodies i and j
                if (i==j){
                    // don't compute force from i to i
                    force_matrix_x[i][j] = 0.; 
                    force_matrix_y[i][j] = 0.; 
                    continue;
                } 
                B_j = &bodies[j];
                // here we compute the force on the body i,
                // split into x and y components and store
                // them in a matrix. 
                double force_x, force_y;
                get_force_components(B_i, B_j, force_x, force_y); 
                // since the matrices are symmetric but opposite sign,
                // we can avoid computing each force twice
                force_matrix_x[i][j] = force_x; 
                force_matrix_y[i][j] = force_y; 
                force_matrix_x[j][i] = -force_x; 
                force_matrix_y[j][i] = -force_y; 
            }
        }
        #ifdef PRINT_FORCE_MATRIX
        std::cout<<"---force matrix x:---"<<std::endl; 
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                std::cout << force_matrix_x[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout<<"---force matrix y:---"<<std::endl; 
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                std::cout << force_matrix_y[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout<<"------"<<std::endl; 
        #endif
        // Time to update the positions and velocities.
        for (int i = 0; i<N; i++){
            B_i = &bodies[i]; 
            double total_x_force = 0.;
            double total_y_force = 0.;
            // The total forces in x and y directions are just the sum of the forces from all the
            // bodies. std::accumulate computes the sum of an array. 
            total_x_force = std::accumulate(force_matrix_x[i], force_matrix_x[i]+N, total_x_force); 
            total_y_force = std::accumulate(force_matrix_y[i], force_matrix_y[i]+N, total_y_force); 
            // We update the body i's position and velocity by calling its
            // apply_force method. 
            B_i->apply_force(total_x_force, total_y_force, dt);
            // now we have sucessfully updated the position of B_i
        }
        time += dt; // dt is defined as a macro on line 2
    }
    #ifdef PRINT_POSITIONS
    std::cout<<"Final positions:"<<std::endl; 
    for (int i = 0; i<N; i++){
        bodies[i].print();
    }
    visualise_bodies(bodies, normalise_val); 
    #endif 


    return 0; 
}