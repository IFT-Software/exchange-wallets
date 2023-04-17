#ifndef BITCOIN_JSON_RPC_H
#define BITCOIN_JSON_RPC_H

// functions that make json-rpc calls to bitcoind

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <stdexcept>
#include "absl/strings/str_join.h"
#include "boost/json.hpp"
#include "net/https.h"

namespace json = boost::json;
namespace pt = boost::property_tree;

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
void GetRawTransaction(std::string tx_id, RpcTx& res);

/**
 * @brief makes the call "getrawmempool true" to bitcoind, and writes the transaction ids that are
 * in the returned mempool to @param res. If the call is unsuccessfull, nothing is written to the
 * result vector, and the function returns false.
 *
 * @param res
 * @return true
 * @return false
 */
void GetMempoolTxs(std::vector<RpcTx>& res);

}  // namespace rpc
}  // namespace bitcoin

#endif