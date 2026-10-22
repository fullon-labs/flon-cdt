#pragma once
#include <vector>
#include "../../core/eosio/check.hpp"
#include "../../core/eosio/name.hpp"
#include "../../core/eosio/crypto.hpp"

namespace eosio {

  /**
   *  @defgroup producer_key Producer Key
   *  @ingroup contracts
   *  @ingroup types
   *  @brief Maps producer with its signing key, used for producer schedule
   */

  /**
   *  Maps producer with its signing key, used for producer schedule
   *
   *  @ingroup producer_key
   */
  struct producer_key {

    /**
     *  Name of the producer
     *
     *  @ingroup producer_key
     */
    name             producer_name;

    /**
     *  Block signing key used by this producer
     *
     *  @ingroup producer_key
     */
    public_key       block_signing_key;

    /// @cond OPERATORS

    friend constexpr bool operator < ( const producer_key& a, const producer_key& b ) {
      return a.producer_name < b.producer_name;
    }

    /// @endcond

    CDT_REFLECT(producer_name, block_signing_key);
  };

   /**
    *  @defgroup producer_schedule Producer Schedule
    *  @ingroup contracts
    *  @ingroup types
    *  @brief Defines both the order, account name, and signing keys of the active set of producers.
    */

   /**
    * Defines both the order, account name, and signing keys of the active set of producers.
    *
    * @ingroup producer_schedule
    */
   struct producer_schedule {
      /**
       * Version number of the schedule. It is sequentially incrementing version number
       */
      uint32_t                     version;

      /**
       * List of producers for this schedule, including its signing key
       */
      std::vector<producer_key>    producers;

      CDT_REFLECT(version, producers);
   };

   /**
    *  @defgroup producer_authority Producer Authority
    *  @ingroup contracts
    *  @ingroup types
    *  @brief Maps producer with its a flexible authority structure, used for producer schedule
    */

   /**
    * pairs a public key with an integer weight
    *
    * @ingroup producer_authority
    */
   struct key_weight {
      /**
       * public key used in a weighted threshold multi-sig authority
       *
       * @brief public key used in a weighted threshold multi-sig authority
       */
      public_key key;

      /**
       * weight associated with a signature from the private key associated with the accompanying public key
       *
       * @brief weight of the public key
       */
      uint16_t   weight;

      CDT_REFLECT(key, weight);
   };

   /**
    * block signing authority version 0
    * this authority allows for a weighted threshold multi-sig per-producer
    *
    * @ingroup producer_authority
    *
    * @brief weighted threshold multi-sig authority
    */
   struct block_signing_authority_v0 {
      /**
       * minimum threshold of accumulated weights from component keys that satisfies this authority
       *
       * @brief minimum threshold of accumulated weights from component keys that satisfies this authority
       */
      uint32_t                    threshold;

      /**
       * component keys and their associated weights
       *
       * @brief component keys and their associated weights
       */
      std::vector<key_weight>     keys;

      bool is_valid()const;

      CDT_REFLECT(threshold, keys);
   };

   /**
    * variant of all possible block signing authorities
    *
    * @ingroup producer_authority
    */
   using block_signing_authority = std::variant<block_signing_authority_v0>;

   /**
    * Maps producer with its signing key, used for producer schedule
    *
    * @ingroup producer_authority
    *
    * @brief Maps producer with its signing key
    */
   struct producer_authority {

      /**
       * Name of the producer
       *
       * @brief Name of the producer
       */
      name             producer_name;

      /**
       * The block signing authority used by this producer
       */
      block_signing_authority       authority;

      friend constexpr bool operator < ( const producer_authority& a, const producer_authority& b ) {
         return a.producer_name < b.producer_name;
      }

      CDT_REFLECT(producer_name, authority);
   };

   /**
    *  Returns back the list of active producer names.
    *
    *  @ingroup producer_schedule
    */
   std::vector<name> get_active_producers();

   /**
    * Return deterministic production statistics for the inclusive slot range.
    * The range must end at the current committed chain head. The head slot is
    * counted as produced by its validated producer; earlier slots without
    * blocks are counted as missed. Results are sorted by producer name.
    */
   struct producer_slot_stats {
      name producer;
      uint32_t expected;
      uint32_t produced;
      uint32_t missed;
      uint32_t longest_missed_streak;
      uint32_t leading_missed_streak;
      uint32_t trailing_missed_streak;
   };

   std::vector<producer_slot_stats> get_producer_slot_stats(uint32_t from_slot,
                                                            uint32_t to_slot);

   /** Monotonic chain-wide gas totals exposed after GAS_FEE_TOTALS activation. */
   struct gas_fee_totals {
      uint64_t total_gas_consumed;
      uint64_t total_gas_value_flon;
   };

   namespace internal {
      extern "C" __attribute__((eosio_wasm_import))
      int32_t get_gas_fee_totals_intrinsic(char*, uint32_t) asm("get_gas_fee_totals");
   }

   inline gas_fee_totals get_gas_fee_totals() {
      gas_fee_totals result{};
      const auto written = internal::get_gas_fee_totals_intrinsic(
         reinterpret_cast<char*>(&result), sizeof(result));
      eosio::check(written == sizeof(result), "failed to read gas fee totals");
      return result;
   }

} /// namespace eosio
