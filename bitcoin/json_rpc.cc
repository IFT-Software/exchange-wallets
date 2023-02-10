#include "bitcoin/json_rpc.h"

#include <iostream>
#include <map>
#include <stdexcept>
#include <type_traits>

#include "absl/strings/str_join.h"
#include "bitcoin/rpc_tx.h"
#include "boost/json.hpp"
#include "net/https.h"

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

bool GetRawTransaction(std::string tx_id, RpcTx& res) {
  std::map<std::string, std::string> headers;
  headers["Content-Type"] = "text/plain";

  std::string post_data = absl::StrCat(
      "{\"jsonrpc\": \"1.0\", \"id\": \"curltest\", \"method\": \"getrawtransaction\", "
      "\"params\": [\"",
      tx_id, "\", true]}");

  std::string response = net::https::Post("http://127.0.0.1:18332/", headers, post_data,
                                          net::https::WriteType::TO_STRING, "", "anan", "anan");

  uint8_t buf[40096];
  json::static_resource mr(buf);
  json::parse_options opts;
  opts.allow_comments = true;
  opts.allow_trailing_commas = true;

  // Parse the JSON string with the custom options
  json::value val = json::parse(response, &mr, opts);

  json::value err = val.as_object()["error"];

  if (!err.is_null()) {
    std::cerr << "rpc call cannot be made: decoderawtransaction \n"
              << "Error: " + json::serialize(err) << std::endl;
    res = RpcTx();
    return false;
  } else {
    res = RpcTx(val.as_object()["result"], true);
    return true;
  }
}

}  // namespace rpc
}  // namespace bitcoin