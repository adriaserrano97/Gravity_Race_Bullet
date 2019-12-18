#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	reference_vec = vec3(0, 0, 0);


	//Here goes the map: absolute yeeet my boi
	/*
	Add_Linear_Map(4, vec3(5 ,0, 7));
	Add_Linear_Map(5, vec3(0, 0, 5), 21.f);
	Add_Linear_Map(6, vec3(7, 0, 5), 21.f);
	Add_Linear_Map(5, vec3(0, 0, 5));
	Add_Linear_Map(6, vec3(5, 0, 5), 21.f);
	*/
	//Add_Circular_Map(int number, vec3 origin_of_rotation, float gap)

	Add_Linear_Map(5, vec3(0,0,10), 10.f);
	Add_Circular_Map(30, vec3(85,0,0),20.f,0.1); 
	Add_Linear_Map(15, vec3(0, 0, -10), 10.f);
	Add_Circular_Map(30, vec3(45, 0, 0), 20.f, 0.1,-1,-1);
	Add_Linear_Map(12, vec3(0, 0, 10), 10.f);
	Create_Ramp(15, 3, 15, vec3(reference_vec),90);
	for (int i = 1; i < 8; i++) {
		Add_Linear_Map(1, vec3(0, 0, 10), 10.f + i);  //this creates a obeerture ilusion. Fucking love these functions
		reference_vec.y += 1;
	}


	
	//littlepad = the roof. Long story here.
	Cube* littlepad = new Cube(2500, 1, 2500);
	littlepad->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
	littlepad->SetPos(0, 15, 0);


	primitives.PushBack(littlepad);

	littlepad->body = App->physics->AddBody(*littlepad, 0);



	Cube* detector = new Cube(10, 10, 4);

	detector->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
	detector->SetPos(-20, 5, 0);

	detector->body = App->physics->AddBody(*detector, 0);
	detector->body->SetAsSensor(true);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	for (uint n = 0; n < primitives.Count(); n++)
	{
		if (primitives[n]->body != nullptr)
		{
			primitives[n]->Update();
		}
	
	}
	return UPDATE_CONTINUE;
}


update_status ModuleSceneIntro::PostUpdate(float dt)
{
	for (uint n = 0; n < primitives.Count(); n++)
	{
		primitives[n]->Render();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::Add_Linear_Map(int number, vec3 separation, float gap)
{

	Cube* Tremenda_pilona;


	for (int i = 0; i < number; i++) {

		Tremenda_pilona = new Cube(2, 10, 2);
		Tremenda_pilona->color.Set(70, 0, 255, 60.f);
		Tremenda_pilona->SetPos(-gap + reference_vec.x, reference_vec.y, reference_vec.z);
		primitives.PushBack(Tremenda_pilona);
		Tremenda_pilona->body = App->physics->AddBody(*Tremenda_pilona, 0);

		Tremenda_pilona = new Cube(2, 10, 2);
		Tremenda_pilona->color.Set(70, 0, 255, 60.f);
		Tremenda_pilona->SetPos(gap + reference_vec.x, reference_vec.y, reference_vec.z);
		primitives.PushBack(Tremenda_pilona);
		Tremenda_pilona->body = App->physics->AddBody(*Tremenda_pilona, 0);

		reference_vec.x += separation.x;
		reference_vec.z += separation.z;

	}
}

void ModuleSceneIntro::Add_Circular_Map(int number, vec3 origin_of_rotation, float gap, float density, int sgn_x, int sgn_z)
{
/*

For a circle with origin (j, k) and radius r:

x(t) = r cos(t) + j
y(t) = r sin(t) + k

where you need to run this equation for t taking values within the range from 0 to 360
*/

	Cube* Tremenda_pilona;
	
	float r = length(origin_of_rotation);
	float x_pos;
	float z_pos;


	for (int i = 1; i < number + 1; i++) {
		
		x_pos = (r - gap/2) * cos(density*i);
		z_pos = (r - gap/2) * sin(density*i);

		Tremenda_pilona = new Cube(2, 10, 2);
		Tremenda_pilona->color.Set(70, 0, 255, 60.f);
		Tremenda_pilona->SetPos(reference_vec.x + sgn_x * x_pos - sgn_x * origin_of_rotation.x, reference_vec.y, reference_vec.z + sgn_z * z_pos - origin_of_rotation.z);
		primitives.PushBack(Tremenda_pilona);
		Tremenda_pilona->body = App->physics->AddBody(*Tremenda_pilona, 0);
		
		x_pos = (r + gap / 2) * cos(density*i);
		z_pos = (r + gap / 2) * sin(density*i);

		Tremenda_pilona = new Cube(2, 10, 2);
		Tremenda_pilona->color.Set(70, 0, 255, 60.f);
		Tremenda_pilona->SetPos(reference_vec.x + sgn_x * x_pos - sgn_x * origin_of_rotation.x, reference_vec.y, reference_vec.z + sgn_z * z_pos - origin_of_rotation.z);
		primitives.PushBack(Tremenda_pilona);
		Tremenda_pilona->body = App->physics->AddBody(*Tremenda_pilona, 0);
	}
	x_pos = (r) * cos(density*number);
	z_pos = (r) * sin(density*number);
	reference_vec.x = reference_vec.x + sgn_x * x_pos - sgn_x * origin_of_rotation.x;
	reference_vec.z = reference_vec.z + sgn_z * z_pos - origin_of_rotation.z;
}

void ModuleSceneIntro::Create_Ramp(float X, float Y, float Z, vec3 pos, float angle)
{
	Cube* Ramp_wannabe;
	Ramp_wannabe = new Cube(X, Y, Z);
	Ramp_wannabe->SetPos(pos.x, pos.y, pos.z);
	Ramp_wannabe->SetRotation(angle,vec3(1,0,0));
	Ramp_wannabe->color.Set(255, 255, 255, 60.f);
	primitives.PushBack(Ramp_wannabe);
	Ramp_wannabe->body = App->physics->AddBody(*Ramp_wannabe, 0);


}

