
#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Particles.hpp"
#include "Random.hpp"


#include "UI.hpp"

class Application
{
    public:
        Application();

        void    run();
    
    private:
        void    update();

    public:
        /* SDL */
        SDL_Window*     window;
        SDL_Renderer    *renderer;

        /* Input */
        float           mouse_x;
        float           mouse_y;
        float           pmouse_x;
        float           pmouse_y;
        bool            mousePressed;
    
    private:
        /* UI */
        Button      reset_button;
        Button      shuffle_button;

        std::vector<ParticleGroupUI> group_ui;

        /* Particles */
    public:
        float           zoom;
        float           x_pos;
        float           y_pos;
        float           scroll;
        
        TTF_Font        *font;

        std::vector<ParticleGroup>  particle_groups;
};



