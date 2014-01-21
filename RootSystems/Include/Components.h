#pragma once

namespace RootForce
{
	namespace ComponentType
	{
		enum ComponentType
		{
			RENDERABLE,
			TRANSFORM,
			POINTLIGHT,
			CAMERA,
			HEALTH,
			PLAYERCONTROL,
			PHYSICS,
			NETWORK,
			LOOKATBEHAVIOR,
			THIRDPERSONBEHAVIOR,
			SCRIPT,
			COLLISION,
			COLLISIONRESPONDER,
			PLAYER,
			ANIMATION,
			PARTICLE,
			TDMRULES, //TODO: name might be inappropriate?
			PLAYERACTION,
			PLAYERPHYSICS,
			ENTITYSTATE,
			SHADOWCASTER,
			DIRECTIONALLIGHT,
			SERVERINFORMATION,
			CLIENT
		};
		
		void Initialize();
		void InitializeServerComponents();
	}

	
}