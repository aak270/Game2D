#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

// Texture wrapper class
class Texture {
public:
    // Initializes variables
    Texture();

    // Deallocates memory
    ~Texture();

    // Loads image at specified path
    bool loadFromFile(std::string path);

    // Deallocates texture
    void free();

    // Renders texture at given point
    void render(int x, int y);

    // Gets image dimensions
    int getWidth();
    int getHeight();

private:
    // The actual hardware texture
    SDL_Texture* texture;

    // Image dimensions
    int width;
    int height;
};

// Screen dimension constants
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// Loads individual image as texture
SDL_Texture* loadTexture(std::string path);

// The window we'll be rendering to
SDL_Window* window = NULL;

// The window renderer
SDL_Renderer* renderer = NULL;

// Scene textures
Texture fooTexture;
Texture backgroundTexture;

int main(int argc, char* args[]) {

    // Starts up SDL and create window
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        // Load media
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        } else {
            // Main loop flag
            bool quit = false;

            // Event handler
            SDL_Event e;

            // While application is running
            while (!quit) {
                // Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    // User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }

                // Clear screen
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer);

                //Render background texture to screen
                backgroundTexture.render(0, 0);

                //Render Foo' to the screen
                fooTexture.render(240, 190);

                //Update screen
                SDL_RenderPresent(renderer);

                // Update screen
                SDL_RenderPresent(renderer);
            }
        }
    }

    // Free resources and close SDL
    close();

    return 0;
}

bool init() {
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    } else {
        // Create window
        window = SDL_CreateWindow("Game2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        } else {
            // Create renderer for window
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                // Initialize renderer color
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia() {
    // Loading success flag
    bool success = true;

    // Load Foo' texture
    if(!fooTexture.loadFromFile("../Assets/foo.png")) {
        printf("Failed to load Foo' texture image!\n");
        success = false;
    }

    // Load background texture
    if (!backgroundTexture.loadFromFile("../Assets/background.png")) {
        printf("Failed to load background texture image!\n");
        success = false;
    }
    //
    ////Load PNG texture
    //SDL_Texture* texture = loadTexture("../Assets/viewport.png");
    //if (texture == NULL) {
    //    printf("Failed to load default image!\n");
    //    success = false;
    //}
    //
    return success;
}

void close() {
    //Free loaded images
    fooTexture.free();
    backgroundTexture.free();

    // Destroy window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = NULL;
    window = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTexture(std::string path) {
    // The final texture
    SDL_Texture* newTexture = IMG_LoadTexture(renderer, path.c_str());
    if (newTexture == NULL) {
        printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    }

    return newTexture;
}

Texture::Texture() {
    // Initialize
    texture = NULL;
    width = 0;
    height = 0;
}

Texture::~Texture() {
    // Deallocate
    free();
}

bool Texture::loadFromFile(std::string path) {
    // Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    } else {
        //Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        } else {
            //Get image dimensions
            width = loadedSurface->w;
            height = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    //Return success
    texture = newTexture;
    return texture != NULL;
}

void Texture::free() {
    // Free texture if it exists
    if (texture != NULL) {
        SDL_DestroyTexture(texture);
        texture = NULL;
        width = 0;
        height = 0;
    }
}

void Texture::render(int x, int y) {
    // Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, width, height };
    SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
}

int Texture::getWidth() {
    return width;
}

int Texture::getHeight() {
    return height;
}