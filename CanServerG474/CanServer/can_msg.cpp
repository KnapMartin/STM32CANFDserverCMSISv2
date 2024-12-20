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
	, m_header{}
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

void CanMsg::setHeader(FDCAN_RxHeaderTypeDef header)
{
	m_header = header;
}

FDCAN_RxHeaderTypeDef CanMsg::getHeader()
{
	return m_header;
}
