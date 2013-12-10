#pragma once

#if defined(_WINDLL)
	#define ROOTENGINE_DLL_EXPORT __declspec(dllexport)
#else
	#define ROOTENGINE_DLL_EXPORT __declspec(dllimport)
#endif

#include <RootEngine/Include/SubsystemSharedContext.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <memory>

namespace RootEngine
{
	namespace SubsystemInit
	{
		enum SubsystemInit
		{
			INIT_NETWORK = 1,
			INIT_INPUT = 2,
			INIT_RENDER = 4,
			INIT_GUI = 8,
			INIT_PHYSICS = 16,
			INIT_SCRIPTING = 32,
			INIT_ALL = INIT_NETWORK | INIT_INPUT | INIT_RENDER | INIT_GUI | INIT_PHYSICS | INIT_SCRIPTING
		};
	}

	class EngineMain
	{
	public:
		EngineMain();
		~EngineMain();

		void Initialize(int p_flags, std::string p_workingDirectory);
		GameSharedContext GetGameSharedContext();
		SubsystemSharedContext GetSubsystemSharedContext();
	private:
		void LoadRender();
#ifndef COMPILE_LEVEL_EDITOR
		void LoadNetwork();
		void LoadInput();
		void LoadGUI();
		//void LoadInputSystem();
		void LoadPhysics();

		void LoadScriptEngine();
#endif

		void* m_networkModule;
		void* m_renderModule;
		void* m_guiModule;
		void* m_inputModule;
		void* m_physicsModule;
		void* m_scriptModule;

		SubsystemSharedContext m_subsystemSharedContext;
		GameSharedContext m_gameSharedContext;

		Logging m_logger;
		MemoryTracker* m_memTracker;
		ResourceManager m_resourceManager;
		Profiling m_profiler;
		Render::RendererInterface* m_renderer;
		ConfigManager m_configManager;
#ifndef COMPILE_LEVEL_EDITOR
		DebugOverlay m_debugOverlay;
		Network::NetworkManager*		m_network;
		GUISystem::GUISystemInterface*	m_gui;
		InputManager::InputInterface* m_inputSys;
		Physics::PhysicsInterface* m_physics;
		Script::ScriptInterface* m_scriptEngine;
#endif

	};
}

extern "C"
{
	typedef RootEngine::GameSharedContext ( *INITIALIZEENGINE )(int, std::string);
	ROOTENGINE_DLL_EXPORT RootEngine::GameSharedContext InitializeEngine(int p_flags, std::string p_workingDirectory);
}