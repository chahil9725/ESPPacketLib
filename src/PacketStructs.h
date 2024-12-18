/**
 * @file PacketStructs.h
 * @brief Core packet structures and definitions for ESPPacketLib
 */

#ifndef _ESP_PACKET_STRUCTS_H_
#define _ESP_PACKET_STRUCTS_H_

#include <Arduino.h>
#include "esp_log.h"

// Packet Configuration
#define ESP_PACKET_MIN_MTU 23
#define ESP_PACKET_MAX_MTU 1500
#define ESP_PACKET_DEFAULT_MTU 250

// Magic Bytes
#define ESP_PACKET_MAGIC 0xAA55

// Packet Types
enum PacketType {
    // Data Packets (0x00-0x0F)
    PACKET_TYPE_SINGLE = 0x00,
    PACKET_TYPE_START  = 0x01,
    PACKET_TYPE_MID    = 0x02,
    PACKET_TYPE_END    = 0x03,
    
    // Control Packets (0x10-0x1F)
    PACKET_TYPE_ACK    = 0x10,
    PACKET_TYPE_NACK   = 0x11,
    PACKET_TYPE_PING   = 0x12,
    PACKET_TYPE_PONG   = 0x13,
    PACKET_TYPE_RESET  = 0x14,
    PACKET_TYPE_SYNC   = 0x15,
    PACKET_TYPE_SYNC_ACK = 0x16
};

// Packet Flags
#define PACKET_FLAG_ENCRYPTED  0x01
#define PACKET_FLAG_ACK_REQ   0x02

// Error Codes
enum PacketError {
    PACKET_OK = 0,
    PACKET_ERROR_MTU_SIZE = -1,
    PACKET_ERROR_INVALID_MAGIC = -2,
    PACKET_ERROR_CRC = -3,
    PACKET_ERROR_SEQUENCE = -4,
    PACKET_ERROR_BUFFER_FULL = -5,
    PACKET_ERROR_TIMEOUT = -6,
    PACKET_ERROR_ENCRYPTION = -7
};

#pragma pack(push, 1)
struct PacketHeader {
    uint8_t  magic;          // Magic bytes (0xAA55)
    uint8_t  type;           // Packet type
    uint8_t  flags;          // Control flags
    uint8_t  messageId;      // Message identifier
    uint16_t fragmentIndex;  // Current fragment number
    uint16_t totalLength;    // Total message length
    uint16_t currentLength;  // Current payload length
    uint8_t  senderId;       // Sender ID (0 for broadcast)
    uint8_t  receiverId;     // Receiver ID (0 for broadcast)
};
#pragma pack(pop)

// Footer structure (CRC16)
#pragma pack(push, 1)
struct PacketFooter {
    uint16_t crc;           // CRC16 of payload
};
#pragma pack(pop)

// Debug Logging Levels
#define ESP_PACKET_LOG_NONE   0
#define ESP_PACKET_LOG_ERROR  1
#define ESP_PACKET_LOG_WARN   2
#define ESP_PACKET_LOG_INFO   3

// Debug Logging Macros
#if defined(ESP_PACKET_DEBUG)
    #define ESP_PACKET_LOG(level, format, ...) \
        if (level <= ESP_PACKET_DEBUG_LEVEL) { \
            esp_log_write(ESP_LOG_INFO, "ESPPacketLib", format, ##__VA_ARGS__); \
        }
#else
    #define ESP_PACKET_LOG(level, format, ...)
#endif

#endif // _ESP_PACKET_STRUCTS_H_