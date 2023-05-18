
#ifdef NODE_TEST

#include <iostream>

#include "node.h"

int main() {

    node::Node* _node = 
        node::Node::by_file( 0 );

    _node->run_interface();

    free( _node );    

}

#endif