#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1800.0f
#define TURN_DEGREES 5.0f * DEGTORAD
#define BRAKE_POWER 80.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

private:
	void HandleInput();
	void FlipVehicle();

public:
	PhysVehicle3D* vehicle;

private:
	float turn;
	float acceleration;
	float brake;
	bool flip;
	PhysBody3D* last_checkpoint;
};