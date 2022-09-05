
#include "UI.hpp"
#include "Application.hpp"
#include <SDL2/SDL_ttf.h>

Button::Button(Application &app, int x, int y, int width, int height, const std::string& title, bool freezed)
    : UIComponent(app, x, y, width, height, title, freezed), pressed(false), toggled(false)
{
    this->rect.x = this->x;
    this->rect.y = this->y;
    this->rect.h = this->height;
    this->rect.w = this->width;
    this->title_box.x = x + 5; 
    this->title_box.y = y + 9;
    this->title_box.w = 0; 
    this->title_box.h = 0;
}
#include <iostream>
Button &Button::operator=(const Button& other)
{
    x = other.x;
    y = other.y;
    width = other.width;
    height = other.height;
    title = other.title;
    pressed = false;
    toggled = false;
    this->rect.x = this->x;
    this->rect.y = this->y;
    this->rect.h = this->height;
    this->rect.w = this->width;
    this->title_box.x = x + 5; 
    this->title_box.y = y + 9;
    this->title_box.w = 0; 
    this->title_box.h = 0;
 
    return (*this);
}



void    Button::init()
{
    SDL_Surface* surface = TTF_RenderText_Solid(this->app.font, this->title.c_str(), (SDL_Color){220, 220, 220, 0}); 
    this->texture = SDL_CreateTextureFromSurface(this->app.renderer, surface);
    SDL_FreeSurface(surface);
}



void	Button::draw()
{
    toggled = false;
    float scroll = !this->freezed ? app.scroll : 0;
	if (app.mouse_x > this->x && app.mouse_x < this->x + this->width && 
		app.mouse_y - scroll > this->y && app.mouse_y - scroll < this->y + this->height)
	{
		if (app.mousePressed)
		{
			SDL_SetRenderDrawColor(app.renderer, 66, 66, 66, 255);
			this->pressed = true;
		}
        else if (pressed)
        {
            toggled = true;
            pressed = false;
            SDL_SetRenderDrawColor(app.renderer, 88, 88, 88, 255);
        }
		else
		{
			this->pressed = false;
			SDL_SetRenderDrawColor(app.renderer, 44, 44, 44, 255);
		}
	}
	else
	{
		SDL_SetRenderDrawColor(app.renderer, 33, 33, 33, 255);
		this->pressed = false;
	}
	
    SDL_Rect r = this->rect;
    if (!this->freezed)
        r.y += this->app.scroll;

	SDL_RenderFillRect(app.renderer, &r);
	SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(app.renderer, &r);

	SDL_QueryTexture(this->texture, NULL, NULL, &this->title_box.w, &this->title_box.h);
	r = this->title_box;
    if (!this->freezed)
        r.y += this->app.scroll;
    SDL_RenderCopy(app.renderer, this->texture, NULL, &r);
}


/* * */

Slider::Slider(Application& app, int x, int y, int width, int height, float progress_start, bool freezed)
    : UIComponent(app, x, y, width, height, std::to_string(static_cast<int>(progress_start * 100)) + " %", freezed), progress(progress_start), centered(false), color(170, 170, 170)
{
    this->rect.x = this->x;
    this->rect.y = this->y;
    this->rect.h = this->height;
    this->rect.w = this->width;
    this->progress_rect.x = this->x;
    this->progress_rect.y = this->y;
    this->progress_rect.h = this->height;
    this->progress_rect.w = this->width * progress;
    this->title_box.x = 0;
    this->title_box.y = 0;
    this->title_box.x = x + 5;
    this->title_box.y = y + 9;
    this->title_format = "%d %%";
    this->process_coefficient = 100;
}


Slider&     Slider::operator=(const Slider& other)
{
    this->x = other.x;
    this->y = other.y;
    this->width = other.width;
    this->height = other.height;
    this->title = other.title;
    this->progress = other.progress;
    this->rect.x = this->x;
    this->rect.y = this->y;
    this->rect.h = this->height;
    this->rect.w = this->width;
    this->progress_rect.x = this->x + this->width * 0.5f;
    this->progress_rect.y = this->y;
    this->progress_rect.h = this->height;
    this->progress_rect.w = this->width * (progress - 0.5f);
    this->title_box.x = 0;
    this->title_box.y = 0;
    this->title_box.x = x + 5;
    this->title_box.y = y + 9;
    this->title_format = other.title_format;
    this->process_coefficient = other.process_coefficient;
    this->color = other.color;
    return (*this);
}


void    Slider::init()
{
    this->title_box.x = this->x;
    this->title_box.y = this->y + 10;

    char title_lbl[256];
    int end;
    if (this->centered)
        end = std::sprintf(title_lbl, this->title_format.c_str(), static_cast<int>((progress - 0.5f) * this->process_coefficient * 2));
    else
        end = std::sprintf(title_lbl, this->title_format.c_str(), static_cast<int>(progress * this->process_coefficient));
    title_lbl[end] = 0;
    this->title = title_lbl;

    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(this->app.font, title_lbl, (SDL_Color){220, 220, 220, 255}, 200); 
    this->texture = SDL_CreateTextureFromSurface(this->app.renderer, surface);
    SDL_FreeSurface(surface);
}


void	Slider::draw()
{
	SDL_SetRenderDrawColor(this->app.renderer, 33, 33, 33, 255);

	if (app.mouse_x > this->x && app.mouse_x < this->x + this->width && 
		app.mouse_y - app.scroll > this->y && app.mouse_y - app.scroll < this->y + this->height + 10)
	{
		if (app.mousePressed)
		{
			progress = ((app.mouse_x - this->x) / this->width);
            
            char title_lbl[256];
            if (this->centered)
                std::sprintf(title_lbl, this->title_format.c_str(), static_cast<int>((progress - 0.5f) * this->process_coefficient * 2));
            else
                std::sprintf(title_lbl, this->title_format.c_str(), static_cast<int>(progress * this->process_coefficient));
            this->title = title_lbl;//std::to_string(static_cast<int>(progress * 100)) + " %";
            SDL_Surface* surface = TTF_RenderText_Solid(this->app.font, this->title.c_str(), (SDL_Color){220, 220, 220, 0}); 
            this->texture = SDL_CreateTextureFromSurface(this->app.renderer, surface);
            SDL_FreeSurface(surface);
		}
		else
		{
			SDL_SetRenderDrawColor(this->app.renderer, 37, 37, 37, 255);
		}
	}
	else
	{
		SDL_SetRenderDrawColor(this->app.renderer, 33, 33, 33, 255);
	}

	SDL_Rect r;
	r.x = this->x;
    r.h = this->height;
	r.w = this->width;
	SDL_Rect bar;

    if (centered)
    {
        bar.x = this->x + this->width * 0.5f;
        bar.w = this->width * (progress - 0.5f);
    }
    else
    {
        bar.w = this->width * progress;
	    bar.x = this->x;
    }
    if (!freezed)
    {
        r.y = this->y + this->app.scroll;
	    bar.y = this->y + this->app.scroll;
    }
    else
	{
        bar.y = this->y;
        r.y = this->y;
    }
    bar.h = this->height;

	
	SDL_RenderFillRect(this->app.renderer, &r);

	SDL_SetRenderDrawColor(this->app.renderer, abs(color.r - 60), abs(color.g - 60), abs(color.b - 60), 100);
	SDL_RenderFillRect(this->app.renderer, &bar);

	SDL_SetRenderDrawColor(this->app.renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(this->app.renderer, &r);
    
    SDL_QueryTexture(this->texture, NULL, NULL, &this->title_box.w, &this->title_box.h);
    r = this->title_box;
    if (!freezed)
        r.y += this->app.scroll;
    SDL_RenderCopy(this->app.renderer, this->texture, NULL, &r);
}

/* * */

Label::Label(Application& app, const std::string& label, int x, int y, bool freezed)
: UIComponent(app, x, y, 0, 0, label, freezed)
{
    this->title_box.x = x;
    this->title_box.y = y;
}

void    Label::init()
{
    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(this->app.font, this->title.c_str(), (SDL_Color){255, 255, 255, 255}, 200); 
    this->texture = SDL_CreateTextureFromSurface(this->app.renderer, surface);
    SDL_FreeSurface(surface);
}

void    Label::draw()
{
    SDL_Rect r = this->title_box;
    if (!this->freezed)
        r.y += this->app.scroll;
 	SDL_SetRenderDrawColor(this->app.renderer, 255, 255, 255, 255);
    SDL_QueryTexture(this->texture, NULL, NULL, &r.w, &r.h);
    SDL_RenderCopy(this->app.renderer, this->texture, NULL, &r);
}


/***/

ParticleGroupUI::ParticleGroupUI(Application& app, int id)
:   UIComponent(app, 10, 300, 200, 200),
    id(id),
    group_title(app, "Group ", 10, 10),

    amount_label(app, "Amount", 10, 10),
    amount_slider(app, 160, 10, 140, 10),

    self_attraction_label(app, "Self Attraction", 10, 50 + (this->background.h + 5) * id),
    self_attraction_slider(app, 160, 10, 140, 10),

    self_repulsion_label(app, "Self Repulsion", 10, 10),
    self_repulsion_slider(app, 160, 10, 140, 10)
{
    this->background.w = 300;
    this->background.x = 5;    
}


void    ParticleGroupUI::init()
{
    this->background.h = 25 * (this->app.particle_groups.size() + 4);
    this->background.y = 5 + (this->background.h + 5) * id;
    this->group_title.title_box.y = 10 + (this->background.h + 5) * id;
    this->amount_label.title_box.y = 25 + (this->background.h + 5) * id;
    this->amount_slider.y = 28 + (this->background.h + 5) * id;
    this->self_attraction_label.title_box.y = 50 + (this->background.h + 5) * id;
    this->self_attraction_slider.y = 53 + (this->background.h + 5) * id;
    this->self_repulsion_label.title_box.y = 75 + (this->background.h + 5) * id;
    this->self_repulsion_slider.y = 77 + (this->background.h + 5) * id;
    this->amount_slider.progress = this->app.particle_groups.at(this->id).amount / 1500.0f;
    this->self_attraction_slider.progress = -this->app.particle_groups.at(this->id).self_attraction.force * 1.6f;
    this->self_repulsion_slider.progress = this->app.particle_groups.at(this->id).self_repulsion.force * 1.6f;
    this->amount_slider.title_format = "%d particles";
    this->amount_slider.process_coefficient = 1500;

    this->group = &this->app.particle_groups.at(this->id);
    this->group_title.title = "Group " + group->name;
    this->group_title.init();
    this->amount_label.init();
    this->amount_slider.init();
    this->self_attraction_label.init();
    this->self_attraction_slider.init();
    this->self_repulsion_label.init();
    this->self_repulsion_slider.init();
    
    int i = 0;
    this->other_interactions_labels.clear();;
    this->other_interactions_slider.clear();;
    for (std::vector<ParticleGroup>::iterator it = this->app.particle_groups.begin(); it != this->app.particle_groups.end(); ++it)
    {
        if (it->id == this->group->id)
            continue;
        this->other_interactions_labels.push_back(Label(app, "Group " + std::to_string(i) + " interaction", 10, 110 + (this->background.h + 5) * id + 25 * i));
        this->other_interactions_slider.push_back(Slider(app, 160, 110 + (this->background.h + 5) * id + 25 * i, 140, 10));
        
        this->other_interactions_slider.at(i).color = it->color;
        this->other_interactions_slider.at(i).centered = true;
        this->other_interactions_slider.at(i).progress = (this->group->other_attractions.at(i).force + 0.6) * 0.833;

        this->other_interactions_labels.at(i).init();
        this->other_interactions_slider.at(i).init();
        
        ++i;
    }
}

void    ParticleGroupUI::draw()
{
    SDL_Rect back = background;
    back.y += this->app.scroll;
    SDL_SetRenderDrawColor(this->app.renderer, 32, 32, 32, 255);
    SDL_RenderFillRect(this->app.renderer, &back);
    SDL_SetRenderDrawColor(this->app.renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(this->app.renderer, &back);
    this->group_title.draw();
    this->amount_label.draw();
    this->amount_slider.draw();
    this->self_attraction_label.draw();
    this->self_attraction_slider.draw();
    this->self_repulsion_label.draw();
    this->self_repulsion_slider.draw();

    this->group->amount = this->amount_slider.progress * 1500;
    this->group->self_attraction.force = -this->self_attraction_slider.progress;
    this->group->self_repulsion.force = this->self_repulsion_slider.progress;
    
    for (std::vector<Label>::iterator it = this->other_interactions_labels.begin(); it != this->other_interactions_labels.end(); ++it)
    {
        it->draw();
    }
    int i = 0;
    for (std::vector<Slider>::iterator it = this->other_interactions_slider.begin(); it != this->other_interactions_slider.end(); ++it)
    {
        it->draw();
        this->group->other_attractions.at(i).force = it->progress * 1.2f - 0.6f;
        ++i;
    }
    
}
