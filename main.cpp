#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <cmath>
//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

//Event handler
SDL_Event e;

// =============================================================================

SDL_Texture* loadTexture( const char* path )
{
  //The final texture
  SDL_Texture* newTexture = NULL;

  //Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load(path);
  if( loadedSurface == NULL )
  {
      printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
  }
  else
  {
    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
    if( newTexture == NULL )
    {
      printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
    }

    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );
  }

  return newTexture;
}


// Starts up SDL and creates window
bool Init(void)
{
  //Initialization flag
  bool success = true;

  //Initialize SDL
  if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
  {
    printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    success = false;
  }
  else
  {
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    // Create window
    gWindow = SDL_CreateWindow( "Arrow", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( gWindow == NULL )
    {
      printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
      success = false;
    }
    else
    {
      gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED );
      if( gRenderer == NULL )
      {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        success = false;
      }
    }
  }

  return success;
}

void Finit(void)
{
  // Destroy window
  SDL_DestroyWindow( gWindow );
  gWindow = NULL;

  //Quit SDL subsystems
  SDL_Quit();
}

// =============================================================================

int main( int argc, char* args[] )
{
  //Initialize SDL
  if(!Init())
  {
    printf("Init() failed\n");
  }
  else
  {
    SDL_Texture* arrowTexture = loadTexture("./arrow.jpg");
    SDL_Rect destination;

    if(!arrowTexture )
    {
      printf("No texture");
    }
    else
    {
      // Main Loop
      bool exit = false;
      SDL_QueryTexture(arrowTexture, NULL, NULL, &destination.w, &destination.h);
      destination.x = (SCREEN_WIDTH /2) - (destination.w /2) ;
      destination.y = (SCREEN_HEIGHT /2) - (destination.h /2) ;

      while(!exit)
      {
        int mousex, mousey;
        SDL_GetMouseState(&mousex, &mousey);
        float angle = (atan2(mousey - destination.y, mousex - destination.x) * 180.0) / M_PI;
        printf("%f\n", angle*10);
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(gRenderer);
        SDL_RenderCopyEx(gRenderer, arrowTexture, NULL, &destination, angle , NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(gRenderer);

        while( SDL_PollEvent( &e ) != 0 )
        {
          //User requests quit
          if( e.type == SDL_QUIT )
          {
            exit = true;
          }
        }
      }
    }
  }

  Finit();
  return 0;
}