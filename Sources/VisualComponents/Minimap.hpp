#ifndef MINIMAP_HPP
#define MINIMAP_HPP

#include "../Typedef.hpp"
#include "../VisualComp.hpp"
#include "../SceneManager.hpp"
#include "Image.hpp"
#include "Label.hpp"
#include "../Entities/Player.hpp"

class Minimap : public VisualComp {
	//Image* arrow;
	Player* player;
	Vector2 worldSize;

	public:
	
	/// \param pos Position à l'écran
	/// \param size Taille du rectangle de la minimap
	/// \param player Instance du Player
	/// \param tex Texture de la carte du monde.
	Minimap(Vector2 pos, Vector2 size, Player* player, Texture* tex /*, Texture* arrow*/) : VisualComp(pos, size) {
		this->mesh = new TexturedMesh2();
		this->player = player;
		((TexturedMesh2*)mesh)->LoadRect(tex->GetId(), pos, size);
		
		/*Vector2 aPos = arrow->GetSize();
		aPos.x = (size.x - aPos.x) / 2;
		aPos.x = (size.y - aPos.y) / 2;
		this->arrow = new Image(arrow, aPos);
		*/

		//TEXTURE COORD : X E [0.0, 1.0], Y E [0.0, 1.0]; 
		//WORLD COORD: X E [-IMGSIZ.X / 2, +IMGSIZ.X / 2], X E [-IMGSIZ.Y / 2, +IMGSIZ.Y / 2],  
		//SRC RECT: X E []
		this->worldSize = tex->GetSize();
		Vector3 playerPosition = player->GetPosition();
		
		Vector2 primePlayerPosition = Vector2(
			(playerPosition.x + (worldSize.x / 2.0)) * (1.0 / worldSize.x), 
			(playerPosition.z + (worldSize.y / 2.0)) * (1.0 / worldSize.y)
		);

		Vector2 pPos2Source = Vector2(
			primePlayerPosition.x + 0.05, 
			primePlayerPosition.y + 0.05
		);

		Vector2 pPos1Source = Vector2( 
			primePlayerPosition.x - 0.05, 
			primePlayerPosition.y - 0.05
		);
		
		((TexturedMesh2*)mesh)->SetTextureCoord(pPos1Source, pPos2Source); 
	}

	~Minimap() {
		//delete arrow;
	}
	
	/// \brief Affiche la minimap.
	void Draw() {
		Vector3 playerPosition = player->GetPosition();
		
		Vector2 primePlayerPosition = Vector2(
			(playerPosition.x + (worldSize.x / 2)) * (1 / worldSize.x), 
			(playerPosition.z + (worldSize.y / 2)) * (1 / worldSize.y)
		);
		Vector2 pPos2Source = Vector2(
			primePlayerPosition.x + 0.06, 
			primePlayerPosition.y + 0.06
		);
		Vector2 pPos1Source = Vector2( 
			primePlayerPosition.x - 0.06, 
			primePlayerPosition.y - 0.06
		);

		((TexturedMesh2*)mesh)->SetTextureCoord(pPos1Source, pPos2Source); 
		mesh->Draw();
	}
};

#endif 
