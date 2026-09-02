#include <eosio/system.h>
#include <stddef.h>

void test_system( void ) {
   struct capi_checksum256 block_id;

   eosio_assert(0, NULL);
   eosio_assert_message(0, NULL, 0);
   eosio_assert_code(0, 0);
   eosio_exit(0);
   current_time();
   get_recent_block_id(1, &block_id);
   get_last_irreversible_block_num();
   is_feature_activated(NULL);
   get_sender();
}
