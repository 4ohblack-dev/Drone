#pragma once
//inline→関数をヘッダ定義し、複数のソースファイルから読み込めるようにする
#include <Arduino.h>
#include <stddef.h>

enum class PacketType : uint8_t{
    command = 1,
    status = 2,
};

enum class Mode : uint8_t {
    Unknown = 0,
    Flight = 1,
    Drive = 2,
};

enum class wheelState : uint8_t{
    unInitialized = 0,
    ready = 1,
    transforming = 2,
    fault = 3,
};

struct __attribute__((packed)) ControlPacket {
    //packet開始の目印
    uint8_t start1 = 0xAA;
    uint8_t start2 = 0x55;
    //通信形式のバージョン
    uint8_t version = 1;
    //指令か状態通知か
    PacketType type = PacketType::command;
    //packetの送信番号
    uint16_t sequence = 0;
    //モード変更要求の番号
    uint16_t requestId = 0;
    //希望するモード
    Mode requestedMode = Mode::Unknown;
    //変形が完了した現在のモード
    Mode currentMode = Mode::Unknown;
    //変形中、完了などの状態
    wheelState state = wheelState::unInitialized;

    uint8_t reserved = 0;

    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t z = 0;
    //通信エラー検出用
    uint16_t crc = 0;    
};
//構造体のバイト数を確認
static_assert(sizeof(ControlPacket) == 20,"ControlPacket size mismatch");

inline uint16_t calculateCRC(const uint8_t* data, size_t length) {
    uint16_t crc = 0xFFFF;

    for (size_t i = 0; i < length; i++) {
        crc ^= static_cast<uint16_t>(data[i]) << 8;

        for (uint8_t bit = 0; bit < 8; bit++) {
            if (crc & 0x8000) {
                crc = static_cast<uint16_t>((crc << 1) ^ 0x1021);
            } else {
                crc = static_cast<uint16_t>(crc << 1);
            }
        }
    }

    return crc;
}

inline bool isValidPacket(const ControlPacket& packet){
    if (packet.start1 != 0xAA||
        packet.start2 != 0x55||
        packet.version != 1||
        packet.reserved != 0){
        return false;
    }

    if (packet.type != PacketType::command &&
        packet.type != PacketType::status){
            return false;
    }

    if (static_cast<uint8_t>(packet.requestedMode) > 2||
        static_cast<uint8_t>(packet.currentMode) > 2||
        static_cast<uint8_t>(packet.state) > 3){
            return false;
    }

    if (packet.type == PacketType::command &&
        packet.requestedMode == Mode::Unknown){
            return false;
    }
    if (packet.x < -1000 || packet.x > 1000 ||
        packet.y < -1000 || packet.y > 1000 ||
        packet.z < -1000 || packet.z > 1000) {
        return false;
    }

    return packet.crc == calculateCRC(reinterpret_cast<const uint8_t*>(&packet),offsetof(ControlPacket,crc));
}