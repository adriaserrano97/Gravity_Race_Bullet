#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class btVector3;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);

	float* GetViewMatrix();

	

private:

	void CalculateViewMatrix();
	btVector3 Cam_Lerp(vec3 origin, vec3 destiny, float lerp);

public:
	
	vec3 X, Y, Z, Position, Reference;
	vec3 InitialPosition;
	bool car_on_ground;
	

private:
	mat4x4 ViewMatrix, ViewMatrixInverse;

	enum class Camera_Modes {MANUAL, FP, FOLLOW, CAM_ERROR};
	Camera_Modes Cam_status;
	vec3 delta_from_car; //distance from the car to the camera pos
	vec3 FP_vec; //point from the chais to camera position in fisrt person view mode

	//We kinda need this. Just for the memes.
	vec3 RotateVec3(vec3 vec, mat3x3 rot);

};