#ifndef PI2020_GOOMBA_HPP
#define PI2020_GOOMBA_HPP

#include "Enemy.hpp"

struct Goomba : public Enemy {
	Goomba(Vector3* playerPosition, list<CTexturedMesh3*>* cMeshes, const char* name = "Goomba", Vector3 front = {0.0, 0.0, 1.0}, Vector3 up = {0.0, 1.0, 0.0}, uint maxHP = 2.0, double movingSpeed = 3.0, double scale = 0.5, double attackRange = 0.1, uint attackDamage = 1.0) : Enemy(name, front, up, playerPosition, cMeshes, maxHP, movingSpeed, scale, attackRange, attackDamage){
		AddDrops(2,"potion50", 25);
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

#endif //PI2020_GOOMBA_HPP
