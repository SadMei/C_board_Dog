// Copyright (c) 2022 ChenJun
// Licensed under the MIT License.

#ifndef RM_SERIAL_DRIVER__PACKET_HPP_
#define RM_SERIAL_DRIVER__PACKET_HPP_

#include <algorithm>
#include <cstdint>
#include <vector>
struct ReceivePacket
{
	uint8_t header = 0xA5;
	uint32_t packat_id = 0;
	uint8_t suggest_fire: 1;
	float offset_yaw = 0;
	float offset_pitch = 0;
	uint16_t checksum = 0;
} __attribute__((packed));
// struct ReceivePacket
// {
// 	// uint8_t header1 = 'v';
// 	// uint8_t header2 = 'e';
// 	// uint8_t header3 = 'l';
// 	float x = 0;
// 	float y = 0;
// 	uint16_t checksum = 0;
//
// } __attribute__((packed));

struct SendPacket
{
	uint8_t header = 0xA5;
 	float yaw;
	float pitch;
	uint8_t state = 0;
	uint8_t autoaim;
	char enemy_color;
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

#endif  // RM_SERIAL_DRIVER__PACKET_HPP_