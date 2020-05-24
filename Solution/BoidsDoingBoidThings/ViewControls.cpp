#include "ViewControls.h"
#include "Engine.h"
#include "Entity.h"

void ViewControls::Update()
{
	// if we're holding the right mouse button, we can take input
	if (Engine::Instance().GetInput().GetMouseButton(GLFW_MOUSE_BUTTON_2))
	{
		double relYaw = (Engine::Instance().GetInput().GetMousePosX() - m_dLastMouseX) * m_dSensitivity;
		double relPitch = (Engine::Instance().GetInput().GetMousePosY() - m_dLastMouseY) * m_dSensitivity;
		glm::vec3 relRot(glm::radians(relPitch), glm::radians(relYaw), 0.0f);
		GetParentEntity()->Rotate(relRot);

		if (Engine::Instance().GetInput().GetKey(GLFW_KEY_W))
		{
			float scaledMoveSpeed = m_fMoveSpeed * Engine::Instance().GetDeltaTime();
			GetParentEntity()->Move(scaledMoveSpeed * GetParentEntity()->GetForward());
		}
		if (Engine::Instance().GetInput().GetKey(GLFW_KEY_S))
		{
			float scaledMoveSpeed = m_fMoveSpeed * Engine::Instance().GetDeltaTime();
			GetParentEntity()->Move(-(scaledMoveSpeed * GetParentEntity()->GetForward()));
		}
		if (Engine::Instance().GetInput().GetKey(GLFW_KEY_A))
		{
			float scaledMoveSpeed = m_fMoveSpeed * Engine::Instance().GetDeltaTime();
			GetParentEntity()->Move(-(scaledMoveSpeed * GetParentEntity()->GetRight()));
		}
		if (Engine::Instance().GetInput().GetKey(GLFW_KEY_D))
		{
			float scaledMoveSpeed = m_fMoveSpeed * Engine::Instance().GetDeltaTime();
			GetParentEntity()->Move(scaledMoveSpeed * GetParentEntity()->GetRight());
		}
	}

	m_dLastMouseX = Engine::Instance().GetInput().GetMousePosX();
	m_dLastMouseY = Engine::Instance().GetInput().GetMousePosY();
}