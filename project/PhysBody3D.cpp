#include "PhysBody3D.h"

#include "Bullet/include/btBulletDynamicsCommon.h"

// =================================================
PhysBody3D::PhysBody3D(btRigidBody* body) : body(body), is_sensor(false)
{}

// ---------------------------------------------------------
PhysBody3D::~PhysBody3D()
{
	delete body;
}


void PhysBody3D::SetAsSensor(bool is_sensor) {

	if (this->is_sensor != is_sensor)
	{
		this->is_sensor = is_sensor;
		if (is_sensor == true)
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		else
			body->setCollisionFlags(body->getCollisionFlags() &~btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
}



// ---------------------------------------------------------
void PhysBody3D::Push(float x, float y, float z)
{
	body->applyCentralImpulse(btVector3(x, y, z));
}

// ---------------------------------------------------------
void PhysBody3D::GetTransform(float* matrix) const
{
	if(body != nullptr && matrix != NULL)
	{
		body->getWorldTransform().getOpenGLMatrix(matrix);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetTransform(const float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		btTransform t;
		t.setFromOpenGLMatrix(matrix);
		body->setWorldTransform(t);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetPos(float x, float y, float z)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(t);
}

vec3 PhysBody3D::GetForward()
{
	mat4x4 transform;
	body->getWorldTransform().getOpenGLMatrix(&transform);

	mat3x3 rotation(transform);

	vec3 forward(0.f, 0.f, 1.f);

	forward = rotation * forward;

	return forward;
}

mat3x3 PhysBody3D::GetRotation()
{
	mat4x4 transform;
	body->getWorldTransform().getOpenGLMatrix(&transform);

	mat3x3 rotation(transform);
	return rotation;
}


mat3x3 PhysBody3D::GetRotationFromQuat(mat4x4 &quat) const
{
	mat3x3 rot;
	rot.M[0] = quat.M[0];
	rot.M[1] = quat.M[1];
	rot.M[2] = quat.M[2];
	rot.M[3] = quat.M[4];
	rot.M[4] = quat.M[5];
	rot.M[5] = quat.M[6];
	rot.M[6] = quat.M[8];
	rot.M[7] = quat.M[9];
	rot.M[8] = quat.M[10];

	return rot;
}


vec3 PhysBody3D::GetPositionFromQuat(mat4x4 &quat) const
{
	vec3 pos;
	pos.x = quat.M[12];
	pos.y = quat.M[13];
	pos.z = quat.M[14];
	return pos;
}


float PhysBody3D::GetScaleFromQuat(mat4x4 &quat) const
{
	return quat.M[15];
}


void PhysBody3D::CopyOnlyRotation(mat4x4 &from_this, mat4x4 &to_this)
{
	to_this[0] = from_this[0];
	to_this[1] = from_this[1];
	to_this[2] = from_this[2];

	to_this[4] = from_this[4];
	to_this[5] = from_this[5];
	to_this[6] = from_this[6];

	to_this[8] = from_this[8];
	to_this[9] = from_this[9];
	to_this[10] = from_this[10];
}