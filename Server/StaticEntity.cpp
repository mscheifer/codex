#include "StaticEntity.h"



StaticEntity::~StaticEntity(void)
{
}

void StaticEntity::serialize(sf::Packet& packet) const {
  Entity::serialize(packet);
  packet << scale;
  packet << static_cast<sf::Uint32>(static_entity_type);
}
void StaticEntity::deserialize(sf::Packet& packet) {
   Entity::deserialize(packet);
   packet >> scale;
   sf::Uint32 type;
   packet >> type;  
   static_entity_type = static_cast<Entity_Type>(type);

}
 
