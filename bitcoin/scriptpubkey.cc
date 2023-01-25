#include "bitcoin/scriptpubkey.h"

#include "util/util.h"

Script::Script(std::vector<uint8_t> script, OutputType lock_type) {
  if (IsValid(script, lock_type)) {
    script_ = script;
    out_type_ = lock_type;
  } else {
    // TODO: custom exceptions
    throw std::invalid_argument("invalid script");
  }
}

// TODO: implement this
bool Script::IsValid(std::vector<uint8_t> script, OutputType out_type) { return true; }

uint32_t Script::size() const { return script_.size(); }

std::vector<uint8_t> Script::data() const { return script_; }

std::vector<uint8_t>::const_iterator Script::begin() const { return script_.begin(); }
std::vector<uint8_t>::const_iterator Script::end() const { return script_.end(); }
const uint8_t& Script::operator[](uint32_t pos) const { return script_[pos]; }

std::string Script::hex() { return util::BytesToHex(script_); }
std::string Script::bin() { return util::BytesToBin(script_); }