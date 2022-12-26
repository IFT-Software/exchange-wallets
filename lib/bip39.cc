// generates seed for the private and public keys using bip39 protocol
#include "bip39.h"

SeedGenerator::SeedGenerator() : passphrase("")
{
  this->GenerateSeed();
}

SeedGenerator::SeedGenerator(std::string passphrase) : passphrase(passphrase)
{
  this->GenerateSeed(passphrase);
}

std::bitset<128> SeedGenerator::GetEntropy()
{
  return this->entropy;
}

std::string SeedGenerator::GetChecksum()
{
  return this->checksum;
}

std::string SeedGenerator::GetEntropyStr()
{
  return (this->entropy).to_string();
}

std::string SeedGenerator::GetMnemonic()
{
  return this->mnemonic;
}

std::bitset<128> SeedGenerator::GenerateEntropy()
{
  int res = RAND_bytes(this->bytes, sizeof(this->bytes));
  if (res != 1)
  {
    throw std::runtime_error("cannot generate entropy");
  }
  std::string hex_bytes = Util::BytesToHex(this->bytes, 16);
  this->entropy = std::bitset<128>(Util::HexToBin(hex_bytes));

  // std::cout << "old entorpy: " << this->entropy << std::endl;

  uint8_t new_bytes[16];
  Util::BinToBytes(this->entropy, new_bytes);
  std::string new_hex_bytes = Util::BytesToHex(new_bytes, 16);
  std::bitset<128> new_entropy = std::bitset<128>(Util::HexToBin(new_hex_bytes));

  // std::cout << "new entropy: " << new_entropy << std::endl;
  // std::cout << "EXPECTED: " << (uint32_t)this->bytes[0] << " REAL: " << (uint32_t)new_bytes[0] << std::endl;
  return this->entropy;
}

std::string SeedGenerator::GenerateChecksum()
{
  int checksum_size = (this->entropy).size() / 32; // 1 bit checksum for every 32 bits of entropy

  uint8_t hex_sha256_bytes[32];
  Crypto::sha256(this->bytes, 16, hex_sha256_bytes);

  std::string hex_sha256 = Util::BytesToString(hex_sha256_bytes, 32);
  std::string bin_sha256 = Util::HexToBin(hex_sha256);

  std::string checksum = bin_sha256.substr(0, checksum_size);

  this->checksum = checksum;

  return checksum;
}

std::string SeedGenerator::GenerateMnemonic()
{
  std::string combined = this->GetEntropyStr() + this->checksum;

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
  this->mnemonic = mnemonic;
  return mnemonic;
}

std::string SeedGenerator::GenerateSeed()
{
  this->GenerateEntropy();
  this->GenerateChecksum();
  this->GenerateMnemonic();
  std::string salt = "mnemonic" + this->passphrase;
  // const uint8_t* salt = (uint8_t*) ((this->mnemonic + this->passphrase).c_str());
  int iterations = 2048;
  int keylength = 64;

  uint8_t seed_bytes[64];
  Crypto::pbkdf2_hmac_sha_512(this->GetMnemonic().c_str(), (uint8_t *)salt.c_str(), salt.size(), iterations, keylength, seed_bytes);

  std::string seed = Util::BytesToString(seed_bytes, 64);
  this->seed = seed;
  return seed;
}

std::string SeedGenerator::GenerateSeed(std::string passphrase)
{
  this->passphrase = passphrase;
  return GenerateSeed();
}

std::string SeedGenerator::GenerateSeedFromEntropy(std::bitset<128> entropy)
{
  Util::BinToBytes(entropy, this->bytes);
  this->entropy = entropy;
  this->GenerateChecksum();
  this->GenerateMnemonic();
  std::string salt = "mnemonic" + this->passphrase;
  // const uint8_t* salt = (uint8_t*) ((this->mnemonic + this->passphrase).c_str());
  int iterations = 2048;
  int keylength = 64;

  uint8_t seed_bytes[64];
  Crypto::pbkdf2_hmac_sha_512(this->GetMnemonic().c_str(), (uint8_t *)salt.c_str(), salt.size(), iterations, keylength, seed_bytes);

  std::string seed = Util::BytesToString(seed_bytes, 64);
  this->seed = seed;
  return seed;
}
