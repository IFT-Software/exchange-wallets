#include <array>
#include <chrono>
#include <cstdint>
#include <future>
#include <iostream>
#include <map>
#include <string>
#include <thread>

#include "absl/strings/str_join.h"
#include "pqxx/pqxx"
#include "secp256k1.h"
#include "third_party/cppzmq/zmq.hpp"
#include "third_party/cppzmq/zmq_addon.hpp"
#include "third_party/gmpxx/gmpxx.h"

#include "bitcoin/address.h"
#include "bitcoin/bip39.h"
#include "bitcoin/crypto.h"
#include "bitcoin/privkey.h"
#include "bitcoin/pubkey.h"
#include "bitcoin/script.h"
#include "bitcoin/scriptpubkey.h"
#include "bitcoin/tx.h"
#include "db/managers/address_manager.h"
#include "db/managers/transaction_manager.h"
#include "db/managers/wallet_manager.h"
#include "db/postgresql.h"
#include "net/comms.h"
#include "net/https.h"
#include "util/util.h"

int main(int argc, char** argv) {
  // Postgresql* db = new Postgresql("postgres", "localhost", 5432, "postgres", "");

  DbWalletManager* db_wallet_mgr = new DbWalletManager(db);
  DbAddressManager* db_address_mgr = new DbAddressManager(db);
  DbTransactionManager* db_transaction_mgr = new DbTransactionManager(db);

  if (db->IsConnected()) {
    std::cout << "Db Connected" << std::endl;
    db_wallet_mgr->CreateTable();
    db_address_mgr->CreateTable();
    db_transaction_mgr->CreateTable();

    json::object res = db_transaction_mgr->Insert(
        {{"data",
          {{"txid", "blablabla"},
           {"version", 0},
           {"inputs",
            {{{"txid", "blablabla"}, {"vout", 3}, {"address", "abc"}},
             {{"txid", "blabla"}, {"vout", 2}, {"address", "def"}}}},
           {"outputs", {{{"address", "abc"}, {"value", 1}}, {{"address", "def"}, {"value", 2}}}},
           {"lock_time", 20}}}});

    // json::object res = db_wallet_mgr->Insert(
    //     {{"data",
    //       {{"name", "Alkim BTC2"},
    //        {"seed",
    //         "43076aea18722554d3993bde416cf3cd43f08bcb04715f7e43b9130898af7d5b0312db488ebd"
    //         "6e8a14604da8c98eb8dbab792d3a1005d4c2b6d654d3598cd6c5"},
    //        {"coin", "BTC"}}},
    //      {"select", {{"id", true}, {"name", true}, {"seed", true}, {"coin", true}}}});

    // std::cout << json::serialize(res) << std::endl;

    // json::object update_res = db_wallet_mgr->Update(
    //     {{"data",
    //       {{"seed",
    //         "24d5e9febf08a91daa5243a25b2d4ec0f09549b195539c188bcabacd7aeec689c933c942a55e4e0eec6524"
    //         "e017c8b31e5cd9ea3f87b29aea126202b2a25055d3"}}},
    //      {"where", {{"id", 20}}},
    //      {"select", {{"id", true}, {"name", true}, {"seed", true}, {"coin", true}}}});

    // std::cout << json::serialize(update_res) << std::endl;

    // json::object delete_res = db_wallet_mgr->Delete(
    //     {{"where", {{"id", 21}}},
    //      {"select", {{"id", true}, {"name", true}, {"seed", true}, {"coin", true}}}});

    // std::cout << json::serialize(delete_res) << std::endl;

    // json::array select_res = db_wallet_mgr->Select(
    //     {{"where", {{"id", 4}}},
    //      {"select", {{"id", true}, {"name", true}, {"seed", true}, {"coin", true}}}});

    // std::cout << json::serialize(select_res) << std::endl;

    // std::map<std::string, std::string> headers;
    // headers["Content-Type"] = "text/plain";

    // std::string rawtx_str =
    //     "020000000001011f3a9ebdaf82ee53ca0509d198d608d5b018e6d62ae840fd072de9c13d6b4f9f00000000000000"
    //     "000001f73c000000000000160014b946dedde9ca6f0e5e5566afe14da7a5aab2005a03483045022100ec5e018521"
    //     "4f517d4286036f1344ebf080666472e14eeff33a612de6d45127c4022009c0e40fff4f87fbf363aabd6e059fbb8e"
    //     "d0d4f5c2eba67280e231166e064a0f01483045022100c6c53c9d3cf978952bfc3b147c4522b8ed3aaba6dc5562d9"
    //     "9a628fc95bcd1a7a02207886e73e533ba01312d01bcf71d5c84abe21ae802f925f9851520497731affb801462103"
    //     "7f566f1c950ee71e5075a8358db812023f10af2c2ea11a3a623cda7c9fbcc07aad21024325de7661ea0de64ef0bc"
    //     "bd0dd9d2e4d9b5fe3b44423aac32c658682f26890dac00000000";

    // std::string post_data = absl::StrCat(
    //     "{\"jsonrpc\": \"1.0\", \"id\": \"curltest\", \"method\": \"decoderawtransaction\", "
    //     "\"params\": [\"",
    //     rawtx_str, "\"]}");
    std::string tx_id = "9f4f6b3dc1e92d07fd40e82ad6e618b0d508d698d10905ca53ee82afbd9e3a1f";

    // std::string post_data = absl::StrCat(
    //     "{\"jsonrpc\": \"1.0\", \"id\": \"curltest\", \"method\": \"decoderawtransaction\", "
    //     "\"params\": [\"",
    //     rawtx_str, "\"]}");

    std::string post_data = absl::StrCat(
        "{\"jsonrpc\": \"1.0\", \"id\": \"curltest\", \"method\": \"getrawtransaction\", "
        "\"params\": [\"",
        tx_id, "\", true]}");

    // std::string response = net::https::Post("http://127.0.0.1:18332/", headers, post_data,
    //                                         net::https::WriteType::TO_STRING, "", "anan",
    //                                         "anan");

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

    std::cout << response << std::endl;
    // json::array outputs;
    // outputs = val.as_object()["result"].as_object()["vout"].as_array();
    // std::cout << "here 1" << std::endl;
    // std::string output_addr =
    //     outputs[0].as_object()["scriptPubKey"].as_object()["address"].as_string().c_str();
    // std::cout << "output address: " << output_addr << std::endl;

    // std::cout << "TXID: " << val.as_object()["result"].as_object()["txid"].as_string().c_str()
    //           << std::endl;

    // std::cout << "before parsing" << std::endl;
    // Transaction* tx = bitcoin::tx::ParseTransaction(val.as_object()["result"]);
    // std::cout << "after parsing" << std::endl;
    Transaction* tx = bitcoin::tx::ParseTransaction(val.as_object()["result"]);

    // std::cout << tx->hex() << std::endl;
    std::vector<Address> input_addresses = tx->GetInputAddresses();
    for (Address a : input_addresses) {
      std::cout << a.GetStr() << std::endl;
    }

    // zmq::context_t ctx(4);

    // comms::SubscriberThread(&ctx);
    // auto thread = std::async(std::launch::async, &(comms::SubscriberThread), &ctx);
    // thread.wait();

    // std::string tx =
    // "010000000152828d67d748482ca6f71cf9f3555643cfa223eed411fb6084531b8a02bebc67010000006a47304402"
    // "202d6aebf16023528228fa79e00a62eae5f325b59ef2e713b2dc0bd67e0d4688cb02201e654a6e81b8d111e8fff6"
    // "e3f4fff17f3c438d09e4e1cba999ebcd7d795785870121037f78faffffefd89a886fc6871efb33a63f8ee4510244"
    // "a8b5ea56887856459502ffffffff02f8350000000000001976a9140cb643567f4950c454476c58501b194bc6901a"
    // "f988acd0200000000000001976a914a5bbd9e5314e0867c2081af2c52aee6dc1743e0d88ac00000000";

    // std::array<uint8_t, 225> res;
    // util::BinToBytes(std::bitset<225 * 8>(util::HexToBin(tx)), res);

    // Transaction* transaction = bitcoin::tx::ParseTransaction(res.begin());

    // pqxx::connection C("postgresql://postgres@localhost:5432");

    // if (C.is_open()) {
    //   std::cout << "actik: " << C.dbname() << std::endl;

    // } else {
    //   std::cout << "bum" << std::endl;
    // }

    // try {
    //   if (C.is_open()) {
    //     std::cout << "actik: " << C.dbname() << std::endl;

    //   } else {
    //     std::cout << "bum" << std::endl;
    //   }

    //   char* sql;

    // sql =
    //     "CREATE TABLE DAI ( "
    //     "ID INT PRIMARY KEY   NOT NULL,"
    //     "NAME           TEXT  NOT NULL,"
    //     "LAST_NAME      CHAR(50) NOT NULL,"
    //     "AGE            INT   NOT NULL,"
    //     "ADDRESS        CHAR(50),"
    //     "SALARY         REAL );";

    // sql =
    //     "ALTER TABLE DAI "
    //     "ADD LAST_NAME    CHAR(50)";

    // sql =
    //     "INSERT INTO DAI (ID,NAME,AGE,ADDRESS,SALARY, LAST_NAME) "
    //     "VALUES (1, 'Paul', 32, 'California', 20000.00, 'dai'); "
    //     "INSERT INTO DAI (ID,NAME,AGE,ADDRESS,SALARY, LAST_NAME) "
    //     "VALUES (2, 'Allen', 25, 'Texas', 15000.00, 'dai'); "
    //     "INSERT INTO DAI (ID,NAME,AGE,ADDRESS,SALARY, LAST_NAME)"
    //     "VALUES (3, 'Teddy', 23, 'Norway', 20000.00, 'dai');"
    //     "INSERT INTO DAI (ID,NAME,AGE,ADDRESS,SALARY, LAST_NAME)"
    //     "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00, 'dai');";

    // transactional object
    // pqxx::work W(C);

    // // execute
    // W.exec(sql);
    // W.commit();
    // std::cout << "records created" << std::endl;

    //   char* sql_select;
    //   sql_select = "SELECT * from DAI";

    //   // nontransactional object
    //   pqxx::nontransaction N(C);

    //   // execute
    //   pqxx::result R(N.exec(sql_select));

    //   for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
    //     std::cout << "ID " << c[0].as<uint32_t>() << std::endl;
    //     std::cout << "Name " << c[1].as<std::string>() << std::endl;
    //     std::cout << "Age " << c[2].as<uint32_t>() << std::endl;
    //     std::cout << "Address " << c[3].as<std::string>() << std::endl;
    //     std::cout << "Salary " << c[4].as<float>() << std::endl;
    //     std::cout << "LastName " << c[5].as<std::string>() << std::endl;
    //   }

    //   std::cout << "done" << std::endl;

    //   C.close();
    // }
    // catch (const std::exception& e) {
    //   std::cerr << e.what() << std::endl;
    // }

    return 0;
  }
