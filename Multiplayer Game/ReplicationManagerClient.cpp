#include "Networks.h"

// TODO(you): World state replication lab session

#include "ReplicationManagerClient.h"

void ReplicationManagerClient::read(const InputMemoryStream& packet)
{
	while (packet.RemainingByteCount() > 0)
	{
		uint32 netID;
		packet >> netID;
		ReplicationAction action = ReplicationAction::None;
		packet >> action;

		switch (action)
		{
		case ReplicationAction::None:
			break;

		case ReplicationAction::Create:
		{
			int tmp = 0;
			packet >> tmp;

			GameObject* go = App->modLinkingContext->getNetworkGameObject(netID);
			bool dummy = false;

			if (go)
				dummy = true;

			go = Instantiate();

			if (!dummy)
			{
				GameObject* to_delete = App->modLinkingContext->getNetworkGameObject(netID, false);
				if (to_delete)
				{
					App->modLinkingContext->unregisterNetworkGameObject(to_delete);
					Destroy(to_delete);
				}

				App->modLinkingContext->registerNetworkGameObjectWithNetworkId(go, netID);
			}

			packet >> go->position.x;
			packet >> go->position.y;
			packet >> go->size.x;
			packet >> go->size.y;
			packet >> go->angle;

			std::string tex;
			packet >> tex;
			go->sprite = App->modRender->addSprite(go);
			packet >> go->sprite->order;
			if (go->sprite)
			{
				if (tex == "spacecraft1.png")
					go->sprite->texture = App->modResources->spacecraft1;
				else if (tex == "spacecraft2.png")
					go->sprite->texture = App->modResources->spacecraft2;
				else if (tex == "spacecraft3.png")
					go->sprite->texture = App->modResources->spacecraft3;
				else if (tex == "laser.png")
					go->sprite->texture = App->modResources->laser;
				else if (tex == "explosion1.png")
				{
					go->sprite->texture = App->modResources->explosion1;
					go->animation = App->modRender->addAnimation(go);
					go->animation->clip = App->modResources->explosionClip;
					App->modSound->playAudioClip(App->modResources->audioClipExplosion);
				}
			}

			BehaviourType type;
			packet >> type;

			if (type == BehaviourType::Spaceship)
				go->behaviour = App->modBehaviour->addSpaceship(go);
			else if (type == BehaviourType::Laser)
				go->behaviour = App->modBehaviour->addLaser(go);

			packet >> go->tag;

			//--------------------------------------------------------------------------------------------

			if (dummy)
				Destroy(go);

			if (tmp == 1)
			{
				App->modLinkingContext->unregisterNetworkGameObject(go);
				Destroy(go);
			}

			break;
		}
			
		case ReplicationAction::Update:
		{
			GameObject* go = App->modLinkingContext->getNetworkGameObject(netID);

				packet >> go->position.x;
				packet >> go->position.y;
				packet >> go->size.x;
				packet >> go->size.y;
				packet >> go->angle;

				if (go->behaviour)
					go->behaviour->read(packet);

			break;
		}
			

		case ReplicationAction::Destroy:
		{
			GameObject* go = App->modLinkingContext->getNetworkGameObject(netID);

			if (go)
			{
				App->modLinkingContext->unregisterNetworkGameObject(go);
				Destroy(go);
			}

			break;
		}
		}
	}
}