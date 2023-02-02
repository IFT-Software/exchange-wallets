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

/**
 * @brief Parses the transaction data @param data which is in json format.
 *
 * @param data
 * @return Transaction*
 */
Transaction* ParseTransaction(json::value& data);

}  // namespace tx
}  // namespace bitcoin

#endif