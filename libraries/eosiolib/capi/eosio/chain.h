/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#pragma once

#include "types.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @addtogroup chain
 *  @ingroup c_api
 *  @brief Defines %C API for querying internal chain state
 *  @{
 */

 /**
  *  Gets the set of active producers.
  *
  *  @param producers - Pointer to a buffer of account names
  *  @param datalen - Byte length of buffer, when passed 0 will return the size required to store full output.
  *
  *  @return uint32_t - Number of bytes actually populated
  *  @pre `producers` is a pointer to a range of memory at least `datalen` bytes long
  *  @post the passed in `producers` pointer gets the array of active producers.
  *
  *  Example:
  *
  *  @code
  *  capi_name producers[21];
  *  uint32_t bytes_populated = get_active_producers(producers, sizeof(capi_name)*21);
  *  @endcode
  */
__attribute__((eosio_wasm_import))
uint32_t get_active_producers( capi_name* producers, uint32_t datalen );

/**
 * Deterministic per-producer statistics for an inclusive slot range ending at
 * the current committed chain head. The head slot is counted as produced by
 * its validated producer; earlier slots without blocks are counted as missed.
 * Results are sorted by producer name. Call with a null buffer and zero length
 * to query the required size, then call again with a sufficiently large buffer.
 *
 * The leading and trailing streaks make longest-missed-streak aggregation
 * across successive head-bounded calls lossless.
 */
typedef struct producer_slot_stats_v0 {
   capi_name producer;
   uint32_t expected;
   uint32_t produced;
   uint32_t missed;
   uint32_t longest_missed_streak;
   uint32_t leading_missed_streak;
   uint32_t trailing_missed_streak;
} producer_slot_stats_v0;

__attribute__((eosio_wasm_import))
int32_t get_producer_slot_stats(uint32_t from_slot, uint32_t to_slot,
                                char* stats, uint32_t datalen);

typedef struct gas_fee_totals_v0 {
   uint64_t total_gas_consumed;
   uint64_t total_gas_value_flon;
} gas_fee_totals_v0;

__attribute__((eosio_wasm_import))
int32_t get_gas_fee_totals(char* totals, uint32_t datalen);

#ifdef __cplusplus
}
#endif
/// @}
