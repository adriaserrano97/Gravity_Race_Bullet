#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"
#include "glmath.h"

class btRigidBody;
class Module;
class vec3;
class mat3x3;
// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	vec3 GetForward();
	mat3x3 GetRotation();

	mat3x3 GetRotationFromQuat(mat4x4 &quat) const;
	vec3 GetPositionFromQuat(mat4x4 &quat) const;
	float GetScaleFromQuat(mat4x4 &quat) const;
	void CopyOnlyRotation(mat4x4 &from_this, mat4x4 &to_this);

	void SetAsSensor(bool is_sensor);

private:
	btRigidBody* body;
	bool is_sensor;

public:
	p2List<Module*> collision_listeners;
};

#endif // __PhysBody3D_H__