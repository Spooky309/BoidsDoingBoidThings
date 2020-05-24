#pragma once
#ifndef H_MESHREND_
#define H_MESHREND_
#include "Component.h"

class MeshRenderer : public Component
{
public:
	void Update() override;
};

#endif