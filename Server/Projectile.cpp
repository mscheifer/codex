#include "Projectile.h"
int Projectile::ID_Counter = 0;
const float Projectile::meleeWidth = 1.0f;
const float Projectile::meleeHeight = 1.0f;
const float Projectile::meleeDepth = 3.0f;
const float Projectile::projWidth = 1.0f;
const float Projectile::projHeight = 1.0f;
const float Projectile::projDepth = 1.0f;


Projectile::Projectile(Map* m):fired(false)
{
	this->map = m;
  charge_counter = sf::Clock();
  BoundingBox* b = new BoundingBox(BoundingObj::vec4_t(0,0,0),BoundingObj::vec3_t(1,0,0),BoundingObj::vec3_t(0,1,0),BoundingObj::vec3_t(0,0,1),
  projWidth/2.f,projHeight/2.f,projDepth/2.f);
  b->setEntity(this);
  id = ID_Counter;
  ID_Counter++;
  boundingObjs.push_back(b);
  reset();
}

Projectile::~Projectile(void)
{
}

void Projectile::reset(){
  charge_counter.restart();
  charging = true;
}

bool Projectile::correctMovementHit( Entity* e ){
  Entity_Type etype = e->getType();
  if( etype == WALL )
    return true;
  else if (etype == PROJECTILE && ((Projectile*) e)->charging){// check same team
    if( owner->isMinotaur() )
      return false;
    else if( ((Projectile*) e)->getOwner()->isMinotaur() )
      return false;

    return true;
  } else if ( etype == PLAYER ){
    return e != owner;
  }
  return false;
}

void Projectile::update(void) {
  if(charging) {
    float cdr = owner->getChargeCD();
    int chargeTime = ProjInfo[magicType].chargeTime;
    //-1 means no upgrades
    if(chargeTime != -1 && charge_counter.getElapsedTime().asMilliseconds() > chargeTime*cdr ) {
      charge_counter.restart();
      setMagicType(upgrade(magicType));
      std::cout << "increase level to " << magicType <<  std::endl;
    }
  } else {
    v3_t distanceTravelled = velocity * ConfigManager::serverTickLengthSec();
    distanceTravelled = correctMovement(distanceTravelled, false, position);

    float mag = distanceTravelled.magnitude();
    if( mag > distanceLeftToTravel ){ //make sure you don't go past the range
      distanceTravelled.normalize();
      distanceTravelled.scale(distanceLeftToTravel);
    }

	  position += distanceTravelled;
  
    //see if travelled full range
    distanceLeftToTravel -= distanceTravelled.magnitude();
    if(distanceLeftToTravel <= 0.0){
      map->destroyProjectile(this);
      return;
    }
  }
  
	updateBounds();
}

void Projectile::setOwner(Player * player){
  owner = player;
}

void Projectile::setStrength(float f) {
	strength = f;
}

float Projectile::getStrength() {
  return strength;
}

void Projectile::setRange(length_t r) {
	range = r;
	distanceLeftToTravel = r;
}

void Projectile::updateBounds(){
  //update the bounding objects
  boundingObjs[0]->setCenter(BoundingObj::vec4_t(position.x, position.y, position.z));
  boundingObjs[0]->rotate(direction,BoundingObj::vec3_t(0,0,1));
  boundingObjs[0]->setCenterOnTree(BoundingObj::vec4_t(position.x, position.y, position.z));
}

void Projectile::updateBoundsSoft(){
  //update the bounding objects
  boundingObjs[0]->setCenter(BoundingObj::vec4_t(position.x, position.y, position.z));
}

void Projectile::handleCollisions() {
  std::vector<std::pair<Entity*,BoundingObj::vec3_t>> entities =  detectCollision();

  for( auto it = entities.begin(); it != entities.end(); it++ ){
    Entity * e = it->first; 
    switch(e->getType()){
    case PLAYER:
      if(e == owner)
        break;
    case WALL:
      if(!charging)
        map->destroyProjectile(this);
      break;
    case PROJECTILE:
      Projectile * proj = (Projectile*) e;
      if(charging){
        setMagicType(combine(proj->getMagicType(), magicType));
        map->destroyProjectile(proj);
        combined = true;
      } else if ( proj->charging ){
        proj->setMagicType(combine(proj->getMagicType(), magicType));
        proj->combined = true;
        map->destroyProjectile(this);
      }
      
      //else {
      //  map->destroyProjectile(proj);
      //  map->destroyProjectile(this);
      //}
      break;
    }
  }
}

void Projectile::clearEvents(){
  combined = false;
  fired = false;
}

void Projectile::fire(v3_t v, float strengthMultiplier) {
  velocity = v * ProjInfo[magicType].speed;
  setRange(ProjInfo[magicType].range); //this also sets travel distance left
  strength = ProjInfo[magicType].strength * strengthMultiplier;

  std::cout << toString();

  fired = true;
  charging = false;
}

MAGIC_POWER Projectile::upgrade( const MAGIC_POWER m ){
  switch( m ){
  case FIR1:
    return FIR2;
  case FIR2:
    return FIR3;
  case ICE1:
    return ICE2;
  case ICE2:
    return ICE3;
  case THU1:
    return THU2;
  case THU2:
    return THU3;
  default:
    return m;
  }
}

MAGIC_POWER Projectile::combine( MAGIC_POWER m1, MAGIC_POWER m2 ){
  if(m1 < m2){
    MAGIC_POWER temp = m2;
    m2 = m1;
    m1 = temp;
  }
  //switch so the row is m1
  return combinations[m1][m2];
}

void Projectile::setMagicType( MAGIC_POWER m, bool melee ) {
  magicType = m;
  charge_counter.restart();
  float size = ProjInfo[magicType].size;
  if(melee){ //set size for melee
    ((BoundingBox*) boundingObjs[0])->setHw( meleeWidth/2.f );
    ((BoundingBox*) boundingObjs[0])->setHh( meleeHeight/2.f );
    ((BoundingBox*) boundingObjs[0])->setHd( meleeDepth/2.f );
  } else {
    ((BoundingBox*) boundingObjs[0])->setHw( size/2.f );
    ((BoundingBox*) boundingObjs[0])->setHh( size/2.f );
    ((BoundingBox*) boundingObjs[0])->setHd( size/2.f );
  }
}

void Projectile::serialize(sf::Packet & packet) const {
  Entity::serialize(packet);
  packet << fired;
  packet << id;
  packet << combined;
  packet << static_cast<sf::Uint32>(magicType);
  //(*owner).serialize(packet);
}

void Projectile::deserialize( sf::Packet & packet ) {
  Entity::deserialize(packet);
  packet >> fired;
  packet >> id;
  packet >> combined;
  sf::Uint32 u32;
  packet >> u32;
  magicType = static_cast<MAGIC_POWER>(u32);
  //delete owner; this segfaults
  //Player* owner = new Player();
  //(*owner).deserialize(packet);
 }

std::string Projectile::toString(){
  std::stringstream ss;
  ss << "mtype " << magicType << std::endl
  << "range " << range << std::endl
  << "strength " << strength << std::endl;
  return ss.str();
}

const MAGIC_POWER Projectile::combinations[18][18] = {
  {FIR2},
  {FIR2,FIR3},
  {FIR3,FIR3,FIR3},
  {G_FI,G_FI,G_FI,ICE2},
  {G_FI,G_FI2,G_FI2,ICE2,ICE3},
  {G_FI,G_FI2,G_FI2,ICE3,ICE3,ICE3},
  {G_FT,G_FT,G_FT,G_IT,G_IT,G_IT,THU2},
  {G_FT,G_FT2,G_FT2,G_IT,G_IT2,G_IT2,THU2,THU3},
  {G_FT,G_FT2,G_FT2,G_IT,G_IT2,G_IT2,THU3,THU3,THU3},
  {G2,G2,G2,G_IT,G_IT,G_IT,G_IT,G_IT,G_IT,G_IT},
  {G_FT,G_FT,G_FT,G2,G2,G2,G_FT,G_FT,G_FT,G2,G_FT},
  {G_FI,G_FI,G_FI,G_FI,G_FI,G_FI,G2,G2,G2,G2,G2,G_FI},
  {G2,G2,G2,G2,G2,G2,G2,G2,G2,G2,G2,G2,G2},
  {G_IT2,G3,G3,G_IT2,G_IT2,G_IT2,G_IT2,G_IT2,G_IT2,G_IT2,G_IT2,G_IT2,G2,G_IT2},
  {G_FT2,G_FT2,G_FT2,G_FT2,G3,G3,G_FT2,G_FT2,G_FT2,G_FT2,G_FT2,G_FT2,G2,G3,G_FT2},
  {G_FI2,G_FI2,G_FI2,G_FI2,G_FI2,G_FI2,G_FI2,G3,G3,G_FI2,G_FI2,G_FI2,G2,G3,G3,G_FI2},
  {G3,G3,G3,G3,G3,G3,G3,G3,G3,G3,G3,G3,G3,G3,G3,G3,G3},
  {B1,B1,B1,B1,B1,B1,B1,B1,B1,B1,B1,B1,B1,B1,B1,B1,B1,B1}
};