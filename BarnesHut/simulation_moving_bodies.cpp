#include "barnes_hut_tee.cpp"
#define dt 1. // Let's say that dt, our time step, is 1 second
#define theta 0.5 // theta is 0.5 by default 
#define max_time 3. // how long we want the simulation to last 

int main(){
    int N = 4; // number of bodies
    Body** body_pointers = generate_random_bodies_pointers(N); 
    Body *B_i; 

    double max_dist = 0.;
    for (int i =0; i<N; i++){
        if (dist_to_center(body_pointers[i])>max_dist){
            max_dist = dist_to_center(body_pointers[i]);
        }
    }
    std::cout<<"max distance is: "<<max_dist<<std::endl; 

    double normalise_val = 1.1* max_dist; 

    double **force_matrix_x = new double* [N];
    double **force_matrix_y = new double* [N];
    for (int i = 0; i<N; i++){
        force_matrix_x[i] = new double[N];
        force_matrix_y[i] = new double[N];
    }

    visualise_bodies(body_pointers, normalise_val, N); 

    double time = 0;

    while (time<DAY){

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
        visualise_bodies(body_pointers, normalise_val, N); 
    }
    #ifdef PRINT_POSITIONS
    std::cout<<"Final positions:"<<std::endl; 
    for (int i = 0; i<N; i++){
        // bodies[i].print();
        body_pointers[i]->print();
    }
    // visualise_bodies(bodies, normalise_val); 
    #endif 
    #ifdef PRINT_POSITIONS_VISUAL
    visualise_bodies(body_pointers, normalise_val, N); 
    #endif

    #ifdef PRINT_FINAL_FORCE_MATRIX
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
            std::cout << std::setprecision(4) << force_matrix_y[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout<<"------"<<std::endl; 
    #endif

    return 0; 
}