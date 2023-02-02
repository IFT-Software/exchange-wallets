#include <numeric>

#include "gtest/gtest.h"
#include "util/util.h"

// this test suite was written by chatgpt :}

TEST(SwapEndianTest, EmptyInput) {
  std::vector<uint8_t> input;
  std::vector<uint8_t> res;
  util::SwapEndian(input, res);
  EXPECT_EQ(input, res);
}

TEST(SwapEndianTest, OddInput) {
  std::vector<uint8_t> input = {1, 2, 3};
  std::vector<uint8_t> expected_output = {3, 2, 1};
  std::vector<uint8_t> res;
  util::SwapEndian(input, res);
  EXPECT_EQ(expected_output, res);
}

TEST(SwapEndianTest, EvenInput) {
  std::vector<uint8_t> input = {1, 2, 3, 4};
  std::vector<uint8_t> expected_output = {4, 3, 2, 1};
  std::vector<uint8_t> res;
  util::SwapEndian(input, res);
  EXPECT_EQ(expected_output, res);
}

TEST(SwapEndianTest, LargeInput) {
  std::vector<uint8_t> input(1000);
  std::iota(input.begin(), input.end(), 0);
  std::vector<uint8_t> expected_output(input.rbegin(), input.rend());
  std::vector<uint8_t> res;
  util::SwapEndian(input, res);
  EXPECT_EQ(expected_output, res);
}

TEST(SwapEndianStrTest, EmptyInput) {
  std::string input = "";
  std::string res = util::SwapEndian(input);
  EXPECT_EQ(input, res);
}

TEST(SwapEndianStrTest, EvenInput) {
  std::string input = "abcd1234";
  std::string expected_output = "3412cdab";
  std::string res = util::SwapEndian(input);
  EXPECT_EQ(expected_output, res);
}

TEST(SwapEndianStrTest, LargeEvenInput) {
  std::string input(2000, 'a');
  std::string expected_output(input.rbegin(), input.rend());
  std::string res = util::SwapEndian(input);
  EXPECT_EQ(expected_output, res);
}

TEST(SwapEndianStrTest, ComplexEvenInput) {
  std::string input = "0123456789abcdef0123456789abcdef";
  std::string expected_output = "efcdab8967452301efcdab8967452301";
  std::string res = util::SwapEndian(input);
  std::cout << res << std::endl;
  EXPECT_EQ(expected_output, res);
}

TEST(SwapEndianStrTest, EvenInputWith0) {
  std::string input = "abcd0012";
  std::string expected_output = "1200cdab";
  std::string res = util::SwapEndian(input);
  EXPECT_EQ(expected_output, res);
}

template <size_t N>
void CheckSwapEndian(const std::array<uint8_t, N>& input, std::array<uint8_t, N>& expected_output) {
  std::array<uint8_t, N> output;
  util::SwapEndian(input, output);
  EXPECT_EQ(output, expected_output);
}

TEST(SwapEndianArrTest, BasicTest) {
  const std::array<uint8_t, 4> input1 = {0x01, 0x02, 0x03, 0x04};
  std::array<uint8_t, 4> expected_output1 = {0x04, 0x03, 0x02, 0x01};
  CheckSwapEndian(input1, expected_output1);

  const std::array<uint8_t, 6> input2 = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  std::array<uint8_t, 6> expected_output2 = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  CheckSwapEndian(input2, expected_output2);

  const std::array<uint8_t, 2> input3 = {0x01, 0x02};
  std::array<uint8_t, 2> expected_output3 = {0x02, 0x01};
  CheckSwapEndian(input3, expected_output3);

  const std::array<uint8_t, 8> input4 = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  std::array<uint8_t, 8> expected_output4 = {0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01};
  CheckSwapEndian(input4, expected_output4);

  std::array<uint8_t, 50> input5;
  std::array<uint8_t, 50> expected_output5;
  std::memset(input5.data(), 0x55, sizeof(input5));
  std::memset(expected_output5.data(), 0x55, sizeof(expected_output5));
  std::reverse_copy(input5.begin(), input5.end(), expected_output5.begin());
  CheckSwapEndian(input5, expected_output5);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}