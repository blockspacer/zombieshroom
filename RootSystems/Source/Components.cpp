#include <RootSystems/Include/Components.h>

namespace RootForce
{
	namespace ComponentType
	{
#ifndef COMPILE_LEVEL_EDITOR
		void Initialize()
		{
			RootForce::Renderable::SetTypeId(RootForce::ComponentType::RENDERABLE);
			RootForce::Transform::SetTypeId(RootForce::ComponentType::TRANSFORM);
			RootForce::PointLight::SetTypeId(RootForce::ComponentType::POINTLIGHT);
			RootForce::HealthComponent::SetTypeId(RootForce::ComponentType::HEALTH);
			RootForce::PlayerControl::SetTypeId(RootForce::ComponentType::PLAYERCONTROL);
			RootForce::Physics::SetTypeId(RootForce::ComponentType::PHYSICS);
			RootForce::Network::NetworkComponent::SetTypeId(RootForce::ComponentType::NETWORK);
			RootForce::Camera::SetTypeId(RootForce::ComponentType::CAMERA);
			RootForce::LookAtBehavior::SetTypeId(RootForce::ComponentType::LOOKATBEHAVIOR);
			RootForce::ThirdPersonBehavior::SetTypeId(RootForce::ComponentType::THIRDPERSONBEHAVIOR);
			RootForce::Script::SetTypeId(RootForce::ComponentType::SCRIPT);
			RootForce::Collision::SetTypeId(RootForce::ComponentType::COLLISION);
			RootForce::CollisionResponder::SetTypeId(RootForce::ComponentType::COLLISIONRESPONDER);
			RootForce::PlayerComponent::SetTypeId(RootForce::ComponentType::PLAYER);
			RootForce::Animation::SetTypeId(RootForce::ComponentType::ANIMATION);
			RootForce::TDMRuleSet::SetTypeId(RootForce::ComponentType::TDMRULES);
			RootForce::ParticleEmitter::SetTypeId(RootForce::ComponentType::PARTICLE);
			RootForce::PlayerActionComponent::SetTypeId(RootForce::ComponentType::PLAYERACTION);
			RootForce::PlayerPhysics::SetTypeId(RootForce::ComponentType::PLAYERPHYSICS);
			RootForce::StateComponent::SetTypeId(RootForce::ComponentType::ENTITYSTATE);
			RootForce::Shadowcaster::SetTypeId(RootForce::ComponentType::SHADOWCASTER);
			RootForce::DirectionalLight::SetTypeId(RootForce::ComponentType::DIRECTIONALLIGHT);
			RootForce::Network::ServerInformationComponent::SetTypeId(RootForce::ComponentType::SERVERINFORMATION);
			RootForce::Network::ClientComponent::SetTypeId(RootForce::ComponentType::CLIENT);
			RootForce::Ragdoll::SetTypeId(RootForce::ComponentType::RAGDOLL);
			RootForce::WaterCollider::SetTypeId(RootForce::ComponentType::WATERCOLLIDER);
			RootForce::AbilityRespawnComponent::SetTypeId(RootForce::ComponentType::ABILITYSPAWN);
			RootForce::TryPickupComponent::SetTypeId(RootForce::ComponentType::TRYPICKUPCOMPONENT);
			RootForce::SoundComponent::SetTypeId(RootForce::ComponentType::SOUND);
		}

		void InitializeServerComponents()
		{
			RootForce::Transform::SetTypeId(RootForce::ComponentType::TRANSFORM);
			RootForce::HealthComponent::SetTypeId(RootForce::ComponentType::HEALTH);
			RootForce::Physics::SetTypeId(RootForce::ComponentType::PHYSICS);
			RootForce::Network::NetworkComponent::SetTypeId(RootForce::ComponentType::NETWORK);
			RootForce::LookAtBehavior::SetTypeId(RootForce::ComponentType::LOOKATBEHAVIOR);
			RootForce::Script::SetTypeId(RootForce::ComponentType::SCRIPT);
			RootForce::Collision::SetTypeId(RootForce::ComponentType::COLLISION);
			RootForce::CollisionResponder::SetTypeId(RootForce::ComponentType::COLLISIONRESPONDER);
			RootForce::PlayerComponent::SetTypeId(RootForce::ComponentType::PLAYER);
			RootForce::TDMRuleSet::SetTypeId(RootForce::ComponentType::TDMRULES);
			RootForce::PlayerActionComponent::SetTypeId(RootForce::ComponentType::PLAYERACTION);
			RootForce::PlayerPhysics::SetTypeId(RootForce::ComponentType::PLAYERPHYSICS);
            RootForce::StateComponent::SetTypeId(RootForce::ComponentType::ENTITYSTATE);
			RootForce::Network::ServerInformationComponent::SetTypeId(RootForce::ComponentType::SERVERINFORMATION);
			RootForce::AbilityRespawnComponent::SetTypeId(RootForce::ComponentType::ABILITYSPAWN);
			RootForce::TryPickupComponent::SetTypeId(RootForce::ComponentType::TRYPICKUPCOMPONENT);
		}
#endif
	}
}