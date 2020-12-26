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
			GameObject* go = App->modLinkingContext->getNetworkGameObject(netID);

			bool dummy = false;

			if (go)
				dummy = true;

			go = Instantiate();

			if (!dummy)
			{
				GameObject* go_delete = App->modLinkingContext->getNetworkGameObject(netID);

				if (go_delete)
				{
					App->modLinkingContext->unregisterNetworkGameObject(go_delete);
					Destroy(go_delete);
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

			// Sprite
			if (go->sprite == nullptr)
			{
				go->sprite = App->modRender->addSprite(go);

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
			}

			packet >> go->sprite->order;

			if (dummy)
			{
				if (go->behaviour)
					go->behaviour->start();

				App->modGameObject->Destroy(go);
			}

			break;
		}
			
		case ReplicationAction::Update:
		{
			GameObject* go = App->modLinkingContext->getNetworkGameObject(netID);

			if (go)
			{
				packet >> go->position.x;
				packet >> go->position.y;
				packet >> go->size.x;
				packet >> go->size.y;
				packet >> go->angle;
			}

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