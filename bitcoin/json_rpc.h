#ifndef BITCOIN_JSON_RPC_H
#define BITCOIN_JSON_RPC_H

// functions that make json-rpc calls to bitcoind

#include "absl/strings/str_join.h"
#include "boost/json.hpp"
#include "net/https.h"

namespace json = boost::json;

class RpcTx;

namespace bitcoin {
namespace rpc {

/**
 * @brief makes the call "decoderawtransaction @param tx_hex" to bitcoind, and writes the result (an
 * RPC_TX object) to @param res. If the call is unsucessful, the constructed RpcTx object is
 * invalid, and the function returns false.
 *
 * @param tx_hex the hex-encoded transaction
 * @param res RpcTx object
 * @return bool
 */
bool DecodeRawTransaction(std::string tx_hex, RpcTx& res);

/**
 * @brief makes the call "getrawtransaction @param tx_id true" to bitcoind, and writes the result
 * (an RPC_TX object) to @param res. If the call is unsucessful, the constructed RpcTx object is
 * invalid, and the function returns false.
 *
 * @param tx_id the transaction id
 * @param res RpcTx object
 * @return bool
 */
bool GetRawTransaction(std::string tx_id, RpcTx& res);

/**
 * @brief Returns the mempool transactions
 *
 * @return true
 * @return false
 */
bool GetMempoolTxs();

}  // namespace rpc
}  // namespace bitcoin

#endif