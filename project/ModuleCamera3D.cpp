#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"


#define CAR App->player->vehicle 

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	InitialPosition = vec3(0, 0, 0);
	delta_from_car = vec3(0, 5, -8);
	FP_vec = vec3(0, 3, 4);
	
	
	Position = vec3(InitialPosition.x,InitialPosition.y, InitialPosition.z);
																				
	Reference = vec3(0.0f, 0.0f,0.0f);



}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;
	Cam_status = Camera_Modes::FOLLOW;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) { //IMPLEMENTED
		Cam_status = Camera_Modes::FOLLOW; }
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) { //NOT IMPLEMENTED
		Cam_status = Camera_Modes::FP; }
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) { //IMPLEMENTED
		Cam_status = Camera_Modes::MANUAL; }
	
	switch (Cam_status) {
	case Camera_Modes::FOLLOW:
	{
		mat4x4 CarQuaternion;
		CAR->GetTransform(&CarQuaternion);
		vec3 car_pos = CAR->GetPositionFromQuat(CarQuaternion);
		vec3 car_forward = CAR->GetForward();


		

		mat3x3 rotation(CarQuaternion);
		vec3 newPos = car_pos + rotation * delta_from_car;

		btVector3 LookAtMe = Cam_Lerp(Position, newPos, 0.17);
		Position = { LookAtMe.x(), delta_from_car.y - car_pos.y, LookAtMe.z() }; //because vec3 = btVec3 would be too fucking easy wouldn't it, Bullet? I'm not even gonna create the overloaded operator. Fuck it
		LookAt(car_pos - vec3(0,car_pos.y,0) + vec3(0, delta_from_car.y, 0));

	}
		break;
	case Camera_Modes::MANUAL:
	{// Implement a debug camera with keys and mouse
		// Now we can make this movememnt frame rate independant!

		vec3 newPos(0, 0, 0);
		float speed = 30.0f * dt;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = 8.0f * dt;

		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

		Position += newPos;
		Reference += newPos;

		// Mouse motion ----------------

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

			Position -= Reference;

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				Y = rotate(Y, DeltaY, X);
				Z = rotate(Z, DeltaY, X);

				if (Y.y < 0.0f)
				{
					Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					Y = cross(Z, X);
				}
			}

			Position = Reference + Z * length(Position);
		}
	}
		break;
	case Camera_Modes::FP:
	{
		mat4x4 CarQuaternion;
		CAR->GetTransform(&CarQuaternion);
		vec3 car_pos = CAR->GetPositionFromQuat(CarQuaternion);
		vec3 car_forward = CAR->GetForward();

		mat3x3 rotation(CarQuaternion);
		vec3 newPos = car_pos + rotation * FP_vec;

		btVector3 LookAtMe = Cam_Lerp(Position, newPos, 1); //First person with lerp is a fucking visual nightmare. Lerp = 1
		Position = { LookAtMe.x(), LookAtMe.y(), LookAtMe.z() }; //because vec3 = btVec3 would be too fucking easy wouldn't it, Bullet? I'm not even gonna create the overloaded operator. Fuck it
		LookAt(car_pos + rotation * vec3(0, FP_vec.y, FP_vec.z));
	}
		break;
	default:
		LOG("Cam error");
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

btVector3 ModuleCamera3D::Cam_Lerp(vec3 origin, vec3 destiny, float lerp)
{
	btVector3 ret;
	ret.setX((1 - lerp)*origin.x + lerp * destiny.x);
	ret.setY((1 - lerp)*origin.y + lerp * destiny.y);
	ret.setZ((1 - lerp)*origin.z + lerp * destiny.z);
	//fancy reformulation of lerp, so it works in case lerp = 1

	return ret;
}

vec3 ModuleCamera3D::RotateVec3(vec3 vec, mat3x3 rot)
{
	vec3 rot_vec;
	rot_vec.x = vec.x*rot.M[0] + vec.y*rot.M[3] + vec.z*rot.M[6];
	rot_vec.y = vec.x*rot.M[1] + vec.y*rot.M[4] + vec.z*rot.M[7];
	rot_vec.z = vec.x*rot.M[2] + vec.y*rot.M[5] + vec.z*rot.M[8];
	
	return rot_vec;
}






