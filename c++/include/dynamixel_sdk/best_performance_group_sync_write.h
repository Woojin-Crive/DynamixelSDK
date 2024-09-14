#ifndef BEST_PERFORMANCE_GROUP_SYNC_WRITE_H
#define BEST_PERFORMANCE_GROUP_SYNC_WRITE_H

#include "port_handler.h"
#include "packet_handler.h"
#include "group_handler.h"

namespace dynamixel {

constexpr size_t MAX_DATA_LENGTH = 4;   // Maximum data length per ID
constexpr size_t MAX_PARAM_SIZE = MAX_ID * (1 + MAX_DATA_LENGTH); // Maximum parameter buffer size

class BestPerformanceGroupSyncWrite : public GroupHandler {
public:
    BestPerformanceGroupSyncWrite(PortHandler* port, PacketHandler* ph, uint16_t start_address, uint16_t data_length);

    ~BestPerformanceGroupSyncWrite();

    bool addParam(uint8_t id, const uint8_t* data);

    void removeParam(uint8_t id);

    bool changeParam(uint8_t id, const uint8_t* data);

    void clearParam();

    int txPacket();

private:
    uint16_t start_address_;
    uint16_t data_length_;

    // Arrays to store IDs and data
    bool is_id_added_[256]; // Index corresponds to ID, true if added
    uint8_t data_list_[256][MAX_DATA_LENGTH];

    uint8_t param_[MAX_PARAM_SIZE]; // Pre-allocated parameter buffer
    uint16_t param_length_;
    bool is_param_changed_;

    void makeParam();
};

} // namespace dynamixel

#endif // BEST_PERFORMANCE_GROUP_SYNC_WRITE_H
