#ifndef FUNC_H
#define FUNC_H
#include <math.h>

Vector3 addVector3(Vector3 v1,Vector3 v2) { // add
	Vector3 result = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	return result;
}
Vector3 subVector3(Vector3 v1,Vector3 v2) { // subtract
	Vector3 result = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	return result;
}
Vector3 scaleVector3(Vector3 vec,float scalar) { // scale
	Vector3 result = { vec.x * scalar, vec.y * scalar, vec.z * scalar };
	return result;
}
Vector3 normVector3(Vector3 vec) { // normalize
	float mag = sqrtf((vec.x*vec.x)+(vec.y*vec.y)+(vec.z*vec.z));
	if(mag==0.0f) return vec;
	Vector3 result = {vec.x/mag,vec.y/mag,vec.z/mag};
	return result;
}
float magVector3(Vector3 vec) { // normalize
	float result = sqrtf((vec.x*vec.x)+(vec.y*vec.y)+(vec.z*vec.z));
	return result;
}

float smoothStart2(float p) {
	return p*p; // pow() is bloat
}
float smoothStart3(float p) {
	return p*p*p;
}
float smoothStop2(float p) {
	return(1-((1-p)*(1-p)));
}
float smoothStop3(float p) {
	return(1-((1-p)*(1-p)*(1-p)));
}

#endif
