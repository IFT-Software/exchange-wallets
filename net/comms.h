#ifndef NET_COMMS_H
#define NET_COMMS_H

#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "third_party/cppzmq/zmq.hpp"
#include "third_party/cppzmq/zmq_addon.hpp"
#include "util/util.h"

namespace bitcoin {
namespace tx {}
}  // namespace bitcoin

namespace comms {

void SubscriberThread(zmq::context_t* ctx);
void SegWitTransactions(zmq::context_t* ctx);

}  // namespace comms
#endif