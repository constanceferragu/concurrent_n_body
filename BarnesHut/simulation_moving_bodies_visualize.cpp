#include "barnes_hut_tree.cpp"
 #define dt 1. // Let's say that dt, our time step, is 1 second
 #define theta 0.5 // theta is 0.5 by default 
 #define max_time 3. // how long we want the simulation to last 

#include <unistd.h>
#include <SFML/Graphics.hpp>

using namespace std;

int main(){
    Body *b_i, *b_j;
    double F_x, F_y;
     // number of bodies 
    int N = 8; 

     // create tree 
    Body* b1 = new Body(0.1, 0.3, 1, 0.6, 0.3);
    Body* b2 = new Body(0.1, 0.4, 2, 0.0, 0.9);
    Body* b3 = new Body(0.6, 0.3, 3, 0.5, 0.0);
    Body* b4 = new Body(0.6, 0.9, 4, -0.7, -0.5);
    Body* b5 = new Body(0, 0.5, 5, -0.2, 0.0);
    Body* b6 = new Body(0.9, 0.9, 6, 0.0, -0.9);
    Body* b7 = new Body(0.55, 0.1, 7, 0.4, -0.3);
    Body* b8 = new Body(0.93, 0.44, 8, 0.8, -0.2);
    std::vector<Body*> bodies{b1, b2, b3, b4, b5, b6, b7, b8};
    double box_dim[4] = {0,0,1,1};
    BH_Tree* tree = new BH_Tree(box_dim, bodies);
    tree->print_tree();

    double max_dist = 0.;
    for (int i =0; i<N; i++){
    if (dist_to_center(bodies[i])>max_dist){
        max_dist = dist_to_center(bodies[i]);
    }
    }
    double normalise_val = 1.1* max_dist;

     // force matrices 
    double force_matrix_x[N]; 
    double force_matrix_y[N]; 

    double time  = 0;

    vector <vector <pair<int,int> > > vec_of_times;
    while (time <= max_time){

        std::cout<<"before:\n";
        for (int i = 0; i<N; i++){
            
            std::cout<<bodies[i]->x<<" - "<<bodies[i]->y<<" \n";
        }

         // compute forces 
        for (int i = 0; i<N; i++){
            b_i = bodies[i]; 
             // compute force on this body 
            F_x = 0.0;
            F_y = 0.0;
            tree->compute_force_on_b(b_i, theta, F_x, F_y);
            force_matrix_x[i] = F_x;
            force_matrix_y[i] = F_y;
             
        }
         //std::cout << force_matrix_x;
         // update (x,y) coordinates 
        vector<pair<int, int> > pos_at_time; 
        for (int i = 0; i<N; i++){
            bodies[i]->apply_force(force_matrix_x[i], force_matrix_y[i], dt);
            double new_x = bodies[i]->x/normalise_val * 380; 
            double new_y = -bodies[i]->y/normalise_val * 380;
            pos_at_time.push_back(make_pair(new_x, new_y));
        }
        std::cout<<"after:\n";
        for (int i = 0; i<N; i++){
            
            std::cout<<bodies[i]->x<<" - "<<bodies[i]->y<<" \n";
        }

        vec_of_times.push_back(pos_at_time);
         // update time 
        time += dt; 
    }


    sf::RenderWindow window(sf::VideoMode(760, 760), "My window");
    // std::cout<<vec_of_times.size()<<std::endl;
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
		usleep(280000);
        t ++;
    }

    return 0;
}