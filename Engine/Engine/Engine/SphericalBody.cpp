#include "SphericalBody.h"
#include <cstdlib>

#define PI 3.14159265359

mat4 rotateAboutAxis(vec3 axis, float radians)
{
	mat4 matrix;

	float c = cos(radians);
	float s = sin(radians);
	float ic = 1 - c;

	float xs = axis.x*s;
	float ys = axis.y*s;
	float zs = axis.z*s;

	float xic = axis.x*ic;
	float yic = axis.y*ic;
	float zic = axis.z*ic;

	/*return mat4(
		axis.x*xic + c, axis.x*yic - zs, axis.x*zic + ys, 0
		axis.y*xic + zs, axis.y*yic + c, axis.y*zic - xs, 0
		axis.z*xic - ys, axis.z*yic + xs, axis.z*zic + c, 0,
		0, 0, 0, 0
	);*/

	matrix[0][0] = axis.x*xic + c;
	matrix[1][0] = axis.x*yic - zs;
	matrix[2][0] = axis.x*zic + ys;

	matrix[0][1] = axis.y*xic + zs;
	matrix[1][1] = axis.y*yic + c;
	matrix[2][1] = axis.y*zic - xs;

	matrix[0][2] = axis.z*xic - ys;
	matrix[1][2] = axis.z*yic + xs;
	matrix[2][2] = axis.z*zic + c;

	return matrix;
}

void addPoint(vector<vec3> *points, vector<vec2> *uvs, vec3 point) {
	(*points).push_back(point);
	vec3 d = normalize(-point);
	(*uvs).push_back(vec2(
		0.5 + (atan2(d.z, d.x) / (2*PI)),
		0.5 - (asin(d.y) / PI)
	));
}

void addTriangle(vector<vec3> *points, vector<vec2> *uvs, vec3 a, vec3 b, vec3 c) {
	addPoint(points, uvs, a);
	addPoint(points, uvs, b);
	addPoint(points, uvs, c);
}


void generateSphere(float radius, int divisions, vector<vec3> *points, vector<vec2> *uvs) {
	(*points).clear();
	(*uvs).clear();

	float angle = PI / float(divisions);

	float theta = 0.f;
	float phi = 0.f;

	for(int i = 0; i < divisions; i++) {
		for(int j = 0; j <= divisions*2; j++) {
			vec3 p0 = radius*vec3(sin(theta)*cos(phi), cos(theta), sin(theta)*sin(phi));
			vec3 p1 = radius*vec3(sin(theta)*cos(phi + angle), cos(theta), sin(theta)*sin(phi + angle));
			vec3 p2 = radius*vec3(sin(theta + angle)*cos(phi + angle), cos(theta + angle), sin(theta + angle)*sin(phi + angle));

			addTriangle(points, uvs, p2, p1, p0);
			
			if(j != 0) {
				vec3 p3 = radius*vec3(sin(theta + angle)*cos(phi), cos(theta + angle), sin(theta + angle)*sin(phi));

				addTriangle(points, uvs, p0, p3, p2);
			}

			theta += angle;
		}

		theta = 0.f;
		phi += angle;
	}
}

const int DIVISIONS = 24;

float scale(float value, float base, bool scaled) {
	if(value == 0.f || !scaled) {
		return value;
	}
	return pow(value, 1.f/base)/10.f;
}

SphericalBody::SphericalBody(SphericalBody *_orbitee, float _radius, float _axialTilt, float _rotPeriod, float _distance, float _orbitalInclination,
								float _orbitalPeriod, int _texId, int _normalTexId, bool _diffuseEnabled, bool scaled) :
					orbitRot((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 2*PI),
					orbitalInclination(_orbitalInclination),
					distance(scale(_distance, 2, scaled)),
					rot(0.f),
					radius(scale(_radius, 2, scaled)),			// scaling is kinda weird but this seems to work
					rotPeriod(_rotPeriod),
					orbitalPeriod(_orbitalPeriod),
					texId(_texId),
					normalTexId(_normalTexId),
					orbitee(NULL),
					diffuseEnabled(_diffuseEnabled),
					axialTilt(_axialTilt)
{
	generateSphere(radius, DIVISIONS, &points, &uvs);
	updatePosition();

	if(_orbitee != NULL) {
		_orbitee->addOrbiter(this);
	}
}



void SphericalBody::addOrbiter(SphericalBody *orbiter) {
	orbiters.push_back(orbiter);
	orbiter->orbitee = this;
}



mat4 SphericalBody::getTranslationMatrix() {
	return translate(mat4(1.f), pos);
}

mat4 SphericalBody::getRotationMatrix() {
	return rotateAboutAxis(vec3(1, 0, 0), axialTilt) * rotateAboutAxis(vec3(0, 1, 0), rot);
}

mat4 SphericalBody::getTransformationMatrix() {
	return getTranslationMatrix() * getRotationMatrix();
}




void SphericalBody::incrementSpherical(float deltaTime) {
	if(abs(orbitalPeriod) > 0.f) {
		// setSpherical(spherical + vec3(0.f, 2*PI*(deltaTime/orbitalPeriod), 0.f));
		// orbitRot += 2*PI*(deltaTime/orbitalPeriod);
		orbitRot += deltaTime * ((2*PI)/orbitalPeriod);
	}
}

void SphericalBody::incrementSpherical() {
	setSpherical(spherical + sphericalInc);
}

void SphericalBody::setSpherical(vec3 _spherical) {
	spherical = _spherical;
	updatePosition();
}


void SphericalBody::incrementRot(float deltaTime) {
	if(abs(rotPeriod) > 0.f) {
		setRotation(rot + (2*PI*(deltaTime/rotPeriod)));
	}
}

void SphericalBody::incrementRot() {
	setRotation(rot + rotInc);
}

void SphericalBody::setRotation(float _rot) {
	rot = _rot;
	updatePosition();
}





void SphericalBody::updatePosition() {
	vec4 np = rotateAboutAxis(vec3(1, 0, 0), orbitalInclination) * rotateAboutAxis(vec3(0, 1, 0), orbitRot) * vec4(distance, 0.f, 0.f, 1.f);
	pos = vec3(np.x, np.y, np.z);
}
