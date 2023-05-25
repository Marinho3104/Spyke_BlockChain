

#ifdef MINER_TEST

#include <iostream>
#include "miner.h"
#include "blockchain_rules.h"

int main() {

    blockchain::update_blockchain_variables();

    miner::Miner _miner;

    _miner.run_interface();

    return 0;

}


#endif