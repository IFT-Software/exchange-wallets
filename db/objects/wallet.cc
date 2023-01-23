#include "db/objects/wallet.h"

#include <iostream>

#include "pqxx/pqxx"

DbWallet::DbWallet(uint32_t id, std::string name, std::string seed, std::string coin)
    : id_(id), name_(name), seed_(seed), coin_(coin) {}

void DbWallet::SetId(uint32_t id) { id_ = id; }
void DbWallet::SetName(std::string name) { name_ = name; }
void DbWallet::SetSeed(std::string seed) { seed_ = seed; }
void DbWallet::SetCoin(std::string coin) { coin_ = coin; }

uint32_t DbWallet::GetId() { return id_; }
std::string DbWallet::GetName() { return name_; }
std::string DbWallet::GetSeed() { return seed_; }
std::string DbWallet::GetCoin() { return coin_; }