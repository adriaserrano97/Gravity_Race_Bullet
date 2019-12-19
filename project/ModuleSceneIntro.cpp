#include "Globals.h"
#include "glmath.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled), time(0), time_start(false), timer_sensor(nullptr)
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
	float roof_height = 12;

	
	//Here goes the map: absolute yeeet my boi

	/*
	//START MAP
	
		//Pink
	current_colors[0] = 1; current_colors[1] = 0; current_colors[2] = 1;
	AddLinearMap(5, vec3(0,0,10), 10.f);
	AddCircularMap(30, vec3(85,0,0),20.f,0.1); 
	AddLinearMap(15, vec3(0, 0, -10), 10.f);
	AddCircularMap(30, vec3(45, 0, 0), 20.f, 0.1,-1,-1);
	for (int i = 1; i < 8; i++) {
		AddLinearMap(1, vec3(0, 0, 10), 8.f +i);  //this creates a obeerture ilusion. Fucking love these functions
	}
	CreateRamp(25, 3, 4, vec3(reference_vec),90);
	reference_vec.y += roof_height;

		//Turquoise. Because according to my girflind that's not fucking blue. It's turquoise. Go fucking figure.
	current_colors[0] = 0; current_colors[1] = 1; current_colors[2] = 1;
	for (int i = 1; i < 8; i++) {
		AddLinearMap(1, vec3(0, 0, 10), 16.f - i);  //this creates a obeerture ilusion. Fucking love these functions
	}
	AddLinearMap(8, vec3(10, 0, 10), 16.f);
	AddCircularMap(50, vec3(85, 0, 0), 30.f, 0.07,-1,1);
	AddLinearMap(12, vec3(-7, 0, -5), 23.f);
	AddLinearMap(12, vec3(0, 0, -5), 16.f);
	CreateRamp(25, roof_height +3, 4, vec3(reference_vec), 90);

		//Pink
	current_colors[0] = 1; current_colors[1] = 0; current_colors[2] = 1;
	reference_vec.y -= roof_height;
	AddLinearMap(6, vec3(0, 0, -5), 16.f);
	AddCircularMap(30, vec3(45, 0, 0), 47.f, 0.1, 1, -1);
	for (int i = 1; i < 7; i++) {
		AddLinearMap(1, vec3(0, 0, 10), 23.f - 2*i);  //this creates a obeerture ilusion. Fucking love these functions
	}
	reference_vec.y -= roof_height;
		//Turquoise. 
	current_colors[0] = 0; current_colors[1] = 1; current_colors[2] = 1;

	//END MAP
	*/

	//START MAP

		//Pink
	current_colors[0] = 1; current_colors[1] = 0; current_colors[2] = 1;
	AddLinearMap(5, vec3(0, 0, 10), 10.f);
	AddCircularMap(30, vec3(85, 0, 0), 20.f, 0.1);
	AddLinearMap(15, vec3(0, 0, -10), 10.f);
	AddCircularMap(30, vec3(45, 0, 0), 20.f, 0.1, -1, -1);
	AddCircularMap(30, vec3(45, 0, 0), 20.f, 0.1, -1, 1);
	AddLinearMap(5, vec3(0, 0, -10), 10.f);
	CreateRamp(25, 3, 25, vec3(reference_vec), 25);

	//Turquoise
	current_colors[0] = 0; current_colors[1] = 1; current_colors[2] = 1;

	reference_vec.y += roof_height;

	Cube* checkpoint2 = new Cube(20, 10, 0.2f);

	checkpoint2->SetPos(reference_vec.x, reference_vec.y, reference_vec.z - 20);

	mat3x3 aux;
	
	aux = GetRotFromAngleAndAxis(180, vec3(0, 0, 1)) * GetRotFromAngleAndAxis(180, vec3(0, 1, 0));

	checkpoint2->transform.rotate(aux);

	checkpoint2->body = App->physics->AddBody(*checkpoint2, 0);
	checkpoint2->body->SetAsSensor(true);
	checkpoint2->body->collision_listeners.add(App->player);


	AddLinearMap(17, vec3(0, 0, -10), 17.f);
	
	AddCircularMap(30, vec3(45, 0, 0), 47.f, 0.1, -1, -1);

	AddLinearMap(8, vec3(5, 0, 10), 30.f);
	AddLinearMap(8, vec3(-5, 0, 5), 35.f);
	AddLinearMap(8, vec3(5, 0, 15), 30.f);

	CreateRamp(60, 3, 25, vec3(reference_vec), 15);

	//Pink
	current_colors[0] = 1; current_colors[1] = 0; current_colors[2] = 1;
	reference_vec.y -= roof_height;

	AddLinearMap(10, vec3(0, 0, 10), 14.f);

	AddCircularMap(15, vec3(45, 0, 0), 30, 0.1, -1, 1);




	Cube* roof = new Cube(2500, 1, 2500);
	roof->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
	roof->SetPos(0, 15, 0);	

	primitives.PushBack(roof);
	roof->body = App->physics->AddBody(*roof, 0);

	
	Cube* timer_sensor_first_checkpoint = new Cube(10, 10, 0.2f);

	timer_sensor_first_checkpoint->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
	timer_sensor_first_checkpoint->SetPos(0, 5, 20);

	timer_sensor_first_checkpoint->body = App->physics->AddBody(*timer_sensor_first_checkpoint, 0);
	timer_sensor_first_checkpoint->body->SetAsSensor(true);
	timer_sensor_first_checkpoint->body->collision_listeners.add(App->player);
	timer_sensor_first_checkpoint->body->collision_listeners.add(this);

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
	if (time_start)
	{
		time += dt;
	}
	
	char title[80];
	sprintf_s(title, "TIME SINCE STARTED: %.1f ", time);
	App->window->SetTitle(title);

	Plane p(0, 1, 0, 0);
	float scale = 3; //change this to scale ground plane
	p.Scale(scale, 0, scale);
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

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2) {

	if (timer_sensor == nullptr)
	{
		timer_sensor = body1;
		time_start = !time_start;
	}

	if (timer_sensor != body1)
	{
		time_start = !time_start;
	}
	

}

void ModuleSceneIntro::AddLinearMap(int number, vec3 separation, float gap)
{

	Cube* pilar;


	for (int i = 0; i < number; i++) {

		pilar = new Cube(2, 4, 2);
		pilar->color.Set(current_colors[0], current_colors[1], current_colors[2], 60.f);
		pilar->SetPos(-gap + reference_vec.x, reference_vec.y, reference_vec.z);
		primitives.PushBack(pilar);
		pilar->body = App->physics->AddBody(*pilar, 0);

		pilar = new Cube(2, 4, 2);
		pilar->color.Set(current_colors[0], current_colors[1], current_colors[2], 60.f);
		pilar->SetPos(gap + reference_vec.x, reference_vec.y, reference_vec.z);
		primitives.PushBack(pilar);
		pilar->body = App->physics->AddBody(*pilar, 0);

		reference_vec.x += separation.x;
		reference_vec.z += separation.z;

	}
}

void ModuleSceneIntro::AddCircularMap(int number, vec3 origin_of_rotation, float gap, float density, int sgn_x, int sgn_z)
{
/*

For a circle with origin (j, k) and radius r:

x(t) = r cos(t) + j
y(t) = r sin(t) + k

where you need to run this equation for t taking values within the range from 0 to 360
*/

	Cube* pilar;
	
	float r = length(origin_of_rotation);
	float x_pos;
	float z_pos;


	for (int i = 1; i < number + 1; i++) {
		
		x_pos = (r - gap/2) * cos(density*i);
		z_pos = (r - gap/2) * sin(density*i);

		pilar = new Cube(2, 4, 2);
		pilar->color.Set(current_colors[0], current_colors[1], current_colors[2], 60.f);
		pilar->SetPos(reference_vec.x + sgn_x * x_pos - sgn_x * origin_of_rotation.x, reference_vec.y, reference_vec.z + sgn_z * z_pos - origin_of_rotation.z);
		primitives.PushBack(pilar);
		pilar->body = App->physics->AddBody(*pilar, 0);
		
		x_pos = (r + gap / 2) * cos(density*i);
		z_pos = (r + gap / 2) * sin(density*i);

		pilar = new Cube(2, 4, 2);
		pilar->color.Set(current_colors[0], current_colors[1], current_colors[2], 60.f);
		pilar->SetPos(reference_vec.x + sgn_x * x_pos - sgn_x * origin_of_rotation.x, reference_vec.y, reference_vec.z + sgn_z * z_pos - origin_of_rotation.z);
		primitives.PushBack(pilar);
		pilar->body = App->physics->AddBody(*pilar, 0);
	}
	x_pos = (r) * cos(density*number);
	z_pos = (r) * sin(density*number);
	reference_vec.x = reference_vec.x + sgn_x * x_pos - sgn_x * origin_of_rotation.x;
	reference_vec.z = reference_vec.z + sgn_z * z_pos - origin_of_rotation.z;
}

void ModuleSceneIntro::CreateRamp(float X, float Y, float Z, vec3 pos, float angle)
{
	Cube* ramp;
	ramp = new Cube(X, Y, Z);
	ramp->SetPos(pos.x, pos.y, pos.z);
	ramp->SetRotation(angle,vec3(1,0,0));
	ramp->color.Set(255, 255, 255, 60.f);
	primitives.PushBack(ramp);
	ramp->body = App->physics->AddBody(*ramp, 0);


}

