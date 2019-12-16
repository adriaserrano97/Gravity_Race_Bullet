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


	//Here goes the map: absolute yeet my boy
	Add_Linear_Map(5, vec3(5 ,0, 7), 10.f);
	Add_Linear_Map(6, vec3(0, 0, 5), 12.f);
	Add_Linear_Map(8, vec3(7, 0, 5), 16.f);
	Add_Linear_Map(10, vec3(0, 0, 5), 12.f);
	
	
	
	/*
	Cube* littlepad = new Cube(1, 0.15f, 1);

	littlepad->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
	littlepad->SetPos(0, 5, 0);


	primitives.PushBack(littlepad);

	littlepad->body = App->physics->AddBody(*littlepad, 1);
	*/
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
		Tremenda_pilona->color.Set(255, 0, 0, 60.f);
		Tremenda_pilona->SetPos(-gap + reference_vec.x, reference_vec.y, reference_vec.z);
		primitives.PushBack(Tremenda_pilona);
		Tremenda_pilona->body = App->physics->AddBody(*Tremenda_pilona, 10000);

		Tremenda_pilona = new Cube(2, 10, 2);
		Tremenda_pilona->color.Set(0, 0, 0, 60.f);
		Tremenda_pilona->SetPos(gap + reference_vec.x, reference_vec.y, reference_vec.z);
		primitives.PushBack(Tremenda_pilona);
		Tremenda_pilona->body = App->physics->AddBody(*Tremenda_pilona, 10000);

		reference_vec.x += separation.x;
		reference_vec.y += separation.y;
		reference_vec.z += separation.z;

	}
}

