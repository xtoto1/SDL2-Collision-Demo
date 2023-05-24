#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
int RECT_X = 250;
int RECT_Y = 150;
int RECT_W = 200;
const int RECT_H = 200;
const int DELAY_MS = 10;
const int RECT_STEP = 30;

int main(int argc, const char* argv[])
{
    bool quit = false;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("SDL2 Starter Project",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // Load the font
    TTF_Font* font = TTF_OpenFont("OpenSans-Regular.ttf", 24);
    if (!font) {
        std::cerr << "TTF_OpenFont error: " << TTF_GetError() << std::endl;
        return 1;
    }

    // Create a surface from the font
    SDL_Color textColor = { 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "COLLISION TEST", textColor);
    if (!textSurface) {
        std::cerr << "TTF_RenderText_Solid error: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Create a texture from the surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!textTexture) {
        std::cerr << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << std::endl;
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Get the texture dimensions
    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    // Position the text box in the top-right corner of the window
    SDL_Rect textRect;
    textRect.x = SDL_GetWindowSurface(window)->w - textWidth - 10;
    textRect.y = 10;
    textRect.w = textWidth;
    textRect.h = textHeight;

    // Create a rectangle for the non-movable cube
    SDL_Rect cubeRect;
    cubeRect.x = 300;
    cubeRect.y = 200;
    cubeRect.w = 50;
    cubeRect.h = 50;

    while (!quit)
    {
        SDL_Delay(10);
        SDL_PollEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_LEFT:
                        RECT_X -= RECT_STEP;
                        break;
                    case SDLK_RIGHT:
                        RECT_X += RECT_STEP;
                        break;
                    case SDLK_UP:
                        RECT_Y -= RECT_STEP;
                        break;
                    case SDLK_DOWN:
                        RECT_Y += RECT_STEP;
                        break;
                }
}
        if (RECT_X < 0)
            RECT_X = SCREEN_WIDTH;
        if (RECT_X > SCREEN_WIDTH)
            RECT_X = 0;
        if (RECT_Y < 0)
            RECT_Y = SCREEN_HEIGHT;
        if (RECT_Y > SCREEN_HEIGHT)
            RECT_Y = 0;
        // Check for collision between the movable rectangle and the non-movable cube
        SDL_Rect rect = { RECT_X, RECT_Y, RECT_W, RECT_H };
        bool collision = SDL_HasIntersection(&cubeRect, &rect);


        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the text
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        // Draw the non-movable cube
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &cubeRect);

        // Draw the movable rectangle
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);

        // Draw a red border around the movable rectangle if there is collision with the non-movable cube
        if (collision) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &rect);
            std::cout << "COLLIDING " << RECT_X << " " <<  RECT_Y << std::endl;
        } else {
            std::cout << "NOT COLLIDING " << RECT_X << " " <<  RECT_Y << std::endl;
        }

        // Update the screen
        SDL_RenderPresent(renderer);
    }

    // Clean up resources
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
