#define STB_IMAGE_IMPLEMENTATION

#include "util/Window.h"
#include "util/Time.h"
#include "input/InputSystem.h"
#include "util/Profiler.h"
#include "render/RenderingSystem.h"
#include "render/Camera.h"
#include "ui/EventSystem.h"
#include "game/states/RaceTurnState.h"
#include "game/GameManager.h"
#include "game/StateManager.h"
#include "input/PlayerController.h"
#include "phys/PhysicsSystem.h"

std::vector<glm::vec3> BSpline(std::vector<glm::vec3>& controlPoints)
{
	std::vector<glm::vec3> bspline;

	// Ensure their are enough points for interpolation.
	if (controlPoints.size() < 3)
	{
		return bspline;
	}

	// Constant values for interpolation.
	const float c0 = 3.0f / 4.0f;
	const float c1 = 1.0f / 4.0f;
	const float c2 = 1.0f / 2.0f;

	int degree = controlPoints.size();

	// Four smoothing operations.
	for (unsigned int u = 0; u < 4; u++)
	{
		// Get first two points.
		bspline.push_back(controlPoints[0]);
		bspline.push_back(c2 * controlPoints[0] + c2 * controlPoints[1]);

		// Interpolate mid points.
		for (int i = 1; i < degree - 2; i++)
		{
			bspline.push_back(c0 * controlPoints[i] + c1 * controlPoints[i + 1]);
			bspline.push_back(c1 * controlPoints[i] + c0 * controlPoints[i + 1]);
		}

		// Get the last two points.
		bspline.push_back(c2 * controlPoints[degree - 1] + c2 * controlPoints[degree - 2]);
		bspline.push_back(controlPoints[degree - 1]);

		// Update the list of control points for the next iteration.
		controlPoints = bspline;
		bspline.clear();
		degree = controlPoints.size();
	}

	return controlPoints;
}

std::vector<glm::vec3> GenerateRoadMesh(std::vector<glm::vec3>& controlPoints, float roadWidth)
{
	// Generate a B-spline from the provided control points.
	std::vector<glm::vec3> points = BSpline(controlPoints);
	
	std::vector<glm::vec3> roadMesh;

	// Ensure there are enough points to make a road segment.
	if (points.size() < 4)
	{
		return roadMesh;
	}

	// Assume normal vector is (0, 1, 0) - temporary
	glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);

	// Calculate tangent and bitangent vectors for each point.
	std::vector<glm::vec3> tangents = std::vector<glm::vec3>();
	std::vector<glm::vec3> bitangents = std::vector<glm::vec3>();
	for (unsigned int i = 0; i < points.size() - 1; i++)
	{
		glm::vec3 p0 = points[i];
		glm::vec3 p1 = points[i + 1];

		// Calculate tangent using direction vector between adjacent points.
		glm::vec3 tangent = glm::normalize(p1 - p0);
		tangents.emplace_back(tangent);
		
		// Calculate bitangent using normal and tangent vectors.
		glm::vec3 bitangent = glm::normalize(glm::cross(normal, tangent));
		bitangents.emplace_back(bitangent);

		// Calculate points lying on sides of the road width.
		glm::vec3 p2 = points[i] + bitangents[i] * roadWidth;
		
		roadMesh.emplace_back(p0);
		roadMesh.emplace_back(p2);
	}

	roadMesh.emplace_back(points[points.size() - 1]);

	return roadMesh;
}

int main()
{
	Window* myWindow = new Window();
	if (!myWindow->Initialize()) return -1;

	InputSystem* inputSystem = InputSystem::GetInstance();

	Profiler* profiler = new Profiler();
	RenderingSystem* renderingSystem = new RenderingSystem(myWindow);

	inputSystem->Initialize(myWindow, renderingSystem);

	Time* time = Time::GetInstance();

	PlayerController* playerController = new PlayerController();
	inputSystem->playerController = playerController;

	PhysicsSystem* physicsSystem = new PhysicsSystem();
	physicsSystem->SetEnvironmentMesh(renderingSystem->peachCastle);
	physicsSystem->SetPlayerCharacterTransform(renderingSystem->goblinTransform);

	// ----------------------- EVENT SYSTEM TESTING ---------------------------
	EventSystem* eventSystem = new EventSystem();
	GameObject gameObject;

	eventSystem->Subscribe(&gameObject);

	std::string eventName = "EventTest";
	int enemyData = 42;
	eventSystem->Notify(eventName, &enemyData);

	// ----------------------- Road generation test ---------------------------
	std::vector<glm::vec3> controlPoints;
	controlPoints.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	controlPoints.push_back(glm::vec3(0.0f, 0.0f, 5.0f));
	controlPoints.push_back(glm::vec3(0.0f, 0.0f, 10.0f));
	controlPoints.push_back(glm::vec3(5.0f, 0.0f, 15.0f));
	controlPoints.push_back(glm::vec3(10.0f, 0.0f, 15.0f));
	controlPoints.push_back(glm::vec3(15.0f, 0.0f, 10.0f));
	controlPoints.push_back(glm::vec3(15.0f, 0.0f, 5.0f));
	controlPoints.push_back(glm::vec3(15.0f, 0.0f, 0.0f));
	controlPoints.push_back(glm::vec3(15.0f, 0.0f, -5.0f));
	controlPoints.push_back(glm::vec3(15.0f, 0.0f, -10.0f));
	controlPoints.push_back(glm::vec3(10.0f, 0.0f, -15.0f));
	controlPoints.push_back(glm::vec3(5.0f, 0.0f, -15.0f));
	controlPoints.push_back(glm::vec3(0.0f, 0.0f, -10.0f));
	controlPoints.push_back(glm::vec3(0.0f, 0.0f, -5.0f));
	controlPoints.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	
	std::vector<glm::vec3> roadMesh = GenerateRoadMesh(controlPoints, 3.0f);

	/*
	std::cout << "x,z" << std::endl;
	for (unsigned int i = 0; i < roadMesh.size(); i++)
	{
		std::cout << roadMesh[i].x << "," << roadMesh[i].z << std::endl;
	}
	*/

	// ------------------------ GAME STATE TESTING ----------------------------

	GameManager gameManager;
	StateManager stateManager;
	//GameState* testState = new RaceTurnState(&gameManager, &stateManager);
	//std::cout << testState->name << std::endl;
	//testState->name = "Race Turn State";
	//std::cout << testState->name << std::endl;
	//stateManager.Initialize(testState);

	// ------------------------------ GAME LOOP -------------------------------
	while (!myWindow->ShouldClose())
	{
		// Update global application time.
		time->Update();

		// Track FPS.
		profiler->Update();

		// Process inputs.
		inputSystem->Update();

		// Physics.
		physicsSystem->Update();

		// Render.
		renderingSystem->Update();
	}

	delete(physicsSystem);
	delete(playerController);
	delete(time);
	delete(eventSystem);
	delete(renderingSystem);
	delete(profiler);
	delete(myWindow);

	//delete(testState);

	return 0;
}