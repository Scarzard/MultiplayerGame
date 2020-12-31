#include "Networks.h"

// TODO(you): Reliability on top of UDP lab session

#include "DeliveryManager.h"

DeliveryManager::~DeliveryManager()
{
	clear();
}

Delivery* DeliveryManager::writeSequenceNumber(OutputMemoryStream& packet)
{

}

bool DeliveryManager::processSequenceNumber(const InputMemoryStream& packet)
{

}

bool DeliveryManager::hasSequenceNumberspendingAck() const
{

}

void DeliveryManager::writeSequenceNumbersPendingAck(OutputMemoryStream& packet)
{

}

void DeliveryManager::processAckdSequenceNumbers(const InputMemoryStream& packet)
{

}

void DeliveryManager::processTimedOutPackets()
{

}

void DeliveryManager::clear()
{

}