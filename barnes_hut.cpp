#include "n_body.cpp"

// find which quadrant a body is in 
// needed to place a body in a child node 
int which_quadrant(Body* body, double box_dimensions[4]){
    // NW -> 0
    // SW -> 1
    // NE -> 2
    // SE -> 3
    // out of bounds -> 10
    if (body->x < box_dimensions[0]){
        return 10;
    }
    if (body->x <= box_dimensions[0]+box_dimensions[2]){
        if (body->y < box_dimensions[1]){
            return 10;
        }
        if (body->y <= box_dimensions[1]+box_dimensions[3]){
            return 0;
        }
        if (body->y <= box_dimensions[1]+(2*box_dimensions[3])){
            return 1;
        }
        return 10;
    }
    if (body->x <= box_dimensions[0]+(2*box_dimensions[2])){
         if (body->y < box_dimensions[1]){
            return 10;
        }
        if (body->y <= box_dimensions[1]+box_dimensions[3]){
            return 2;
        }
        if (body->y <= box_dimensions[1]+(2*box_dimensions[3])){
            return 3;
        }
        return 10;
    }
    return 10;
}

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
    //double top_left_corner_x;
    //double top_left_corner_y;
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

    // node constructor with boundaries 
    Node(double box_dimensions[4]) {
        this->box_dimensions[0] = box_dimensions[0];
        this->box_dimensions[1] = box_dimensions[1];
        this->box_dimensions[2] = box_dimensions[2];
        this->box_dimensions[3] = box_dimensions[3];

        this->quad_nodes[0] = this->quad_nodes[1] = this->quad_nodes[2] = this->quad_nodes[3] = NULL;

        this->parent = NULL;
        this->body = NULL;

        this->center_of_mass[0] = this->center_of_mass[1] = 0.0;
        this->total_mass = 0.0;

        this->contains_body = false;
        this->external = true;
    }

    Node(Node* parent, int quadrant){
        this->parent = parent;
        this->contains_body = false;
        // --> body not necessary 

        this->external = true;
        // --> center of mass and total mass not necessary
        // 4 children not necessary  

        // create new quadrant dimensions from parent
        if (quadrant = 0){
            // NE 
            this->box_dimensions[0] = parent->box_dimensions[0];
            this->box_dimensions[1] = parent->box_dimensions[1];
        }
        if (quadrant = 1){
            // NE 
            this->box_dimensions[0] = parent->box_dimensions[0]+parent->box_dimensions[2];
            this->box_dimensions[1] = parent->box_dimensions[1];
        }
        if (quadrant = 2){
            // NE 
            this->box_dimensions[0] = parent->box_dimensions[0];
            this->box_dimensions[1] = parent->box_dimensions[1]+parent->box_dimensions[3];
        }
        else {
            // NE 
            this->box_dimensions[0] = parent->box_dimensions[0]+parent->box_dimensions[2];
            this->box_dimensions[1] = parent->box_dimensions[1]+parent->box_dimensions[3];
        }
        this->box_dimensions[2] = parent->box_dimensions[2]/2;
        this->box_dimensions[3] = parent->box_dimensions[3]/2;

        this->quad_nodes[0] = this->quad_nodes[1] = this->quad_nodes[2] = this->quad_nodes[3] = NULL;
        this->body = NULL;

        this->center_of_mass[0] = this->center_of_mass[1] = 0.0;
        this->total_mass = 0.0;
    }
    Node(Body* body){
        this->body;
    }
};


//----------------------------------------------------------------------------------------------------

class BH_Tree {
protected:
    Node* root;    

public:
    BH_Tree() {
        this->root = new Node();
    };
    ~BH_Tree();
    void add_bodies(std::vector<Body*> bodies);
    void insert(Node* node, Body* body);
};

void BH_Tree::add_bodies(std::vector<Body*> bodies){
    for (int k = 0; k < bodies.size(); k++){
        int quad = which_quadrant(bodies[k], root->box_dimensions);
        insert(root->quad_nodes[quad], bodies[k]);
    }
     
};

void BH_Tree::insert(Node *node, Body* body){
    // internal node
    if (!(node->external)){
        int new_quad = which_quadrant(body, node->box_dimensions);
        insert(node->quad_nodes[new_quad], body);
        
    }

    // now only external nodes: either contain a body or not

    // node does not contain a body 
    else if (!node->contains_body){
        // necessarily an external node
        node->body = body;
        node->contains_body = true;
        node->center_of_mass[0] = body->x;
        node->center_of_mass[1] = body->y;
        node->total_mass = body->mass;
    }

    // node contains body
    else if (node->external){
        // now it will not be external 
        node->external = false;

        // create children 
        Node* new_NW = new Node(node, 0);
        Node* new_NE = new Node(node, 1);
        Node* new_SW = new Node(node, 2);
        Node* new_SE = new Node(node, 3);
        node->quad_nodes[0] = new_NW;
        node->quad_nodes[1] = new_NE;
        node->quad_nodes[2] = new_SW;
        node->quad_nodes[3] = new_SE;

        int new_quad = which_quadrant(node->body, node->box_dimensions);
        insert(node->quad_nodes[new_quad],node->body);
        int quad = which_quadrant(body, node->box_dimensions);
        insert(node->quad_nodes[quad],body);
    }

};

void new_split(double box_dimensions[4], int quadrant){
    if (quadrant == 0){
        box_dimensions[2] = box_dimensions[2]/2;
        box_dimensions[3] = box_dimensions[3]/2;
    }
    else if (quadrant == 0){
        box_dimensions[2] = box_dimensions[2]/2;
        box_dimensions[3] = box_dimensions[3]/2;
        box_dimensions[0] += box_dimensions[2];
    }
    else if (quadrant == 0){
        box_dimensions[2] = box_dimensions[2]/2;
        box_dimensions[3] = box_dimensions[3]/2;
        box_dimensions[1] += box_dimensions[3];
    }
    else if (quadrant == 0){
        box_dimensions[2] = box_dimensions[2]/2;
        box_dimensions[3] = box_dimensions[3]/2;
        box_dimensions[0] += box_dimensions[2];
        box_dimensions[1] += box_dimensions[3];
    }
}

int main(){
    std::cout<<"hello yay";
    return 0;
}


