/*
 * can_msg.h
 *
 *  Created on: Dec 20, 2024
 *      Author: knap-linux
 */

#ifndef CAN_MSG_H_
#define CAN_MSG_H_

#include "main.h"
#include <stdint.h>

constexpr uint8_t msgLen{8};

class CanMsg {
public:
	CanMsg();
	virtual ~CanMsg();

	void setData(const uint8_t *data, uint8_t size);
	void setHeader(FDCAN_RxHeaderTypeDef header);
	FDCAN_RxHeaderTypeDef getHeader();

private:
	uint8_t m_length;
	uint8_t m_data[msgLen];
	FDCAN_RxHeaderTypeDef m_header;

};

#endif /* CAN_MSG_H_ */
