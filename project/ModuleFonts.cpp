#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleFonts.h"

#include<string.h>
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_surface.h"
#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib") 


// Constructor
ModuleFonts::ModuleFonts(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

// Destructor
ModuleFonts::~ModuleFonts()
{}

bool ModuleFonts::Start()
{
	bool ret = true;

	for (uint i = 0; i < MAX_TEXTURES; ++i) {
		textures[i] = nullptr;
	}


	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);
	if ((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}
	
	renderer2D = SDL_CreateRenderer(App->window->window, -1, 0); //we'll use it to render fonts in out 3D world

	typo_1 = Load("assets/font1.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789~'!@#$%^&*()-_+=[]{}| :;¨º<>,./?", 1);


	return ret;
}

bool ModuleFonts::CleanUp()
{
	LOG("Freeing textures and Image library");

	for (uint i = 0; i < MAX_TEXTURES; ++i) {
		if (textures[i] != nullptr) {
			SDL_DestroyTexture(textures[i]);
		}
	}
	if (typo_1 != -1) {
		UnLoad(typo_1);
	}
	IMG_Quit();
	return true;
}

// Load new font from file path
int ModuleFonts::Load(const char* texture_path, const char* characters, uint rows)
{
	int id = -1; // we'll use this to track wether or not the function is working as intended. If not, it will return this -1

	int width;
	int height;

	if (texture_path == nullptr || characters == nullptr || rows == 0)
	{
		LOG("Could not load font");
		return id;
	}

	SDL_Texture* tex = LoadTexture(texture_path);

	if (tex == nullptr || strlen(characters) >= MAX_FONT_CHARS)
	{
		LOG("Could not load font at %s with characters '%s'", texture_path, characters);
		return id;
	}

	id = 0;
	for (; id < MAX_FONTS; ++id)
		if (fonts[id].graphic == nullptr)
			break;

	if (id == MAX_FONTS)
	{
		LOG("Cannot load font %s. Array is full (max %d).", texture_path, MAX_FONTS);
		return id;
	}

	SDL_QueryTexture(tex, NULL, NULL, &width, &height);

	fonts[id].graphic = tex; // graphic: pointer to the texture
	fonts[id].rows = rows; // rows: rows of characters in the texture
	fonts[id].len = strlen(characters); // len: length of the table

	strcpy_s(fonts[id].table, characters);
	fonts[id].row_chars = fonts[id].len;
	fonts[id].char_h = height / rows;
	fonts[id].char_w = width / strlen(characters);

	LOG("Successfully loaded BMP font from %s", texture_path);

	return id;
}

void ModuleFonts::UnLoad(int font_id)
{
	if (font_id >= 0 && font_id < MAX_FONTS && fonts[font_id].graphic != nullptr)
	{
		UnloadTexture(fonts[font_id].graphic);
		fonts[font_id].graphic = nullptr;
		LOG("Successfully Unloaded BMP font_id %d", font_id);
	}
}

// Render text using a bitmap font
void ModuleFonts::BlitText(int x, int y, int font_id, const char* text) const
{
	if (text == nullptr || font_id < 0 || font_id >= MAX_FONTS || App->fonts->fonts[font_id].graphic == nullptr)
	{
		LOG("Unable to render text with bmp font id %d", font_id);
		return;
	}

	Font* font = &App->fonts->fonts[font_id];
	SDL_Rect rect;
	uint len = strlen(text);

	rect.w = font->char_w;
	rect.h = font->char_h;

	for (int i = 0; i < len; i++)
	{
		bool foundCharacter = false;
		int j = 0;
		do
		{
			if (font->table[j] == text[i]) {
				rect.x = j * font->char_w;
				rect.y = 0;
				App->fonts->Blit(font->graphic, x, y, &rect, false);
				x += font->char_w;
				foundCharacter = true;
			}
			else
				j++;

		} while (!foundCharacter && j < font->len);
	}
}

// Load / Unload new texture from file path
SDL_Texture* const ModuleFonts::LoadTexture(const char* path)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(path);

	if (surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
		LOG("Sadness");
	}
	else
	{
		
		texture = SDL_CreateTextureFromSurface(renderer2D, surface);
		

		if (texture == NULL)
		{
			LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			bool room = false;
			for (int i = 0; i < MAX_TEXTURES && !room; ++i)
			{
				if (textures[i] == nullptr)
				{
					textures[i] = texture;
					room = true;
				}
			}
			if (room == false)
				LOG("Texture buffer overflow");
		}

		SDL_FreeSurface(surface);
	}

	return texture;
}

bool ModuleFonts::UnloadTexture(SDL_Texture * texture)
{
	bool ret = false;

	if (texture != nullptr)
	{
		for (int i = 0; i < MAX_TEXTURES && !ret; ++i)
		{
			if (textures[i] == texture)
			{
				textures[i] = nullptr;
				ret = true;
			}
		}
		SDL_DestroyTexture(texture);
	}

	return ret;
}

bool ModuleFonts::Blit(SDL_Texture * texture, int x, int y, SDL_Rect * section, bool flip, float speed)
{
	bool ret = true;
	SDL_Rect rect;
	rect.x = x/*(int)((camera.x + camera_offset.x) * speed) +x */  * SCREEN_SIZE;
	rect.y = y/*(int)((camera.y + camera_offset.y) * speed) + y*/ * SCREEN_SIZE;

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	//rect.w *= SCREEN_SIZE;
	//rect.h *= SCREEN_SIZE;

	if (SDL_RenderCopyEx(renderer2D, texture, section, &rect, NULL, NULL, (flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE)) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}
				