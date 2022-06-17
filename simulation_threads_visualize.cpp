#include "n_body.cpp"
#include <numeric>
#include <iomanip>      // std::setprecision

#include <vector>
#include <unistd.h>
#include <SFML/Graphics.hpp>

#define dt 1. // Let's say that dt, our time step, is 1 second
#define HOUR 3600.
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

void do_nothing(){}; 

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
     
 
    int chunk_size = N/num_threads; // at first we assume num threads divides N
    int num_diag = N/chunk_size; // This will be equal to num_threads if num_threads divides N
    int rest = N - num_diag * chunk_size; // This will be 0 if num_diag == num_threads
    
    // TO RUN WITH EARTH MOON SIMULATION (3 MOONS ACTUALLY) UNCOMMENT THESE LINES
    // Body* earth = new Body(0, 0, 5.972e24, 0, 0); // Earth is not moving and in the center. Its mass is 80 times that of the moon
    // Body* moon1 = new Body(-3.844e8, 0, 7.348e22, 0., 1000.); // Moon's starting position is at (1,0), i.e. to the right of the earth. It has 0 x velocity and -12.8 y velocity
    // Body* moon2 = new Body(0, 3.844e8, 7.348e22, 1000.,  0.); // Moon's starting position is at (1,0), i.e. to the right of the earth. It has 0 x velocity and -12.8 y velocity
    // Body* moon3 = new Body(3.844e8, 0, 7.348e22, 0., -1000.); // Moon's starting position is at (1,0), i.e. to the right of the earth. It has 0 x velocity and -12.8 y velocity
    // earth->name = "E";
    // moon1->name = "1";
    // moon2->name = "2";
    // moon3->name = "3";
    // N = 4;
    // std::vector<Body*> body_pointers{earth,moon1,moon2,moon3};
    // Body* body_pointers[] = {earth,moon1, moon2, moon3};
    // ________________________________________________________
    Body** body_pointers = generate_random_bodies_pointers(N); 
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

    double time = 0;
    std::vector <std::vector <std::pair<int,int> > > vec_of_times;

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
            // std::cout<<"doing diagonal "<<diag_i<<std::endl; 
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
                    // std::cout<<"useless thread"<<std::endl;
                    workers[thread_i] = std::thread(&do_nothing);
                    workers[thread_i].detach(); // we just get rid of the thread
                }
            }

            for (int thread_i = 0; thread_i < num_threads; ++thread_i) {
                if (workers[thread_i].joinable()){
                    workers[thread_i].join();
                }
            }
            // std::cout<<"threads joined"<<std::endl; 

        }
  
        // Time to update the positions and velocities.
        std::vector<std::pair<int, int> > pos_at_time;
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

            if (fmod(time, DAY) == 0) {
                double new_x = body_pointers[i]->x/normalise_val * 380; 
                double new_y = -body_pointers[i]->y/normalise_val * 380;
                pos_at_time.push_back(std::make_pair(new_x, new_y));
            }
        }
        if (fmod(time, DAY)==0) {
            vec_of_times.push_back(pos_at_time);
        }
        time += dt; // dt is defined as a macro on line 2
    }




    sf::RenderWindow window(sf::VideoMode(760, 760), "My window");
	int t = 0;
    while (t < vec_of_times.size()) // we want the number of time frames we visualize
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        // At this point, vec_of_bodies is full of all we are going to draw
        
        for (int i = 0; i < vec_of_times[0].size(); i++) { // N == vec_of_times[0].size() by the way
            sf::CircleShape shape(3.f); // dot 
            shape.setFillColor(sf::Color(0, 0, 0)); // black dot 
            double x = vec_of_times[t][i].first;
            double y = vec_of_times[t][i].second;
            // cout<<x<<' '<<y<<endl;
            shape.setPosition(x+380, y+380); 
           
            window.draw(shape); // we can draw a lot of shapes and not display them until all of the bodies have been drawn
        }
        
        // now that all bodies for time t have been displayed, we can display the window
        window.display();
		// sleep(1); // sleeping so that it stays on the screen for at least a second, otherwise it goes by too fast
        usleep(250000);
		t++;
    }

    return 0; 
}