#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void RestartTime();

private:
	
	//put "number" pairs of pilars with a separation between them of "gap". The local position of each pilar is "vec3 separation" from the last pillar added. 
	//This function automatically updates the vector reference_vec, used to track where should the next pair of pilars appear
	void AddLinearMap(int number, vec3 separation, float gap = 15); 
	
	//put "number" pairs of pilars with a separation between them of "gap". All those pillars are on the circumference of relative origin "origin_of_rotation"
	//all of the pillars are on radius norm(origin_of rotation) +/- gap/2. "density" does determine how little is the distance between pillar spawns
	//sgn_x and sgn_z are used to determine the quadrant you're placing your imaginary circle on, thus reguling the direction that the pairs of pillars follow.
	//This function automatically updates the vector reference_vec, used to track where should the next pair of pilars appear
	void AddCircularMap(int number, vec3 origin_of_rotation, float gap = 15, float density = 0.3f, int sgn_x = 1, int sgn_z = 1); 
	//ramp = cube rotated 45 degrees. Yeah. Deal with it.
	void CreateRamp(float X, float Y, float Z, vec3 pos, float angle, vec3 direction = vec3(1, 0, 0));

	void CreateBarrier(int x, int z);

public:
	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	p2DynArray<Primitive*> primitives;

private:
	vec3 reference_vec; //we'll use this to create the map. It's gonna be fucking awesome.
	int current_colors[3];
	double time;
	bool time_start;
	PhysBody3D* timer_sensor;
};
