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

bool IsSegwit(uint8_t* data);

}  // namespace tx
}  // namespace bitcoin

#endif