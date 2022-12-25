#include <string>
#include <iostream>

#include "lib/bip39.h"

int main(int argc, char **argv)
{
  SeedGenerator *seedGenerator = new SeedGenerator();
  std::cout << seedGenerator->GenerateSeed() << std::endl;
  return 0;
}
