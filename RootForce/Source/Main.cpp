#include <Main.h>
#include <stdexcept>
#include <exception>
#include <gtest/gtest.h>
#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>

#include <Utility/ECS/Include/Shared/RenderingSystem.h>
#include <Utility/ECS/Include/Shared/LightSystem.h>
#include <CameraSystem.h>
#include <PhysicsSystem.h>


#include <glm/glm.hpp>

#include <RootForce/Include/ComponentExporter.h>
#include <RootForce/Include/ComponentImporter.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#undef main

TEST(Test, Foo) 
{
	int a = 0;
	EXPECT_TRUE(a == 0);
}

int main(int argc, char* argv[]) 
{
	std::string path(argv[0]);
	std::string rootforcename = "Rootforce.exe";
	path = path.substr(0, path.size() - rootforcename.size());
	try 
	{
		if (argc > 1 && strcmp(argv[1], "-test") == 0)
		{
			testing::InitGoogleTest(&argc, argv);

			int result = RUN_ALL_TESTS();
			std::cin.get();
			return result;
		}
		else
		{
			Main m(path);
			m.Start();
		}
	} 
	catch (std::exception& e) 
	{
		// TODO: Log exception message
		std::cout << e.what() << "\n";
		std::cin.get();
		return 1;
	} 
	catch (...) 
	{
		// TODO: Log unknown exception message
		std::cin.get();
		return 1;
	}
	
	return 0;
}

Main::Main(std::string p_workingDirectory) 
	: m_running(true), m_world(6)
{
	m_engineModule = DynamicLoader::LoadSharedLibrary("RootEngine.dll");

	INITIALIZEENGINE libInitializeEngine = (INITIALIZEENGINE)DynamicLoader::LoadProcess(m_engineModule, "InitializeEngine");

	g_engineContext = libInitializeEngine(RootEngine::SubsystemInit::INIT_ALL, p_workingDirectory);

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0) 
	{
		// TODO: Log error and throw exception (?)
	}

	// TODO: Make these parameters more configurable.
	m_window = std::shared_ptr<SDL_Window>(SDL_CreateWindow(
			"Root Force",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN),
		SDL_DestroyWindow);
	if (m_window == nullptr) 
	{
		// TODO: Log error and throw exception (?)
	}
}

Main::~Main() 
{
	m_world.GetEntityExporter()->Export(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets\\Levels\\test_2.world");
	SDL_Quit();
	DynamicLoader::FreeSharedLibrary(m_engineModule);
}

void Main::Start() 
{
	g_engineContext.m_renderer->SetupSDLContext(m_window.get());

	RootForce::Renderable::SetTypeId(0);
	RootForce::Transform::SetTypeId(1);
	RootForce::PointLight::SetTypeId(2);
	RootForce::PlayerControl::SetTypeId(3);
	RootForce::PhysicsAccessor::SetTypeId(4);
	RootForce::Camera::SetTypeId(5);
	RootForce::LookAtBehavior::SetTypeId(6);
	RootForce::ThirdPersonBehavior::SetTypeId(7);

	// Initialize the system for controlling the player.
	std::vector<RootForce::Keybinding> keybindings(4);
	keybindings[0].Bindings.push_back(SDL_SCANCODE_UP);
	keybindings[0].Bindings.push_back(SDL_SCANCODE_W);
	keybindings[0].Action = RootForce::PlayerAction::MOVE_FORWARDS;


	keybindings[1].Bindings.push_back(SDL_SCANCODE_DOWN);
	keybindings[1].Bindings.push_back(SDL_SCANCODE_S);
	keybindings[1].Action = RootForce::PlayerAction::MOVE_BACKWARDS;

	keybindings[2].Bindings.push_back(SDL_SCANCODE_LEFT);
	keybindings[2].Bindings.push_back(SDL_SCANCODE_A);
	keybindings[2].Action = RootForce::PlayerAction::STRAFE_LEFT;

	keybindings[3].Bindings.push_back(SDL_SCANCODE_RIGHT);
	keybindings[3].Bindings.push_back(SDL_SCANCODE_D);
	keybindings[3].Action = RootForce::PlayerAction::STRAFE_RIGHT;

	m_PlayerControlSystem = std::shared_ptr<RootForce::PlayerControlSystem>(new RootForce::PlayerControlSystem(&m_world));
	m_PlayerControlSystem->SetInputInterface(g_engineContext.m_inputSys);
	m_PlayerControlSystem->SetLoggingInterface(g_engineContext.m_logger);
	m_PlayerControlSystem->SetKeybindings(keybindings);
	m_PlayerControlSystem->SetPhysicsInterface(g_engineContext.m_physics);

	// Initialize physics system
	RootForce::PhysicsSystem* physicsSystem = new RootForce::PhysicsSystem(&m_world);
	physicsSystem->SetPhysicsInterface(g_engineContext.m_physics);
	physicsSystem->SetLoggingInterface(g_engineContext.m_logger);
	m_world.GetSystemManager()->AddSystem<RootForce::PhysicsSystem>(physicsSystem, "PhysicsSystem");



	m_world.GetEntityImporter()->SetImporter(Importer);
	m_world.GetEntityExporter()->SetExporter(Exporter);


	// Initialize render and point light system.
	RootForce::RenderingSystem* renderingSystem = new RootForce::RenderingSystem(&m_world);
	m_world.GetSystemManager()->AddSystem<RootForce::RenderingSystem>(renderingSystem, "RenderingSystem");

	renderingSystem->SetLoggingInterface(g_engineContext.m_logger);
	renderingSystem->SetRendererInterface(g_engineContext.m_renderer);

	Render::DirectionalLight dl;
	dl.m_color = glm::vec4(1,0,1,1);
	dl.m_direction = glm::vec3(0,0,-1);

	RootForce::CameraSystem* cameraSystem = new RootForce::CameraSystem(&m_world);
	m_world.GetSystemManager()->AddSystem<RootForce::CameraSystem>(cameraSystem, "CameraSystem");

	RootForce::LookAtSystem* lookAtSystem = new RootForce::LookAtSystem(&m_world);
	m_world.GetSystemManager()->AddSystem<RootForce::LookAtSystem>(lookAtSystem, "LookAtSystem");

	RootForce::ThirdPersonBehaviorSystem* thirdPersonBehaviorSystem = new RootForce::ThirdPersonBehaviorSystem(&m_world);
	m_world.GetSystemManager()->AddSystem<RootForce::ThirdPersonBehaviorSystem>(thirdPersonBehaviorSystem, "ThirdPersonBehaviorSystem");

	// Setup lights.
	g_engineContext.m_renderer->SetAmbientLight(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

	Render::DirectionalLight directional;
	directional.m_color = glm::vec4(0.2f,0.2f,0.1f,1);
	directional.m_direction = glm::vec3(1, 0, 0);

	g_engineContext.m_renderer->AddDirectionalLight(directional, 0);
	
	//////////////////////////////////////////////////////////////////////////
	//CREATE RED LIGHT
	//////////////////////////////////////////////////////////////////////////
	ECS::Entity* red = m_world.GetEntityManager()->CreateEntity();

	RootForce::Transform* redTrans = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(red);
	redTrans->m_position = glm::vec3(1.0f, 3.0f, 0.0f);
	redTrans->m_scale = glm::vec3(0.1f);

	RootForce::PointLight* redPL = m_world.GetEntityManager()->CreateComponent<RootForce::PointLight>(red);
	redPL->m_color = glm::vec4(0.4f, 0.0f, 0.0f, 1.0f);
	redPL->m_attenuation = glm::vec3(0.0f, 0.0f, 1.0f);
	redPL->m_range = 2.0f;

	g_engineContext.m_renderer->AddDirectionalLight(dl, 0);

	RootForce::PointLightSystem* pointLightSystem = new RootForce::PointLightSystem(&m_world, g_engineContext.m_renderer);
	m_world.GetSystemManager()->AddSystem<RootForce::PointLightSystem>(pointLightSystem, "PointLightSystem");


	RootForce::AbilitySystem* abilitySystem = new RootForce::AbilitySystem(&m_world, g_engineContext.m_renderer);
	m_world.GetSystemManager()->AddSystem<RootForce::AbilitySystem>(abilitySystem, "AbilitySystem");

	//////////////////////////////////////////////////////////////////////////
	//CREATE BLUE LIGHT
	//////////////////////////////////////////////////////////////////////////
	ECS::Entity* blue = m_world.GetEntityManager()->CreateEntity();

	// Import test world.
	m_world.GetEntityImporter()->Import(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets\\Levels\\test_2.world");


	//RootForce::PhysicsAccessor* physaccessor = m_world.GetEntityManager()->CreateComponent<RootForce::PhysicsAccessor>(m_world.GetTagManager()->GetEntityByTag("Player"));
	//int temp[1] = {0};
	//physaccessor->m_handle = temp;
	


	g_engineContext.m_gui->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	g_engineContext.m_gui->LoadURL("debug.html");
	g_engineContext.m_debugOverlay->SetView(g_engineContext.m_gui->GetView());


	m_world.GetGroupManager()->RegisterEntity("Lights", blue);
	m_world.GetGroupManager()->RegisterEntity("Lights", red);

	//////////////////////////////////////////////////////////////////////////
	//CREATE CAMERA
	//////////////////////////////////////////////////////////////////////////

	ECS::Entity* cameraEntity = m_world.GetEntityManager()->CreateEntity();
	RootForce::Transform* cameraTransform = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(cameraEntity);
	cameraTransform->m_position = glm::vec3(0.0f, 0.0f, 10.0f);
	RootForce::Camera* camera = m_world.GetEntityManager()->CreateComponent<RootForce::Camera>(cameraEntity);
	camera->m_near = 0.1f;
	camera->m_far = 1000.0f;
	camera->m_fov = 75.0f;
	m_world.GetTagManager()->RegisterEntity("Camera", cameraEntity);
	RootForce::LookAtBehavior* cameraLookAt = m_world.GetEntityManager()->CreateComponent<RootForce::LookAtBehavior>(cameraEntity);
	cameraLookAt->m_targetTag = "Player";
	RootForce::ThirdPersonBehavior* cameraThirdPersonBehavior = m_world.GetEntityManager()->CreateComponent<RootForce::ThirdPersonBehavior>(cameraEntity);
	cameraThirdPersonBehavior->m_targetTag = "Player";
	cameraThirdPersonBehavior->m_displacement = glm::vec3(0.0f, 4.0f, -8.0f);

	//Plane at bottom
	float normal[3] = {0,1,0};
	float position[3] = {0, -2, 0};
	float dir[3] = {0,0,-1};
	float grav[3] = {0,0,0};
	float pos[3] = {0,1,-1};
	float rot[4] = {0,-1,1,-1};
	g_engineContext.m_physics->CreatePlane(normal, position);
	RootEngine::Physics::AbilityPhysicsInfo test;
	test.m_collidesWorld = true;
	test.m_direction = dir;
	test.m_entityId = -1;
	test.m_gravity = grav;
	test.m_height = 1;
	test.m_mass = 1;
	test.m_position = pos;
	test.m_radius = 1;
	test.m_orientation = rot;
	test.m_shape = RootEngine::Physics::AbilityShape::SHAPE_CONE;
	test.m_speed = 1;
	test.m_type = RootEngine::Physics::PhysicsType::TYPE_ABILITY;
	g_engineContext.m_physics->AddAbilityToWorld(test);

	m_world.GetGroupManager()->UnregisterEntity("Lights", red);

	m_world.GetGroupManager()->PrintEntitiesInGroup("Lights");

	m_world.GetGroupManager()->PrintEntitiesInGroup("Camera");

	RootForce::Transform* t = m_world.GetEntityManager()->GetComponent<RootForce::Transform>(m_world.GetTagManager()->GetEntityByTag("Player"));
	glm::vec3 a = t->m_position;
	// Start the main loop
	uint64_t old = SDL_GetPerformanceCounter();
	while (m_running)
	{
		uint64_t now = SDL_GetPerformanceCounter();
		float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
		old = now;
	
		g_engineContext.m_debugOverlay->Clear();

		m_world.SetDelta(dt);
		g_engineContext.m_debugOverlay->AddHTML(std::to_string(dt).c_str(), RootEngine::TextColor::GRAY, false);
		HandleEvents();
		
		m_PlayerControlSystem->Process();
		abilitySystem->Process();

		g_engineContext.m_renderer->Clear();
		g_engineContext.m_physics->Update(dt);

		// Update Game systems.
		pointLightSystem->Process();
		physicsSystem->Process();
		thirdPersonBehaviorSystem->Process();
		lookAtSystem->Process();
		renderingSystem->Process();
		cameraSystem->Process();

		// Update Engine systems.
		g_engineContext.m_renderer->Render();
		//g_engineContext.m_renderer->RenderLines();

		g_engineContext.m_gui->Update();
		g_engineContext.m_gui->Render();

		g_engineContext.m_renderer->Swap();
	}
}

void Main::HandleEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type) 
		{
		case SDL_QUIT: 
			m_running = false;
			break;

		default:
			if (g_engineContext.m_inputSys != nullptr)
				g_engineContext.m_inputSys->HandleInput(event);
			if(g_engineContext.m_gui != nullptr)
				g_engineContext.m_gui->HandleEvents(event);
		}
	}
}
