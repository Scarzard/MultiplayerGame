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
		packet << (*it).first;
		packet << (*it).second;

		if ((*it).second == ReplicationAction::Create)
		{
			GameObject* go = App->modLinkingContext->getNetworkGameObject((*it).first, false);

			if (!go)
			{
				go = Instantiate();
				go->networkId = (*it).first;
				packet << 1;
			}
			else
				packet << 0;

			packet << go->position.x;
			packet << go->position.y;
			packet << go->size.x;
			packet << go->size.y;
			packet << go->angle;

			std::string tex = "";
			if (go->sprite)
			{
				tex = go->sprite->texture->filename;
				packet << tex;
				packet << go->sprite->order;
			}
			else
			{
				packet << tex;
				packet << 0;
			}


			if (go->behaviour)
				packet << go->behaviour->type();
			else
				packet << BehaviourType::None;

			packet << go->tag;
		}
		else if ((*it).second == ReplicationAction::Update)
		{
			GameObject* go = App->modLinkingContext->getNetworkGameObject((*it).first, true);

			if (go)
			{
				packet << go->position.x;
				packet << go->position.y;
				packet << go->size.x;
				packet << go->size.y;
				packet << go->angle;
			}
		}

		ReplicationCommand command;
		command.action = (*it).second;
		command.networkID = (*it).first;
	}

	rep_commands.clear();
}

ReplicationManagerDeliveryDelegate::ReplicationManagerDeliveryDelegate(ReplicationManagerServer* RepManagerServer)
{
	this->RepManagerServer = RepManagerServer;
}

ReplicationManagerDeliveryDelegate::~ReplicationManagerDeliveryDelegate()
{
}

void ReplicationManagerDeliveryDelegate::onDeliverySuccess(DeliveryManager* deliveryManager)
{
}

void ReplicationManagerDeliveryDelegate::onDeliveryFailure(DeliveryManager* deliveryManager)
{
	for (const ReplicationCommand& replicationCommand : rep_commands)
	{
		switch (replicationCommand.action)
		{
		case ReplicationAction::Create:
		{
			if (App->modLinkingContext->getNetworkGameObject(replicationCommand.networkID) != nullptr)
			{
				RepManagerServer->create(replicationCommand.networkID);
			}
			break;
		}

		case ReplicationAction::Update:
		{
			if (App->modLinkingContext->getNetworkGameObject(replicationCommand.networkID) != nullptr)
			{
				RepManagerServer->update(replicationCommand.networkID);
			}
			break;
		}

		case ReplicationAction::Destroy:
		{
			if (App->modLinkingContext->getNetworkGameObject(replicationCommand.networkID) == nullptr)
			{
				RepManagerServer->destroy(replicationCommand.networkID);
			}
			break;
		}
		}
	}
}

void ReplicationManagerDeliveryDelegate::AddCommand(const ReplicationCommand& replicationCommand)
{
	rep_commands.push_back(replicationCommand);
}