#include "socketio/comms.h"

#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "third_party/cppzmq/zmq.hpp"
#include "third_party/cppzmq/zmq_addon.hpp"

#include "util/util.h"

namespace comms {

void SubscriberThread(zmq::context_t* ctx) {
  //  Prepare our context and subscriber
  zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
  subscriber.connect("tcp://127.0.0.1:29000");
  //  Thread3 opens ALL envelopes
  subscriber.set(zmq::sockopt::subscribe, "rawtx");
  while (1) {
    // Receive all parts of the message
    std::vector<zmq::message_t> recv_msgs;
    zmq::recv_result_t result = zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
    assert(result && "recv failed");
    // assert(*result == 2);
    std::cout << "Thread: [" << recv_msgs[0].to_string() << "] "
              << util::BytesToHex((uint8_t*)recv_msgs[1].data(), recv_msgs[1].size()) << std::endl;
  }

}  // namespace
}  // namespace comms
   // namespace comms