#ifndef ITEM_MESH_HPP
#define ITEM_MESH_HPP

#include "../Meshes/CollisionableMesh.hpp"
#include "ItemInfo.hpp"

#define ITEM_MESH3_BOX_RADIUS 0.5
#define ITEM_MESH3_SCALING_FACTOR 0.40
#define ITEM_MESH3_SCALED_RADIUS ITEM_MESH3_BOX_RADIUS * ITEM_MESH3_SCALING_FACTOR

/// 
/// \brief Mesh qui contient l'information d'un item.
///
struct ItemMesh3 : public CTexturedMesh3 {
	ushort count; 		///< Quatité d'items que la mesh contient.
	ItemInfo* iteminfo; ///< Information du type d'item contenu.

	/// \param info Information de l'item.
	/// \param count Quantité d'item
	ItemMesh3(ItemInfo* info, ushort count) : CTexturedMesh3() {
		Matrix44d scaling = Matrix44d();
		scaling.LoadScale(0.40);
		this->count = count; 
		this->iteminfo = info;
		this->CopyMesh(GetResource("crateobj", TexturedMesh3));
		this->LoadMeshInfo(GetResource("crateobj", TexturedMesh3)->info);
		((_Mesh3*)this)->info = this->info;
		this->Transform(scaling);
		this->SetTextureID(this->iteminfo->texture->GetId());
	}
};

#endif
