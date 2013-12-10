#pragma once
#include <Bullet/btBulletDynamicsCommon.h>
#include <Bullet/btBulletCollisionCommon.h>
#include <Bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <RootEngine/Physics/Include/BulletCharacter.h>
#include <RootEngine/Physics/Include/DebugDrawer.h>
#include <vector>
class KinematicController 
{
public:
	KinematicController(void);
	~KinematicController(void);
	void Init(btDiscreteDynamicsWorld* p_world,int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices,
		float* p_vertexBuffer, int p_vertexStride, float* p_position, float* p_rotation, float p_mass, float p_maxSpeed, float p_modelHeight,
		float p_stepHeight );
	//Assumes the float* is a float[3]
	void Walk(float* p_dir, float p_dt);
	void Jump();
	void Knockback(float* p_velocity, float p_dt);
	void Update(float p_dt);
	void SetOrientation(float* p_orientation);
	void RemovePlayer();
	void SetUserPointer(void* p_userPointer);
	void SetDebugDrawer(DebugDrawer* p_drawer) const {m_kinController->debugDraw(p_drawer);}

	btVector3 GetPosition() const {return m_kinController->getGhostObject()->getWorldTransform().getOrigin();}
	btQuaternion GetOrientation() const {return m_kinController->getGhostObject()->getWorldTransform().getRotation();};

private:
	btDiscreteDynamicsWorld* m_dynamicWorld;
	btPairCachingGhostObject* m_ghostObject;
	btMotionState* m_motionState;
	BulletCharacter* m_kinController;
	bool m_hasStepped;
	float m_maxSpeed;
	float m_stepHeight;
	float m_heightOffset;
	float m_mass;
	bool m_hasBeenKnockbacked;

};
