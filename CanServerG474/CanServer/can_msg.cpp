/*
 * can_msg.cpp
 *
 *  Created on: Dec 20, 2024
 *      Author: knap-linux
 */

#include "can_msg.h"

#include <string.h>

CanMsg::CanMsg()
	: m_length{msgLen}
	, m_data{}
	, m_rxHeader{}
	, m_txHeader{}
{
	memset(m_data, 0, m_length);
}

CanMsg::~CanMsg()
{
}

void CanMsg::setData(const uint8_t *data, uint8_t size)
{
	if (size > m_length)
	{
		size = m_length;
	}
	memcpy(&m_data, data, size);
}

uint8_t CanMsg::getData(uint8_t *dest)
{
	memcpy(&m_data, dest, m_length);
	return m_length;
}

void CanMsg::setRxHeader(FDCAN_RxHeaderTypeDef header)
{
	m_rxHeader = header;
}

FDCAN_RxHeaderTypeDef CanMsg::getRxHeader()
{
	return m_rxHeader;
}

void CanMsg::setTxHeader(FDCAN_TxHeaderTypeDef header)
{
	m_txHeader = header;
}

FDCAN_TxHeaderTypeDef CanMsg::getTxHeader()
{
	return m_txHeader;
}
