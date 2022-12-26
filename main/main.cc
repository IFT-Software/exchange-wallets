#include <string>
#include <iostream>

#include "lib/bip39.h"

int main(int argc, char **argv)
{
  std::bitset<128> entropy1("00101101100101001101101110000001011000110011100100010100110111011111110010000100011101110010111111010100101000001001000001000000");

  SeedGenerator *seedGenerator = new SeedGenerator();
  std::cout << "from entropy1: " << seedGenerator->GenerateSeedFromEntropy(entropy1) << std::endl;
  // std::cout << "----------------------" << std::endl;
  // std::cout << "entropy 1: " << seedGenerator->GenerateEntropy() << std::endl;
  // std::cout << "entropy 2: " << seedGenerator->GenerateEntropy() << std::endl;
  // std::cout << "entropy 3: " << seedGenerator->GenerateEntropy() << std::endl;
  // std::cout << "entropy 4: " << seedGenerator->GenerateEntropy() << std::endl;
  // std::cout << "entropy 5: " << seedGenerator->GenerateEntropy() << std::endl;
  // std::cout << "entropy 6: " << seedGenerator->GenerateEntropy() << std::endl;
  // std::cout << "entropy 7: " << seedGenerator->GenerateEntropy() << std::endl;
  // std::cout << "entropy 8: " << seedGenerator->GenerateEntropy() << std::endl;
  return 0;
}
