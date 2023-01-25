#include "socketio/comms.h"

#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "third_party/cppzmq/zmq.hpp"
#include "third_party/cppzmq/zmq_addon.hpp"

#include "bitcoin/tx.h"
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

    uint8_t* data = (uint8_t*)recv_msgs[1].data();

    Transaction* tx = bitcoin::tx::ParseTransaction(data);

    std::cout << "Thread: " << util::BytesToHex((uint8_t*)recv_msgs[1].data(), recv_msgs[1].size())
              << std::endl;

    std::string tx_str = tx->json();
    std::cout << "tx_str " << tx_str << std::endl;
    // std::cout << "is equal: "
    //           << (util::BytesToHex((uint8_t*)recv_msgs[1].data(), recv_msgs[1].size()) == tx_str)
    //           << std::endl;
  }
}

// A thread that listens for raw transactions and prints out the ones that are segwit
// transactions.
void SegWitTransactions(zmq::context_t* ctx) {
  zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
  subscriber.connect("tcp://127.0.0.1:29000");
  subscriber.set(zmq::sockopt::subscribe, "rawtx");

  while (1) {
    // Receive all parts of the message
    std::vector<zmq::message_t> recv_msgs;
    zmq::recv_result_t result = zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
    assert(result && "recv failed");
    uint8_t* data = (uint8_t*)recv_msgs[1].data();

    // std::cout << "Thread: " << util::BytesToHex((uint8_t*)recv_msgs[1].data(),
    // recv_msgs[1].size())
    //           << std::endl;

    if (bitcoin::tx::IsSegwit(data)) {
      std::cout << "Thread: "
                << util::BytesToHex((uint8_t*)recv_msgs[1].data(), recv_msgs[1].size())
                << std::endl;
    }
  }
};

}  // namespace comms
