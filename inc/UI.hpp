
#pragma once
#ifndef UI_HPP
#define UI_HPP

class Button;
class Slider;

#include <SDL2/SDL.h>
#include <string>
#include "Particles.hpp"
#include <vector>

class Application;



class UIComponent
{
    public:
        int         x;
        int         y;
        int         width;
        int         height;
        std::string title;

        Application     &app;

        UIComponent(Application &app, int x = 0, int y = 0, int width = 10, int height = 10, const std::string& title = "")
        : x(x), y(y), width(width), height(height), title(title), app(app)
        {}

        UIComponent&    operator=(const UIComponent& other)
        {
            x = other.x;
            y = other.y;
            width = other.width;
            height = other.height;
            title = other.title;     
            return (*this);
        }

        virtual void        init() = 0;
        virtual void        draw() = 0;
};


class   Button : public UIComponent
{
    public:
        // logic
        bool        pressed;
        bool        toggled;

        // render
        SDL_Rect        rect;
        SDL_Rect        title_box;
        SDL_Texture*    texture;

        Button(Application &app, int x = 0, int y = 0, int width = 10, int height = 10, const std::string& title = "");
        virtual ~Button() { SDL_DestroyTexture(this->texture); }

        Button &operator=(const Button& other);

        void        init();
        void        draw();
};


class  Slider : public UIComponent
{
    public:
        // logic
        float       progress;
        std::string title_format;
        float       process_coefficient;

        // render
        SDL_Rect    rect;
        SDL_Rect    progress_rect;
        SDL_Rect    title_box;
        SDL_Texture *texture;

        Slider(Application& app, int x = 0, int y = 0, int width = 10, int height = 10, float progress_start = 0);
        virtual ~Slider() {}

        Slider&     operator=(const Slider& other);

        void        init();
        void        draw();
};


class Label : public UIComponent
{
    public:
        SDL_Rect    title_box;
        SDL_Texture *texture;

        Label(Application& app, const std::string& label, int x, int y);
        virtual ~Label() {}

        void        init();
        void        draw();
};


class ParticleGroupUI : public UIComponent
{
    public:
        ParticleGroup*  group;
        int             id;

        // render
        SDL_Rect        background;

        SDL_Rect        toggle;
        Label           group_title;

        Label           amount_label;
        Slider          amount_slider;
        
        Label           self_attraction_label;
        Slider          self_attraction_slider;
        
        Label           self_repulsion_label;
        Slider          self_repulsion_slider;

        std::vector<Label>     other_interactions_labels;
        std::vector<Slider>    other_interactions_slider;

        ParticleGroupUI(Application& app, int id);
        virtual ~ParticleGroupUI() {}

        void    init();
        void    draw();
};

#endif