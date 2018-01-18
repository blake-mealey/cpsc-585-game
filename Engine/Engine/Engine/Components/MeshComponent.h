#pragma once

#include "Component.h"
#include "../Systems/Content/Mesh.h"
#include <string>
#include "../Entities/Transform.h"
#include "../Systems/Content/Material.h"
#include "../Systems/Content/Texture.h"

class MeshComponent : public Component {
public:
	Transform transform;		// Temporary?

	ComponentType GetType() override;
	void HandleEvent(Event* event) override;
	
	MeshComponent(std::string meshPath, Material *_material, std::string texturePath);

	void SetEntity(Entity* _entity) override;

	Mesh* GetMesh() const;
private:
	Mesh *mesh;
public:
	Material *material;
	Texture *texture;
};
