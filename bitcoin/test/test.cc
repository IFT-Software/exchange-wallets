#include <iostream>
#include <map>
#include <string>

#include "absl/strings/str_join.h"
#include "net/https.h"

#include "bitcoin/transaction.h"
#include "bitcoin/tx.h"
#include "gtest/gtest.h"

// using namespace boost;

// TEST(Transaction, InputCtorTests) {}

TEST(Transaction, InputTest) {
  std::cout << "here " << std::endl;

  std::string tx1 =
      "020000000001011f3a9ebdaf82ee53ca0509d198d608d5b018e6d62ae840fd072de9c13d6b4f9f00000000000000"
      "000001f73c000000000000160014b946dedde9ca6f0e5e5566afe14da7a5aab2005a03483045022100ec5e018521"
      "4f517d4286036f1344ebf080666472e14eeff33a612de6d45127c4022009c0e40fff4f87fbf363aabd6e059fbb8e"
      "d0d4f5c2eba67280e231166e064a0f01483045022100c6c53c9d3cf978952bfc3b147c4522b8ed3aaba6dc5562d9"
      "9a628fc95bcd1a7a02207886e73e533ba01312d01bcf71d5c84abe21ae802f925f9851520497731affb801462103"
      "7f566f1c950ee71e5075a8358db812023f10af2c2ea11a3a623cda7c9fbcc07aad21024325de7661ea0de64ef0bc"
      "bd0dd9d2e4d9b5fe3b44423aac32c658682f26890dac00000000";

  std::cout << "hey " << std::endl;

  std::map<std::string, std::string> headers;
  headers["Content-Type"] = "text/plain";

  std::cout << "hey 1" << std::endl;

  // std::string post_data = absl::StrCat(
  //     "{\"jsonrpc\": \"1.0\", \"id\": \"curltest\", \"method\": \"decoderawtransaction\", "
  //     "\"params\": [\"",
  //     tx1, "\"]}");

  std::cout << "hey 2" << std::endl;

  // std::string response = net::https::Post("http://127.0.0.1:18332/", headers, post_data,
  //                                         net::https::WriteType::TO_STRING, "", "anan", "anan");

  std::cout << "hey 3" << std::endl;

  // uint8_t buf[4096];
  // json::static_resource mr(buf);
  // // Create a parse_options object
  // json::parse_options opts;
  // // Enable the allow_comments option
  // opts.allow_comments = true;

  // // Enable the allow_trailing_commas option
  // opts.allow_trailing_commas = true;

  // // Parse the JSON string with the custom options
  // json::value val = json::parse(response, &mr, opts);

  // std::cout << response << std::endl;
  // std::cout << "TXID: " << val.as_object()["result"].as_object()["txid"].as_string().c_str()
  //           << std::endl;

  // Transaction* tx = bitcoin::tx::ParseTransaction(val.as_object()["result"]);
  // std::cout << tx->hex() << std::endl;
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
