/*
 * can_server.h
 *
 *  Created on: Dec 20, 2024
 *      Author: knap-linux
 */

#ifndef CAN_SERVER_H_
#define CAN_SERVER_H_

#include "main.h"
#include "can_msg.h"
#include "cmsis_os2.h"

class CanServer
{
	public:
		CanServer();
		virtual ~CanServer();

		enum class State
		{
			None,
			Idle,
			Receive,
			Transmit,
			Error
		};

		enum class Error
		{
			Ok,
			Init,
			Update,
			Filter,
			IsrTx,
			IsrRx,
			Send,
			Get
		};

		Error init();
		Error update();
		void setHandleCan(FDCAN_HandleTypeDef *hfdcan);
		void setHandleQueueRxCan(osMessageQueueId_t *queueRxCan);
		void setHandleSemaphoreTxCan(osSemaphoreId_t *semTxCan);
		Error updateCanRxFifo0Interrupt(FDCAN_HandleTypeDef *hfdcan, uint32_t isrType);
		Error updateCanRxFifo1Interrupt(FDCAN_HandleTypeDef *hfdcan, uint32_t isrType);
		Error updateCanTxInterrupt(FDCAN_HandleTypeDef *hfdcan);
		Error sendMsg(CanMsg *msg);
		Error sendMsg(FDCAN_TxHeaderTypeDef txHeader, uint8_t *data, uint8_t size);
		Error getMsg(CanMsg *msg);

	private:
		Error setupFilter();

		State m_state;
		FDCAN_HandleTypeDef *m_hfdcan;
		osMessageQueueId_t *m_queueRxCan;
		osSemaphoreId_t *m_semTxCan;
};

#endif /* CAN_SERVER_H_ */
