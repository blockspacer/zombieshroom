#include "ScriptGenerator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <QtCore/QDir>

namespace AbilityEditorNameSpace
{

	ScriptGenerator::ScriptGenerator()
	{
		m_damage = 0.0f;
		m_knockback = 0.0f;
	}

	ScriptGenerator::~ScriptGenerator()
	{

	}

	void ScriptGenerator::GenerateScript( QString p_filePath, QString p_name, Entity* p_entity, OnCreate* p_onCreate, OnCollide* p_onCollide, OnDestroy* p_onDestroy )
	{
		//p_scriptName.chop(4);
		//m_name = p_folderName.toStdString();

		/* THIS IS IF WE WANT TO PUT SCRIPTS IN A FOLDER
		p_filePath.chop(p_filePath.size() - p_filePath.lastIndexOf("/") - 1);
		QDir thedir;
		thedir.setCurrent(p_filePath);
		//QDir().setCurrent(p_filePath);
		if(!thedir.exists(p_name))
		{
			thedir.mkdir(p_name);
		}*/
		m_entity = p_entity;

		p_filePath.chop(p_filePath.size() - p_filePath.lastIndexOf("/") - 1);

		m_name = p_name.toStdString();
		m_file.open(p_filePath.toStdString() + m_name + ".lua");

		//Begin writing in the file
		m_file << m_name << " = {};\n";
		
		WriteVariables();
		m_file << "\n";
		WriteChargeDone();
		m_file << "\n";
		WriteChannelingDone();
		m_file << "\n";
		WriteOnCreate(p_onCreate);
		m_file << "\n";
		WriteOnCollide(p_onCreate, p_onCollide);
		m_file << "\n";
		WriteOnDestroy(p_onDestroy);

		m_file.close();

	}

	void ScriptGenerator::WriteVariables()
	{
		for (unsigned int j = 0; j < m_entity->GetComponents()->size(); j++)
		{
			if(m_entity->GetComponents()->at(j)->m_type == AbilityComponents::ComponentType::DAMAGE)
			{
				m_damage = ((AbilityComponents::Damage*)m_entity->GetComponents()->at(j))->m_damage;
				m_file << m_name << ".damage = " << m_damage << ";\n";
				m_file << m_name << ".damageNew = " << m_damage << ";\n";
			}
			else if(m_entity->GetComponents()->at(j)->m_type == AbilityComponents::ComponentType::KNOCKBACK)
			{
				m_knockback = ((AbilityComponents::Knockback*)m_entity->GetComponents()->at(j))->m_knockback;
				m_file << m_name << ".knockback = " << m_knockback << ";\n";
				m_file << m_name << ".knockbackNew = " << m_knockback << ";\n";
			}
		}
		m_file << m_name << ".cooldown = " << m_entity->GetCooldown() << ";\n";
		m_file << m_name << ".charges = " << m_entity->GetCharges() << ";\n";
		m_file << m_name << ".chargeTime = " << m_entity->GetChargeTime() << ";\n";
		m_file << m_name << ".channelingTime = " << m_entity->GetChannelingTime() << ";\n";
		m_file << m_name << ".duration = " << m_entity->GetDuration() << ";\n";
	}

	void ScriptGenerator::WriteChargeDone()
	{
		float chargeReq = 0.0f, chargeFac = 1.0f;

		m_file << "function " << m_name << ".ChargeDone (time, userId, actionId)\n";
		for (unsigned int i = 0; i < m_entity->GetComponents()->size(); i++)
		{
			if(m_entity->GetComponents()->at(i)->m_type == AbilityComponents::ComponentType::CHARGEVARIABLES)
			{
				chargeReq = ((AbilityComponents::ChargeVariables*)m_entity->GetComponents()->at(i))->m_chargeReq;
				chargeFac = ((AbilityComponents::ChargeVariables*)m_entity->GetComponents()->at(i))->m_chargeFactor;
			}
		}
		if (chargeReq > 0.0f)
		{
			m_file << "\tif time >= " << m_name << ".chargeTime * " << chargeReq << " then\n\t";
		}
		if (chargeFac >= 1.0f && m_entity->GetChargeTime() > 0.0f)
		{
			m_file << "\t" << m_name << ".damage = " << m_name << ".damage * ((time * " << chargeFac << ") / " << m_name << ".chargeTime);\n";
			m_file << "\t" << m_name << ".knockback = " << m_name << ".knockback * ((time * " << chargeFac << ") / " << m_name << ".chargeTime);\n";
		}
		m_file << "\t" << m_name << ".OnCreate(userId, actionId);\n";
		if (chargeReq > 0.0f)
		{
			m_file << "\tend\n";
		}
		m_file << "end\n";
	}

	void ScriptGenerator::WriteChannelingDone()
	{
		m_file << "function " << m_name << ".ChannelingDone (time, userId, actionId)\n";

		//Remove stuff, or create other stuff

		m_file << "end\n";
	}

	void ScriptGenerator::WriteOnCreate( OnCreate* p_onCreate)
	{
		m_file << "function " << m_name << ".OnCreate (userId, actionId)\n";

			m_file << "\t--Entities\n";
			m_file << "\tlocal self = Entity.New();\n";
			m_file << "\tlocal casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);\n";
			m_file << "\tlocal networkEnt = Network.New(self, userId, actionId);\n";

			m_file << "\t--Components\n";
			m_file << "\tlocal transformComp = Transformation.New(self);\n";
			m_file << "\tlocal collisionComp = Collision.New(self);\n";
			m_file << "\tlocal colRespComp = CollisionResponder.New(self);\n";
			m_file << "\tlocal physicsComp = Physics.New(self);\n";
			m_file << "\tlocal scriptComp = Script.New(self, \"" << m_name << "\");\n";
			m_file << "\tlocal timerComp = Timer.New(self, " << m_name << ".duration);\n";

			m_file << "\t--Setting stuff\n";
			if(m_entity->DoesComponentExist(AbilityComponents::ComponentType::PHYSICS))
				m_file << "\tcollisionComp:CreateHandle(self, 1, false);\n";
			else //The '1' is Type::ABILITY
				m_file << "\tcollisionComp:CreateHandle(self, 1, true);\n";
			m_file << "\tcolRespComp:SetContainer(collisionComp);\n";

			//Some standard values
			//Collision Shape variables
			float radius = 1.0f;
			float height = 1.0f;
			int colShape = 3;
			//Physics variables
			float mass = 1.0f;
			QVector3D grav = QVector3D(0,9.82f,0);
			//Positions
			int startEnum, targetEnum;
			QVector3D startPos = QVector3D(0,0,0);
			QVector3D targetPos = QVector3D(0,0,0);
			//Velocity
			int direction = 0;
			float speed = 0.0f;
			//Collision variable
			bool colWithWorld = false;
			//Setting values
			for (unsigned int i = 0; i < m_entity->GetComponents()->size(); i++)
			{
				if (m_entity->GetComponents()->at(i)->m_type == AbilityComponents::ComponentType::COLLISIONSHAPE)
				{
					radius = ((AbilityComponents::CollisionShape*)m_entity->GetComponents()->at(i))->m_radius;
					height = ((AbilityComponents::CollisionShape*)m_entity->GetComponents()->at(i))->m_height;
					colShape = ((AbilityComponents::CollisionShape*)m_entity->GetComponents()->at(i))->m_CollisionShape;
				}
				else if (m_entity->GetComponents()->at(i)->m_type == AbilityComponents::ComponentType::PHYSICS)
				{
					mass = ((AbilityComponents::Physics*)m_entity->GetComponents()->at(i))->m_mass;
					grav = ((AbilityComponents::Physics*)m_entity->GetComponents()->at(i))->m_gravity;
					colWithWorld = ((AbilityComponents::Physics*)m_entity->GetComponents()->at(i))->m_collide;
				}
				else if (m_entity->GetComponents()->at(i)->m_type == AbilityComponents::ComponentType::STARTPOS)
				{
					startEnum = ((AbilityComponents::StartPos*)m_entity->GetComponents()->at(i))->m_startPos;
					startPos = ((AbilityComponents::StartPos*)m_entity->GetComponents()->at(i))->m_absolutePos;
				}
				else if (m_entity->GetComponents()->at(i)->m_type == AbilityComponents::ComponentType::TARGPOS)
				{
					targetEnum = ((AbilityComponents::TargetPos*)m_entity->GetComponents()->at(i))->m_targPos;
					targetPos = ((AbilityComponents::TargetPos*)m_entity->GetComponents()->at(i))->m_absolutePos;
				}
				else if (m_entity->GetComponents()->at(i)->m_type == AbilityComponents::ComponentType::VELOCITY)
				{
					direction = ((AbilityComponents::Velocity*)m_entity->GetComponents()->at(i))->m_direction;
					speed = ((AbilityComponents::Velocity*)m_entity->GetComponents()->at(i))->m_speed;
				}
				//Rotation and stuff will be set by the followPlayer System
			}
			//Writing values

			if (m_entity->DoesComponentExist(AbilityComponents::ComponentType::VELOCITY))
			{
				if (direction == AbilityComponents::Velocity::FORWARD)
				{
					m_file << "\tlocal dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();\n";
					m_file << "\tlocal rotQuat = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetQuaternion();\n";
					m_file << "\ttransformComp:GetOrient():SetOrientation(rotQuat);\n";
					m_file << "\ttransformComp:GetOrient():Pitch(-90);\n";
					m_file << "\trotQuat = transformComp:GetOrient():GetQuaternion();\n";
				}
				else if (direction == AbilityComponents::Velocity::BACKWARD)
				{
					m_file << "\tlocal dirVec = -Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();\n";
					m_file << "\tlocal rotQuat = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetQuaternion();\n";
					m_file << "\ttransformComp:GetOrient():SetOrientation(rotQuat);\n";
					m_file << "\ttransformComp:GetOrient():Pitch(90);\n";
					m_file << "\trotQuat = transformComp:GetOrient():GetQuaternion();\n";
				}
				else if (direction == AbilityComponents::Velocity::UP)
				{
					m_file << "\tlocal dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetUp();\n";
				}
				else if (direction == AbilityComponents::Velocity::DOWN)
				{
					m_file << "\tlocal dirVec = -Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetUp();\n";
					m_file << "\tlocal rotQuat = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetQuaternion();\n";
					m_file << "\ttransformComp:GetOrient():SetOrientation(rotQuat);\n";
					m_file << "\ttransformComp:GetOrient():Pitch(180);\n";
					m_file << "\trotQuat = transformComp:GetOrient():GetQuaternion();\n";
				}
				else if (direction == AbilityComponents::Velocity::RIGHT)
				{
					m_file << "\tlocal dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetRight();\n";
					m_file << "\tlocal rotQuat = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetQuaternion();\n";
					m_file << "\ttransformComp:GetOrient():SetOrientation(rotQuat);\n";
					m_file << "\ttransformComp:GetOrient():Roll(-90);\n";
					m_file << "\trotQuat = transformComp:GetOrient():GetQuaternion();\n";
				}
				else if (direction == AbilityComponents::Velocity::LEFT)
				{
					m_file << "\tlocal dirVec = -Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetRight();\n";
					m_file << "\tlocal rotQuat = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetQuaternion();\n";
					m_file << "\ttransformComp:GetOrient():SetOrientation(rotQuat);\n";
					m_file << "\ttransformComp:GetOrient():Roll(90);\n";
					m_file << "\trotQuat = transformComp:GetOrient():GetQuaternion();\n";
				}
			}
			else
			{
				m_file << "\tlocal dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();\n";
				m_file << "\tlocal rotQuat = Quat.New(0,0,0,1);\n";
			}

			if (m_entity->DoesComponentExist(AbilityComponents::ComponentType::STARTPOS))
			{
				m_file << "\tlocal tempPos = casterEnt:GetTransformation():GetPos();\n"; 
				m_file << "\tlocal startPos = Vec3.New((tempPos.x + dirVec.x * 3), (2 + tempPos.y + dirVec.y * 3), (tempPos.z + dirVec.z * 3));\n"; 
				if (startEnum == AbilityComponents::StartPos::ONCASTER)
				{
					//m_file << "\tstartPos = Vec3.New((tempPos.x + dirVec.x * 3), (2 + tempPos.y + dirVec.y * 3), (tempPos.z + dirVec.z * 3));\n"; 
				}
				else if (startEnum == AbilityComponents::StartPos::UNDERCASTER)
				{
					m_file << "\tstartPos = Vec3.New(tempPos.x, tempPos.y - 2, tempPos.z);\n"; 
				}
				else if (startEnum == AbilityComponents::StartPos::ENEMYPLAYER)
				{
					m_file << "\tlocal entityAtAim = physicsComp:GetPlayerAtAim(collisionComp:GetHandle(), tempPos, Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront(), 10000);\n";
					m_file << "\tif entityAtAim:DoesExist() then\n";
					m_file << "\t\tif entityAtAim:GetType(collisionComp) == PhysicsType.TYPE_PLAYER and self:GetPlayerComponent():GetTeamId() ~= entityAtAim:GetPlayerComponent():GetTeamId() then\n";
					m_file << "\t\t\tstartPos = entityAtAim:GetTransformation():GetPos();\n";
					m_file << "\t\tend\n";
					m_file << "\tend\n";
				}
				else if (startEnum == AbilityComponents::StartPos::FRIENDPLAYER)
				{
					m_file << "\tlocal tempPos = casterEnt:GetTransformation():GetPos();\n"; 
					m_file << "\tlocal entityAtAim = physicsComp:GetPlayerAtAim(collisionComp:GetHandle(), tempPos, Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront(), 10000);\n";
					m_file << "\tif entityAtAim:DoesExist() then\n";
					m_file << "\t\tif entityAtAim:GetType(collisionComp) == PhysicsType.TYPE_PLAYER and self:GetPlayerComponent():GetTeamId() == entityAtAim:GetPlayerComponent():GetTeamId() then\n";
					m_file << "\t\t\tstartPos = entityAtAim:GetTransformation():GetPos();\n";
					m_file << "\t\tend\n";
					m_file << "\tend\n";
				}
				else// if (startEnum == AbilityComponents::StartPos::ABSOLUTE)
				{
					m_file << "\tlocal startPos = Vec3.New(" << startPos.x() << ", " << startPos.y() << ", " << startPos.z() << ");\n"; 
				}
			}
			else
			{
				m_file << "\tlocal startPos = Vec3.New(0,0,0);\n";
			}

			if (m_entity->DoesComponentExist(AbilityComponents::ComponentType::TARGPOS))
			{
				if (startEnum == AbilityComponents::TargetPos::ONAIM)
				{
					//tarPos = rayCast.getPosition()
					//SetTargetPosition(tarPos);
				}
				else if (startEnum == AbilityComponents::TargetPos::ENEMYPLAYER)
				{
					//if getClosestPlayer == enemy
					//tarPos = enemy.getPosition()
					//SetTargetPosition(tarPos);
				}
				else if (startEnum == AbilityComponents::TargetPos::FRIENDLYPLAYER)
				{
					//if getClosestPlayer == friend
					//tarPos = friend.getPosition()
					//SetTargetPosition(tarPos);
				}
				else if (startEnum == AbilityComponents::TargetPos::ABSOLUTE)
				{
					//SetTargetPosition(Vec3.New(targetPos));
				}
			}

			if(m_entity->DoesComponentExist(AbilityComponents::ComponentType::COLLISIONSHAPE))
			{
				if (colShape == AbilityComponents::CollisionShape::CYLINDER)
				{
					m_file << "\tphysicsComp:BindCylinderShape(collisionComp, startPos, rotQuat, "<<height<<", "<<radius<<", "<<mass<<", "<<(colWithWorld ? "true" : "false")<<");\n";
				}
				else if (colShape == AbilityComponents::CollisionShape::CONE)
				{
					m_file << "\tphysicsComp:BindConeShape(collisionComp, startPos, rotQuat, "<<height<<", "<<radius<<", "<<mass<<", "<<(colWithWorld ? "true" : "false")<<");\n";
				}
				else if (colShape == AbilityComponents::CollisionShape::SPHERE)
				{
					m_file << "\tphysicsComp:BindSphereShape(collisionComp, startPos, rotQuat, "<<radius<<", "<<mass<<", "<<(colWithWorld ? "true" : "false")<<");\n";
				}
				else if (colShape == AbilityComponents::CollisionShape::RAY)
				{
					m_file << "\tphysicsComp:BindNoShape(collisionComp:GetHandle(), startPos, rotQuat);\n";
					m_file << "\tphysicsComp:ShootRay(collisionComp:GetHandle(), startPos, dirVec, "<< height <<");\n";
				}
				else if (colShape == AbilityComponents::CollisionShape::MESH)
				{
					m_file << "\tphysicsComp:BindMeshShape(collisionComp, startPos, rotQuat, Vec3.New(1,1,1), "/*<-- scale, what do?*/<<mass<<", "<<(colWithWorld ? "true" : "false")<<");\n";
				}
			}

			if(m_entity->DoesComponentExist(AbilityComponents::ComponentType::PHYSICS))
			{
				m_file << "\tphysicsComp:SetVelocity(collisionComp, Vec3.New(dirVec.x * " << speed << ", dirVec.y * " << speed << ", dirVec.z * " << speed << "));\n";
				m_file << "\tphysicsComp:SetGravity(collisionComp, Vec3.New(" << grav.x() << ", " << grav.y() << ", " << grav.z() << "));\n";
			}
			
			m_file << "\ttransformComp:SetPos(startPos);\n";

			m_file << "\n";



		//Client only components:
			m_file << "\tif Global.IsClient then\n";
		for (unsigned int j = 0; j < m_entity->GetComponents()->size(); j++)
		{
			// Setting values
			if(m_entity->GetComponents()->at(j)->m_type == AbilityComponents::ComponentType::ABILITYMODEL)
			{
				m_file << "\t\tlocal renderComp = Renderable.New(self);\n";
				m_file << "\t\trenderComp:SetModel(\"" << ((AbilityComponents::AbilityModel*)m_entity->GetComponents()->at(j))->m_modelName << "\");\n";
				m_file << "\t\trenderComp:SetMaterial(\"" << ((AbilityComponents::AbilityModel*)m_entity->GetComponents()->at(j))->m_material << "\");\n";
				m_file << "\t\trenderComp:SetMaterialDiffuse(\"" << ((AbilityComponents::AbilityModel*)m_entity->GetComponents()->at(j))->m_materialDiffuse << "\");\n";
				m_file << "\t\trenderComp:SetMaterialSpecular(\"" << ((AbilityComponents::AbilityModel*)m_entity->GetComponents()->at(j))->m_materialSpecular << "\");\n";
				m_file << "\t\trenderComp:SetMaterialNormal(\"" << ((AbilityComponents::AbilityModel*)m_entity->GetComponents()->at(j))->m_materialNormal << "\");\n";
				m_file << "\t\trenderComp:SetMaterialEffect(\"" << ((AbilityComponents::AbilityModel*)m_entity->GetComponents()->at(j))->m_materialEffect << "\");\n";
			}
			else if (m_entity->GetComponents()->at(j)->m_type == AbilityComponents::ComponentType::ABILITYPARTICLE)
			{
				m_file << "\t\tlocal particleComp = ParticleEmitter.New(self, \"" << ((AbilityComponents::AbilityParticle*)m_entity->GetComponents()->at(j))->m_particleName << "\");\n";
			}
			else if (m_entity->GetComponents()->at(j)->m_type == AbilityComponents::ComponentType::WATER)
			{
				m_file << "\t\tlocal waterComp = WaterCollider.New(self);\n";
				m_file << "\t\twaterComp:SetDisturbPower(" << ((AbilityComponents::Water*)m_entity->GetComponents()->at(j))->m_power << ");\n";
				m_file << "\t\twaterComp:SetDisturbInterval(" << ((AbilityComponents::Water*)m_entity->GetComponents()->at(j))->m_interval << ");\n";
				m_file << "\t\twaterComp:SetRadius(" << ((AbilityComponents::Water*)m_entity->GetComponents()->at(j))->m_radius << ");\n";
			}
			else if (m_entity->GetComponents()->at(j)->m_type == AbilityComponents::ComponentType::SOUND)
			{
				if(((AbilityComponents::Sound*)m_entity->GetComponents()->at(j))->m_loop)
					m_file << "\t\tSoundSystem.PlayOnce(\"" << ((AbilityComponents::Sound*)m_entity->GetComponents()->at(j))->m_soundName << "\", " << ((AbilityComponents::Sound*)m_entity->GetComponents()->at(j))->m_volume << ", " << ((AbilityComponents::Sound*)m_entity->GetComponents()->at(j))->m_rangeMin << ", " << ((AbilityComponents::Sound*)m_entity->GetComponents()->at(j))->m_rangeMax << ", startPos);\n";
				else
				{
					m_file << "\t\tlocal soundComp = Soundable.New(self);\n";
					m_file << "\t\tlocal soundComp:SetSound(\"" << ((AbilityComponents::Sound*)m_entity->GetComponents()->at(j))->m_soundName << "\", bit32.bor(SoundMode.SOUND_LOOP_NORMAL, SoundMode.SOUND_3D, SoundMode.SOUND_3D_LINEARSQUAREROLLOFF));\n";
					m_file << "\t\tlocal soundComp:SetVolume(" << ((AbilityComponents::Sound*)m_entity->GetComponents()->at(j))->m_volume << ");\n";
					m_file << "\t\tlocal soundComp:SetRange(" << ((AbilityComponents::Sound*)m_entity->GetComponents()->at(j))->m_rangeMin << ", " << ((AbilityComponents::Sound*)m_entity->GetComponents()->at(j))->m_rangeMax << ");\n";
					m_file << "\t\tlocal soundComp:Play();\n";
				}
			}
		}
			m_file << "\tend\n";

		WriteNewAbilities(p_onCreate);

		m_file << "end\n";
	}

	void ScriptGenerator::WriteOnCollide( OnCreate* p_onCreate, OnCollide* p_onCollide)
	{

		int dmgEnum = 0, knockEnum = 0;
		//float dmg = 0.0f, knockb = 0.0f;

		m_file << "function " << m_name << ".OnCollide (self, entity)\n";
		m_file << "if entity:DoesExist() then\n";
		// TODO : Knockback, do damage, change stat

		for (unsigned int i = 0; i < m_entity->GetComponents()->size(); i++)
		{
			if (m_entity->GetComponents()->at(i)->m_type == AbilityComponents::ComponentType::DAMAGE)
			{
				dmgEnum = ((AbilityComponents::Damage*)m_entity->GetComponents()->at(i))->m_affectedPlayers;
				m_damage = ((AbilityComponents::Damage*)m_entity->GetComponents()->at(i))->m_damage;
			}
			else if (m_entity->GetComponents()->at(i)->m_type == AbilityComponents::ComponentType::KNOCKBACK)
			{
				knockEnum = ((AbilityComponents::Knockback*)m_entity->GetComponents()->at(i))->m_affectedPlayers;
				m_knockback = ((AbilityComponents::Knockback*)m_entity->GetComponents()->at(i))->m_knockback;
			}
		}

		m_file << "\tlocal hitCol = entity:GetCollision();\n";
		m_file << "\tlocal hitPhys = entity:GetPhysics();\n";
		m_file << "\tlocal type = hitPhys:GetType(hitCol);\n";

		if (m_damage != 0.0f || m_knockback != 0.0f)
		{
			m_file << "\tif type == PhysicsType.TYPE_PLAYER then\n";
				m_file << "\t\tlocal targetPlayerComponent = entity:GetPlayerComponent();\n";
				m_file << "\t\tlocal abilityOwnerNetwork = self:GetNetwork();\n";
				m_file << "\t\tlocal abilityOwnerId = abilityOwnerNetwork:GetUserId();\n";
				m_file << "\t\tlocal abilityOwnerEntity = Entity.GetEntityByNetworkID(abilityOwnerId, ReservedActionID.CONNECT, 0);\n";
				m_file << "\t\tlocal abilityOwnerPlayerComponent = abilityOwnerEntity:GetPlayerComponent();\n";

				//Do damage
				if (m_entity->DoesComponentExist(AbilityComponents::ComponentType::DAMAGE))
				{
					if(dmgEnum == AbilityComponents::Damage::ENEMIES)
						m_file << "\t\tif abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then\n";
					else if(dmgEnum == AbilityComponents::Damage::FRIENDLIES)
						m_file << "\t\tif abilityOwnerPlayerComponent:GetTeamId() == targetPlayerComponent:GetTeamId() then\n";
					else if(dmgEnum == AbilityComponents::Damage::EVERYONE)
						m_file << "\n"; //always

						m_file << "\t\t\tlocal health = entity:GetHealth();\n";
						m_file << "\t\t\tif not health:IsDead() then\n";
							m_file << "\t\t\t\tlocal network = entity:GetNetwork();\n";
							m_file << "\t\t\t\tlocal receiverId = network:GetUserId();\n";
							m_file << "\t\t\t\thealth:Damage(abilityOwnerId, " << m_name << ".damage, receiverId);\n";
						m_file << "\t\t\tend\n";
					if(dmgEnum == AbilityComponents::Damage::ENEMIES || dmgEnum == AbilityComponents::Damage::FRIENDLIES)
						m_file << "\t\tend\n";
				}
				//Knockback
				if (m_entity->DoesComponentExist(AbilityComponents::ComponentType::KNOCKBACK))
				{
					if(dmgEnum == AbilityComponents::Knockback::ENEMIES)
						m_file << "\t\tif abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then\n";
					else if(dmgEnum == AbilityComponents::Knockback::FRIENDLIES)
						m_file << "\t\tif abilityOwnerPlayerComponent:GetTeamId() == targetPlayerComponent:GetTeamId() then\n";
					else if(dmgEnum == AbilityComponents::Knockback::EVERYONE)
						m_file << "\n"; //always

						m_file << "\t\t\tlocal hitPos = entity:GetTransformation():GetPos();\n";
						m_file << "\t\t\tlocal selfPos = self:GetTransformation():GetPos();\n";
						m_file << "\t\t\thitPhys:KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), " << m_name << ".knockback);\n";

					if(dmgEnum == AbilityComponents::Knockback::ENEMIES || dmgEnum == AbilityComponents::Knockback::FRIENDLIES)
						m_file << "\t\tend\n";
				}
			m_file << "\tend\n";
		}

		WriteNewAbilities(p_onCollide);

		m_file << "end\n";
		m_file << "end\n";
	}

	void ScriptGenerator::WriteOnDestroy( OnDestroy* p_onDestroy)
	{
		m_file << "function " << m_name << ".OnDestroy (self)\n";

		//Remove stuff
		WriteNewAbilities(p_onDestroy);

		m_file << "\tEntity.Remove(self);\n";

		m_file << "end\n";
	}

	void ScriptGenerator::WriteNewAbilities( MainOnEvent* p_onEvent )
	{
		for (unsigned int i = 0; i < p_onEvent->GetConditions()->size(); i++)
		{
			m_file << "\t" << p_onEvent->GetConditions()->at(i)->GetCode().toStdString() << "\n";
			std::vector<QString> scriptNames = p_onEvent->GetEntityNames(i);
			for (unsigned int j = 0; j < scriptNames.size(); j++)
			{
				scriptNames.at(j).chop(4); 
				//m_file << "\t\trequire(" << scriptNames.at(j).toStdString() << ");\n";
				m_file << "\t\t" << scriptNames.at(j).toStdString() << ".OnCreate(network:GetUserId(), network:GetActionId());\n"; //TODO : Maybe take a closer look at this
			}
			m_file << "\tend\n";
		}
	}

}

