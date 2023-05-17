
#ifdef NODE_TEST

#include <iostream>

#include "node.h"

int main() {

    node::Node* _node = 
        node::Node::get_new_node();

    free( _node );    

}

#endif