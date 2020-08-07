#pragma once
#ifndef H_CAMERA
#define H_CAMERA
#include "Component.h"

class Camera : public Component
{
public:
	void Update() override;
};

#endif