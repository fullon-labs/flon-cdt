#include <eosio/eosio.hpp>
#include <eosio/system.hpp>

class [[eosio::contract]] block_reference_data : public eosio::contract {
public:
   using eosio::contract::contract;

   [[eosio::action]]
   void read(uint32_t block_num) {
      eosio::checksum256 block_id;
      const bool found = eosio::get_recent_block_id(block_num, block_id);
      const uint32_t lib = eosio::get_last_irreversible_block_num();
      (void)found;
      (void)lib;
   }
};
