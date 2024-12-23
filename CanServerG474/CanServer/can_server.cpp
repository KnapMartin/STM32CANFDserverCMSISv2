/*
 * can_server.cpp
 *
 *  Created on: Dec 20, 2024
 *      Author: knap-linux
 */

#include "can_server.h"

#include <string.h>


CanServer::CanServer()
	: m_state{State::None}
	, m_hfdcan{nullptr}
	, m_queueRxCan{nullptr}
	, m_semTxCan{nullptr}
{
}

CanServer::~CanServer()
{
}

CanServer::Error CanServer::init()
{
	if (this->setupFilter() != Error::Ok)
	{
		return Error::Init;
	}
	// activate rx and tx interrupts
	if (HAL_FDCAN_ActivateNotification(m_hfdcan, FDCAN_IT_TX_COMPLETE | FDCAN_IT_RX_FIFO0_NEW_MESSAGE,
			FDCAN_TX_BUFFER0 | FDCAN_TX_BUFFER1 | FDCAN_TX_BUFFER2) != HAL_OK)
	{
		return Error::Init;
	}
	if (HAL_FDCAN_Start(m_hfdcan) != HAL_OK)
	{
		return Error::Ok;
	}

	if (m_hfdcan == nullptr) return Error::Init;
	if (m_queueRxCan == nullptr) return Error::Init;

	return Error::Ok;
}

CanServer::Error CanServer::update()
{

	switch (m_state)
	{
		case State::Idle:
		{

		}
		case State::Receive:
		{

		}
		case State::Transmit:
		{

		}
		case State::Error:
		{

		}
		case State::None:
		{

		}
	}

	return Error::Ok;
}

void CanServer::setHandleCan(FDCAN_HandleTypeDef *hfdcan)
{
	m_hfdcan = hfdcan;
}

CanServer::Error CanServer::updateCanRxFifo0Interrupt(FDCAN_HandleTypeDef *hfdcan, uint32_t isrType)
{
	if (hfdcan->Instance == m_hfdcan->Instance)
	{
		if ((isrType & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != 0)
		{
			FDCAN_RxHeaderTypeDef rxHeader;
			uint8_t rxData[8];

			if (HAL_FDCAN_GetRxMessage(m_hfdcan, FDCAN_RX_FIFO0, &rxHeader, rxData) == HAL_OK)
			{
				CanMsg msg;
				msg.setData(rxData, 8);
				if (osMessageQueuePut(*m_queueRxCan, &msg, 0, 0) != osOK)
				{
					return Error::IsrRx;
				}
			}
			else
			{
				return Error::IsrRx;
			}
		}
	}

	return Error::Ok;
}

CanServer::Error CanServer::updateCanRxFifo1Interrupt(FDCAN_HandleTypeDef *hfdcan, uint32_t isrType)
{
	if (hfdcan->Instance == m_hfdcan->Instance)
	{
		if ((isrType & FDCAN_IT_RX_FIFO1_NEW_MESSAGE) != 0)
		{
			FDCAN_RxHeaderTypeDef rxHeader;
			uint8_t rxData[8];

			if (HAL_FDCAN_GetRxMessage(m_hfdcan, FDCAN_RX_FIFO1, &rxHeader, rxData) == HAL_OK)
			{
				CanMsg msg;
				msg.setData(rxData, 8);
				if (osMessageQueuePut(*m_queueRxCan, &msg, 0, 0) != osOK)
				{
					return Error::IsrRx;
				}
			}
			else
			{
				return Error::IsrRx;
			}
		}
	}

	return Error::Ok;
}

CanServer::Error CanServer::updateCanTxInterrupt(FDCAN_HandleTypeDef *hfdcan)
{
	if (hfdcan->Instance == m_hfdcan->Instance)
	{
		if (osSemaphoreRelease(*m_semTxCan) != osOK)
		{
			return Error::IsrTx;
		}
	}

	return Error::Ok;
}

void CanServer::setHandleQueueRxCan(osMessageQueueId_t *queueRxCan)
{
	m_queueRxCan = queueRxCan;
}

void CanServer::setHandleSemaphoreTxCan(osSemaphoreId_t *semTxCan)
{
	m_semTxCan = semTxCan;
}

CanServer::Error CanServer::sendMsg(CanMsg *msg)
{
	if (osSemaphoreAcquire(*m_semTxCan, osWaitForever) != osOK)
	{
		return Error::Send;
	}
	uint8_t txData[8];
	msg->getData(txData);
	FDCAN_TxHeaderTypeDef txHeader = msg->getTxHeader();
	if (HAL_FDCAN_AddMessageToTxFifoQ(m_hfdcan, &txHeader, txData) != HAL_OK)
	{
		return Error::Send;
	}

	return Error::Ok;
}

CanServer::Error CanServer::getMsg(CanMsg *msg)
{
	if (osMessageQueueGet(*m_queueRxCan, &msg, NULL, osWaitForever) != osOK)
	{
		return Error::Get;
	}

	return Error::Ok;
}

CanServer::Error CanServer::sendMsg(FDCAN_TxHeaderTypeDef txHeader, uint8_t *data, uint8_t size)
{
	// TODO: size check

	if (osSemaphoreAcquire(*m_semTxCan, osWaitForever) != osOK)
	{
		return Error::Send;
	}
	if (HAL_FDCAN_AddMessageToTxFifoQ(m_hfdcan, &txHeader, data) != HAL_OK)
	{
		return Error::Send;
	}

	return Error::Ok;
}

CanServer::Error CanServer::setupFilter() // TODO: generalise
{
	FDCAN_FilterTypeDef canFilterConfig;
	canFilterConfig.IdType = FDCAN_STANDARD_ID;
	canFilterConfig.FilterIndex = 0;
	canFilterConfig.FilterType = FDCAN_FILTER_RANGE;
	canFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	canFilterConfig.FilterID1 = 0x000;
	canFilterConfig.FilterID2 = 0x010;

	if (HAL_FDCAN_ConfigFilter(m_hfdcan, &canFilterConfig) != HAL_OK)
	{
		return Error::Filter;
	}
	return Error::Ok;
}
