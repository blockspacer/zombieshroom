#include <Main.h>

#include <stdexcept>

#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Render/Include/Vertex.h>


#include <gtest/gtest.h>

#include <RootForce/Include/RawMeshPrimitives.h>
#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>

#include <RenderingSystem.h>
#include <PlayerControlSystem.h>

#include <exception>

#include <glm/glm.hpp>

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
	: m_running(true)
{

	m_engineModule = DynamicLoader::LoadSharedLibrary("RootEngine.dll");

	INITIALIZEENGINE libInitializeEngine = (INITIALIZEENGINE)DynamicLoader::LoadProcess(m_engineModule, "InitializeEngine");

	m_engineContext = libInitializeEngine(RootEngine::SubsystemInit::INIT_ALL, p_workingDirectory);

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0) 
	{
		// TODO: Log error and throw exception (?)
	}

	// TODO: Make these parameters more configurable.
	m_window = std::shared_ptr<SDL_Window>(SDL_CreateWindow(
			"Root Force",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			1280,
			720,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN),
		SDL_DestroyWindow);
	if (m_window == nullptr) 
	{
		// TODO: Log error and throw exception (?)
	}

}

Main::~Main() 
{
	SDL_Quit();
	DynamicLoader::FreeSharedLibrary(m_engineModule);
}

void Main::Start() 
{
	m_engineContext.m_renderer->SetupSDLContext(m_window.get());
	m_engineContext.m_resourceManager->LoadEffect("test");
	m_engineContext.m_resourceManager->LoadEffect("DiffuseTexture");
	m_engineContext.m_resourceManager->LoadEffect("2D_GUI");
	m_engineContext.m_resourceManager->LoadCollada("testchar");

	m_engineContext.m_gui->Initalize(1280, 720);
	std::shared_ptr<Rocket::Core::ElementDocument> document = m_engineContext.m_gui->AttachDocument("Assets//GUI//demo.rml");
	std::shared_ptr<Rocket::Core::ElementDocument> debugdoc = m_engineContext.m_gui->AttachDocument("Assets//GUI//debug.rml");

	Render::Uniforms uniforms;
	uniforms.m_normal = glm::mat4(1);
	uniforms.m_world = glm::mat4(1);
	
	m_engineContext.m_gui->SetEffect( m_engineContext.m_resourceManager->GetEffect("2D_GUI"));

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

	ECS::ComponentSystem* playerControlSystem = m_world.GetSystemManager()->CreateSystem<RootForce::PlayerControlSystem>("PlayerControlSystem");
	dynamic_cast<RootForce::PlayerControlSystem*>(playerControlSystem)->SetInputInterface(m_engineContext.m_inputSys);
	dynamic_cast<RootForce::PlayerControlSystem*>(playerControlSystem)->SetLoggingInterface(m_engineContext.m_logger);
	dynamic_cast<RootForce::PlayerControlSystem*>(playerControlSystem)->SetKeybindings(keybindings);

	// Initialize the system for rendering the scene.
	ECS::ComponentSystem* renderingSystem = m_world.GetSystemManager()->CreateSystem<RootForce::RenderingSystem>("RenderingSystem");
	reinterpret_cast<RootForce::RenderingSystem*>(renderingSystem)->SetLoggingInterface(m_engineContext.m_logger);
	reinterpret_cast<RootForce::RenderingSystem*>(renderingSystem)->SetRendererInterface(m_engineContext.m_renderer);

	m_world.GetSystemManager()->InitializeSystems();

	

	// Setup a dummy player entity and add components to it
	ECS::Entity* guy = m_world.GetEntityManager()->CreateEntity();

	RootForce::Transform* guyTransform = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(guy);
	guyTransform->m_position = glm::vec3(0.0f, 0.0f, 0.0f);

	Utility::Cube quad(Render::VertexType::VERTEXTYPE_1P);

	Render::MeshInterface* mesh = m_engineContext.m_renderer->CreateMesh();
	mesh->Init(reinterpret_cast<Render::Vertex1P*>(quad.m_vertices), quad.m_numberOfVertices, quad.m_indices, quad.m_numberOfIndices);

	RootForce::Renderable* guyRenderable = m_world.GetEntityManager()->CreateComponent<RootForce::Renderable>(guy);
	guyRenderable->m_mesh = m_engineContext.m_resourceManager->GetModel("testchar")->m_meshes[0];
	//guyRenderable->m_mesh = mesh;

	Render::Material guyMaterial;
	//guyMaterial.m_effect = m_engineContext.m_resourceManager->GetEffect("DiffuseTexture");
	guyMaterial.m_effect = m_engineContext.m_resourceManager->GetEffect("test");
	guyRenderable->m_material = guyMaterial;

	RootForce::PlayerInputControlComponent* guyControl = m_world.GetEntityManager()->CreateComponent<RootForce::PlayerInputControlComponent>(guy);

	int facesTotal = m_engineContext.m_resourceManager->GetModel("testchar")->numberOfFaces;
	int verticesTotal = m_engineContext.m_resourceManager->GetModel("testchar")->numberOfVertices;
	int indicesTotal = m_engineContext.m_resourceManager->GetModel("testchar")->numberOfIndices;
	float* tempVertices = (float*)malloc(verticesTotal * 3 * sizeof(float));
	for(int i = 0; i < verticesTotal; i ++)
	{
		tempVertices[i*3] = m_engineContext.m_resourceManager->GetModel("testchar")->meshPoints[i].x;  // 0, 3, 6, 9
		tempVertices[i*3 + 1] = m_engineContext.m_resourceManager->GetModel("testchar")->meshPoints[i].y; //1, 4, 7, 10
		tempVertices[i*3 + 2] = m_engineContext.m_resourceManager->GetModel("testchar")->meshPoints[i].z;  //2,5,8,11   
	}
	int* tempIndices = (int*)malloc(indicesTotal * sizeof(int));
	tempIndices = (int*)&m_engineContext.m_resourceManager->GetModel("testchar")->meshIndices[0];

	float pos[3] = {0,2,2};
	float rot[3] = {0,0,0};
	int handle = m_engineContext.m_physics->AddPlayerObjectToWorld(facesTotal, &tempIndices[0], 3 * sizeof(int), verticesTotal, &tempVertices[0], 3*sizeof(float), pos, rot,5.0f ,10.f, 2, 0.3f);
	float normal[3] = {0,1,0};
	float position[3] = {0, -5, 0};
	m_engineContext.m_physics->CreatePlane(normal, position);
	float normal2[3] = {0,0,1};
	float position2[3] = {0, 0, -50};
	m_engineContext.m_physics->CreatePlane(normal2, position2);
	float speed[3] = {0, 5, -5};
	float x[3];
	float ballpos[3] = {0,-1, 2};
	int ballHandle = m_engineContext.m_physics->CreateSphere(1, 5,ballpos );
	float ballspeed[3] = {0, 0, 5};
	// Start the main loop
	uint64_t old = SDL_GetPerformanceCounter();
	while (m_running)
	{
		uint64_t now = SDL_GetPerformanceCounter();
		float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
		old = now;

		HandleEvents();
		// TODO: Update game state
		// TODO: Render and present game
		m_engineContext.m_physics->GetPlayerPos(handle, x);
		//  m_engineContext.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Collisionshape x: %f y: %f z: %f", x[0], x[1], x[2]);


		if(m_engineContext.m_inputSys->GetKeyState(SDL_Scancode::SDL_SCANCODE_SPACE) == RootEngine::InputManager::KeyState::DOWN)
			m_engineContext.m_physics->PlayerJump(handle, 10.0f);
		if(m_engineContext.m_inputSys->GetKeyState(SDL_Scancode::SDL_SCANCODE_LCTRL) == RootEngine::InputManager::KeyState::DOWN_EDGE )
		{
			m_engineContext.m_physics->PlayerKnockback(handle, speed , 20.0f);
		}
		if(m_engineContext.m_inputSys->GetKeyState(SDL_Scancode::SDL_SCANCODE_RCTRL) == RootEngine::InputManager::KeyState::DOWN_EDGE)
		{
			m_engineContext.m_physics->SetDynamicObjectVelocity(ballHandle, ballspeed);
		}
		 guyTransform->m_position = glm::vec3(x[0], x[1], x[2]);
		m_engineContext.m_physics->Update(dt);

		m_engineContext.m_renderer->Clear();


		playerControlSystem->Process(dt);
		renderingSystem->Process(dt);

		m_engineContext.m_gui->Update(now/(float)SDL_GetPerformanceFrequency());
		m_engineContext.m_renderer->Swap();
	}
	document->RemoveReference();
	debugdoc->RemoveReference();
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
			if (m_engineContext.m_inputSys != nullptr)
				m_engineContext.m_inputSys->HandleInput(event);
			if (m_engineContext.m_gui != nullptr)
				m_engineContext.m_gui->HandleEvent(event);
		}
	}
}

