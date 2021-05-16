#pragma once
#include <glm/glm.hpp>

struct GLFWwindow;

namespace awesome {
	class Camera
	{
	public:
		void Tick(double deltaTimeSec, GLFWwindow* renderingContext);
		inline glm::mat4 const & GetViewMatrix() { return ViewMatrix; }
		
		void UpdateRotation(float pitchDelta, float yawDelta);
	private:
		void ProcessInput(double deltaTimeSec, GLFWwindow* renderingContext);		

		const float CameraMoveSpeed = 0.005f; /* NDC/sec */	
		
		glm::vec3 Position{0.0f, 0.0f, 2.0f};
		float Pitch{ 0.0f };
		float Yaw{ -90.0f };

		/* Camera space */
		glm::vec3 Forward{-Position};
		glm::vec3 Right{};
		glm::vec3 Up{};

		glm::mat4 ViewMatrix{ glm::mat4(1.0f) };
	};
}
