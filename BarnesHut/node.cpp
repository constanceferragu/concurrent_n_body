#include "../n_body.cpp"
//----------------------------------------------------------------------------------------------------

class Node {
public:
    double center_of_mass[2];
    double total_mass;

    // does the node contain a body 
    bool contains_body;

    // external = end node (no children)
    // all children are NULL 
    bool external;

    // to get coordinates of the quadrant(s)
    // left corner x, left corner y, x length, y length
    double box_dimensions[4];
    //double bottom_left_corner_x;
    //double bottom_left_corner_y;
    //double x_length;
    //double y_length;

    // std::mutex lock;

    Node* quad_nodes[4]; 
    Node* parent;
    Body* body;

    Node() {
        this->box_dimensions[0] = this->box_dimensions[1] = 
                                this->box_dimensions[2] = this->box_dimensions[3] = 0.0;

        this->quad_nodes[0] = this->quad_nodes[1] = this->quad_nodes[2] = this->quad_nodes[3] = NULL;

        this->parent = NULL;
        this->body = NULL;

        this->center_of_mass[0] = this->center_of_mass[1] = 0.0;
        this->total_mass = 0.0;

        this->contains_body = false;
        this->external = false;

    }

    Node(Node* parent, int quadrant){
        this->parent = parent;
        this->contains_body = false;
        // --> body not necessary 

        this->external = true;
        // --> center of mass and total mass not necessary
        // 4 children not necessary  

        // create new quadrant dimensions from parent
        this->box_dimensions[2] = parent->box_dimensions[2]/2;
        this->box_dimensions[3] = parent->box_dimensions[3]/2;
        if (quadrant == 0){
            // NW 
            this->box_dimensions[0] = parent->box_dimensions[0];
            this->box_dimensions[1] = parent->box_dimensions[1]+this->box_dimensions[3];
        }
        else if (quadrant == 1){
            // NE 
            this->box_dimensions[0] = parent->box_dimensions[0]+this->box_dimensions[2];
            this->box_dimensions[1] = parent->box_dimensions[1]+this->box_dimensions[3];
        }
        else if (quadrant == 2){
            // SW 
            this->box_dimensions[0] = parent->box_dimensions[0];
            this->box_dimensions[1] = parent->box_dimensions[1];
        }
        else {
            // SE 
            this->box_dimensions[0] = parent->box_dimensions[0]+this->box_dimensions[2];
            this->box_dimensions[1] = parent->box_dimensions[1];
        }
        

        this->quad_nodes[0] = this->quad_nodes[1] = this->quad_nodes[2] = this->quad_nodes[3] = NULL;
        this->body = NULL;

        this->center_of_mass[0] = this->center_of_mass[1] = 0.0;
        this->total_mass = 0.0;
    }

    // node constructor with boundaries 
    Node(double box_dimensions[4]) {
        this->box_dimensions[0] = box_dimensions[0];
        this->box_dimensions[1] = box_dimensions[1];
        this->box_dimensions[2] = box_dimensions[2];
        this->box_dimensions[3] = box_dimensions[3];

        this->quad_nodes[0] = new Node(this, 0);
        this->quad_nodes[1] = new Node(this, 1);
        this->quad_nodes[2] = new Node(this, 2);
        this->quad_nodes[3] = new Node(this, 3);

        this->parent = NULL;
        this->body = NULL;

        this->center_of_mass[0] = this->center_of_mass[1] = 0.0;
        this->total_mass = 0.0;

        this->contains_body = false;
        this->external = true;
    }

 
};
