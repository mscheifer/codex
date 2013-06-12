#include "Projectile.h"
int Projectile::ID_Counter = 0;
const float Projectile::meleeWidth = 1.5f; //left right width
const float Projectile::meleeHeight = 7.0f; //how far
const float Projectile::meleeDepth = 5.0f; //up and down width
const float Projectile::projWidth = 1.0f;
const float Projectile::projHeight = 1.0f;
const float Projectile::projDepth = 1.0f;
const float Projectile::chargeWidth = 3.0f;
const float Projectile::chargeHeight = 3.0f;
const float Projectile::chargeDepth = 3.0f;


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
  lockon = nullptr;
  reset();
}

void Projectile::reset(){
  clearEvents();
  charge_counter.restart();
  charging = true;
  combined = false;
  lockon = nullptr;
}

bool Projectile::correctMovementHit( Entity* e ){
  Entity_Type etype = e->getType();
  if( etype == WALL )
    return true;
  else if (etype == PROJECTILE && ((Projectile*) e)->charging){
    //same team check TODO decide to add or not
    if( StringToNumber<float>(ConfigManager::configMap["friendlyCombineOnly"]) == 1) 
      return sameTeam((Projectile*)e);
    else
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
      getOwner()->upgraded = true;
      charge_counter.restart(); 
      setMagicType(upgrade(magicType), false, true);
    }
  } else {
    v3_t distanceTravelled;
    if(lockon != nullptr && lockon->live && lockon->getCharging()){
      v3_t newDirection = lockon->position - position;
      newDirection.normalize();
      newDirection.scale(velocity.magnitude());
      velocity = newDirection;
      distanceTravelled = velocity * ConfigManager::serverTickLengthSec();
    } else {
      distanceTravelled = velocity * ConfigManager::serverTickLengthSec();
    }
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
  bool destroy = false;
  for( auto it = entities.begin(); it != entities.end(); it++ ){
    Entity * e = it->first; 
    switch(e->getType()){
    case PLAYER:
      if(e == owner)
        break;
      else{
        bool friendlyFire = StringToNumber<float>(ConfigManager::configMap["friendlyfire"]) == 1;
        if( !(!friendlyFire && sameTeam((Player*)e)) )
          ((Player*)e)->attackBy(this);
        destroy = true;
      }
    case WALL:
      if(!charging)
        destroy = true;
      break;
    case PROJECTILE:
      Projectile * proj = (Projectile*) e;
      //I am moving and proj is charging and we same team
      //same team logic is handled by correct move hit
      if ( !this->charging && proj->charging ){ //the other one is charging
        proj->setMagicType(combine(proj->getMagicType(), magicType), false, true);
        proj->combined = true;
        destroy = true;
        //map->destroyProjectile(this);
      }
      break;
    }
    
  }

  if(destroy){
    live=false;
    //map->destroyProjectile(this);
  }
}

void Projectile::clearEvents(){
  combined = false;
  fired = false;
}

void Projectile::fire(v3_t v, float strengthMultiplier, Projectile* p) {
  lockon = p;
  setMagicType(magicType,false,false);
  velocity = v * ProjInfo[magicType].speed;
  setRange(ProjInfo[magicType].range); //this also sets travel distance left
  strength = ProjInfo[magicType].strength * strengthMultiplier;
  fired = true;
  charging = false;
}

void Projectile::fireMutiple(v3_t v, float strengthMultiplier, int number, Projectile* p) {
  lockon = p;
  setMagicType(magicType,false,false);
  velocity = v * ProjInfo[magicType].speed;
  setRange(ProjInfo[magicType].range); //this also sets travel distance left
  strength = ProjInfo[magicType].strength * strengthMultiplier;
  fired = true;
  charging = false;
  
  double slice = 50.0/number;
  double counter = 0;
  double start = -20.0; 
  double PI = 3.14159265;
  for( int i = 0; i < number ; i++) {
    double adjusted = (start + counter)* PI /180.0;

    double xp = velocity.x*cos(adjusted) - velocity.y*sin(adjusted);
    double yp = velocity.x*sin(adjusted) + velocity.y*cos(adjusted);
    counter += slice;
    Projectile* pj = map->produceProjectile();
    pj->setVelocity(v3_t(xp,yp,velocity.z));
    pj->setDirection(pj->velocity);
    pj->setOwner(owner);
    pj->setMagicType(magicType, false, false);
    pj->setRender(true);
    pj->setPosition(getPosition());
    pj->setRange(ProjInfo[magicType].range); //this also sets travel distance left
    pj->strength = ProjInfo[magicType].strength * strengthMultiplier;
    pj->fired = true;
    pj->charging = false;
  }
     // x' = xcos@ - ysin@
    // y' = xsin@ + ycos@ 
   //std::cout << toString();
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
  case B1:
    return B2;
  case B2:
    return B3;
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

bool Projectile::sameTeam( Projectile * p ){
  return p->getOwner()->isMinotaur() == getOwner()->isMinotaur();
}

bool Projectile::sameTeam( Player * p ){
  return p->isMinotaur() == getOwner()->isMinotaur();
}

void Projectile::setMagicType( MAGIC_POWER m, bool melee, bool charge ) {
  magicType = m;
  charge_counter.restart();
  float size = ProjInfo[magicType].size;
  if(melee){ //set size for melee
    ((BoundingBox*) boundingObjs[0])->setHw( meleeWidth/2.f );
    ((BoundingBox*) boundingObjs[0])->setHh( meleeHeight/2.f );
    ((BoundingBox*) boundingObjs[0])->setHd( meleeDepth/2.f );
  } else if(charge){
    ((BoundingBox*) boundingObjs[0])->setHw( chargeWidth/2.f );
    ((BoundingBox*) boundingObjs[0])->setHh( chargeHeight/2.f );
    ((BoundingBox*) boundingObjs[0])->setHd( chargeDepth/2.f );
  } else {
    ((BoundingBox*) boundingObjs[0])->setHw( size/2.f );
    ((BoundingBox*) boundingObjs[0])->setHh( size/2.f );
    ((BoundingBox*) boundingObjs[0])->setHd( size/2.f );
  }
  updateBounds();
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

std::string Projectile::toString2(){
  std::stringstream packet;
  packet << fired;
  packet << id;
  packet << combined;
  packet << static_cast<sf::Uint32>(magicType);
  return packet.str();
}

std::string Projectile::toString(){
  std::stringstream ss;
  ss << "mtype " << spellNames[magicType] << std::endl
  //<< "range " << range << std::endl
  //<< "strength " << strength << std::endl
  //<< "owner "<< owner->player_id <<std::endl
  << "bbox " << this->boundingObjs[0]->toString() << std::endl;
  return ss.str();
}

const MAGIC_POWER Projectile::combinations[20][20] = {
  {FIR2},
  {FIR3,FIR3},
  {FIR3,FIR3,FIR3},
  {G_FI,G_FI,G_FI,ICE2},
  {G_FI,G_FI2,G_FI2,ICE3,ICE3},
  {G_FI,G_FI2,G_FI2,ICE3,ICE3,ICE3},
  {G_FT,G_FT,G_FT,G_IT,G_IT,G_IT,THU2},
  {G_FT,G_FT2,G_FT2,G_IT,G_IT2,G_IT2,THU3,THU3},
  {G_FT,G_FT2,G_FT2,G_IT,G_IT2,G_IT2,THU3,THU3,THU3},
  {G2,G2,G2,G_IT,G_IT,G_IT,G_IT,G_IT,G_IT,G_IT},
  {G_FT,G_FT,G_FT,G2,G2,G2,G_FT,G_FT,G_FT,G2,G_FT},
  {G_FI,G_FI,G_FI,G_FI,G_FI,G_FI,G2,G2,G2,G2,G2,G_FI},
  {G2,G2,G2,G2,G2,G2,G2,G2,G2,G2,G2,G2,G2},
  {G_IT2,G3,G3,G_IT2,G_IT2,G_IT2,G_IT2,G_IT2,G_IT2,G_IT2,G_IT2,G_IT2,G2,G_IT2},
  {G_FT2,G_FT2,G_FT2,G_FT2,G3,G3,G_FT2,G_FT2,G_FT2,G_FT2,G_FT2,G_FT2,G2,G3,G_FT2},
  {G_FI2,G_FI2,G_FI2,G_FI2,G_FI2,G_FI2,G_FI2,G3,G3,G_FI2,G_FI2,G_FI2,G2,G3,G3,G_FI2},
  {G3,G3,G3,G3,G3,G3,G3,G3,G3,G3,G3,G3,G3,G3,G3,G3,G3},
  {B1,B1,B1,B1,B1,B1,B1,B1,B1,B1,B1,B1,B1,B1,B1,B1,B1,B1},
  {B2,B2,B2,B2,B2,B2,B2,B2,B2,B2,B2,B2,B2,B2,B2,B2,B2,B1,B2},
  {B3,B3,B3,B3,B3,B3,B3,B3,B3,B3,B3,B3,B3,B3,B3,B3,B3,B1,B2,B3}
};