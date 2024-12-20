/*
 * can_server.h
 *
 *  Created on: Dec 20, 2024
 *      Author: knap-linux
 */

#ifndef CAN_SERVER_H_
#define CAN_SERVER_H_

#include "main.h"
#include "cmsis_os2.h"
#include "can_msg.h"

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
			IsrRx
		};

		Error init();
		Error update();
		void setHandleCan(FDCAN_HandleTypeDef *hfdcan);
		void setHandleQueueRxCan(osMessageQueueId_t *queueRxCan);
		Error updateCanRxFifo0Interrupt(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifoITs);
		Error updateCanRxFifo1Interrupt(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifoITs);
		Error updateCanTxInterrupt();

	private:
		Error setupFilter();

		State m_state;
		FDCAN_HandleTypeDef *m_hfdcan;
		osMessageQueueId_t m_queueRxCan;
};

#endif /* CAN_SERVER_H_ */
