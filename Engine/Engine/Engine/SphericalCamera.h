#ifndef SPHERICAL_CAMERA_H
#define SPHERICAL_CAMERA_H

#include "glm/glm.hpp"

using namespace glm;
using namespace std;

class SphericalCamera{
public:
	vec3 dir;
	vec3 up;
	vec3 right;
	vec3 pos;

	SphericalCamera();
	SphericalCamera(vec3 _spherical, vec3 _offset);

	mat4 getMatrix();
	void incrementSpherical(vec3 _spherical);
	void setSpherical(vec3 _spherical);
	void translate(vec3 _offset);
	void setOffset(vec3 _offset);
private:
	vec3 spherical;
	vec3 offset;
	void updatePositionAndDirection();
};










#endif
