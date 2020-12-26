#pragma once

// TODO(you): World state replication lab session

class ReplicationManagerServer 
{
public:

	void create(uint32 networkID);
	void update(uint32 networkID);
	void destroy(uint32 networkID);

	void write(OutputMemoryStream& packet);
};