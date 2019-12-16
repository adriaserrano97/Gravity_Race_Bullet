#ifndef __ModuleFonts_H__
#define __ModuleFonts_H__

#include "Module.h"
#include "SDL\include\SDL_pixels.h"
#include "SDL_image\include\SDL_image.h"
#include "SDL/include/SDL.h"




#define MAX_FONTS 10
#define MAX_FONT_CHARS 256
#define MAX_TEXTURES 10

class SDL_Texture;


struct Font
{
	char table[MAX_FONT_CHARS];
	SDL_Texture* graphic = nullptr;
	uint rows, len, char_w, char_h, row_chars;
};

class ModuleFonts : public Module
{
public:

	ModuleFonts(Application* app, bool start_enabled = true);
	~ModuleFonts();

	bool Start();
	// Load Font
	int Load(const char* texture_path, const char* characters, uint rows = 1);
	void UnLoad(int font_id);

	// Create a surface from text
	void BlitText(int x, int y, int bmp_font_id, const char* text) const;
	SDL_Texture* const ModuleFonts::LoadTexture(const char* path);
	bool ModuleFonts::UnloadTexture(SDL_Texture * texture);
	bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, bool flip, float speed = 1.0f);
	//Used to render in 2D
	SDL_Renderer* renderer2D = nullptr;

public:
	

	Font fonts[MAX_FONTS];
	SDL_Texture* textures[MAX_TEXTURES];
};


#endif // __ModuleFonts_H__