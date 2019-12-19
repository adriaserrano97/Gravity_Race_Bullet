#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL), flip(false), last_checkpoint(nullptr)
{
	turn = acceleration = brake = 0.0f;
	
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(3.5, 1.5f, 5);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = 400.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 400;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.6f;
	float wheel_radius = 1.4f;
	float wheel_width = 0.9f;
	float suspensionRestLength = 0.4f;

	// Car properties ----------------------------------------
	/*car.chassis_size.Set(3.5, 1.5f, 5);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = 700.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 1200;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.6f;
	float wheel_radius = 1.4f;
	float wheel_width = 0.9f;
	float suspensionRestLength = 0.4f;*/

	// Don't change anything below this line ------------------ que no haga que?

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius + 0.5f);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width + 0.3;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius + 0.5f);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width + 0.3;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius - 0.5f);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius - 0.5f);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(App->camera->InitialPosition.x, App->camera->InitialPosition.y, App->camera->InitialPosition.z);
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{

	turn = acceleration = brake = 0.0f;

	HandleInput();

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	char title[400];
	sprintf_s(title, "%.1f Km/h  vec forward is: %.1f x %.1f y %.1f z ", vehicle->GetKmh(), vehicle->GetForward().x, vehicle->GetForward().y, vehicle->GetForward().z);
	//App->window->SetTitle(title);


	return UPDATE_CONTINUE;
}


void ModulePlayer::HandleInput() {

	//Debug: "M" to reset position
	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		if (last_checkpoint == nullptr)
		{
			vehicle->SetPos(App->camera->InitialPosition.x, App->camera->InitialPosition.y, App->camera->InitialPosition.z);
		}

		else
		{
			mat4x4 aux;
			last_checkpoint->GetTransform(&aux);
			vehicle->SetTransform(&aux);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || (App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_TRIGGERRIGHT] > MID_JOYSTICK))
	{
		if (flip)
			acceleration = -MAX_ACCELERATION;
		
		else
			acceleration = MAX_ACCELERATION;
	}

	if ((App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_TRIGGERRIGHT] < MID_JOYSTICK) && (App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_TRIGGERRIGHT] > JOYSTICK_DEAD_ZONE))
	{
		if (flip)
			acceleration = -MAX_ACCELERATION * 0.6;

		else
			acceleration = MAX_ACCELERATION * 0.6;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTX] < -MID_JOYSTICK)
	{
		if (flip) {

			if (turn > -TURN_DEGREES)
				turn = -TURN_DEGREES;
		}

		else {

			if (turn < TURN_DEGREES)
			turn = TURN_DEGREES;
		}
	}

	if ((App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTX] < -JOYSTICK_DEAD_ZONE) && (App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTX] > -MID_JOYSTICK))
	{
		if (flip) {

			if (turn > -TURN_DEGREES)
				turn = -TURN_DEGREES * 0.5;
		}

		else {

			if (turn < TURN_DEGREES)
				turn = TURN_DEGREES * 0.5;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || (App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTX] > MID_JOYSTICK))
	{
		if (flip) {

			if (turn < TURN_DEGREES)
				turn = TURN_DEGREES;
		}

		else {

			if (turn > -TURN_DEGREES)
				turn = -TURN_DEGREES;
			
		}
	}

	if ((App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTX] < MID_JOYSTICK) && (App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTX] > JOYSTICK_DEAD_ZONE))
	{
		if (flip) {

			if (turn < TURN_DEGREES)
				turn = TURN_DEGREES * 0.5;
		}

		else {

			if (turn > -TURN_DEGREES)
				turn = -TURN_DEGREES * 0.5;

		}
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || (App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_TRIGGERLEFT] > MID_JOYSTICK))
	{
		if (flip)
			acceleration = MAX_ACCELERATION;

		else
			acceleration = -MAX_ACCELERATION;
	}

	if ((App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_TRIGGERLEFT] < MID_JOYSTICK) && (App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_TRIGGERLEFT] > JOYSTICK_DEAD_ZONE))
	{
		if (flip)
			acceleration = MAX_ACCELERATION * 0.6f;

		else
			acceleration = -MAX_ACCELERATION * 0.6f;
	}

	if (App->input->pad.L1 == true)
	{
		brake = BRAKE_POWER;
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN || App->input->pad.R1 == true)
	{
		FlipVehicle();
	}

}


void ModulePlayer::FlipVehicle() {

	App->physics->FlipGravity();
	float y_adjustment;
	App->camera->car_on_ground = !(App->camera->car_on_ground);
	

	if (flip == true)
	{
		y_adjustment = -1.5;
		flip = false;
	}

	else if (flip == false)
	{
		y_adjustment = 1.5;
		flip = true;
	}
	

	mat4x4* player_aux = new mat4x4();
	vehicle->GetTransform(&player_aux[0][0]);
	mat3x3 flipcar = GetRotFromAngleAndAxis(180, vec3(0,0,1));
	mat3x3 auxrot(*player_aux);
	auxrot = auxrot * flipcar;
	player_aux->rotate(auxrot);
	player_aux->M[13] += y_adjustment;
	vehicle->SetTransform(&player_aux[0][0]);
}


void ModulePlayer::OnCollision(PhysBody3D* body1, PhysBody3D* body2) {
	
	last_checkpoint = body1;
	
}