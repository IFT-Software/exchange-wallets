#include <iostream>
#include <map>
#include <string>

#include "absl/strings/str_join.h"
#include "gtest/gtest.h"
#include "net/https.h"

#include "bitcoin/json_rpc.h"
#include "bitcoin/rpc_tx.h"
#include "bitcoin/transaction.h"

// using namespace boost;

TEST(RawTransactionTest, ValidTransaction) {
  std::string tx_id = "4b78c7e8fd7ad2cc820661ec3a7c48b54e6271c4ea093456d2778338a4edf910";
  RpcTx res_tx;
  ASSERT_TRUE(bitcoin::rpc::GetRawTransaction(tx_id, res_tx));
  ASSERT_TRUE(res_tx.IsValid());

  // std::string tx_hex = res_tx.GetHex();
  std::string tx_hex =
      "010000000001010000000000000000000000000000000000000000000000000000000000000000ffffffff210347"
      "e924045fdadf630c0800005132530000000000000a2f70656761706f6f6c2f00000000020000000000000000266a"
      "24aa21a9eda45ee46e3ce922a903ba64e0b928317f12585aa8237811444658c19b0bf264ace7c72a000000000017"
      "a914e25a7e08e6e072900eba0e4a933069846dd7e77b870120000000000000000000000000000000000000000000"
      "000000000000000000000000000000";

  std::cout << "tx hex: " << tx_hex << std::endl;
  RpcTx res_hx;
  ASSERT_TRUE(bitcoin::rpc::DecodeRawTransaction(tx_hex, res_hx));
  ASSERT_TRUE(res_hx.IsValid());
}

TEST(GetRawTransactionTest, InvalidTransaction) {
  std::string tx_id = "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";
  RpcTx res_tx;
  (bitcoin::rpc::GetRawTransaction(tx_id, res_tx));
  std::cout << "invalid data: " << res_tx.GetData() << std::endl;
  ASSERT_FALSE(bitcoin::rpc::GetRawTransaction(tx_id, res_tx));
  // ASSERT_FALSE(res_tx.IsValid());
}

TEST(DecodeRawTransactionTest, InvalidTransaction) {
  std::string tx_hex =
      "01000000000102ec212afe12df31e13456c2a1acc94f125278dba7b674c571bc0fdbc20c73a8130000000000ffff"
      "ffff01705d1e00000000001600148db50eb52063ea9d98b3eac91489a90f738986f603483045022100bb44ad87b3"
      "3373fd4c0e938945d29a99f4b89cb6067390ae8ad2c5dc49cfe741022024a6098f9a4fd7baae1d7dca7ccf0022ca"
      "d77d3a712e57c568dd129e8720976b012103989d253b17a6a0f41838b84ff0d20e8898f9d7b1a98f2564da4cc29d"
      "cf8581d95c14a0ee7a142d267c1f36714e4a8f75612f20a797207508ac85856d196ad4937576a9148db50eb52063"
      "ea9d98b3eac91489a90f738986f68763ac6776a9148db50eb52063ea9d98b3eac91489a90f738986f68804ef7a0a"
      "64b175ac6800000000";

  RpcTx res_hex;
  ASSERT_FALSE(bitcoin::rpc::DecodeRawTransaction(tx_hex, res_hex));
  ASSERT_FALSE(res_hex.IsValid());
}

TEST(ParseTransactionTest, ValidCoinbaseTransaction) {
  std::string tx_id = "d4cb98935358e22c4acc800e7078f2a954a8d48e9d885db95830f76c1f50ca78";

  RpcTx rpc_tx;
  bitcoin::rpc::GetRawTransaction(tx_id, rpc_tx);
  Transaction* tx1 = rpc_tx.ParseTransaction();
  std::cout << rpc_tx.GetHex() << std::endl;
  EXPECT_EQ(rpc_tx.GetHex(), tx1->hex());

  // std::string tx_hex =
  //     "010000000001010000000000000000000000000000000000000000000000000000000000000000ffffffff520338"
  //     "ea244c4c5c207465727261706f6f6c2e696f20626c6f636b206d696e656420627920636c65616e20656e65726779"
  //     "2076322e36382e30205c00000000b18985c3bcf66c00000521800001ce0000000000ffffffff02c59a2600000000"
  //     "00160014988568d5632eb2d863296f5e512a135d416025bc0000000000000000266a24aa21a9ed5d9e1d1e535b57"
  //     "10fb2c7d7a4c47c8b56407672a86781c688e44fbc1ce6372b2012000000000000000000000000000000000000000"
  //     "0000000000000000000000000000000000";

  // RpcTx rpc_hex;
  // bitcoin::rpc::GetRawTransaction(tx_hex, rpc_hex);
  // Transaction* tx2 = rpc_hex.ParseTransaction();
  // EXPECT_EQ(rpc_hex.GetHex(), tx2->hex());
}

TEST(InputCtorTest, InputCtorTests) {}

TEST(Transaction, InputTest) {}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
