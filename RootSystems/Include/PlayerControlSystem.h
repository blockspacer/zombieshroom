#pragma once

#ifndef COMPILE_LEVEL_EDITOR

#include <map>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/PhysicsSystem.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootEngine/InputManager/Include/InputInterface.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Physics/Include/RootPhysics.h>
#include <RakNet/RakPeerInterface.h>

namespace RootForce
{
	/** Component owned by a player entity that will be controlled via input. */
	

	/** 
		Enumerate all the actions a player can do via input.

		TODO: Need to consider parameters
	*/
	namespace PlayerAction
	{
		enum PlayerAction
		{
			NONE,
			MOVE_FORWARDS,
			MOVE_BACKWARDS,
			STRAFE_RIGHT,
			STRAFE_LEFT,
			ORIENTATE,
			SELECT_ABILITY1,
			SELECT_ABILITY2,
			SELECT_ABILITY3,
			ACTIVATE_ABILITY_PRESSED,
			ACTIVATE_ABILITY_RELEASED,
			JUMP_PRESSED,
			JUMP_RELEASED
		};
	}

	/** An association between a set of keys and a player action. */
	struct Keybinding
	{
		PlayerAction::PlayerAction Action;
		PlayerAction::PlayerAction ActionUp;
		std::vector<SDL_Scancode> Bindings;
		bool Edge;

		Keybinding();
		Keybinding(SDL_Scancode binding, PlayerAction::PlayerAction action);
	};


	/** This system will update the player entity depending on input received. */
	class PlayerControlSystem : public ECS::VoidSystem
	{
	public:
		PlayerControlSystem(ECS::World* p_world);

		void SetKeybindings(const std::vector<Keybinding>& keybindings);
		void SetLoggingInterface(Logging* p_logger);
		void SetInputInterface(RootEngine::InputManager::InputInterface* p_inputManager);
		void SetPhysicsInterface(RootEngine::Physics::PhysicsInterface* p_physics);
		
		void Process();
		void SetClientPeer(RakNet::RakPeerInterface* p_clientPeer);
	private:
		static Network::ActionID_t s_nextActionID;
		std::vector<Keybinding> m_keybindings;

		glm::ivec2 m_deltaMouseMovement;
		glm::vec2 m_angle;
		std::vector<PlayerAction::PlayerAction> m_inputtedActionsCurrentFrame;
		std::vector<PlayerAction::PlayerAction> m_inputtedActionsPreviousFrame;

		Logging* m_logger;
		RootEngine::InputManager::InputInterface* m_inputManager;
		RootEngine::Physics::PhysicsInterface* m_physics;
		RakNet::RakPeerInterface* m_clientPeer;
	};
}

#endif