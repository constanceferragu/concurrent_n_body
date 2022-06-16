#include "n_body.cpp"
#include <numeric>
#include <iomanip>      // std::setprecision
#define dt 1. // Let's say that dt, our time step, is 1 second
#define DAY 86400. // Seconds in a day 
#define WEEK 604800. // Seconds in a week
#define MAX_TIME 604800. // Let's say that we want to simulate one month, or approx. 28 days
// #define MAX_TIME 1 // Let's say that we want to simulate one month, or approx. 28 days
// #define PRINT_FORCE_MATRIX
// #define PRINT_FINAL_FORCE_MATRIX
#define PRINT_POSITIONS_VISUAL
// #define PRINT_POSITIONS
// #define PRINT_BEFORE_AFTER_APPLY_FORCE



void fill_force_matrix(double **matrix_x,double **matrix_y, Body **bodies, int start_i, int end_i, int start_j, int end_j){
    Body *B_i, *B_j; 
    for (int i = start_i; i<end_i; i++){
        B_i = bodies[i];
        for (int j = start_j; j<end_j; j++){
            if (i==j){
                // don't compute force from i to i
                matrix_x[i][j] = 0.; 
                matrix_y[i][j] = 0.; 
                continue;
            } 
            B_j = bodies[j]; 
            double force_x, force_y;
            get_force_components(B_i, B_j, force_x, force_y); 
            // since the matrices are symmetric but opposite sign,
            // we can avoid computing each force twice
            matrix_x[i][j] = force_x; 
            matrix_y[i][j] = force_y; 
            matrix_x[j][i] = -force_x; 
            matrix_y[j][i] = -force_y; 
        }
    }

    return;
}

int main(int argc, char* argv[]){
    
    if (argc < 3) {
        std::cout << "Usage: ./simulation_threads num_bodies num_threads" << std::endl;
        return 0;
    }
    int N = std::stoi(argv[1]); // Num bodies 
    int num_threads = std::stoi(argv[2]);
    if (N<1 || num_threads<1){
        std::cout << "Must have at least 1 body and at least 1 thread" << std::endl;
        return 0;
    }
    std::vector<Body> bodies = generate_random_bodies(N);
     

    // int N = 4; // number of bodies
    // int num_threads = 2; // number of threads 
    int chunk_size = N/num_threads; // at first we assume num threads divides N
    int num_diag = N/chunk_size; // This will be equal to num_threads if num_threads divides N
    int rest = N - num_diag * chunk_size; // This will be 0 if num_diag == num_threads
    
    /*
    Body earth(0, 0, 5.972e24, 0, 0); // Earth is not moving and in the center. Its mass is 80 times that of the moon
    Body moon1(-3.844e8, 0, 7.348e22, 0., 1000.); // Moon's starting position is at (1,0), i.e. to the right of the earth. It has 0 x velocity and -12.8 y velocity
    Body moon2(0, 3.844e8, 7.348e22, 1000.,  0.); // Moon's starting position is at (1,0), i.e. to the right of the earth. It has 0 x velocity and -12.8 y velocity
    Body moon3(3.844e8, 0, 7.348e22, 0., -1000.); // Moon's starting position is at (1,0), i.e. to the right of the earth. It has 0 x velocity and -12.8 y velocity
    earth.name = "E";
    moon1.name = "1";
    moon2.name = "2";
    moon3.name = "3";
    std::vector<Body> bodies{earth,moon1,moon2,moon3};
    */
    
    Body* earth = new Body(0, 0, 5.972e24, 0, 0); // Earth is not moving and in the center. Its mass is 80 times that of the moon
    Body* moon1 = new Body(-3.844e8, 0, 7.348e22, 0., 1000.); // Moon's starting position is at (1,0), i.e. to the right of the earth. It has 0 x velocity and -12.8 y velocity
    // Body* moon2 = new Body(0, 3.844e8, 7.348e22, 1000.,  0.); // Moon's starting position is at (1,0), i.e. to the right of the earth. It has 0 x velocity and -12.8 y velocity
    // Body* moon3 = new Body(3.844e8, 0, 7.348e22, 0., -1000.); // Moon's starting position is at (1,0), i.e. to the right of the earth. It has 0 x velocity and -12.8 y velocity
    earth->name = "E";
    moon1->name = "1";
    // moon2->name = "2";
    // moon3->name = "3";
    
    // std::vector<Body*> body_pointers{earth,moon1,moon2,moon3};
    Body* body_pointers[] = {earth,moon1};
    // Body** body_pointers = generate_random_bodies_pointers(N); 
    Body *B_i; 
    std::vector<std::thread> workers(num_threads);

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

    #ifdef PRINT_POSITIONS
    std::cout<<"Initial positions:"<<std::endl; 
    for (int i = 0; i<N; i++){
        // bodies[i].print(normalise_val);
        body_pointers[i]->print(normalise_val);
    }
    // visualise_bodies(bodies, normalise_val); 
    #endif
    #ifdef PRINT_POSITIONS_VISUAL
    visualise_bodies(body_pointers, normalise_val, N); 
    #endif
    double time = 0;

    while (time<WEEK){
        // First we compute the forces between all of the bodies
        // we delegate the work to the fill_force_matrix function
        // fill_force_matrix(force_matrix_x, force_matrix_y, body_pointers, 0,N, 0,N);
        // SHOULD ONLY FILL DIAGONAL
        for (int diag_i=0; diag_i< num_diag; diag_i++){
            // we fill in the matrix, one diagonal at a time.
            // if this is the matrix, we compute A, then B, then C
            // A  B  C
            // X  A  B
            // X  X  A
            for (int thread_i = 0; thread_i < num_threads; thread_i++) {
                // for the diagonal, start_i=start_j and start_j = end_j
                int start_i = thread_i*chunk_size + diag_i*chunk_size;
                int end_i   = (1+thread_i)*chunk_size + diag_i*chunk_size;
                int start_j = thread_i*chunk_size;
                int end_j   = (1+thread_i)*chunk_size;
                if (start_i<N){
                    // we are inside the matrix
                    workers[thread_i] = std::thread(&fill_force_matrix, force_matrix_x, force_matrix_y, body_pointers, start_i, end_i, start_j, end_j);
                } else {
                    // we are outside the matrix - we do nothing
                    workers[thread_i] = std::thread(&fill_force_matrix, force_matrix_x, force_matrix_y, body_pointers, 0, 0, 0, 0);
                }
            }
            // Here we do the rest
            int start_i_rest = N - rest;
            int start_j_rest = N - rest;

            for (int thread_i = 0; thread_i < num_threads; ++thread_i) {
                workers[thread_i].join();
            }
        }
        // SumMapThread(start_block, end, f, results[num_threads - 1]);


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
            // B_i = &bodies[i]; 
            B_i = body_pointers[i]; 
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