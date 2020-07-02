#ifndef PI2020_ZOMBIE_HPP
#define PI2020_ZOMBIE_HPP

#include "Enemy.hpp"

struct Zombie : public Enemy {
	Zombie(Vector3* playerPosition, list<CTexturedMesh3*>* cMeshes, const char* name = "Zombie", Vector3 front = {0.0, 0.0, 1.0}, Vector3 up = {0.0, 1.0, 0.0}, uint maxHP = 10.0, double movingSpeed = 2.5, double scale = 1.0, double attackRange = 0.5, uint attackDamage = 2.0) : Enemy(name, front, up, playerPosition, cMeshes, maxHP, movingSpeed, scale, attackRange, attackDamage){
		AddDrops(2,"potion50", 55);
	}
	
	/// \brief Débute le combat avec le joueur
	/// L'ennemi commence à attaquer l'ennemi selon son attaque
	/// \return Dommage reçu par le joueur.
	uint Attack(){
		if(chrono->InDelay(1.0)){
			chrono->Reset();
			return attackDamage;
		}
		return 0;
	}
};

#endif //PI2020_ZOMBIE_HPP
