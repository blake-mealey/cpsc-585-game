#ifndef SPHERICAL_BODY_H
#define SPHERICAL_BODY_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"
#include <vector>

using namespace glm;
using namespace std;

class SphericalBody{
public:
	vec3 pos;
	float radius;
	vector<vec3> points;
	vector<vec2> uvs;

	float rotInc;
	vec3 sphericalInc;
	float axialTilt;

	float orbitalInclination;
	float orbitRot;
	float distance;

	float orbitalPeriod;
	float rotPeriod;

	vector<SphericalBody*> orbiters;
	SphericalBody* orbitee;

	int texId;
	int normalTexId;
	bool diffuseEnabled;

	SphericalBody(SphericalBody *orbitee, float _radius, float _axialTilt, float rotPeriod, float _distance,
		float _orbitalInclination, float orbitalPeriod, int _texId, int _normalTexId, bool _diffuseEnabled, bool scaled);

	void addOrbiter(SphericalBody *orbiter);


	mat4 getTranslationMatrix();
	mat4 getRotationMatrix();
	mat4 getTransformationMatrix();
	void incrementSpherical(float deltaTime);
	void incrementSpherical();
	void setSpherical(vec3 _spherical);
	void incrementRot(float deltaTime);
	void incrementRot();
	void setRotation(float _rot);
private:
	vec3 spherical;
	float rot;
	void updatePosition();
};










#endif
