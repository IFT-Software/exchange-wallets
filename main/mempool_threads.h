#ifndef MEMPOOL_THREADS_H
#define MEMPOOL_THREADS_H

#include "bitcoin/json_rpc.h"
#include "bitcoin/rpc_tx.h"
#include "bitcoin/transaction.h"
#include "main/global_vars.h"
#include "main/thread_sync.h"
#include "util/exceptions.h"

#include <iostream>
#include <mutex>
#include <thread>

void ListenMempool(GlobalVars& globalVars, ThreadSync& threadSync);
void WriteDatabase(GlobalVars& globalVars, ThreadSync& threadSync);

#endif