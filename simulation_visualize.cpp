#include "n_body.cpp"
#include <numeric>
#include <vector>
#include <unistd.h>
#include <SFML/Graphics.hpp>

#define dt 1. // Let's say that dt, our time step, is 1 second
#define DAY 86400. // Seconds in a day 
#define WEEK 604800. // Seconds in a week
#define MAX_TIME 604800. // Let's say that we want to simulate one month, or approx. 28 days

using namespace std;

int main(){
    Body *B_i, *B_j;
    
    // int N = 20; // number of bodies
    // std::vector<Body> bodies = generate_random_bodies(N);
    // double max_dist = 0.;
    // for (size_t i =0; i<bodies.size(); i++){
    //     if (dist_to_center(&bodies[i])>max_dist){
    //         max_dist = dist_to_center(&bodies[i]);
    //     }
    // }
    // std::cout<<"max distance is: "<<max_dist<<std::endl; 

    int N = 2;
    std::vector<Body> bodies = generate_earth_moon();
    double max_dist = 0.;
    for (size_t i =0; i<bodies.size(); i++){
        if (dist_to_center(&bodies[i])>max_dist){
            max_dist = dist_to_center(&bodies[i]);
        }
    }
    std::cout<<"max distance is: "<<max_dist<<std::endl; 

    
    double normalise_val = 1.1* max_dist; //
    // =================    
    double force_matrix_x[N][N]; 
    double force_matrix_y[N][N]; 

    vector <vector <pair<int,int> > > vec_of_times;
    double time = 0;
    while (time<WEEK) {
        // First we compute the forces between all of the bodies 
        for (int i = 0; i<N; i++){
            B_i = &bodies[i]; 
            for (int j = i; j < N; j++){
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

        // Time to update the positions and velocities.
        vector<pair<int, int> > pos_at_time;
        for (int i = 0; i < N; i++){
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
            if (fmod(time, DAY) == 0) {
                double new_x = bodies[i].x/normalise_val * 380; 
                double new_y = bodies[i].y/normalise_val * 380;
                pos_at_time.push_back(make_pair(new_x, new_y));
            }

            // so that at the end of the for loop pos_at_time contains the positions of each body at time time 
        }
        // and we can safely add pos_at_time to vec_of_times
        if (fmod(time, DAY)==0) {
            vec_of_times.push_back(pos_at_time);
        }
        time += dt; // dt is defined as a macro on line 2
    }

    // at the end of the while loop, I have vecofbodies full of positions that I want to draw on canvas

    // cout<<vec_of_times.size();

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
            cout<<x<<' '<<y<<endl;
            shape.setPosition(x+380, y+380); 
            sf::Vector2f position = shape.getPosition();
           
            window.draw(shape); // we can draw a lot of shapes and not display them until all of the bodies have been drawn
        }
        
        // now that all bodies for time t have been displayed, we can display the window
        window.display();
		sleep(2); // sleeping so that it stays on the screen for at least a second, otherwise it goes by too fast
		t ++;
    }



    return 0; 
}