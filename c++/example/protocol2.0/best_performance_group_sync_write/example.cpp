#include "best_performance_group_sync_write.h"

// Create instances of PortHandler and PacketHandler
dynamixel::PortHandler* portHandler = dynamixel::PortHandler::getPortHandler("/dev/ttyUSB0");
dynamixel::PacketHandler* packetHandler = dynamixel::PacketHandler::getPacketHandler(2.0);

// Open port and set baudrate
if (portHandler->openPort() && portHandler->setBaudRate(57600)) {
    // Create an instance of BestPerformanceGroupSyncWrite
    dynamixel::BestPerformanceGroupSyncWrite groupSyncWrite(portHandler, packetHandler, 30, 2); // data_length = 2

    // Prepare data to send
    uint8_t id = 1;
    uint8_t data[2] = {0x20, 0x00}; // Example data

    // Add parameters
    if (!groupSyncWrite.addParam(id, data)) {
        printf("Failed to add parameter for ID %d\n", id);
    }

    // Transmit packet
    int result = groupSyncWrite.txPacket();

    // Handle the result
    if (result != COMM_SUCCESS) {
        printf("Error: %s\n", packetHandler->getTxRxResult(result));
    }

    // Clear parameters if needed
    groupSyncWrite.clearParam();
}

// Close port
portHandler->closePort();
