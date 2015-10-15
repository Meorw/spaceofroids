#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <SDL.h>
#include "RenderChimp.h"
#include "Platform.h"

int main(int argc, char *argv[])
{
	printf("Launching RenderChimp for Mac\n");
	
	i32 ret = Platform::init();
	if (ret) {
		printf("Platform init returned %d\n", ret);
		exit(1);
	}
    
    /* Cleanup */
	SDL_Quit();
	
    return 0;
}
