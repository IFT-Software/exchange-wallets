// generates seed for the private and public keys using bip39 protocol
#include "lib/bip39.h"

#include <array>
#include <bitset>
#include <string>

BIP39::BIP39() : passphrase_("") { GenerateSeed(); }

BIP39::BIP39(std::string passphrase) : passphrase_(passphrase) { GenerateSeed(passphrase); }

const std::bitset<128> BIP39::GetEntropy() { return entropy_; }

const std::bitset<4> BIP39::GetChecksum() { return checksum_; }

const std::string BIP39::GetMnemonic() { return mnemonic_; }

const std::array<uint8_t, 64> BIP39::GetSeed() { return seed_; }

const std::string BIP39::GetEntropyStr() { return entropy_.to_string(); }

const std::string BIP39::GetSeedStr() { return util::BytesToHex(seed_); }

const std::string BIP39::GetChecksumStr() { return checksum_.to_string(); }

const std::bitset<128> BIP39::GenerateEntropy() {
  int res = RAND_bytes(bytes_.begin(), bytes_.size());
  if (res != 1) {
    throw std::runtime_error("cannot generate entropy");
  }
  std::string hex_bytes = util::BytesToHex(bytes_);
  entropy_ = std::bitset<128>(util::HexToBin(hex_bytes));

  return entropy_;
}

const std::bitset<4> BIP39::GenerateChecksum() {
  int checksum_size = (entropy_).size() / 32;  // 1 bit checksum for every 32 bits of entropy

  std::array<uint8_t, 32> hex_sha256_bytes;
  util::crypto::SHA256(bytes_.begin(), bytes_.size(), hex_sha256_bytes.begin());

  std::string hex_sha256 = util::BytesToHex(hex_sha256_bytes);
  std::string bin_sha256 = util::HexToBin(hex_sha256);

  // std::string checksum_str = bin_sha256.substr(0, checksum_size);

  checksum_ = std::bitset<4>(bin_sha256.substr(0, checksum_size));
  return checksum_;
}

const std::string BIP39::GenerateMnemonic() {
  std::string combined = GetEntropyStr() + GetChecksumStr();

  std::vector<uint64_t> int_list = util::UnpackBitStr(combined, 11, 0);

  std::vector<std::string> wordlist;
  std::vector<std::string> words;
  std::string mnemonic = "";

  wordlist = util::MakeWordList("lib/wordlist.txt");

  for (uint64_t i : int_list) {
    std::string s = wordlist[i];
    if (!s.empty() && s[s.length() - 1] == '\n') {
      s.erase(s.length() - 1);
    }
    mnemonic += s + " ";
  }

  if (!mnemonic.empty()) {
    mnemonic.erase(mnemonic.length() - 1);
  }

  // std::cout << mnemonic << std::endl;
  mnemonic_ = mnemonic;
  return mnemonic;
}

const std::array<uint8_t, 64> BIP39::GenerateSeed() {
  GenerateEntropy();
  GenerateChecksum();
  GenerateMnemonic();
  const std::string salt = "mnemonic" + passphrase_;
  // const uint8_t* salt = (uint8_t*) ((this->mnemonic +
  // this->passphrase).c_str());
  int iterations = 2048;
  int keylength = 64;

  util::crypto::PBKDF2_HMAC_SHA512(GetMnemonic().c_str(), (const uint8_t*)salt.c_str(), salt.size(),
                                   iterations, keylength, seed_.begin());
  return seed_;
}

const std::array<uint8_t, 64> BIP39::GenerateSeed(std::string& passphrase) {
  passphrase_ = passphrase;
  return GenerateSeed();
}

const std::array<uint8_t, 64> BIP39::GenerateSeedFromEntropy(std::bitset<128>& entropy) {
  util::BinToBytes(entropy, bytes_);
  entropy_ = entropy;

  GenerateChecksum();
  GenerateMnemonic();

  const std::string salt = "mnemonic" + passphrase_;
  // const uint8_t* salt = (uint8_t*) ((this->mnemonic +
  // this->passphrase).c_str());
  int iterations = 2048;
  int keylength = 64;

  util::crypto::PBKDF2_HMAC_SHA512(GetMnemonic().c_str(), (const uint8_t*)salt.c_str(), salt.size(),
                                   iterations, keylength, seed_.begin());

  // std::string seed = util::BytesToString(seed_bytes, 64);
  // this->seed = seed_bytes;
  return seed_;
}
