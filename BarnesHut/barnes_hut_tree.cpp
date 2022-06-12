#include "utils.cpp"

//----------------------------------------------------------------------------------------------------

double compute_distance(double center_of_mass[2], Body* b){
    // computes distance between a body and a center of mass 
    return pow(pow(b->x - center_of_mass[0], 2) 
                        + pow(b->y - center_of_mass[1],2), 0.5);
}

double compare_ratio(Node* node, Body* b, double theta){
    double distance = compute_distance(node->center_of_mass, b);
    return node->box_dimensions[3]/distance <= theta;
}

void get_force_components_com(Node* node, Body* b, double &F_x, double &F_y){ 
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

void compute_force_on_b_aux(Node* node, Body* b, double theta, double &F_x, double &F_y){
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
            get_force_components_com(node, node->body, Fx_temp, Fy_temp);
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
    void compute_force_on_b(Body* b, double theta, double &F_x, double &F_y);
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

void print_node(Node* node, int depth){
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
    double& com = root->total_mass;
    std::cout<<"mass: "<<com<<"\n";
    int depth = 0;
    print_tree_children(this->root, depth);
}

void BH_Tree::compute_force_on_b(Body* b, double theta, double &F_x, double &F_y){
    compute_force_on_b_aux(this->root, b, theta, F_x, F_y);
}



// int main(){
//     std::cout<<"hello yay \n ";
//     Body* b1 = new Body(0.1, 0.3, 1, 0, 0);
//     Body* b2 = new Body(0.1, 0.4, 2, 0, 0);
//     Body* b3 = new Body(0.6, 0.3, 3, 0, 0);
//     Body* b4 = new Body(0.6, 0.9, 4, 0, 0);
//     Body* b5 = new Body(0, 0.5, 5, 0, 0);
//     Body* b6 = new Body(0.9, 0.9, 6, 0, 0);
//     Body* b7 = new Body(0.55, 0.1, 7, 0, 0);
//     Body* b8 = new Body(0.93, 0.44, 8, 0, 0);
//     std::vector<Body*> bodies{b1, b2, b3, b4, b5, b6, b7, b8};
//     double box_dim[4] = {0,0,1,1};
//     BH_Tree* tree = new BH_Tree(box_dim, bodies);
//     tree->print_tree();
//     return 0;
// }


