// generates seed for the private and public keys using bip39 protocol
#include "bip39.h"

BIP39::BIP39() : passphrase_("")
{
  GenerateSeed();
}

BIP39::BIP39(std::string passphrase) : passphrase_(passphrase)
{
  GenerateSeed(passphrase);
}

std::bitset<128> BIP39::GetEntropy()
{
  return entropy_;
}

std::bitset<4> BIP39::GetChecksum()
{
  return checksum_;
}

std::string BIP39::GetMnemonic()
{
  return mnemonic_;
}

std::array<uint8_t, 64> BIP39::GetSeed()
{
  return seed_;
}

std::string BIP39::GetEntropyStr()
{
  return entropy_.to_string();
}

std::string BIP39::GetSeedStr()
{
  return Util::BytesToHex(seed_.begin(), seed_.size());
}

std::string BIP39::GetChecksumStr()
{
  return checksum_.to_string();
}

std::bitset<128> BIP39::GenerateEntropy()
{
  int res = RAND_bytes(bytes_.begin(), bytes_.size());
  if (res != 1)
  {
    throw std::runtime_error("cannot generate entropy");
  }
  std::string hex_bytes = Util::BytesToHex(bytes_.begin(), bytes_.size());
  entropy_ = std::bitset<128>(Util::HexToBin(hex_bytes));

  return entropy_;
}

std::bitset<4> BIP39::GenerateChecksum()
{
  int checksum_size = (entropy_).size() / 32; // 1 bit checksum for every 32 bits of entropy

  uint8_t hex_sha256_bytes[32];
  Crypto::SHA256(bytes_.begin(), bytes_.size(), hex_sha256_bytes);

  std::string hex_sha256 = Util::BytesToHex(hex_sha256_bytes, 32);
  std::string bin_sha256 = Util::HexToBin(hex_sha256);

  // std::string checksum_str = bin_sha256.substr(0, checksum_size);

  checksum_ = std::bitset<4>(bin_sha256.substr(0, checksum_size));
  return checksum_;
}

std::string BIP39::GenerateMnemonic()
{
  std::string combined = this->GetEntropyStr() + this->GetChecksumStr();

  std::vector<uint64_t> int_list = Util::UnpackBitStr(combined, 11, 0);

  std::vector<std::string> wordlist;
  std::vector<std::string> words;
  std::string mnemonic = "";

  wordlist = Util::MakeWordList("lib/wordlist.txt");

  for (uint64_t i : int_list)
  {
    std::string s = wordlist[i];
    if (!s.empty() && s[s.length() - 1] == '\n')
    {
      s.erase(s.length() - 1);
    }
    mnemonic += s + " ";
  }

  if (!mnemonic.empty())
  {
    mnemonic.erase(mnemonic.length() - 1);
  }

  // std::cout << mnemonic << std::endl;
  mnemonic_ = mnemonic;
  return mnemonic;
}

std::array<uint8_t, 64> BIP39::GenerateSeed()
{
  this->GenerateEntropy();
  this->GenerateChecksum();
  this->GenerateMnemonic();
  std::string salt = "mnemonic" + passphrase_;
  // const uint8_t* salt = (uint8_t*) ((this->mnemonic + this->passphrase).c_str());
  int iterations = 2048;
  int keylength = 64;

  Crypto::PBKDF2_HMAC_SHA512(this->GetMnemonic().c_str(), (uint8_t *)salt.c_str(), salt.size(), iterations, keylength, seed_.begin());
  return seed_;
}

std::array<uint8_t, 64> BIP39::GenerateSeed(std::string passphrase)
{
  passphrase_ = passphrase;
  return GenerateSeed();
}

std::array<uint8_t, 64> BIP39::GenerateSeedFromEntropy(std::bitset<128> entropy)
{
  Util::BinToBytes(entropy, bytes_.begin());
  entropy_ = entropy;
  this->GenerateChecksum();
  this->GenerateMnemonic();
  std::string salt = "mnemonic" + passphrase_;
  // const uint8_t* salt = (uint8_t*) ((this->mnemonic + this->passphrase).c_str());
  int iterations = 2048;
  int keylength = 64;

  Crypto::PBKDF2_HMAC_SHA512(this->GetMnemonic().c_str(), (uint8_t *)salt.c_str(), salt.size(), iterations, keylength, seed_.begin());

  // std::string seed = Util::BytesToString(seed_bytes, 64);
  // this->seed = seed_bytes;
  return seed_;
}
