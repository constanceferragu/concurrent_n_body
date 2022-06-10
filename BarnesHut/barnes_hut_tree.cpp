#include "../n_body.cpp"
#include "utils.cpp"
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


//----------------------------------------------------------------------------------------------------

class BH_Tree {
protected:
    Node* root;    

public:
    BH_Tree() {
        this->root = new Node();
    };
    BH_Tree(double box_dimensions[4]){
        this->root = new Node(box_dimensions);
    }
    BH_Tree(double box_dimensions[4], std::vector<Body*> bodies){
        this->root = new Node(box_dimensions);
        add_bodies(bodies);
        std::cout<<"done creating tree \n";
    }
    ~BH_Tree();
    void add_bodies(std::vector<Body*> bodies);
    void insert(Node* node, Body* body);
    void print_tree();
};


void BH_Tree::add_bodies(std::vector<Body*> bodies){
    for (int k = 0; k < bodies.size(); k++){
        int quad = which_quadrant(bodies[k], root->box_dimensions);
        if (quad >= 10){
            std::cout<<"Body out of bounds!! \n";
            return;
        }
        insert(this->root->quad_nodes[quad], bodies[k]);

    }
     
}


void BH_Tree::insert(Node *node, Body* b1){
    // internal node

    if (!(node->external)){
        node->total_mass += b1->mass;
        node->center_of_mass[0] = ((node->center_of_mass[0]*node->total_mass) + 
                                    (b1->x*b1->mass))/(b1->mass + node->total_mass);

        node->center_of_mass[1] = ((node->center_of_mass[1]*node->total_mass) + 
                                    (b1->y*b1->mass))/(b1->mass + node->total_mass);
        int new_quad = which_quadrant(b1, node->box_dimensions);
        if (new_quad >= 10){
            std::cout<<" Body out of bounds!! \n";
            return;
        }
        
        insert(node->quad_nodes[new_quad], b1);
        
    }

    // now only external nodes: either contain a body or not

    // node does not contain a body 
    else if (!node->contains_body){
        // necessarily an external node
        
        node->body = b1;
        node->contains_body = true;
        node->center_of_mass[0] = b1->x;
        node->center_of_mass[1] = b1->y;
        node->total_mass = b1->mass;
    }

    // node contains body
    else if (node->external){
        // now it will not be external 

        node->external = false;
        node->total_mass += b1->mass;
        node->center_of_mass[0] = ((node->center_of_mass[0]*node->total_mass) + 
                                    (b1->x*b1->mass))/(b1->mass + node->total_mass);

        node->center_of_mass[1] = ((node->center_of_mass[1]*node->total_mass) + 
                                    (b1->y*b1->mass))/(b1->mass + node->total_mass);

        // create children 
        node->quad_nodes[0] = new Node(node, 0);
        node->quad_nodes[1] = new Node(node, 1);
        node->quad_nodes[2] = new Node(node, 2);
        node->quad_nodes[3] = new Node(node, 3);

        int new_quad = which_quadrant(node->body, node->box_dimensions);

        if (new_quad >= 10){
            std::cout<<new_quad<<" Body out of bounds 1!! \n";
            return;
        }

        insert(node->quad_nodes[new_quad], node->body);

        int quad = which_quadrant(b1, node->box_dimensions);
        if (quad >= 10){
            std::cout<<" Body out of bounds 2!! \n";
            return;
        }

        insert(node->quad_nodes[quad], b1);
        node->body = NULL;
        node->contains_body = false;
    }

}

void print_node(Node* node){
    if (node->contains_body){
        std::cout<<node->body->mass;
    }
    else{
        std::cout<<"empty";
    }
}

void print_tree_children(Node* node){
    print_node(node->quad_nodes[0]);
    print_node(node->quad_nodes[1]);
    print_node(node->quad_nodes[2]);
    print_node(node->quad_nodes[3]);
    std::cout<<"\n";
    if (!node->quad_nodes[0]->external){
        print_tree_children(node->quad_nodes[0]);
    }
    if (!node->quad_nodes[1]->external){
    print_tree_children(node->quad_nodes[1]);
    }
    if (!node->quad_nodes[2]->external){
        print_tree_children(node->quad_nodes[2]);
    }
    if (!node->quad_nodes[3]->external){
        print_tree_children(node->quad_nodes[3]);
    }
}

void BH_Tree::print_tree(){
    print_tree_children(this->root);
}



int main(){
    std::cout<<"hello yay \n ";
    Body* b1 = new Body(0.1, 0.3, 1, 0, 0);
    Body* b2 = new Body(0.1, 0.4, 2, 0, 0);
    Body* b3 = new Body(0.6, 0.3, 3, 0, 0);
    Body* b4 = new Body(0.6, 0.9, 4, 0, 0);
    std::vector<Body*> bodies{b1, b2, b3, b4};
    double box_dim[4] = {0,0,1,1};
    BH_Tree* tree = new BH_Tree(box_dim, bodies);
    tree->print_tree();
    return 0;
}


