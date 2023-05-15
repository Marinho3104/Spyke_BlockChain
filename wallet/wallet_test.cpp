
#ifdef WALLET_TEST

#include <iostream>

#include "wallet.h"
#include "blockchain_rules.h"

int main() {

    blockchain::update_blockchain_variables();

    wallet::Wallet* _wallet = 
        wallet::Wallet::get_new_wallet( 0 );

    _wallet->run();

    free( _wallet );

}

#endif