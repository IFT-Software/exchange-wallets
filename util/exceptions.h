#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

class BitcoindNetworkException : public std::exception {

private:
  std::string message_;
  std::string err_ = "";

public:
  BitcoindNetworkException(const std::string &message) : message_(message) {}
  BitcoindNetworkException(const std::string &message, const std::string &err)
      : message_(message), err_(err) {}
  virtual const char *what() const noexcept override {
    return message_.c_str();
  }

  const char *error() const noexcept { return err_.c_str(); }
};

class BitcoindResponseException : public std::exception {

private:
  std::string message_;
  std::string err_ = "";

public:
  BitcoindResponseException(const std::string &message) : message_(message) {}
  BitcoindResponseException(const std::string &message, const std::string &err)
      : message_(message), err_(err) {}

  virtual const char *what() const noexcept override {
    return message_.c_str();
  }

  const char *error() const noexcept { return err_.c_str(); }
};

class JSONAllocationException : public std::exception {

private:
  std::string message_;
  std::string err_ = "";

public:
  JSONAllocationException(const std::string &message) : message_(message) {}
  JSONAllocationException(const std::string &message, const std::string &err)
      : message_(message), err_(err) {}

  virtual const char *what() const noexcept override {
    return message_.c_str();
  }

  const char *error() const noexcept { return err_.c_str(); }
};

#endif