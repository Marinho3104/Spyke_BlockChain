
#ifdef WALLET_TEST

#include <iostream>

#include "wallet.h"
#include "blockchain_rules.h"

int main() {

    blockchain::update_blockchain_variables();

    wallet::Wallet* _wallet = 
        wallet::Wallet::by_file( 0 );

    _wallet->run();

    // _wallet->save_key_pair( 0 );

    free( _wallet );

}

#endif