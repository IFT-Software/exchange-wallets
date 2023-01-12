#include <array>
#include <chrono>
#include <cstdint>
#include <future>
#include <iostream>
#include <string>
#include <thread>

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
#include "socketio/comms.h"
#include "util/util.h"

int main(int argc, char** argv) {
  zmq::context_t ctx(4);

  comms::SubscriberThread(&ctx);
  auto thread = std::async(std::launch::async, &(comms::SubscriberThread), &ctx);
  thread.wait();

  // std::string tx =
  //     "010000000152828d67d748482ca6f71cf9f3555643cfa223eed411fb6084531b8a02bebc67010000006a47304402"
  //     "202d6aebf16023528228fa79e00a62eae5f325b59ef2e713b2dc0bd67e0d4688cb02201e654a6e81b8d111e8fff6"
  //     "e3f4fff17f3c438d09e4e1cba999ebcd7d795785870121037f78faffffefd89a886fc6871efb33a63f8ee4510244"
  //     "a8b5ea56887856459502ffffffff02f8350000000000001976a9140cb643567f4950c454476c58501b194bc6901a"
  //     "f988acd0200000000000001976a914a5bbd9e5314e0867c2081af2c52aee6dc1743e0d88ac00000000";

  // std::array<uint8_t, 225> res;
  // util::BinToBytes(std::bitset<225 * 8>(util::HexToBin(tx)), res);

  // Transaction* transaction = bitcoin::tx::ParseTransaction(res.begin());

  // pqxx::connection C("postgresql://postgres@localhost:5432");

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
