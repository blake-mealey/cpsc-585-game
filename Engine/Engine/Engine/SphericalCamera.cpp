#include "SphericalCamera.h"
#include <cstdio>
#include <iostream>

#define PI 3.14159265359

SphericalCamera::SphericalCamera():	dir(vec3(0, 0, -1)), 
					right(vec3(1, 0, 0)), 
					up(vec3(0, 1, 0)),
					pos(vec3(0, 0, 0)),
					offset(vec3(0, 0, 0)),
					spherical(vec3(PI/3, PI/3, 1))
{}

SphericalCamera::SphericalCamera(vec3 _offset, vec3 _spherical): spherical(_spherical), offset(_offset)
{
	updatePositionAndDirection();
}




/*
	[ Right 0 ]
	[ Up 	0 ]
	[ -Dir	0 ]
	[ 0 0 0 1 ]
*/

mat4 SphericalCamera::getMatrix(){
	mat4 cameraRotation = mat4(
			vec4(right, 0),
			vec4(up, 0),
			vec4(-dir, 0),
			vec4(0, 0, 0, 1));

	mat4 translation = mat4 (
			vec4(1, 0, 0, 0),
			vec4(0, 1, 0, 0),
			vec4(0, 0, 1, 0),
			vec4(-pos, 1));

	return transpose(cameraRotation)*translation;
}



void SphericalCamera::incrementSpherical(vec3 _spherical) {
	setSpherical(spherical + _spherical);
}

void SphericalCamera::setSpherical(vec3 _spherical) {
	spherical = _spherical;
	updatePositionAndDirection();
}




void SphericalCamera::setOffset(vec3 _offset) {
	offset = _offset;
	updatePositionAndDirection();
}

void SphericalCamera::translate(vec3 _offset) {
	offset += _offset;
	updatePositionAndDirection();
}




void SphericalCamera::updatePositionAndDirection() {
	pos = offset + spherical.z * vec3(
		sin(spherical.x) * cos(spherical.y),
		cos(spherical.x),
		sin(spherical.x) * sin(spherical.y)
	);

	dir = normalize(offset - pos);
	right = normalize(cross(dir, vec3(0, 1, 0)));
	up = normalize(cross(right, dir));
}
