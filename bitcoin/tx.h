#ifndef BITCOIN_TX_H
#define BITCOIN_TX_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "bitcoin/address.h"
#include "bitcoin/scriptpubkey.h"
#include "bitcoin/transaction.h"
#include "util/util.h"

namespace bitcoin {
namespace tx {
Transaction* ParseTransaction(uint8_t* data);

/**
 * @brief Extracts the txids of the transactions that are being used as the inputs
 * of the transaction @param data.
 *
 * @param data
 * @return std::vector<std::array<uint8_t, 32>>
 */
std::vector<std::array<uint8_t, 32>> ExtractInputTXIDs(uint8_t* data);

/**
 * @brief Extracts the addresses of the transactions that are being used as the inputs
 * of the transaction @param data. Communicates with the database to access the addresses.
 *
 * @param data
 * @return std::vector<Address>
 */
std::vector<Address> ExtractInputAddresses(uint8_t* data);

/**
 * @brief Extracts the output addresses from the scriptpubkeys of the outputs of the transaction
 * @param data.
 *
 * @param data
 * @return std::vector<Address>
 */
std::vector<Address> ExtractOutputAddresses(uint8_t* data);

bool IsSegwit(uint8_t* data);

}  // namespace tx
}  // namespace bitcoin

#endif