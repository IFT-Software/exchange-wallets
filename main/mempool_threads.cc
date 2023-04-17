#include "bitcoin/json_rpc.h"
#include "bitcoin/rpc_tx.h"
#include "bitcoin/transaction.h"
#include "main/global_vars.h"
#include "main/thread_sync.h"
#include "util/exceptions.h"

#include <iostream>
#include <mutex>
#include <thread>

namespace json = boost::json;

void ListenMempool(GlobalVars& globalVars, ThreadSync& threadSync) {
  // TODO: is there a garbage collection thing for this bc i don't wanna create
  // a vector that will stay forever for every 10ms
  std::vector<RpcTx> mempool_txs;
  while (true) {
    // std::unique_lock<std::mutex> lock(threadSync.mempool_mutex);

    std::cout << "listenmempool has the lock" << std::endl;
    try {
      std::cout << "trying" << std::endl;
      bitcoin::rpc::GetMempoolTxs(mempool_txs);

      std::cout << "get txs" << std::endl;
      {
        std::cout << "trying to lock" << std::endl;
        std::lock_guard<std::mutex> lock(threadSync.mempool_mutex);
        globalVars.mempool->push(&mempool_txs);
        std::cout << "pushed new transactions" << std::endl;
        // TODO: automaticaly release when lock goes out of scope??
      }
      threadSync.mempool_cv.notify_one();
    }
    // TODO: should be doing some external logging here...
    catch (BitcoindNetworkException e) {
      std::cerr << e.what() << std::endl;
    } catch (BitcoindResponseException e) {
      // std::cerr << e.what() << std::endl;
    } catch (JSONAllocationException e) {
      std::cerr << e.what() << std::endl;
    }

    // TODO: how to determine this? really important...
    std::this_thread::sleep_for(std::chrono::seconds(10));
  }
}

void WriteDatabase(GlobalVars& globalVars, ThreadSync& threadSync) {
  while (true) {
    std::cout << "inside writedatabse" << std::endl;
    std::unique_lock<std::mutex> lock(threadSync.mempool_mutex);
    std::cout << "acquired the lock, waiting on cv" << std::endl;
    threadSync.mempool_cv.wait(lock, [&globalVars] { return !(globalVars.mempool->empty()); });
    std::cout << "cv met" << std::endl;

    // std::cout << "writedatabase acquired the lock" << std::endl;

    std::cout << "mempool size: " << globalVars.mempool->size() << std::endl;

    std::vector<RpcTx>* curr_mempool;
    std::vector<Transaction*> curr_txs;

    curr_mempool = globalVars.mempool->front();
    globalVars.mempool->pop();

    for (RpcTx rpc_tx : *curr_mempool) {
      try {
        curr_txs.push_back(rpc_tx.ParseTransaction());
      } catch (std::exception e) {
        // TODO: better error handling!!!!
        std::cerr << e.what() << std::endl;
      }
    };

    for (auto it = globalVars.seen_txs->begin(); it != globalVars.seen_txs->end();) {
      // std::cout << (*it) << std::endl;
      auto contains = std::find_if(std::begin(curr_txs), std::end(curr_txs),
                                   [it](Transaction* tx) { return tx->GetTxID() == (*it); });

      // if a previously seen transaction is not in the current mempool, it
      // means it is not going to appear in the mempool, so there is no need to
      // keep track of it.
      if (contains == std::end(curr_txs)) {
        it = globalVars.seen_txs->erase(it);
      } else {
        ++it;
      }
    };

    // check to the database (instead of seen_txs) if the application is starting
    if (globalVars.is_start_mempool) {
      std::cout << "here" << std::endl;
      for (Transaction* tx : curr_txs) {
        json::array res = globalVars.db_tx_mgr->Select({{"select",
                                                         {
                                                             {"txid", true},
                                                         }},
                                                        {"where",
                                                         {
                                                             {"txid", tx->GetTxID()},
                                                         }}});

        if (res.size() > 0) {
          globalVars.seen_txs->insert(tx->GetTxID());
        }
      };
    }

    for (Transaction* tx : curr_txs) {
      // if previously seen, skip
      if (globalVars.seen_txs->count(tx->GetTxID()) != 0) {
        continue;
      }

      // if not previously seen, mark it as seen and write it to the database
      globalVars.seen_txs->insert(tx->GetTxID());
      {
        std::lock_guard<std::mutex> lock(threadSync.database_mutex);
        json::object res = globalVars.db_tx_mgr->Insert({{"data", tx->json()},
                                                         {"select",
                                                          {{"txid", true},
                                                           {"version", true},
                                                           {"inputs", true},
                                                           {"outputs", true},
                                                           {"lock_time", true}}}});
      }
    }

    std::cout << "inserted transaction to the database" << std::endl;

    // is_start_mempool is toggled after the first iteration of WriteDatabase()
    globalVars.is_start_mempool = false;
  }
}
