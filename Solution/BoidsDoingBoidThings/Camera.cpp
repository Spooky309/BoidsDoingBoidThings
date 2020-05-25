#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Entity.h"
#include "Engine.h"

void Camera::Update()
{
	glm::mat4 view = glm::lookAt(GetParentEntity()->GetPosition(), GetParentEntity()->GetPosition() + GetParentEntity()->GetForward(), glm::vec3(0.0f, 1.0f, 0.0f));
	Engine::Instance().GetRenderer3D().UpdateViewMatrix(view);
}