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

class CanMsg
{
	public:
		CanMsg();
		virtual ~CanMsg();

		void setData(const uint8_t *data, uint8_t size);
		uint8_t getData(uint8_t *dest);
		void setRxHeader(FDCAN_RxHeaderTypeDef header);
		FDCAN_RxHeaderTypeDef getRxHeader();
		void setTxHeader(FDCAN_TxHeaderTypeDef header);
			FDCAN_TxHeaderTypeDef getTxHeader();

	private:
		uint8_t m_length;
		uint8_t m_data[msgLen];
		FDCAN_RxHeaderTypeDef m_rxHeader;
		FDCAN_TxHeaderTypeDef m_txHeader;
};

#endif /* CAN_MSG_H_ */
