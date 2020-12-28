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

void ReplicationManagerServer::write(OutputMemoryStream& packet)
{
	if (rep_commands.size() == 0)
		return;

	for (std::map<uint32, ReplicationAction>::iterator it = rep_commands.begin(); it != rep_commands.end(); ++it)
	{
		if ((*it).second == ReplicationAction::Create)
		{
			packet << (*it).first;
			packet << (*it).second;

			GameObject* go = App->modLinkingContext->getNetworkGameObject((*it).first, true);

			/*if (!go)
			{
				go = Instantiate();
				go->networkId = (*it).first;
				packet << 1;
			}
			else
			{
				packet << 0;
			}*/

			packet << go->position.x;
			packet << go->position.y;
			packet << go->size.x;
			packet << go->size.y;
			packet << go->angle;

			if (go->behaviour)
				packet << go->behaviour->type();
			else
				packet << BehaviourType::None;

		}
		else if ((*it).second == ReplicationAction::Update)
		{
			GameObject* go = App->modLinkingContext->getNetworkGameObject((*it).first, true);

			packet << go->position.x;
			packet << go->position.y;
			packet << go->size.x;
			packet << go->size.y;
			packet << go->angle;
		}
		/*ReplicationCommand command;
		command.action = (*it).second;
		command.networkID = (*it).first;*/
	}

	rep_commands.clear();
}