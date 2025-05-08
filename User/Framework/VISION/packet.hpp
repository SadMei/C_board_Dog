// Copyright (c) 2022 ChenJun
// Licensed under the MIT License.

#ifndef RM_SERIAL_DRIVER__PACKET_HPP_
#define RM_SERIAL_DRIVER__PACKET_HPP_

#include <algorithm>
#include <cstdint>
#include <vector>

struct MotorState
{
	uint8_t mode;
	float q;
	float dq;
	float ddq;
	float tauEst;
	float q_raw;
	float dq_raw;
	float ddq_raw;
	int8_t temperature;
} __attribute__((packed));

struct MotorCmd
{
	uint8_t mode;
	float q;
	float dq;
	float tau;
	float kp;
	float kd;
} __attribute__((packed));

struct IMU
{
	float quaternion[4];
	float gyroscope[3];
	float accelerometer[3];
	float rpy[3];
	int8_t temperature;
} __attribute__((packed));

struct ReceivePacket
{
	uint8_t header = 0xA5;
	uint8_t levelFlag = 0; // 0 low 1 high
	uint16_t commVersion = 1;
	uint16_t robotID = 3;
	MotorCmd motorCmd[12];
	uint16_t checksum = 0;
} __attribute__((packed));

struct SendPacket
{
	uint8_t header = 0xA5;
	uint8_t levelFlag = 0; // 0 low 1 high
	uint16_t commVersion = 1;
	uint16_t robotID = 3;
//	uint32_t SN = 0;
//	uint8_t bandwidth = 0;
	IMU imu;
	MotorState motorState[12];
	uint32_t tick;
	uint16_t checksum = 0;
} __attribute__((packed));

inline ReceivePacket fromVector(const uint8_t* data)
{
	ReceivePacket packet;
	std::copy(data, data + sizeof(ReceivePacket), reinterpret_cast<uint8_t*>(&packet));
	return packet;
}

inline uint8_t* toVector(const SendPacket& data)
{
	uint8_t packet[sizeof(SendPacket)];
	std::copy(
			reinterpret_cast<const uint8_t*>(&data),
			reinterpret_cast<const uint8_t*>(&data) + sizeof(SendPacket), packet);
	return packet;
}
extern ReceivePacket vision_pkt;
extern uint16_t CDC_Checker, Last_CDC_Checker;
#endif  // RM_SERIAL_DRIVER__PACKET_HPP_