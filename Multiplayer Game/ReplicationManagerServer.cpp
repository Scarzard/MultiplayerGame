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
	for (std::map<uint32, ReplicationAction>::iterator it = rep_commands.begin(); it != rep_commands.end(); ++it)
	{
		if ((*it).second == ReplicationAction::Create)
		{
			packet << (*it).first;
			packet << (*it).second;

			GameObject* go = App->modLinkingContext->getNetworkGameObject((*it).first, false);

			if (!go)
			{
				go = Instantiate();
				go->networkId = (*it).first;
				packet << 1;
			}
			else
			{
				packet << 0;
			}

			packet << go->position.x;
			packet << go->position.y;
			packet << go->size.x;
			packet << go->size.y;
			packet << go->angle;

		}
		else if ((*it).second == ReplicationAction::Update)
		{
			GameObject* go = App->modLinkingContext->getNetworkGameObject((*it).first);

			packet << go->position.x;
			packet << go->position.y;
			packet << go->size.x;
			packet << go->size.y;
			packet << go->angle;

			if (go->behaviour)
				go->behaviour->write(packet);
		}
		ReplicationCommand command;
		command.action = (*it).second;
		command.networkID = (*it).first;
	}
	rep_commands.clear();
}