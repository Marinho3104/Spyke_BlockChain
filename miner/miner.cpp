

#include "consensus.h"
#include "wallet.h"
#include "miner.h"
#include "node.h"

// Compiler libs
#include <iostream>
#include <thread>


miner::Miner::~Miner() {}

miner::Miner::Miner() { 

    wallet = 
        wallet::Wallet::by_file( 0 );
    
    node = 
        node::Node::by_file( 0, wallet->wallet_information.public_key );

}

void miner::Miner::run_interface() {

    std::cout << "\t\t--> Miner <--\n" << std::endl;

    std::cout << "0: Run" << std::endl;

    std::cout << "\n --> "; 

    std::string _; std::cin >> _;

    switch ( *_.c_str() )
    {
    case '0': 
    
        // Launch all proccess for miner execution
        launch_miner_proccesses();

        // Start node
        node->run();

        // Run node infterface for information
        node->run_interface();

        break;
    
    default:
        break;
    }

}

void miner::Miner::launch_miner_proccesses() {

    // Launch the consensus proccess
    std::thread(
        &consensus::consensus_main_process,
        this
    ).detach();

}

