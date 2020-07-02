#ifndef PI2020_WITCH_HPP
#define PI2020_WITCH_HPP

#include "Enemy.hpp"

class Witch : public Enemy {
    CTexturedMesh3* fireball; ///< Mesh de la boulle de feu
public:
    /// \param name Le nom de l'ennemie.
    /// \param position Position de l'ennemie.
    /// \param front Vecteur de face de l'ennemie.
    /// \param up Vecteur vers le haut de l'ennemie.
    Witch( Vector3* playerPosition, list<CTexturedMesh3*>* cMeshes, const char* name = "Witch", Vector3 front = {0.0, 0.0, 1.0}, Vector3 up = {0.0, 1.0, 0.0}, uint maxHP = 5.0, double movingSpeed = 3.0, double scale = 1.2, double attackRange = 10.0, uint attackDamage = 3.0, const char* damageType = "magic") : Enemy(name, front, up, playerPosition, cMeshes, maxHP, movingSpeed, scale, attackRange, attackDamage){
        fireball = new CTexturedMesh3();
        Texture* tex = GetResource("FireballColor", Texture);
        fireball->CopyMesh(GetResource("Fireball", TexturedMesh3));
        fireball->LoadMeshInfo(GetResource("Fireball", TexturedMesh3)->info);
        fireball->SetTextureID(tex->GetId());
        AddDrops(2,"potion50", 50);
		Matrix44<double> size;
		size.LoadScale(scale * 0.05);
		fireball->Transform(size);
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
    
    void DrawFireball(){
        fireball->MoveTo(position);
        fireball->hitbox->MoveTo(position);
        fireball->Draw();
    }
};

#endif //PI2020_WITCH_HPP
