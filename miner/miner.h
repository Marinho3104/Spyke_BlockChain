

#ifndef MINER_MINER_H
#define MINER_MINER_H

// Forward
namespace node {

    struct Node;

}

namespace wallet {

    struct Wallet;

}

namespace miner {

    struct Miner {

        // Miner node for p2p communications
        node::Node* node;

        // Miner Wallet info
        wallet::Wallet* wallet;

        ~Miner(); Miner();

        void run_interface();

        void launch_miner_proccesses();

    };

}

#endif