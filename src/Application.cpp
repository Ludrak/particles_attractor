
#include "Random.hpp"

#include "Application.hpp"
#include "UI.hpp"
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>



static void	generate_interactions(std::vector<ParticleGroup>& groups)
{
	int i = 0;
	for (std::vector<ParticleGroup>::iterator it = groups.begin(); it < groups.end(); ++it)
	{

		it->self_attraction.force = -randFloat(0, 0.6f);
		it->self_repulsion.force = randFloat(0, 0.6f);

	//	std::cout << "group " << i << " self attraction: " << it->self_attraction.force << std::endl;
	//	std::cout << "group " << i << " self repulsion: " << it->self_repulsion.force << std::endl;
		int j = 0;
		for (std::vector<ParticleGroup>::iterator others = groups.begin(); others < groups.end(); ++others)
		{
			it->other_attractions.push_back(Interaction(*others, randFloat(-0.6f, 0.6f)));
	//		std::cout << "group " << i << " -> " << j << " interaction: " << it->other_attractions.rbegin()->force << std::endl;
			j++;
		}
		i++;
	}
}


Application::Application()
: 
  mouse_x(0),
  mouse_y(0),
  pmouse_x(0),
  pmouse_y(0),
  mousePressed(false),

  reset_button(*this, 5, 955, 300, 28, "Reset Init"),
  shuffle_button(*this, 5, 925, 300, 28, "Shuffle Params"),

  zoom(0.4f),
  x_pos(600),
  y_pos(500)
{
    SDL_Init(SDL_INIT_VIDEO);

    this->window = SDL_CreateWindow(
		"Life",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		1200,
		1000,
		SDL_WINDOW_OPENGL
	);

    if (this->window == nullptr)
	{
		SDL_Log("Unable to create window : %s", SDL_GetError());
		return ;
	}

    this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (this->renderer == nullptr)
	{
		SDL_Log("Unable to create renderer : %s", SDL_GetError());
		return ;
	}


//	this->particle_groups = std::vector<ParticleGroup>();
	this->particle_groups.push_back(ParticleGroup(this->renderer, 400, Color(20, 255, 100)));
	this->particle_groups.push_back(ParticleGroup(this->renderer, 400, Color(70, 200, 255)));
	this->particle_groups.push_back(ParticleGroup(this->renderer, 400, Color(255, 100, 20)));
	this->particle_groups.push_back(ParticleGroup(this->renderer, 400, Color(255, 255, 255)));
	//generate_interactions(this->particle_groups);
	//this->particle_groups.at(0).self_attraction.force = 0.3;
//	this->particle_groups.at(0).self_repulsion.force = -0.3;
	generate_interactions(this->particle_groups);

	this->font = TTF_OpenFont("res/font/Monaco.ttf", 12);

	this->reset_button.init();
	this->shuffle_button.init();// = Button(*this, 5, 45, 200, 28, "Shuffle Params");

	int i = 0;
	for (std::vector<ParticleGroup>::iterator it = this->particle_groups.begin(); it != this->particle_groups.end(); ++it)
	{
		this->group_ui.push_back(ParticleGroupUI(*this, i));
		this->group_ui.at(i).init();
		++i;
	}
}

void    Application::run()
{
    while (1)
	{
		SDL_Event	event;

		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				SDL_Log("Closing window : quit event recieved.");
				break;
			}

			if (event.type == SDL_MOUSEWHEEL)
			{
				if (event.wheel.y > 0)
				{
					if (this->zoom > 0.26f)
        			this->zoom *= 0.90;
				}
				else 
				{
					if (this->zoom < 3.5f)
						this->zoom *= 1.1;
				}

				std::cout << zoom << std::endl;
			}

			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				this->mousePressed = true;
			}

			if (event.type == SDL_MOUSEBUTTONUP)
			{
				this->mousePressed = false;
			}
		}

		int x, y;
		SDL_GetMouseState(&x, &y);
		this->pmouse_x = this->mouse_x;
		this->pmouse_y = this->mouse_y;
		this->mouse_x = x;
		this->mouse_y = y;

		if (this->mousePressed && this->mouse_x > 300)
		{
			this->x_pos += this->mouse_x - this->pmouse_x;
			this->y_pos += this->mouse_y - this->pmouse_y;
		}

        this->update();
		//usleep(200);
        SDL_RenderPresent(this->renderer);
    }
}
#include <iostream>
void    Application::update()
{
	SDL_Rect background;
	background.x = 0;
	background.y = 0;
	background.w = 1200;
	background.h = 1000;
	SDL_SetRenderDrawColor(this->renderer, 12, 12, 12, 255);
	SDL_RenderFillRect(this->renderer, &background);
    for (std::vector<ParticleGroup>::iterator it = this->particle_groups.begin(); it < this->particle_groups.end(); ++it)
	{
		it->update_particles();
		it->draw_particles(this->zoom, this->x_pos, this->y_pos);
	}

	this->reset_button.draw();
	this->shuffle_button.draw();

	for (std::vector<ParticleGroupUI>::iterator it = this->group_ui.begin(); it != this->group_ui.end(); ++it)
	{
		it->draw();
	}

	if (this->reset_button.toggled)
	{
		for (std::vector<ParticleGroup>::iterator it = this->particle_groups.begin(); it != this->particle_groups.end(); ++it)
		{
			it->shuffle_values();
		}
	
		return ;
	}

	if (this->shuffle_button.toggled)
	{
		for (std::vector<ParticleGroup>::iterator it = this->particle_groups.begin(); it != this->particle_groups.end(); ++it)
		{
			it->init_particles();
			generate_interactions(this->particle_groups);
		} 
		for (std::vector<ParticleGroupUI>::iterator it = this->group_ui.begin(); it != this->group_ui.end(); ++it)
		{
			it->init();
		} 
		return ;
	}
}


