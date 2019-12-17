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
	
	
	
	
	Cube* littlepad = new Cube(2500, 1, 2500);

	Add_Linear_Map(5, vec3(0,0,10), 10.f);
	Add_Circular_Map(30, vec3(85,0,0),20.f,0.1);
	
	

	littlepad->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
	littlepad->SetPos(0, 15, 0);


	primitives.PushBack(littlepad);

	littlepad->body = App->physics->AddBody(*littlepad, 0);
	
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

		Tremenda_pilona = new Cube(2, 20, 2);
		Tremenda_pilona->color.Set(255, 0, 0, 60.f);
		Tremenda_pilona->SetPos(-gap + reference_vec.x, reference_vec.y, reference_vec.z);
		primitives.PushBack(Tremenda_pilona);
		Tremenda_pilona->body = App->physics->AddBody(*Tremenda_pilona, 0);

		Tremenda_pilona = new Cube(2, 20, 2);
		Tremenda_pilona->color.Set(255, 255, 255, 60.f);
		Tremenda_pilona->SetPos(gap + reference_vec.x, reference_vec.y, reference_vec.z);
		primitives.PushBack(Tremenda_pilona);
		Tremenda_pilona->body = App->physics->AddBody(*Tremenda_pilona, 0);

		reference_vec.x += separation.x;
		reference_vec.y += separation.y;
		reference_vec.z += separation.z;

	}
}

void ModuleSceneIntro::Add_Circular_Map(int number, vec3 origin_of_rotation, float gap, float density)
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

	for (int i = 1; i < number +1; i++) {
		
		x_pos = (r - gap/2) * cos(density*i);
		z_pos = (r - gap/2) * sin(density*i);

		Tremenda_pilona = new Cube(2, 20, 2);
		Tremenda_pilona->color.Set(255, 0, 0, 60.f);
		Tremenda_pilona->SetPos(reference_vec.x + x_pos - origin_of_rotation.x, reference_vec.y, reference_vec.z + z_pos - origin_of_rotation.z);
		primitives.PushBack(Tremenda_pilona);
		Tremenda_pilona->body = App->physics->AddBody(*Tremenda_pilona, 0);
		
		x_pos = (r + gap / 2) * cos(density*i);
		z_pos = (r + gap / 2) * sin(density*i);

		Tremenda_pilona = new Cube(2, 20, 2);
		Tremenda_pilona->color.Set(255, 255, 255, 60.f);
		Tremenda_pilona->SetPos(reference_vec.x + x_pos - origin_of_rotation.x, reference_vec.y, reference_vec.z + z_pos - origin_of_rotation.z);
		primitives.PushBack(Tremenda_pilona);
		Tremenda_pilona->body = App->physics->AddBody(*Tremenda_pilona, 0);

		
		
		/*
		z_pos = (r + gap / 2) * cos(0.2*i) + origin_of_rotation.x +reference_vec.z;
		x_pos = (r + gap / 2) * sin(0.2*i) + origin_of_rotation.z +reference_vec.x;

		Tremenda_pilona = new Cube(2, 20, 2);
		Tremenda_pilona->color.Set(255, 0, 0, 60.f);
		Tremenda_pilona->SetPos(x_pos, reference_vec.y, z_pos);
		primitives.PushBack(Tremenda_pilona);
		Tremenda_pilona->body = App->physics->AddBody(*Tremenda_pilona, 0);
		
		z_pos = (r - gap / 2) * cos(0.2*i) + origin_of_rotation.x +reference_vec.z;
		x_pos = (r - gap / 2) * sin(0.2*i) + origin_of_rotation.z +reference_vec.x;
		Tremenda_pilona = new Cube(2, 20, 2);
		Tremenda_pilona->color.Set(255, 255, 255, 60.f);
		Tremenda_pilona->SetPos(x_pos, reference_vec.y, z_pos);
		primitives.PushBack(Tremenda_pilona);
		Tremenda_pilona->body = App->physics->AddBody(*Tremenda_pilona, 0);
		*/
	}
	reference_vec.x = x_pos;
	reference_vec.z = z_pos;
}

