
#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include "Constants.hpp"
#include "Random.hpp"
#include <string>

struct Color
{
    int r;
    int g;
    int b;
    int a;

    Color(int r, int g, int b, int a = 255)
        : r(r), g(g), b(b), a(a)
        {}
    
    Color(const Color& c)
        : r(c.r), g(c.g), b(c.b), a(c.a)
        {}
};

struct Particle
{
    float x;
    float y;
    float vel_x;
    float vel_y;

    Particle(float x = randFloat(-SPACE_BOUNDARY, SPACE_BOUNDARY), float y = randFloat(-SPACE_BOUNDARY, SPACE_BOUNDARY))
    : x(x), y(y), vel_x(0), vel_y(0)
    {}
};




class ParticleGroup;

struct Interaction
{
    const ParticleGroup&    group;
    float                   force;

    public:
        Interaction(const ParticleGroup& group, const float force)
            : group(group), force(force)
            {}
        
        Interaction(const Interaction& i)
            : group(i.group), force(i.force)
            {}

        void    compute(ParticleGroup& effect_group);
};





class ParticleGroup
{
    public:
        ParticleGroup(SDL_Renderer* renderer, int id, int amount, Color color, const std::string& name)
            : id(id), name(name), renderer(renderer), amount(amount), color(color), self_attraction(*this, 0), self_repulsion(*this, 0)
            {
                srand(time(NULL));
                init_particles();
            }
        
        ParticleGroup(const ParticleGroup& p)
            : id(p.id), name(p.name), renderer(p.renderer), particles(p.particles), amount(p.amount), color(p.color), self_attraction(*this, p.self_attraction.force), self_repulsion(*this, p.self_repulsion.force), other_attractions()
            {
                srand(time(NULL));
                for (std::vector<Interaction>::const_iterator it = p.other_attractions.cbegin(); it < p.other_attractions.cend(); ++it)
                {
                    this->other_attractions.push_back(Interaction(*it));
                }
            }

        void    init_particles();
        void    shuffle_values();

        void    draw_particles(float zoom, float x, float y);

        void    update_particles();

        const std::vector<Particle>&    getParticles() const { return (this->particles); }
    
    public:
        int                     id;
        std::string             name;
        SDL_Renderer            *renderer;

        std::vector<Particle>   particles;
        int                     amount;
        Color                   color;

        Interaction                 self_attraction;
        Interaction                 self_repulsion;
        std::vector<Interaction>    other_attractions;
};