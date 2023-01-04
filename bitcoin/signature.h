#ifndef BITCOIN_SIGNATURE_H
#define BITCOIN_SIGNATURE_H

#include <cstdint>
#include <vector>

class ScriptSig {
 private:
  std::vector<uint8_t> signature;

 public:
  ScriptSig(std::vector<uint8_t> sig);

  bool IsValid();
  bool IsValid(std::vector<uint8_t> sig);
};

#endif