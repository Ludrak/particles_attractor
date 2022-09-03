#include "Random.hpp"

float randFloat(float min, float max)
{
    srand(time(NULL));
	
    return ((static_cast<float>(arc4random()) / (static_cast<float>(RAND_MAX) * 2)) * (max - min) + min);
}
