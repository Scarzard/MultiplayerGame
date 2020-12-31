# Multiplayer Game 

by Victor Chen 

## Statement

Implementing a simple 2D game prototype using an engine provided in class.

## Controls

- Down Arrow: Go forward

- Left Arrow: Shoot

- A: Rotate counter clockwise

- D: Rotate clockwise

## Features

### Developed by Victor

#### Completely Achieved:

- Handles 8 players at runtime

- Joining and leaving at runtime

- World State Replication: This tecnique makes sure that all player are seeing the same. The server writes a gameObject's state. It also serializes all its fields and then after an interval, sends a replication packet to all the connected clients. They read the packet and deserializes its fields

- Input redundancy: This tecnique makes sure that even when there's packet loss, players can still play correctly

#### Atempted without success:

- Delivery Manager: This tecnique notifies about failed replications. If this happens, the delivery manager sends the command again.

#### Problems

DeliveryManager could not be implemented due to poor time management. The functionalities are all done but for some reason it messes with World State Replication. Meaning that players don't process the packets correctly, so they have their screen frozen. With a little bit more time it would've been fully implemented.

Release and delivery will be made without Delivery Manager but it will still be in the repository.
