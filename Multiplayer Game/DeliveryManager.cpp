#include "Networks.h"

// TODO(you): Reliability on top of UDP lab session

#include "DeliveryManager.h"

DeliveryManager::~DeliveryManager()
{
	clear();
}

Delivery* DeliveryManager::writeSequenceNumber(OutputMemoryStream& packet)
{
	packet << NextOutgoingSequenceNumber;

	Delivery* delivery = new Delivery();

	delivery->dispatchTime = Time.time;
	delivery->sequenceNumber = NextExpectedSequenceNumber++;

	PendingDeliveries.push_back(delivery);

	return delivery;
}

bool DeliveryManager::processSequenceNumber(const InputMemoryStream& packet)
{
	uint32 ReceivedSequenceNumber;
	packet >> ReceivedSequenceNumber;

	if (ReceivedSequenceNumber >= NextExpectedSequenceNumber)
	{
		PendingAcks.push_back(ReceivedSequenceNumber);
		NextExpectedSequenceNumber = ReceivedSequenceNumber + 1;
		return true;
	}

	return false;
}

bool DeliveryManager::hasSequenceNumberspendingAck() const
{
	return !PendingAcks.empty();
}

void DeliveryManager::writeSequenceNumbersPendingAck(OutputMemoryStream& packet)
{
	if (hasSequenceNumberspendingAck())
	{
		for (uint32 size : PendingAcks)
			packet << size;
	}
	PendingAcks.clear();
}

void DeliveryManager::processAckdSequenceNumbers(const InputMemoryStream& packet)
{
	while (packet.RemainingByteCount() > 0)
	{
		uint32 seqNumber = 0;
		packet >> seqNumber;
		bool found = false;

		for (std::vector<Delivery*>::iterator it = PendingDeliveries.begin(); it != PendingDeliveries.end(); it++)
		{
			if ((*it)->sequenceNumber == seqNumber) 
			{
				if ((*it)->delegate)
					(*it)->delegate->onDeliverySuccess(this);
				
				PendingDeliveries.erase(it);
				found = true;
				break;
			}
		}
	}
}

void DeliveryManager::processTimedOutPackets()
{
	while (!PendingDeliveries.empty())
	{
		Delivery* delivery = PendingDeliveries.front();

		if (Time.time - delivery->dispatchTime >= DELIVERY_INTERVAL_SECONDS)
		{
			delivery->delegate->onDeliveryFailure(this);
			PendingDeliveries.erase(PendingDeliveries.begin());
			delete delivery;
		}
		else
			break;
	}
}

void DeliveryManager::clear()
{
	while (!PendingDeliveries.empty())
	{
		Delivery* delivery = PendingDeliveries.front();
		PendingDeliveries.erase(PendingDeliveries.begin());
		delete delivery;
	}

	PendingDeliveries.clear();
	PendingAcks.clear();

	NextOutgoingSequenceNumber = 0;
	NextExpectedSequenceNumber = 0;
}