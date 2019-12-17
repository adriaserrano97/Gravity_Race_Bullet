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

private:
	// separation is the vec that exists between pillars. {3,0,3} will make a diagonal line with 3 meters between pillars
	void Add_Linear_Map(int number, vec3 separation, float gap = 15);
	// Origin of rotation = from reference vec, coordinates of the origin of the rotation that creates the map. number= how maany pair of pilons. gap= distance horizontal that separates pilars. //density = distance vertical that separates pilalrs. start_from= first iteration. It matters for the angles. sng = only use -1 or +1
	void Add_Circular_Map(int number, vec3 origin_of_rotation, float gap = 15, float density = 0.3f, int sgn_x = 1, int sgn_z = 1); 



public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

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
};
