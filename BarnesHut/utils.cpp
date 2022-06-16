#include "barnes_hut_node.cpp"

//----------------------------------------------------------------------------------------------------


int which_quadrant(Body* body, double box_dimensions[4]){
    // NW -> 0
    // NE -> 1
    // SW -> 2
    // SE -> 3
    // out of bounds -> 10 + 

    // Finds which quadrant a body is in given the dimensions of the box
    // This function is needed when placing a body in child nodes 

    if (body->x < box_dimensions[0]){
        return 10;
    }
    if (body->x <= box_dimensions[0]+(box_dimensions[2]/2)){
        if (body->y < box_dimensions[1]){
            return 11;
        }
        if (body->y <= box_dimensions[1]+(box_dimensions[3]/2)){
            return 2;
        }
        if (body->y <= box_dimensions[1]+box_dimensions[3]){
            return 0;
        }
        return 12;
    }
    if (body->x <= box_dimensions[0]+(box_dimensions[2])){
         if (body->y < box_dimensions[1]){
            return 13;
        }
        if (body->y <= box_dimensions[1]+(box_dimensions[3]/2)){
            return 3;
        }
        if (body->y <= box_dimensions[1]+box_dimensions[3]){
            return 1;
        }
        return 14;
    }
    return 15;
}

double compute_distance(double center_of_mass[2], Body* b){
    // computes distance between a body and a center of mass 

    return pow(pow(b->x - center_of_mass[0], 2) 
                        + pow(b->y - center_of_mass[1],2), 0.5);
}

double compare_ratio(Node* node, Body* b, double theta){
    // Determines if an internal node is sufficiently 
    // far away from a body using the threshold value, theta = 0.5

    double distance = compute_distance(node->center_of_mass, b);
    return node->box_dimensions[3]/distance <= theta;
}

void get_force_components_com(Node* node, Body* b, double &F_x, double &F_y){ 
    // Computes the force components for a Center of Mass 

    double dist = pow( b->x - node->center_of_mass[0] , 2) 
                + pow(b->y - node->center_of_mass[1] , 2);
    double magnitude = ((GRAVITY * b->mass * node->total_mass)/dist);
    double adj, opp, hyp;
    adj = b->x - node->center_of_mass[0];
    opp = b->y - node->center_of_mass[1]; 
    hyp = sqrt(pow(adj,2)+pow(opp,2));

    // recall that cos(theta) = adj/hyp
    // recall that sin(theta) = opp/hyp
    F_x = magnitude * adj/hyp;
    F_y = magnitude * opp/hyp;
}


