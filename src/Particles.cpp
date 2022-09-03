
#include "Random.hpp"

#include "Particles.hpp"
#include "Constants.hpp"
#include <cstdlib>
#include <ctime>

#include <random>

#include <iostream>

static void  draw_circle(SDL_Renderer *renderer, float x, float y, float zoom = 0.4f, float off_x = 1500, float off_y = 1200)
{
    SDL_Rect rect;
    
    rect.x = (x - 4) * zoom + off_x;
    rect.y = (y - 4) * zoom + off_y;
    rect.w = 8 * zoom;
    rect.h = 8 * zoom;
    SDL_RenderFillRect(renderer, &rect);
    //SDL_RenderDrawPoint(renderer, x * zoom + off_x, y * zoom + off_y);
    /*
    for (int w = 0; w < r * 2; w++)
    {
        for (int h = 0; h < r * 2; h++)
        {
            int dx = r - w;
            int dy = r - h;
            if ((dx*dx + dy*dy) <= (r * r))
            {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }*/
}





void    ParticleGroup::init_particles()
{
    this->other_attractions.clear();
    this->particles.clear();
    for (int i = 0; i < amount; ++i)
        this->particles.push_back(Particle());
    this->shuffle_values();
}

void    ParticleGroup::shuffle_values()
{
    for (std::vector<Particle>::iterator it = this->particles.begin(); it < this->particles.end(); ++it)
    {
        it->x = randFloat(-SPACE_BOUNDARY, SPACE_BOUNDARY);
        it->y = randFloat(-SPACE_BOUNDARY, SPACE_BOUNDARY);
        it->vel_x = 0;
        it->vel_y = 0;
    }
}


void    ParticleGroup::draw_particles(float zoom, float x, float y)
{
    SDL_SetRenderDrawColor(renderer, this->color.r, this->color.g, this->color.b, this->color.a);
    for (std::vector<Particle>::iterator it = this->particles.begin(); it < this->particles.end(); ++it)
    {
        draw_circle(this->renderer, it->x, it->y, zoom, x, y);
    }
}

void    ParticleGroup::update_particles()
{
    while (this->particles.size() < (size_t)amount)
        this->particles.push_back(Particle());
    while (this->particles.size() > (size_t)amount)
        this->particles.erase(this->particles.begin());
    this->self_attraction.compute(*this);
    this->self_repulsion.compute(*this);

    for (std::vector<Interaction>::iterator it = this->other_attractions.begin(); it != this->other_attractions.end(); ++it)
    {
        it->compute(*this);
    }
}


void    Interaction::compute(ParticleGroup& effect_group)
{
    for (std::vector<Particle>::iterator p_1 = effect_group.particles.begin(); p_1 != effect_group.particles.end(); ++p_1)
    {
        float force_x = 0;
        float force_y = 0;

        // sum forces of all other particles
        for (std::vector<Particle>::const_iterator p_2 = this->group.getParticles().cbegin(); p_2 != this->group.getParticles().cend(); p_2++)
        {
            float dx = p_1->x - p_2->x;
            float dy = p_1->y - p_2->y;
            float d = sqrtf(dx*dx + dy*dy);

            if (d > 0 && d < LOCAL_VISION)
            {
                float f = this->force * (1.0f / (d));
                force_x += (f * dx);
                force_y += (f * dy);
            }
        }

        p_1->vel_x += force_x;
        p_1->vel_y += force_y;


        /* squared constraints */
        /*
        if (p_1->vel_x > SPEED_OF_LIGHT)
            p_1->vel_x = SPEED_OF_LIGHT;
        if (p_1->vel_x < -SPEED_OF_LIGHT)
            p_1->vel_x = -SPEED_OF_LIGHT;


        if (p_1->vel_y > SPEED_OF_LIGHT)
            p_1->vel_y = SPEED_OF_LIGHT;
        if (p_1->vel_y < -SPEED_OF_LIGHT)
            p_1->vel_y = -SPEED_OF_LIGHT;
        */

       /* rounded constraints */
       
        float mag = sqrtf(p_1->vel_x*p_1->vel_x + p_1->vel_y*p_1->vel_y);
        if (mag > SPEED_OF_LIGHT)
        {
            p_1->vel_x /= mag;
            p_1->vel_y /= mag;
            p_1->vel_x *= SPEED_OF_LIGHT;
            p_1->vel_y *= SPEED_OF_LIGHT;
        }

        p_1->x += p_1->vel_x * 0.1;
        p_1->y += p_1->vel_y * 0.1;

        if (p_1->x < -SPACE_BOUNDARY || p_1->x > SPACE_BOUNDARY) {
             p_1->vel_x *= -1;
             p_1->x = (p_1->x < -SPACE_BOUNDARY) ? -SPACE_BOUNDARY : SPACE_BOUNDARY;
        }
        if (p_1->y < -SPACE_BOUNDARY || p_1->y > SPACE_BOUNDARY) {
             p_1->vel_y *= -1;
             p_1->y = (p_1->y < -SPACE_BOUNDARY) ? -SPACE_BOUNDARY : SPACE_BOUNDARY;
        }
    }
}

