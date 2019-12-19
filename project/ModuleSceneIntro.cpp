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
	float roof_height = 10;

	
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
	reference_vec.y += 2;
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

	Cube* checkpoint3 = new Cube(20, 10, 0.2f);

	checkpoint3->SetPos(reference_vec.x, reference_vec.y, reference_vec.z - 20);

	checkpoint3->body = App->physics->AddBody(*checkpoint3, 0);
	checkpoint3->body->SetAsSensor(true);
	checkpoint3->body->collision_listeners.add(App->player);

	AddCircularMap(30, vec3(45, 0, 0), 30, 0.1, -1, 1);

	AddLinearMap(20, vec3(0, 0, -10), 30.f);

	AddCircularMap(30, vec3(45, 0, 0), 30, 0.1, -1, -1);
	
	
	//CreateBarrier();
	CreateRamp(25, 3, 40, vec3(reference_vec), -15, vec3(1, 0, 0));
	
	//Turquoise
	current_colors[0] = 0; current_colors[1] = 1; current_colors[2] = 1;
	reference_vec.y += roof_height;
	

	AddLinearMap(17, vec3(0, 0, 10), 17.f);

	Cube* checkpoint4 = new Cube(20, 10, 0.2f);

	checkpoint4->SetPos(reference_vec.x, reference_vec.y, reference_vec.z - 20);

	aux = GetRotFromAngleAndAxis(180, vec3(0, 0, 1));

	checkpoint4->transform.rotate(aux);

	checkpoint4->body = App->physics->AddBody(*checkpoint4, 0);
	checkpoint4->body->SetAsSensor(true);
	checkpoint4->body->collision_listeners.add(App->player);



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


	Cube* start_signal = new Cube(5, 10, 5);
	start_signal->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
	start_signal->SetPos(11, 5, 20);

	primitives.PushBack(start_signal);
	start_signal->body = App->physics->AddBody(*start_signal, 0);

	Cube* start_signal2 = new Cube(5, 10, 5);
	start_signal2->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
	start_signal2->SetPos(-11, 5, 20);

	primitives.PushBack(start_signal2);
	start_signal2->body = App->physics->AddBody(*start_signal2, 0);


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

void ModuleSceneIntro::CreateRamp(float X, float Y, float Z, vec3 pos, float angle, vec3 direction)
{
	Cube* ramp;
	ramp = new Cube(X, Y, Z);
	ramp->SetPos(pos.x, pos.y, pos.z);

	ramp->SetRotation(angle,direction);
	ramp->color.Set(255, 255, 255, 60.f);
	primitives.PushBack(ramp);
	ramp->body = App->physics->AddBody(*ramp, 0);


}


void ModuleSceneIntro::CreateBarrier() {

	int aux_h = 5; //definir la altura de la rampa
	Cube* pillar = new Cube(10, aux_h, 0.01); //el pilar que aguanta la rampa
	pillar->color.Set(0, 1, 0);
	pillar->SetPos(reference_vec.x, reference_vec.y, reference_vec.z); //cambiar x,z para donde quieres el bicho

	primitives.PushBack(pillar);
	pillar->body = App->physics->AddBody(*pillar, 0);
	Cube* ramp_pillar = new Cube(10, 0.3, 50); //la rampa en si. la z es como de larga es
	ramp_pillar->color.Set(0, 1, 0);
	ramp_pillar->SetPos(0, aux_h / 2 + 0.5, 20); //esto da igual porque el hinge lo va a forzar a estar con el pilar
	primitives.PushBack(ramp_pillar);
	ramp_pillar->body = App->physics->AddBody(*ramp_pillar, 10); //este ultimo parametro es la masa de la rampa. a mas suave, mas facil de mover
	App->physics->AddConstraintHinge(*pillar->body, *ramp_pillar->body, vec3(0, aux_h / 2 + 1, 0), vec3(0, -0.3 / 2 + 0.5, 3), vec3(1, 0, 0), vec3(1, 0, 0), true);
	//									1st body        2nd body    punto local del 1st body  punto local del 2nd body    eje de movimiento.			true= los bichos no colision
	//															    donde se aplica hinge     donde se aplica hinge    estos ejes te dan una rampa.      entre ellos dos


}


void ModuleSceneIntro::RestartTime() {

	time = 0;
	time_start = false;
	timer_sensor = nullptr;
}