#include "node.cpp"
//----------------------------------------------------------------------------------------------------

// find which quadrant a body is in 
// needed to place a body in a child node 
int which_quadrant(Body* body, double box_dimensions[4]){
    // NW -> 0
    // NE -> 1
    // SW -> 2
    // SE -> 3
    // out of bounds -> 10
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

void new_split(double box_dimensions[4], int quadrant){
    if (quadrant == 0){
        box_dimensions[2] = box_dimensions[2]/2;
        box_dimensions[3] = box_dimensions[3]/2;
        box_dimensions[1] += box_dimensions[3];
    }
    else if (quadrant == 1){
        box_dimensions[2] = box_dimensions[2]/2;
        box_dimensions[3] = box_dimensions[3]/2;
        box_dimensions[0] += box_dimensions[2];
        box_dimensions[1] += box_dimensions[3];
    }
    else if (quadrant == 2){
        box_dimensions[2] = box_dimensions[2]/2;
        box_dimensions[3] = box_dimensions[3]/2;
    }
    else if (quadrant == 3){
        box_dimensions[2] = box_dimensions[2]/2;
        box_dimensions[3] = box_dimensions[3]/2;
        box_dimensions[0] += box_dimensions[2];
        box_dimensions[1] += box_dimensions[3];
    }
}