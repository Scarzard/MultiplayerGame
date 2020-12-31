#pragma once

// TODO(you): Reliability on top of UDP lab session

class DeliveryManager;

class DeliveryDelegate
{
public:
	virtual void onDeliverySuccess(DeliveryManager* deliveryManager) = 0;
	virtual void onDeliveryFailure(DeliveryManager* deliveryManager) = 0;
};

struct Delivery
{
	uint32 sequenceNumber = 0;
	double dispatchTime = 0.0;
	DeliveryDelegate* delegate = nullptr;
};

class DeliveryManager
{
public:
	~DeliveryManager();

	//For senders to write a new seq. numbers into a packet
	Delivery* writeSequenceNumber(OutputMemoryStream& packet);

	//For recievers to process the seq. number from an incoming packet
	bool processSequenceNumber(const InputMemoryStream& packet);

	//For recievers to write ack'ed seq. numbers into a packet
	bool hasSequenceNumberspendingAck() const;
	void writeSequenceNumbersPendingAck(OutputMemoryStream& packet);

	//For senders to process ack'ed seq. numbers from a packet
	void processAckdSequenceNumbers(const InputMemoryStream& packet);
	void processTimedOutPackets();

	void clear();


private:
	// Sender side
	uint32	NextOutgoingSequenceNumber = 0;
	std::vector<Delivery*> PendingDeliveries;

	//Receiver side
	uint32	NextExpectedSequenceNumber = 0;
	std::vector<uint32> PendingAcks;

};