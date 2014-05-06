#include "Treenity.h"
#include <RootTools/Treenity/Include/TreenityMain.h>
#include <QtWidgets/QApplication>
#include <SDL2/SDL.h>
#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootTools/Treenity/Include/EngineActions.h>

#include <RootEngine/Include/RootEngine.h>
#include <RootEngine/InputManager/Include/InputInterface.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>

#include <RootSystems/Include/Components.h>

#include <RootTools/Treenity/Include/ComponentImporter.h>
#include <RootTools/Treenity/Include/ComponentExporter.h>
#include <QString>
#include <QStyleFactory>

#include <RootForce/Include/LuaAPI.h>
#include <RootEngine/Script/Include/ScriptManager.h>

#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/GameSharedContext.h>

#include <RootSystems/Include/Network/NetworkTypes.h>
#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/MessageHandlers.h>

RootEngine::GameSharedContext g_engineContext;
ECS::World* g_world;
RootForce::Network::NetworkEntityMap g_networkEntityMap;
RootForce::Network::DeletedNetworkEntityList g_networkDeletedList;

#undef main
int main(int argc, char *argv[])
{
	std::string path(argv[0]);
	std::string rootforcename = "Treenity.exe";
	path = path.substr(0, path.size() - rootforcename.size());

	QApplication a(argc, argv);
	
	a.setStyle(QStyleFactory::create("Fusion"));

	QPalette darkPalette;
	darkPalette.setColor(QPalette::Window, QColor(53,53,53));
	darkPalette.setColor(QPalette::WindowText, Qt::white);
	darkPalette.setColor(QPalette::Base, QColor(25,25,25));
	darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
	darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
	darkPalette.setColor(QPalette::ToolTipText, Qt::white);
	darkPalette.setColor(QPalette::Text, Qt::white);
	darkPalette.setColor(QPalette::Button, QColor(53,53,53));
	darkPalette.setColor(QPalette::ButtonText, Qt::white);
	darkPalette.setColor(QPalette::BrightText, Qt::red);
	darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

	darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
	darkPalette.setColor(QPalette::HighlightedText, Qt::black);

	qApp->setPalette(darkPalette);

	qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");



	TreenityMain m(path);

	uint64_t old = SDL_GetPerformanceCounter();
	while(m.IsRunning())
	{
		uint64_t now = SDL_GetPerformanceCounter();
		float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
		old = now;

		//w.Update(dt);
		a.processEvents(QEventLoop::AllEvents);
		m.Update(dt);
	}

	return 0;
}

TreenityMain::TreenityMain(const std::string& p_path)
	: m_engineActions(&m_world, this)
	, m_projectManager(&m_world)
	, m_worldSystem(&m_world, &g_engineContext)
{
	g_world = &m_world;

	// Load & Initialize Root Engine.
	m_engineModule = DynamicLoader::LoadSharedLibrary("RootEngine.dll");

	if (m_engineModule == nullptr)
	{
		throw std::runtime_error("Failed to load RootEngine - please check your installation");
	}

	INITIALIZEENGINE libInitializeEngine = (INITIALIZEENGINE)DynamicLoader::LoadProcess(m_engineModule, "InitializeEngine");

	if (libInitializeEngine == nullptr)
	{
		throw std::runtime_error("Failed to load RootEngine - please check your installation");
	}

	g_engineContext = libInitializeEngine(
		RootEngine::SubsystemInit::INIT_INPUT |
		RootEngine::SubsystemInit::INIT_RENDER |
		RootEngine::SubsystemInit::INIT_SCRIPTING, p_path);

	// Initialize SDL.
	if (SDL_Init(SDL_INIT_TIMER) != 0) 
	{
		throw std::runtime_error("Failed to initialize SDL");
	}

	m_treenityEditor.CreateOpenGLContext();
	m_treenityEditor.SetEngineInterface(&m_engineActions);
	m_treenityEditor.SetProjectManager(&m_projectManager);
	
	// Initialize components and preallocate memory.
	RootForce::ComponentType::Initialize();

	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Renderable>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Transform>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::PointLight>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Camera>(10);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::HealthComponent>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerControl>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::ControllerActions>(10);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Physics>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Network::NetworkComponent>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::LookAtBehavior>(20);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::ThirdPersonBehavior>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Script>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Collision>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::CollisionResponder>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerComponent>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Animation>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::ParticleEmitter>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::TDMRuleSet>(10);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerActionComponent>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerPhysics>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::StateComponent>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Shadowcaster>(1);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::DirectionalLight>(10);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Network::ClientComponent>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Network::ServerInformationComponent>(10);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Ragdoll>(100);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::WaterCollider>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::AbilitySpawnComponent>(100);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::TryPickupComponent>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::SoundComponent>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::TimerComponent>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::FollowComponent>(1000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::HomingComponent>(1000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::RayComponent>(1000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::DamageAndKnockback>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Scalable>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::StatChange>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::KillAnnouncement>(10);

	// Bind c++ functions and members to Lua.
	RootForce::LuaAPI::RegisterLuaTypes(g_engineContext.m_script->GetLuaState());

	m_renderingSystem = new RootForce::RenderingSystem(&m_world);
	m_renderingSystem->SetRendererInterface(g_engineContext.m_renderer);
	m_renderingSystem->SetLoggingInterface(g_engineContext.m_logger);

	m_cameraSystem = new RootForce::CameraSystem(&m_world, &g_engineContext);
	m_world.GetSystemManager()->AddSystem<RootForce::CameraSystem>(m_cameraSystem);

	m_transformInterpolationSystem = new RootForce::TransformInterpolationSystem(&m_world);
	m_world.GetSystemManager()->AddSystem<RootForce::TransformInterpolationSystem>(m_transformInterpolationSystem);

	m_world.GetSystemManager()->AddSystem<RootForce::RenderingSystem>(m_renderingSystem);

	m_scriptSystem = new RootForce::ScriptSystem(&m_world);
	m_world.GetSystemManager()->AddSystem<RootForce::ScriptSystem>(m_scriptSystem);

	m_controllerActionSystem = new RootForce::ControllerActionSystem(&m_world);
	m_world.GetSystemManager()->AddSystem<RootForce::ControllerActionSystem>(m_controllerActionSystem);

	m_lookAtSystem = new RootForce::LookAtSystem(g_world, &g_engineContext);
	g_world->GetSystemManager()->AddSystem<RootForce::LookAtSystem>(m_lookAtSystem);

	m_shadowSystem = new RootForce::ShadowSystem(g_world);
	g_world->GetSystemManager()->AddSystem<RootForce::ShadowSystem>(m_shadowSystem);

	m_pointLightSystem = new RootForce::PointLightSystem(g_world, &g_engineContext);
	g_world->GetSystemManager()->AddSystem<RootForce::PointLightSystem>(m_pointLightSystem);

	m_directionalLightSystem = new RootForce::DirectionalLightSystem(g_world, &g_engineContext);
	g_world->GetSystemManager()->AddSystem<RootForce::DirectionalLightSystem>(m_directionalLightSystem);

	m_world.GetEntityImporter()->SetImporter(Importer);
	m_world.GetEntityExporter()->SetExporter(Exporter);

	m_treenityEditor.CreateNewScene();

	// Display treenity editor.
	m_treenityEditor.show();

	g_engineContext.m_inputSys->LockMouseToCenter(false);
	SDL_SetRelativeMouseMode(SDL_FALSE);

	m_selectedEntityMaterial = g_engineContext.m_renderer->CreateMaterial("SelectedMaterial");
	m_selectedEntityMaterial->m_effect = g_engineContext.m_resourceManager->LoadEffect("Mesh_Selected");
}

TreenityMain::~TreenityMain()
{
	delete m_renderingSystem;
	delete m_cameraSystem;
	delete m_transformInterpolationSystem;
	delete m_scriptSystem;
	delete m_controllerActionSystem;
	delete m_lookAtSystem;
	delete m_shadowSystem;
	delete m_pointLightSystem;
	delete m_directionalLightSystem;

	SDL_Quit();

	DynamicLoader::FreeSharedLibrary(m_engineModule);
}

bool TreenityMain::IsRunning()
{
	return m_treenityEditor.IsRunning();
}

void TreenityMain::HandleEvents()
{
	if (g_engineContext.m_inputSys != nullptr)
	{
		g_engineContext.m_inputSys->Reset();
		g_engineContext.m_inputSys->SetMousePos(glm::ivec2(QCursor::pos().x(), QCursor::pos().y()));

		HandleAltModifier();
	}

	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		// Ignore mouse motions events while in editor mode.
		if(event.type == SDL_MOUSEMOTION)
			continue;

		if (g_engineContext.m_inputSys != nullptr)
			g_engineContext.m_inputSys->HandleInput(event);
	}
}

void TreenityMain::HandleAltModifier()
{
	Qt::KeyboardModifiers modifers = QApplication::keyboardModifiers();
	if(m_altMode)
	{
		if((modifers & Qt::AltModifier) == 0)
		{		
			SDL_Event keyEvent;
			keyEvent.type = SDL_KEYUP;
			keyEvent.key.keysym.scancode = SDL_SCANCODE_LALT;
			keyEvent.key.repeat = false;
			SDL_PushEvent(&keyEvent);

			m_altMode = false;
		}
	}
	else
	{
		if((modifers & Qt::AltModifier) != 0)
		{	
			SDL_Event keyEvent;
			keyEvent.type = SDL_KEYDOWN;
			keyEvent.key.keysym.scancode = SDL_SCANCODE_LALT;
			keyEvent.key.repeat = false;
			SDL_PushEvent(&keyEvent);

			m_altMode = true;
		}
	}
}

void TreenityMain::ProcessWorldMessages()
{
	auto msgs = m_world.GetMessages();
	for(auto itr = msgs.begin(); itr != msgs.end(); ++itr) 
	{
		switch (itr->m_type)
		{
			case ECS::MessageType::ENTITY_ADDED:
			{
				m_projectManager.EntityAdded(itr->m_entity);
				m_treenityEditor.EntityCreated(itr->m_entity);
			} break;

			case ECS::MessageType::ENTITY_REMOVED:
			{
				m_projectManager.EntityRemoved(itr->m_entity);
				m_treenityEditor.EntityRemoved(itr->m_entity);
			} break;

			case ECS::MessageType::COMPONENT_ADDED:
			{
				m_treenityEditor.ComponentCreated(itr->m_entity, itr->m_compType);
			} break;
			
			case ECS::MessageType::COMPONENT_REMOVED:
			{
				m_treenityEditor.ComponentRemoved(itr->m_entity, itr->m_compType);
			} break;

			case ECS::MessageType::TAG_ADDED:
			{
				m_treenityEditor.TagAdded(itr->m_entity, itr->m_tagGroupName);
			} break;

			case ECS::MessageType::TAG_REMOVED:
			{
				m_treenityEditor.TagRemoved(itr->m_entity, itr->m_tagGroupName);
			} break;

			case ECS::MessageType::ENTITY_ADDED_TO_GROUP:
			{
				m_treenityEditor.EntityAddedToGroup(itr->m_entity, itr->m_tagGroupName);
			} break;

			case ECS::MessageType::ENTITY_REMOVED_FROM_GROUP:
			{
				m_treenityEditor.EntityRemovedFromGroup(itr->m_entity, itr->m_tagGroupName);
			} break;
		}

		g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Message Type %d - Entity ID: %d - Component Type: %d", itr->m_type, itr->m_entity->GetId(), itr->m_compType);
	}
}

void TreenityMain::Update(float dt)
{
	m_world.SetDelta(dt);

	HandleEvents();

	ProcessWorldMessages();
	m_world.GetEntityManager()->CleanUp();

	m_worldSystem.Process();
	m_controllerActionSystem->Process();	
	m_lookAtSystem->Process();
	m_cameraSystem->Process();
	m_scriptSystem->Process();
	m_transformInterpolationSystem->Process();
	m_shadowSystem->Process();
	m_directionalLightSystem->Process();
	m_pointLightSystem->Process();
	m_renderingSystem->Process();
	
	RenderSelectedEntity();

	g_engineContext.m_renderer->Clear();
	g_engineContext.m_renderer->Render();
	g_engineContext.m_renderer->Swap();
}

void TreenityMain::RenderSelectedEntity()
{
	for(auto itr = m_treenityEditor.GetSelection().begin(); itr != m_treenityEditor.GetSelection().end(); ++itr)
	{
		ECS::Entity* entity = (*itr);

		RootForce::Transform* transform = m_world.GetEntityManager()->GetComponent<RootForce::Transform>(entity);
		RootForce::Renderable* renderable = m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(entity);

		if(renderable == nullptr)
			continue;

		m_renderingSystem->m_matrices[entity].m_model = glm::translate(glm::mat4(1.0f), transform->m_interpolatedPosition);
		m_renderingSystem->m_matrices[entity].m_model = glm::rotate(m_renderingSystem->m_matrices[entity].m_model, transform->m_orientation.GetAngle(), transform->m_orientation.GetAxis());
		m_renderingSystem->m_matrices[entity].m_model = glm::scale(m_renderingSystem->m_matrices[entity].m_model, transform->m_scale);
		m_renderingSystem->m_matrices[entity].m_normal = glm::mat4(glm::transpose(glm::inverse(glm::mat3(m_renderingSystem->m_matrices[entity].m_model))));

		Render::RenderJob job;	
		job.m_mesh = renderable->m_model->m_meshes[0];
		job.m_material = m_selectedEntityMaterial;	
		job.m_params = renderable->m_params;
		job.m_forward = renderable->m_forward;
		job.m_refractive = renderable->m_refractive;
		job.m_params[Render::Semantic::MODEL] = &m_renderingSystem->m_matrices[entity].m_model;
		job.m_renderPass = RootForce::RenderPass::RENDERPASS_EDITOR;
		job.m_position = transform->m_interpolatedPosition;

		g_engineContext.m_renderer->AddRenderJob(job);

	}
}