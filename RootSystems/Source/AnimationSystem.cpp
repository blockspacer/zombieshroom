#ifndef COMPILE_LEVEL_EDITOR
#include <AnimationSystem.h>

namespace RootForce
{
	void AnimationSystem::Init()
	{
		m_animations.Init(m_world->GetEntityManager());
		m_renderables.Init(m_world->GetEntityManager());
	}

	void AnimationSystem::Begin()
	{
	}

	void AnimationSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		
		if(m_logger)
		{
			Renderable* renderable = m_renderables.Get(p_entity);
			Animation* animation = m_animations.Get(p_entity);

			if(animation->UpperBodyAnim.m_animClip == AnimationClip::RAGDOLL || animation->LowerBodyAnim.m_animClip == AnimationClip::RAGDOLL)
				return;
			const aiScene* tempScene = renderable->m_model->m_animation->GetScene();

			float TicksPerSecond = (float)(tempScene->mAnimations[0]->mTicksPerSecond != 0 ? tempScene->mAnimations[0]->mTicksPerSecond : 25.0f);

			UpdateUpperBodyAnimation(renderable, animation, TicksPerSecond);
			UpdateLowerBodyAnimation(renderable, animation, TicksPerSecond);

			//Begin with lower body, which includes the hips(root node in animation)
			ReadNodeHeirarchyLower(tempScene->mRootNode, glm::mat4(1.0), animation, renderable, tempScene);
		}
	}

	//UPPER
	void AnimationSystem::UpdateUpperBodyAnimation( Renderable* p_renderable, Animation* p_animation, float p_ticksPerSecond )
	{
		if(p_animation->UpperBodyAnim.m_animClip != p_animation->UpperBodyAnim.m_prevAnimClip)
		{
			if(p_animation->UpperBodyAnim.m_locked == 2)
				p_animation->UpperBodyAnim.m_animClip = p_animation->UpperBodyAnim.m_prevAnimClip;
			else
			{
				std::cout << "UPPER ANIMATION CHANGED TO " << p_animation->UpperBodyAnim.m_animClip << std::endl;
				p_animation->UpperBodyAnim.m_animTime = 0.0f;
				p_animation->UpperBodyAnim.m_prevAnimClip = p_animation->UpperBodyAnim.m_animClip;
				p_animation->UpperBodyAnim.m_blending = true;
				p_animation->UpperBodyAnim.m_blendTime = 0.0f;
				if(p_animation->UpperBodyAnim.m_locked == 1)
					p_animation->UpperBodyAnim.m_locked = 2;
			}
		}

		if(!p_animation->UpperBodyAnim.m_blending)
		{
			p_animation->UpperBodyAnim.m_animTime += m_world->GetDelta();
			if(p_animation->UpperBodyAnim.m_locked == 2 && p_animation->UpperBodyAnim.m_animTime * p_ticksPerSecond >= (float)p_renderable->m_model->m_animation->GetAnimClip(p_animation->UpperBodyAnim.m_animClip)->m_duration)
			{
				p_animation->UpperBodyAnim.m_locked = 0;
				return;
			}
		}
		else
		{
			p_animation->UpperBodyAnim.m_blendTime += m_world->GetDelta();
			if (p_animation->UpperBodyAnim.m_blendTime >= m_blendTime)
			{
				p_animation->UpperBodyAnim.m_blending = false;
			}
		}

		float TimeInTicks		= p_animation->UpperBodyAnim.m_animTime * p_ticksPerSecond;
		m_upperAnimTime			= ((float)p_renderable->m_model->m_animation->GetAnimClip(p_animation->UpperBodyAnim.m_animClip)->m_startTime) + fmod(TimeInTicks, (float)p_renderable->m_model->m_animation->GetAnimClip(p_animation->UpperBodyAnim.m_animClip)->m_duration);
	}

	//LOWER
	void AnimationSystem::UpdateLowerBodyAnimation( Renderable* p_renderable, Animation* p_animation, float p_ticksPerSecond )
	{
		if(p_animation->LowerBodyAnim.m_animClip != p_animation->LowerBodyAnim.m_prevAnimClip)
		{
			if(p_animation->LowerBodyAnim.m_locked == 2)
				p_animation->LowerBodyAnim.m_animClip = p_animation->LowerBodyAnim.m_prevAnimClip;
			else
			{
				p_animation->LowerBodyAnim.m_animTime = 0.0f;
				p_animation->LowerBodyAnim.m_prevAnimClip = p_animation->LowerBodyAnim.m_animClip;
				p_animation->LowerBodyAnim.m_blending = true;
				p_animation->LowerBodyAnim.m_blendTime = 0.0f;
				if(p_animation->LowerBodyAnim.m_locked == 1)
					p_animation->LowerBodyAnim.m_locked = 2;
			}
		}

		if(!p_animation->LowerBodyAnim.m_blending)
		{
			p_animation->LowerBodyAnim.m_animTime += m_world->GetDelta();
			if(p_animation->LowerBodyAnim.m_locked == 2 && p_animation->LowerBodyAnim.m_animTime * p_ticksPerSecond >= (float)p_renderable->m_model->m_animation->GetAnimClip(p_animation->LowerBodyAnim.m_animClip)->m_duration)
			{
				p_animation->LowerBodyAnim.m_locked = 0;
				return;
			}
		}
		else
		{
			p_animation->LowerBodyAnim.m_blendTime += m_world->GetDelta();
			if (p_animation->LowerBodyAnim.m_blendTime >= m_blendTime)
			{
				p_animation->LowerBodyAnim.m_blending = false;
			}
		}

		float TimeInTicks		= p_animation->LowerBodyAnim.m_animTime * p_ticksPerSecond;
		m_lowerAnimTime			= ((float)p_renderable->m_model->m_animation->GetAnimClip(p_animation->LowerBodyAnim.m_animClip)->m_startTime) + fmod(TimeInTicks, (float)p_renderable->m_model->m_animation->GetAnimClip(p_animation->LowerBodyAnim.m_animClip)->m_duration);
	}

	

	void AnimationSystem::ReadNodeHeirarchyLower(const aiNode* pNode, const glm::mat4& ParentTransform, Animation* p_anim, Renderable* p_render, const aiScene* p_aiScene)
	{    
		std::string NodeName(pNode->mName.data);

		const aiAnimation* pAnimation = p_aiScene->mAnimations[0];

		aiMatrix4x4 am = pNode->mTransformation;
		glm::mat4x4 gm = glm::mat4x4();
		memcpy(&gm[0][0], &am[0][0], sizeof(aiMatrix4x4));
		glm::mat4 NodeTransformation(glm::transpose(gm));

		const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

		if (pNodeAnim) 
		{
			/*
			// Interpolate scaling and generate scaling transformation matrix
			aiVector3D Scaling;
			CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
			glm::mat4 ScalingM = glm::scale(glm::mat4(1.0f), glm::vec3(Scaling.x, Scaling.y, Scaling.z));*/
			// Interpolate rotation and generate rotation transformation matrix
			aiQuaternion RotationQ;
			if(p_anim->LowerBodyAnim.m_blending)
				CalcBlendedRotation(RotationQ, p_anim, pNodeAnim, p_render->m_model->m_animation->GetAnimClip(p_anim->LowerBodyAnim.m_animClip)->m_startFrame, p_anim->LowerBodyAnim.m_blendRot[pNodeAnim->mNodeName.C_Str()], p_anim->LowerBodyAnim.m_blendTime );
			else
			{
				CalcInterpolatedRotation(RotationQ, m_lowerAnimTime, pNodeAnim, p_anim);
				p_anim->LowerBodyAnim.m_blendRot[pNodeAnim->mNodeName.C_Str()] = RotationQ;
			}
			
			aiMatrix3x3 am3 = RotationQ.GetMatrix(); 
			glm::mat3 gm3 = glm::mat3();
			
			memcpy(&gm3[0][0], &am3[0][0], sizeof(aiMatrix3x3));

			glm::mat4 RotationM = glm::mat4(glm::transpose(gm3));

			// Interpolate translation and generate translation transformation matrix
			aiVector3D Translation;
			if(p_anim->LowerBodyAnim.m_blending)
				CalcBlendedPosition(Translation, p_anim, pNodeAnim, p_render->m_model->m_animation->GetAnimClip(p_anim->LowerBodyAnim.m_animClip)->m_startFrame, p_anim->LowerBodyAnim.m_blendPos[pNodeAnim->mNodeName.C_Str()], p_anim->LowerBodyAnim.m_blendTime);
			else
			{
				CalcInterpolatedPosition(Translation, m_lowerAnimTime, pNodeAnim, p_anim);
				p_anim->LowerBodyAnim.m_blendPos[pNodeAnim->mNodeName.C_Str()] = Translation;
			}
			
			glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), glm::vec3(Translation.x, Translation.y, Translation.z));

			// Combine the above transformations
			NodeTransformation =  TranslationM * RotationM;
		}
		glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

		if (p_render->m_model->m_animation->BoneExists(NodeName)) 
		{
			unsigned int BoneIndex = p_render->m_model->m_animation->GetIndexFromBoneName(NodeName);
			p_anim->m_bones[BoneIndex] = /*p_render->m_model->m_animation->GetGlobalInverseTransform() * */  GlobalTransformation * p_render->m_model->m_animation->GetBoneOffset(BoneIndex);
		}

		for (unsigned int i = 0 ; i < pNode->mNumChildren ; i++) 
		{
			if(i != 2)
				ReadNodeHeirarchyLower(pNode->mChildren[i], GlobalTransformation, p_anim, p_render, p_aiScene);
			else
				ReadNodeHeirarchyUpper(pNode->mChildren[i], GlobalTransformation, p_anim, p_render, p_aiScene);
		}
	}
	
	//Calc upper animation bones
	void AnimationSystem::ReadNodeHeirarchyUpper(const aiNode* pNode, const glm::mat4& ParentTransform, Animation* p_anim, Renderable* p_render, const aiScene* p_aiScene )
	{
		std::string NodeName(pNode->mName.data);

		const aiAnimation* pAnimation = p_aiScene->mAnimations[0];

		aiMatrix4x4 am = pNode->mTransformation;
		glm::mat4x4 gm = glm::mat4x4();
		memcpy(&gm[0][0], &am[0][0], sizeof(aiMatrix4x4));
		glm::mat4 NodeTransformation(glm::transpose(gm));

		const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

		if (pNodeAnim) 
		{
			/*
			// Interpolate scaling and generate scaling transformation matrix
			aiVector3D Scaling;
			CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
			glm::mat4 ScalingM = glm::scale(glm::mat4(1.0f), glm::vec3(Scaling.x, Scaling.y, Scaling.z));*/
			// Interpolate rotation and generate rotation transformation matrix
			aiQuaternion RotationQ;
			if(p_anim->UpperBodyAnim.m_blending)
				CalcBlendedRotation(RotationQ, p_anim, pNodeAnim, p_render->m_model->m_animation->GetAnimClip(p_anim->UpperBodyAnim.m_animClip)->m_startFrame, p_anim->UpperBodyAnim.m_blendRot[pNodeAnim->mNodeName.C_Str()], p_anim->UpperBodyAnim.m_blendTime);
			else
			{
				CalcInterpolatedRotation(RotationQ, m_upperAnimTime, pNodeAnim, p_anim);
				p_anim->UpperBodyAnim.m_blendRot[pNodeAnim->mNodeName.C_Str()] = RotationQ;
			}
			
			aiMatrix3x3 am3 = RotationQ.GetMatrix(); 
			glm::mat3 gm3 = glm::mat3();
			
			memcpy(&gm3[0][0], &am3[0][0], sizeof(aiMatrix3x3));

			glm::mat4 RotationM = glm::mat4(glm::transpose(gm3));

			// Interpolate translation and generate translation transformation matrix
			aiVector3D Translation;
			if(p_anim->UpperBodyAnim.m_blending)
				CalcBlendedPosition(Translation, p_anim, pNodeAnim, p_render->m_model->m_animation->GetAnimClip(p_anim->UpperBodyAnim.m_animClip)->m_startFrame, p_anim->UpperBodyAnim.m_blendPos[pNodeAnim->mNodeName.C_Str()], p_anim->UpperBodyAnim.m_blendTime);
			else
			{
				CalcInterpolatedPosition(Translation, m_upperAnimTime, pNodeAnim, p_anim);
				p_anim->UpperBodyAnim.m_blendPos[pNodeAnim->mNodeName.C_Str()] = Translation;
			}
			
			glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), glm::vec3(Translation.x, Translation.y, Translation.z));

			// Combine the above transformations
			NodeTransformation =  TranslationM * RotationM;
		}
		glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

		if (p_render->m_model->m_animation->BoneExists(NodeName)) 
		{
			unsigned int BoneIndex = p_render->m_model->m_animation->GetIndexFromBoneName(NodeName);
			p_anim->m_bones[BoneIndex] = /*p_render->m_model->m_animation->GetGlobalInverseTransform() * */  GlobalTransformation * p_render->m_model->m_animation->GetBoneOffset(BoneIndex);
		}

		for (unsigned int i = 0 ; i < pNode->mNumChildren ; i++) 
		{
			ReadNodeHeirarchyUpper(pNode->mChildren[i], GlobalTransformation, p_anim, p_render, p_aiScene);
		}
	}

	unsigned int AnimationSystem::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{    
		for (unsigned int i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) 
		{
			if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) 
			{
				return i;
			}
		}

		assert(0);

		return 0;
	}


	unsigned int AnimationSystem::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumRotationKeys > 0);

		for (unsigned int i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++) 
		{
			if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) 
			{
				return i;
			}
		}

		assert(0);

		return 0;
	}

	void AnimationSystem::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim, Animation* p_anim)
	{
		if (pNodeAnim->mNumPositionKeys == 1) 
		{
			Out = pNodeAnim->mPositionKeys[0].mValue;
			return;
		}

		unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
		unsigned int NextPositionIndex = (PositionIndex + 1);
		assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
		float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
		const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}


	void AnimationSystem::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim, Animation* p_anim)
	{
		// we need at least two values to interpolate...
		if (pNodeAnim->mNumRotationKeys == 1) 
		{
			Out = pNodeAnim->mRotationKeys[0].mValue;
			return;
		}

		unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
		unsigned int NextRotationIndex = (RotationIndex + 1);
		assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
		float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
		const aiQuaternion& EndRotationQ   = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;    
		aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
		Out = Out.Normalize();
	}

	const aiNodeAnim* AnimationSystem::FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
	{
		for (unsigned int i = 0 ; i < pAnimation->mNumChannels ; i++) 
		{
			const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

			if (std::string(pNodeAnim->mNodeName.data) == NodeName) 
			{
				return pNodeAnim;
			}
		}

		return NULL;
	}
	void AnimationSystem::End()
	{

	}

	void AnimationSystem::SetLoggingInterface(Logging::LoggingInterface* p_logger)
	{
		m_logger = p_logger;
	}

	void AnimationSystem::SetGameSharedContext( RootEngine::GameSharedContext* p_context )
	{
		m_context = p_context;
	}

	void AnimationSystem::CalcBlendedPosition( aiVector3D& Out, Animation* p_anim, const aiNodeAnim* pNodeAnim, unsigned int p_toKeyFrame, const aiVector3D& p_startPosInNewPose, float p_animBlendTime )
	{
		float Factor = p_animBlendTime / m_blendTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = p_startPosInNewPose;
		const aiVector3D& End = pNodeAnim->mPositionKeys[p_toKeyFrame].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}

	void AnimationSystem::CalcBlendedRotation( aiQuaternion& Out, Animation* p_anim, const aiNodeAnim* pNodeAnim, unsigned int p_toKeyFrame, const  aiQuaternion& p_startRotInNewPose, float p_animBlendTime )
	{
		float Factor = p_animBlendTime / m_blendTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiQuaternion& StartRotationQ = p_startRotInNewPose;
		const aiQuaternion& EndRotationQ   = pNodeAnim->mRotationKeys[p_toKeyFrame].mValue;    
		aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
		Out = Out.Normalize();
	}


}
#endif