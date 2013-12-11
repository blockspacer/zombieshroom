#pragma once
#include <Bullet/btBulletDynamicsCommon.h>
#include <Bullet/btBulletCollisionCommon.h>
#include <Bullet/BulletDynamics/Character/btKinematicCharacterController.h>
#include <Bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
class BulletCharacter : public btKinematicCharacterController 
{
public:
	BulletCharacter(btPairCachingGhostObject* p_ghostObject, btConvexShape* p_convexShape, btScalar p_stepHeight);
	~BulletCharacter(void);
	
	void Init();
	void playerStep(btCollisionWorld* collisionWorld, btScalar dt);
	void Knockback(const btVector3& p_direction, float p_power);
	void setWalkDirection(const btVector3& walkDirection);
	bool IsKnockbacked();

private:
	bool m_hasBeenKnockbacked;
	btVector3 m_knockbackVelocity;
};

