# Gravity_Race_Bullet
Game developed in Bullet for Physics project at CITM university 

## Credits
Platformer game made by 2 students at CITM, Terrassa, Spain: 

José Luís Redondo Tello: <https://github.com/jose-tello> <br>
Adriá Serrano López: <https://github.com/adriaserrano97> <br>

## Installation

Currently there is no installation required, just open the .exe provided in the download file

## Usage

### General Controls :
	ESC: Close the game
	
### Debug Controls:<br>
	F1 Show primiteves (1 in show to shoot sphere primitives)
	M: Teleport car to last checkpoint
	2 / R1: Invert gravity 
	n / START: Reset the game to t0 conditions

### Player Controls: <br>
        W / R2: Add acceleration
        A / stick to left: Steer left
        S / L2: Substract acceleration
        D / stick to right: Steer right


## Innovation
	Check out our map generator functions! AddLinearMap & AddCircularMap

## Worthwhile considerations:
Victory condition: reach the second pair of yellow pillars while following the track. Good luck!
Loose condition: run out of time (keep track of it in window name).
Follow the layout: pink on the ground and blue on the cealing. Hit all checkpoints in time to win the game! If you run out of time, you'll have to start again
R2 / L2 add acceleration, not movement: if you've been pressing those buttons for a while, the car has enough acc to move on it's own!
Each time you see a white ramp, go towards it: once you're in, swich gravity! this way you'll hit the checkpoint without loosing momentum... saving precious time!


## License

SDL license:<br>
Simple DirectMedia Layer<br>
Copyright (C) 1997-2016 Sam Lantinga <slouken@libsdl.org><br>
SDL_image:  An example image loading library for use with SDL<br>
Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org><br>
SDL_mixer:  An audio mixer library based on the SDL library<br>
Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org><br>

  
This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:
  
1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required. 
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.



