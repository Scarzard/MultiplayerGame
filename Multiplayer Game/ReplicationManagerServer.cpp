#include "Networks.h"

// TODO(you): World state replication lab session

#include "ReplicationManagerServer.h"

void ReplicationManagerServer::create(uint32 networkID)
{
	rep_commands[networkID] = ReplicationAction::Create;
}

void ReplicationManagerServer::update(uint32 networkID)
{
	rep_commands[networkID] = ReplicationAction::Update;
}

void ReplicationManagerServer::destroy(uint32 networkID)
{
	rep_commands[networkID] = ReplicationAction::Destroy;
}

void ReplicationManagerServer::write(OutputMemoryStream& packet, ReplicationCommand action)
{

}