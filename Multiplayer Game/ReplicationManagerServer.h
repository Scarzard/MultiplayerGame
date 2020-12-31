#pragma once

// TODO(you): World state replication lab session

#include <map>

class ReplicationManagerServer 
{
public:

	void create(uint32 networkID);
	void update(uint32 networkID);
	void destroy(uint32 networkID);

	void write(OutputMemoryStream& packet);

	std::map<uint32, ReplicationAction> rep_commands;
};

class ReplicationManagerDeliveryDelegate : public DeliveryDelegate
{
public:
	ReplicationManagerDeliveryDelegate(ReplicationManagerServer* RepManagerServer);
	~ReplicationManagerDeliveryDelegate();

	void onDeliverySuccess(DeliveryManager* deliveryManager) override;
	void onDeliveryFailure(DeliveryManager* deliveryManager) override;

	void AddCommand(const ReplicationCommand& replicationCommand);

private:
	ReplicationManagerServer* RepManagerServer;
	std::vector<ReplicationCommand> rep_commands;
};