#include "utils.cpp"

//----------------------------------------------------------------------------------------------------
// Implementation of BARNES HUT TREE 

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
    void compute_force_on_b(Body* b, double theta, double &F_x, double &F_y);
};


void BH_Tree::add_bodies(std::vector<Body*> bodies){
    // Adds a vector of bodies to the Barnes Hut Tree 
    this->root->external = false;
    for (int k = 0; k < bodies.size(); k++){
        int quad = which_quadrant(bodies[k], root->box_dimensions);
        if (quad >= 10){
            std::cout<<"Body out of bounds!! \n";
            return;
        }
        this->root->total_mass += bodies[k]->mass;
        this->root->center_of_mass[0] = ((this->root->center_of_mass[0]*this->root->total_mass) + 
                                    (bodies[k]->x*bodies[k]->mass))/
                                    (bodies[k]->mass + this->root->total_mass);

        this->root->center_of_mass[1] = ((this->root->center_of_mass[1]*this->root->total_mass) + 
                                    (bodies[k]->y*bodies[k]->mass))/
                                    (bodies[k]->mass + this->root->total_mass);
        insert(this->root->quad_nodes[quad], bodies[k]);

    }
     
}

void BH_Tree::insert(Node *node, Body* b1){
    // Inserts a body from a given node

    // First we consider the internal node case 
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

    // Then we consider the external node case
    // There are two subcases:
        // contains a body 
        // does not contain a body 

    // Node does not contain a body 
    else if (!node->contains_body){
        // We insert the body at this node  
        // The node is still an external node       
        node->body = b1;
        node->contains_body = true;
        node->center_of_mass[0] = b1->x;
        node->center_of_mass[1] = b1->y;
        node->total_mass = b1->mass;
    }

    // Node contains body
    else if (node->external){
        // This node will become an internal node
        // We will add 4 children to this node
        // Then we will call the insert function on the previously existing body 
        // and the new body to be inserted 

        node->external = false;
        node->total_mass += b1->mass;
        node->center_of_mass[0] = ((node->center_of_mass[0]*node->total_mass) + 
                                    (b1->x*b1->mass))/(b1->mass + node->total_mass);

        node->center_of_mass[1] = ((node->center_of_mass[1]*node->total_mass) + 
                                    (b1->y*b1->mass))/(b1->mass + node->total_mass);

        // Create children 
        node->quad_nodes[0] = new Node(node, 0);
        node->quad_nodes[1] = new Node(node, 1);
        node->quad_nodes[2] = new Node(node, 2);
        node->quad_nodes[3] = new Node(node, 3);

        // Insert previously existing body into the correct child 
        int new_quad = which_quadrant(node->body, node->box_dimensions);

        if (new_quad >= 10){
            std::cout<<new_quad<<" Body out of bounds 1!! \n";
            return;
        }

        insert(node->quad_nodes[new_quad], node->body);

        // Insert new body into the correct child 
        int quad = which_quadrant(b1, node->box_dimensions);
        if (quad >= 10){
            std::cout<<" Body out of bounds 2!! \n";
            return;
        }

        insert(node->quad_nodes[quad], b1);
        
        // Update current node - does not contain a body anymore 
        node->body = NULL;
        node->contains_body = false;
    }

}

void print_node(Node* node, int depth){
    // Prints a node 

    for (int j = 0; j < depth; j++){
            std::cout<<"\t";
        }
    std::cout<<"- ";
    if (node->contains_body){
        std::cout<<node->body->mass<<"\n";
    }
    else{
        double& com = node->total_mass;
        std::cout<<"mass: "<<com<<"\n";
    }
}

void print_tree_children(Node* node, int depth){
    // Auxiliary function that prints a Barnes Hut Tree  

    depth++;
    print_node(node->quad_nodes[0], depth);
    if (!node->quad_nodes[0]->external){
        print_tree_children(node->quad_nodes[0], depth);
    }
    print_node(node->quad_nodes[1], depth);
    if (!node->quad_nodes[1]->external){
    print_tree_children(node->quad_nodes[1], depth);
    }
    print_node(node->quad_nodes[2], depth);
    if (!node->quad_nodes[2]->external){
        print_tree_children(node->quad_nodes[2], depth);
    }
    print_node(node->quad_nodes[3], depth);
    if (!node->quad_nodes[3]->external){
        print_tree_children(node->quad_nodes[3], depth);
    } 
}

void BH_Tree::print_tree(){
    // Prints a Barnes Hut Tree 

    double& com = root->total_mass;
    std::cout<<"mass: "<<com<<"\n";
    int depth = 0;
    print_tree_children(this->root, depth);
}


void compute_force_on_b_aux(Node* node, Body* b, double theta, double &F_x, double &F_y){
    // Auxiliary function for the recursive function that computes 
    // the forces acting on b 
    double Fx_temp;
    double Fy_temp;
    if (node->external && node->contains_body){
        if (node->body == b){
            return;
        }
        else{
            get_force_components(b, node->body, Fx_temp, Fy_temp);
            F_x += Fx_temp;
            F_y += Fy_temp;
        }
    }

    else if (!node->external){
        // internal 
        if (compare_ratio(node, b, theta)){
            get_force_components_com(node, b, Fx_temp, Fy_temp);
            F_x += Fx_temp;
            F_y += Fy_temp;
        }
        else {
            compute_force_on_b_aux(node->quad_nodes[0], b, theta, F_x, F_y);
            compute_force_on_b_aux(node->quad_nodes[1], b, theta, F_x, F_y);
            compute_force_on_b_aux(node->quad_nodes[2], b, theta, F_x, F_y);
            compute_force_on_b_aux(node->quad_nodes[3], b, theta, F_x, F_y);
        }
    }
}

void BH_Tree::compute_force_on_b(Body* b, double theta, double &F_x, double &F_y){
    // Function that computes the forces acting on b from this tree 

    compute_force_on_b_aux(this->root, b, theta, F_x, F_y);
}



