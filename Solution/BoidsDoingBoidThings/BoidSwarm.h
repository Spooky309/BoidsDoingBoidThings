#pragma once
#ifndef H_BOIDSWARM
#define H_BOIDSWARM
#include "Component.h"

// The position of the parent entity of this component acts as the "target" for the boids

class BoidSwarm : public Component
{
public:
	void Update() override;
private:
	unsigned int m_iNumBoids = 100;

};

#endif