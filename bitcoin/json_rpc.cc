#include "bitcoin/json_rpc.h"

#include <iostream>
#include <map>
#include <new>
#include <stdexcept>
#include <type_traits>

#include "absl/strings/str_join.h"
#include "bitcoin/rpc_tx.h"
#include "boost/json.hpp"
#include "json_rpc.h"
#include "net/https.h"
#include "rpc_tx.h"
#include "util/exceptions.h"

namespace bitcoin {
namespace rpc {

bool DecodeRawTransaction(std::string tx_hex, RpcTx& res) {
  std::map<std::string, std::string> headers;
  headers["Content-Type"] = "text/plain";

  std::string post_data = absl::StrCat(
      "{\"jsonrpc\": \"1.0\", \"id\": \"curltest\", \"method\": \"decoderawtransaction\", "
      "\"params\": [\"",
      tx_hex, "\"]}");

  std::string response = net::https::Post("http://127.0.0.1:18332/", headers, post_data,
                                          net::https::WriteType::TO_STRING, "", "anan", "anan");

  // TODO: how should the size be determined?

  uint8_t buf[40096];
  json::static_resource mr(buf);
  // Create a parse_options object
  json::parse_options opts;
  // Enable the allow_comments option
  opts.allow_comments = true;
  // Enable the allow_trailing_commas option
  opts.allow_trailing_commas = true;

  // Parse the JSON string with the custom options
  json::value val = json::parse(response, &mr, opts);

  // TODO: this code assumes json::parse ALWAYS returns an object containing field "error"
  json::value err = val.as_object()["error"];

  if (!err.is_null()) {
    std::cerr << "rpc call cannot be made: decoderawtransaction \n"
              << "Error: " + json::serialize(err) << std::endl;
    res = RpcTx();
    return false;
  } else {
    res = RpcTx(val.as_object()["result"], false);
    return true;
  }
}

void GetRawTransaction(std::string tx_id, RpcTx& res) {
  std::map<std::string, std::string> headers;
  headers["Content-Type"] = "text/plain";

  std::string post_data = absl::StrCat(
      "{\"jsonrpc\": \"1.0\", \"id\": \"curltest\", \"method\": \"getrawtransaction\", "
      "\"params\": [\"",
      tx_id, "\", true]}");

  std::string response;
  try {
    response = net::https::Post("http://127.0.0.1:18332/", headers, post_data,
                                net::https::WriteType::TO_STRING, "", "anan", "anan");
  } catch (std::runtime_error e) {
    throw BitcoindNetworkException("Can't connect to bitcoin deamon.");
  }

  uint8_t buf[400096];
  json::static_resource mr(buf);
  json::parse_options opts;
  opts.allow_comments = true;
  opts.allow_trailing_commas = true;

  // Parse the JSON string with the custom options
  json::value val;

  try {
    val = json::parse(response, &mr, opts);
  } catch (std::exception e) {
    throw JSONAllocationException(
        "Allocation error occured while parsing bitcoind response. (GetRawTransaction)", e.what());
  }

  json::value err = val.as_object()["error"];

  if (!err.is_null()) {
    throw BitcoindResponseException("Can't parse the bitcoind response. (GetRawTransaction)",
                                    json::serialize(err));
  } else {
    res = RpcTx(val.as_object()["result"], true);
  }
}

void GetMempoolTxs(std::vector<RpcTx>& res) {
  std::map<std::string, std::string> headers;
  headers["Content-Type"] = "text/plain";

  std::string post_data = absl::StrCat(
      "{\"jsonrpc\": \"1.0\", \"id\": \"curltest\", \"method\": \"getrawmempool\", "
      "\"params\": [true]}");

  std::string response;

  std::cout << "inside getmempool 1" << std::endl;

  try {
    response = net::https::Post("http://127.0.0.1:18332/", headers, post_data,
                                net::https::WriteType::TO_STRING, "", "anan", "anan");
  } catch (std::runtime_error e) {
    throw BitcoindNetworkException("Can't connect to bitcoin deamon.");
  }

  std::cout << "inside getmempool 2" << std::endl;

  uint8_t buf[400096];
  json::static_resource mr(buf);
  json::parse_options opts;
  opts.allow_comments = true;
  opts.allow_trailing_commas = true;
  std::cout << "inside getmempool 3" << std::endl;

  // TODO: how to determine this size? This seems like a temporary solution.
  // Parse the JSON string with the custom options. Use a bigger buffer if parsing throws an
  // exception because of buffer size.

  json::value val;

  try {
    val = json::parse(response, &mr, opts);
  } catch (std::exception e) {
    throw JSONAllocationException(
        "Allocation error occured while parsing bitcoind response. (GetMempoolTxs)", e.what());
  }

  std::cout << "inside getmempool 4" << std::endl;

  json::value err = val.as_object()["error"];
  if (!err.is_null()) {
    throw BitcoindResponseException("Can't parse the bitcoind response. (GetMempoolTxs)",
                                    json::serialize(err));
  }

  std::cout << "inside getmempool 5" << std::endl;

  std::string s = json::serialize(val.as_object()["result"].as_object());

  // std::cout << val.as_object()["result"] << std::endl;

  std::stringstream ss;
  ss << s;
  // std::cout << ss.str() << std::endl;

  pt::ptree root;

  std::cout << "inside getmempool 6" << std::endl;
  // pt::read_json(ss, root);

  try {
    pt::read_json(ss, root);
  } catch (std::exception e) {
    std::cout << "inside getmempool, throwing exception" << std::endl;
    throw BitcoindResponseException("Can't parse the bitcoind response. (GetMempoolTxs)", e.what());
  }

  std::cout << "inside getmempool 7" << std::endl;

  for (const auto& t : root) {
    RpcTx rpc_res;
    GetRawTransaction(t.first, rpc_res);
    res.push_back(rpc_res);
  }

  std::cout << "returning cout" << std::endl;
}

}  // namespace rpc
}  // namespace bitcoin