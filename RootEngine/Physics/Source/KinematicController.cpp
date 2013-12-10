#include "KinematicController.h"
#include "Bullet/BulletCollision/CollisionShapes/btShapeHull.h"


KinematicController::KinematicController( void )
{

}

KinematicController::~KinematicController( void )
{

}
void KinematicController::RemovePlayer()
{

}

void KinematicController::Init( btDiscreteDynamicsWorld* p_world,int p_numTriangles, int* p_indexBuffer, int p_indexStride, int p_numVertices, 
							float* p_vertexBuffer, int p_vertexStride, float* p_position, float* p_rotation, float p_mass, float p_maxSpeed, float p_modelHeight, float p_stepHeight )
{
	m_dynamicWorld = p_world;
	m_heightOffset = p_modelHeight / 2.0f;
	m_stepHeight = p_stepHeight;
	m_maxSpeed = p_maxSpeed;
	m_mass = p_mass;

	//Shape
	btTriangleIndexVertexArray* indexVertexArray = new btTriangleIndexVertexArray(p_numTriangles, p_indexBuffer, p_indexStride, p_numVertices , (btScalar*) p_vertexBuffer, p_vertexStride);
	btConvexShape* objectMeshShape = new btConvexTriangleMeshShape(indexVertexArray);
	//Cull unneccesary vertices to improve performance 
	btShapeHull* objectHull = new btShapeHull(objectMeshShape);
	btScalar margin = objectMeshShape->getMargin();
	objectHull->buildHull(margin);
	btConvexHullShape* simplifiedObject = new btConvexHullShape();
	for(int i = 0; i < objectHull->numVertices(); i++)
	{
		simplifiedObject->addPoint(objectHull->getVertexPointer()[i], false);
	}
	simplifiedObject->recalcLocalAabb();
	//End shape

	//Set Inertia
	btVector3 fallInertia =  btVector3(0,0,0);
	simplifiedObject->calculateLocalInertia(p_mass,fallInertia);
	//btCollisionShape* simplifiedObject = new btCapsuleShape(0.5, 2);

	//Set startpos and start rotation and bind them to a motionstate
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(p_position[0],p_position[1],p_position[2]));
	startTransform.setRotation(btQuaternion(p_rotation[0],p_rotation[1], p_rotation[2],1));
	m_motionState = new btDefaultMotionState(startTransform);

	
	m_ghostObject = new btPairCachingGhostObject();
	m_ghostObject->setCollisionShape(simplifiedObject);
	//m_ghostObject->setUserPointer(this);
	m_ghostObject->setActivationState(DISABLE_DEACTIVATION);
	m_ghostObject->setCollisionFlags(/*m_ghostObject->getCollisionFlags() | */btCollisionObject::CF_CHARACTER_OBJECT/* |btCollisionObject::CF_NO_CONTACT_RESPONSE*/);
	
	

	//indexVertexArray = 0;
	//objectMeshShape = 0;
	//objectHull = 0;
	//delete indexVertexArray;

	//delete objectMeshShape;
	//delete objectHull;

	m_kinController = new BulletCharacter(m_ghostObject, simplifiedObject, p_stepHeight);
	
	m_kinController->setGravity(9.82f);
	m_kinController->setJumpSpeed(5);

	m_hasBeenKnockbacked = false;
	
	m_dynamicWorld->addCollisionObject(m_ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	m_dynamicWorld->addAction(m_kinController);

}


void KinematicController::Walk(float* p_dir, float p_dt)
{
	btVector3 temp = btVector3(p_dir[0], p_dir[1], p_dir[2]);
	temp.normalize();
	m_kinController->setVelocityForTimeInterval(temp*m_maxSpeed, p_dt);
	if(!m_kinController->IsKnockbacked() && !m_hasStepped)
	{
		m_kinController->playerStep(m_dynamicWorld, p_dt);
		m_hasStepped = true;
	}
	
	//m_kinController->setWalkDirection(temp*p_dt);
	//m_kinController->playerStep(m_dynamicWorld, p_dt);
}

void KinematicController::Update(float p_dt)
{
	
	m_hasStepped = false;
	m_kinController->updateAction(m_dynamicWorld, p_dt);
	m_kinController->setWalkDirection(btVector3(0,0,0));

	/*if (!m_kinController->onGround())
	{
		Walk(&m_knockbackVelocity[0], p_dt);
	}
	else
	{
		m_hasBeenKnockbacked = false;
	}*/
	//m_kinController->updateAction(m_dynamicWorld, p_dt);
	
}

void KinematicController::Jump()
{
	if(m_kinController->canJump())
		m_kinController->jump();
}

void KinematicController::Knockback(float* p_velocity, float p_power )
{
	
	m_kinController->Knockback(p_velocity, p_power);
	
}

void KinematicController::SetOrientation( float* p_orientation )
{
	m_kinController->getGhostObject()->getWorldTransform().setRotation(btQuaternion(p_orientation[0], p_orientation[1], p_orientation[2], p_orientation[3]));
}

void KinematicController::SetUserPointer( void* p_userPointer )
{
	m_ghostObject->setUserPointer(p_userPointer);
}