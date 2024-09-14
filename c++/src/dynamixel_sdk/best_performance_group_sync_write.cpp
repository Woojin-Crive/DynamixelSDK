#include "best_performance_group_sync_write.h"
#include <cstring> // For memcpy, memset

namespace dynamixel {

BestPerformanceGroupSyncWrite::BestPerformanceGroupSyncWrite(PortHandler* port, PacketHandler* ph, uint16_t start_address, uint16_t data_length)
    : GroupHandler(port, ph),
      start_address_(start_address),
      data_length_(data_length),
      param_length_(0),
      is_param_changed_(false) {
    // Initialize arrays
    memset(is_id_added_, 0, sizeof(is_id_added_));
}

BestPerformanceGroupSyncWrite::~BestPerformanceGroupSyncWrite() {
    clearParam();
}

bool BestPerformanceGroupSyncWrite::addParam(uint8_t id, const uint8_t* data) {
    if (id == 0 || id > MAX_ID || data_length_ > MAX_DATA_LENGTH) {
        return false; // Invalid ID or data length
    }

    if (is_id_added_[id]) {
        return false; // ID already exists
    }

    // Mark ID as added and copy data
    is_id_added_[id] = true;
    memcpy(data_list_[id], data, data_length_);

    is_param_changed_ = true;
    return true;
}

void BestPerformanceGroupSyncWrite::removeParam(uint8_t id) {
    if (id == 0 || id > MAX_ID) {
        return; // Invalid ID
    }

    if (!is_id_added_[id]) {
        return; // ID not added
    }

    // Mark ID as not added
    is_id_added_[id] = false;
    is_param_changed_ = true;
}

bool BestPerformanceGroupSyncWrite::changeParam(uint8_t id, const uint8_t* data) {
    if (id == 0 || id > MAX_ID || !is_id_added_[id]) {
        return false; // Invalid ID or ID not added
    }

    memcpy(data_list_[id], data, data_length_);
    is_param_changed_ = true;
    return true;
}

void BestPerformanceGroupSyncWrite::clearParam() {
    memset(is_id_added_, 0, sizeof(is_id_added_));
    param_length_ = 0;
    is_param_changed_ = false;
}

void BestPerformanceGroupSyncWrite::makeParam() {
    param_length_ = 0;

    for (uint8_t id = 1; id <= MAX_ID; ++id) {
        if (is_id_added_[id]) {
            param_[param_length_++] = id; // ID
            memcpy(&param_[param_length_], data_list_[id], data_length_);
            param_length_ += data_length_;
        }
    }

    is_param_changed_ = false;
}

int BestPerformanceGroupSyncWrite::txPacket() {
    if (is_param_changed_) {
        makeParam();
    }

    if (param_length_ == 0) {
        return COMM_NOT_AVAILABLE;
    }

    return ph_->syncWriteTxOnly(port_, start_address_, data_length_, param_, param_length_);
}

} // namespace dynamixel
