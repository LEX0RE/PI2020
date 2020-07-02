#ifndef PI2020_AWKWARD_HPP
#define PI2020_AWKWARD_HPP

#include "Enemy.hpp"

struct Awkward : public Enemy {
private:
	bool startExplosion; ///< Début l'autodestruction
public:
	Awkward(Vector3* playerPosition, list<CTexturedMesh3*>* cMeshes, const char* name = "Awkwards", Vector3 front = {0.0, 0.0, 1.0}, Vector3 up = {0.0, 1.0, 0.0}, uint maxHP = 3.0, double movingSpeed = 4.5, double scale = 1.0, double attackRange = 2.0, uint attackDamage = 10.0) : Enemy(name, front, up, playerPosition, cMeshes, maxHP, movingSpeed, scale, attackRange, attackDamage){
		AddDrops(2,"potion50", 40);
		startExplosion = false;
	}
	
	/// \brief Débute le combat avec le joueur
	/// L'ennemi commence à attaquer l'ennemi selon son attaque
	/// \return Dommage reçu par le joueur.
	uint Attack(){
		if(startExplosion) {
			if(chrono->InDelay(3.0)) {
				dead = true;
				enableDrops = false;
				return attackDamage;
			}
		} else {
			startExplosion = true;
			chrono->Reset();
		}
		return 0;
	}
	
};

#endif //PI2020_AWKWARD_HPP
