#ifndef COMMS_COMMS_H
#define COMMS_COMMS_H

#include "third_party/cppzmq/zmq.hpp"
#include "third_party/cppzmq/zmq_addon.hpp"

namespace comms {

void SubscriberThread(zmq::context_t* ctx);

}  // namespace comms
#endif