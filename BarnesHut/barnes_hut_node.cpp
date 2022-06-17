#include "../n_body.cpp"


//----------------------------------------------------------------------------------------------------

// Implementation of a node of a Barnes Hut Tree 
// Quick overview: this node represents a box or a body.
// If it is a body, it is a "terminal"/"external" node. 
// If it is a body, it represents a box that contains bodies. 
    // in this case, its four children represent its 4 quadrants 
    // (or a single body in one of the quadrants)

class Node {
public:
    double center_of_mass[2];
    double total_mass;

    // does the node contain a body 
    bool contains_body;

    // external = end node (no children)
    bool external;

    // Coordinates of the quadrant(s)
    // left bottom corner x, left bottom corner y, x length, y length
    // from these dimensions we can compute the coordinates of 
    // the box and its quadrants 
    double box_dimensions[4];

    // std::mutex lock;

    // Four children - nodes of each quadrant in this box
    Node* quad_nodes[4]; 

    Node* parent;
    Body* body;

    // Creates an empty node: external and does not contain a body 
    Node() {
        this->box_dimensions[0] = this->box_dimensions[1] = 
                                this->box_dimensions[2] = this->box_dimensions[3] = 0.0;

        this->quad_nodes[0] = this->quad_nodes[1] = this->quad_nodes[2] = this->quad_nodes[3] = NULL;

        this->parent = NULL;
        this->body = NULL;

        this->center_of_mass[0] = this->center_of_mass[1] = 0.0;
        this->total_mass = 0.0;

        this->contains_body = false;
        this->external = true;

    }

    // Creates a node from a parent node, specifying which quadrant of the parent it is 
    Node(Node* parent, int quadrant){
        this->parent = parent;
        // does not contain a body for the moment
        this->contains_body = false;
        
        this->external = true;

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
