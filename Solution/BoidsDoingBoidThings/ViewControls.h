#pragma once
#ifndef H_VIEWCONTROLS
#define H_VIEWCONTROLS
#include "Component.h"

class ViewControls : public Component
{
public:
	void Update() override;
private:
	double m_dLastMouseX;
	double m_dLastMouseY;
	double m_dSensitivity = 0.5;
	float m_fMoveSpeed = 50.0f;
};

#endif