#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "api.h"
#include "settings.h"

int main(int argc, char** argv) {
	// Initialize standard MinUI core libraries and subsystems
	SDL_Surface* screen = GFX_init(MODE_MAIN);
	if (!screen) {
		fprintf(stderr, "Failed to initialize video\n");
		return 1;
	}

	PAD_init();
	PWR_init();
	SETTINGS_init();
	SETTINGS_open();

	int quit = 0;
	int dirty = 1;
	uint32_t last_tick = SDL_GetTicks();

	while (!quit && SETTINGS_isOpen()) {
		PAD_poll();
		
		SETTINGS_update(&dirty);
		SETTINGS_handleInput(&dirty, &quit);

		// Sleep management (handled by PWR subsystem)
		int show_setting = 0;
		PWR_update(&dirty, &show_setting, NULL, NULL);

		if (dirty) {
			GFX_clear(screen);
			SETTINGS_draw(screen, show_setting);
			GFX_flip(screen);
			dirty = 0;
		}

		// Keep frame rate stable at ~60fps
		uint32_t now = SDL_GetTicks();
		if (now - last_tick < 16) {
			SDL_Delay(16 - (now - last_tick));
		}
		last_tick = SDL_GetTicks();
	}

	SETTINGS_close();
	SETTINGS_quit();
	PWR_quit();
	PAD_quit();
	GFX_quit();

	return 0;
}
