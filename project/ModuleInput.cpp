#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
	
	gameController1 = nullptr;

	for (uint i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
		gameController1States[i] = KEY_IDLE;
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}


	if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0)
	{
		LOG("SDL_JOYSTICK could not initialize! SDL Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
		}
	}


	//Load GameController
	if (!SDL_GameControllerGetAttached(gameController1)) {
		gameController1 = SDL_GameControllerOpen(0);
		/*if (gameController1 == NULL)
			LOG("Warning: Unable to open game controller 1! SDL Error: %s\n", SDL_GetError());*/
	}


	SDL_GameControllerUpdate();

	//Obtain the current button values of GamePad 1
	for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i) {
		if (SDL_GameControllerGetButton(gameController1, (SDL_GameControllerButton)i) == 1) {
			if (gameController1States[i] == KEY_IDLE)
				gameController1States[i] = KEY_DOWN;
			else
				gameController1States[i] = KEY_REPEAT;
		}
		else {
			if (gameController1States[i] == KEY_REPEAT || gameController1States[i] == KEY_DOWN)
				gameController1States[i] = KEY_UP;
			else
				gameController1States[i] = KEY_IDLE;
		}
	}

	//Obtain the current axis value of GamePad 1
	for (int i = 0; i < SDL_CONTROLLER_AXIS_MAX; ++i) {
		gameController1AxisValues[i] = SDL_GameControllerGetAxis(gameController1, (SDL_GameControllerAxis)i);
	}

	//Obtaind the direction of Gamepad 1
	if ((App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_DOWN) || (App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_REPEAT) || gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTY] < -JOYSTICK_DEAD_ZONE)
	{
		pad.up = true;
	}
	else
		pad.up = false;


	if ((App->input->keyboard[SDL_SCANCODE_S] == KEY_STATE::KEY_DOWN) || (App->input->keyboard[SDL_SCANCODE_S] == KEY_STATE::KEY_REPEAT) || gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTY] > JOYSTICK_DEAD_ZONE)
	{
		pad.down = true;
	}
	else
		pad.down = false;


	if ((App->input->keyboard[SDL_SCANCODE_A] == KEY_STATE::KEY_DOWN) || (App->input->keyboard[SDL_SCANCODE_A] == KEY_STATE::KEY_REPEAT) || gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTX] < -JOYSTICK_DEAD_ZONE)
	{
		pad.left = true;
	}
	else
		pad.left = false;

	if ((App->input->keyboard[SDL_SCANCODE_D] == KEY_STATE::KEY_DOWN) || (App->input->keyboard[SDL_SCANCODE_D] == KEY_STATE::KEY_REPEAT) || gameController1AxisValues[SDL_CONTROLLER_AXIS_LEFTX] > JOYSTICK_DEAD_ZONE)
	{
		pad.right = true;
	}
	else
		pad.right = false;

	if ((App->input->keyboard[SDL_SCANCODE_U] == KEY_STATE::KEY_DOWN) || (App->input->gameController1States[SDL_CONTROLLER_BUTTON_X] == KEY_DOWN))
	{
		pad.x = true;
	}
	else
		pad.x = false;

	if ((App->input->keyboard[SDL_SCANCODE_J] == KEY_STATE::KEY_DOWN) || (App->input->gameController1States[SDL_CONTROLLER_BUTTON_A] == KEY_DOWN))
	{
		pad.a = true;
	}
	else
		pad.a = false;

	if ((App->input->keyboard[SDL_SCANCODE_I] == KEY_STATE::KEY_DOWN) || (App->input->gameController1States[SDL_CONTROLLER_BUTTON_Y] == KEY_DOWN))
	{
		pad.y = true;
	}
	else
		pad.y = false;

	if ((App->input->keyboard[SDL_SCANCODE_K] == KEY_STATE::KEY_DOWN) || (App->input->gameController1States[SDL_CONTROLLER_BUTTON_B] == KEY_DOWN))
	{
		pad.b = true;
	}
	else
		pad.b = false;

	if ((App->input->keyboard[SDL_SCANCODE_O] == KEY_STATE::KEY_DOWN) || (App->input->gameController1States[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER] == KEY_DOWN))
	{
		pad.R1 = true;
	}
	else
		pad.R1 = false;

	if ((App->input->keyboard[SDL_SCANCODE_L] == KEY_STATE::KEY_DOWN) || (App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_TRIGGERRIGHT] > JOYSTICK_DEAD_ZONE))
	{
		pad.R2 = true;
	}
	else
		pad.R2 = false;

	if ((App->input->keyboard[SDL_SCANCODE_I] == KEY_STATE::KEY_DOWN) || (App->input->gameController1States[SDL_CONTROLLER_BUTTON_LEFTSHOULDER] == KEY_DOWN) || (App->input->gameController1States[SDL_CONTROLLER_BUTTON_LEFTSHOULDER] == KEY_REPEAT))
	{
		pad.L1 = true;
	}
	else
		pad.L1 = false;

	if ((App->input->keyboard[SDL_SCANCODE_K] == KEY_STATE::KEY_DOWN) || (App->input->gameController1AxisValues[SDL_CONTROLLER_AXIS_TRIGGERLEFT] > JOYSTICK_DEAD_ZONE))
	{
		pad.L2 = true;
	}
	else
		pad.L2 = false;

	if ((App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN) || (App->input->gameController1States[SDL_CONTROLLER_BUTTON_START] == KEY_DOWN))
	{
		pad.start = true;
	}
	else
		pad.start = false;




	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}