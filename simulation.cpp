#include "n_body.cpp"

int main(){
    int N = 3;
    int Num_steps = 10; 
    double force_matrix[N][N]; 
    Body *B_i, *B_j; 
    double force_ij; 
    
    std::vector<Body> bodies = generate_random_bodies(N);
    std::cout<<"starting test"<<std::endl; 
    for (int i = 0; i<N; i++){
        bodies[i].print();
    }

    for (int k = 0; k<Num_steps; k++){
        // First we compute the forces between all of the bodies 
        for (int i = 0; i<N; i++){
            B_i = &bodies[i]; 
            for (int j =0; j<N; j++){
                // we want to compute the forces between bodies i and j
                if (i==j){continue;} // don't compute force from i to i 
                B_j = &bodies[j];
                force_ij = get_force(B_i, B_j); 

                // this matrix is symmetric, we are calling get force twice for each body - MUST FIX
                force_matrix[i][j] = force_ij; 
            }
        }
        // Now we have computed the forces between all the bodies and stored them in a matrix
        // Time to update the positions and velocities.
        // For simplicities sake, I have this in two for loops, obviously we are looping over
        // the same stuff twice so we should move this into a single loop later. 
        for (int i = 0; i<N; i++){
            B_i = &bodies[i]; 

            // okay here we need to do some physics computations. 
            
        }
    }



    return 0; 
}