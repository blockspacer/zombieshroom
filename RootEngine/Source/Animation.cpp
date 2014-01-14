#include <RootEngine/Include/Animation.h>
#include <iostream>
namespace RootEngine
{
	namespace RootAnimation
	{
		Animation::Animation() : m_numBones(0)
		{
		}

		Animation::~Animation()
		{
		}

		bool Animation::BoneExists( std::string p_boneName )
		{
			if(m_boneMapping.find(p_boneName) == m_boneMapping.end())
				return false;
			else
				return true;
		}
	
		void Animation::PushBoneInfo( RootEngine::RootAnimation::BoneInfo p_boneInfo )
		{
			m_boneInfo.push_back(p_boneInfo);
		}

		unsigned int Animation::GetNumBones()
		{
			return m_numBones;
		}

		void Animation::SetNumBones( unsigned int p_numBones )
		{
			m_numBones = p_numBones;
		}

		void Animation::MapBone( std::string p_boneName, unsigned int p_index )
		{
			m_boneMapping[p_boneName] = p_index;
		}

		unsigned int Animation::GetIndexFromBoneName( std::string p_boneName )
		{
			return m_boneMapping[p_boneName];
		}


		BoneInfo* Animation::GetBoneInfo( unsigned int p_index )
		{
			return &m_boneInfo[p_index];
		}

		void Animation::SetAiImporter( std::shared_ptr<Assimp::Importer> p_aiImporter )
		{
			m_aiImporter = p_aiImporter;
			aiMatrix4x4 am = m_aiImporter->GetScene()->mRootNode->mChildren[0]->mTransformation;
			glm::mat4x4 gm = glm::mat4x4();
			memcpy(&gm[0][0], &am[0][0], sizeof(aiMatrix4x4));
			m_globalInverseTransform = glm::transpose(gm);
		}

		const aiScene* Animation::GetScene()
		{
			return m_aiImporter->GetScene();
		}

		glm::mat4x4 Animation::GetGlobalInverseTransform()
		{
			return m_globalInverseTransform;
		}

		glm::mat4x4 Animation::GetBoneOffset( unsigned int p_index )
		{
			return m_boneInfo[p_index].m_boneOffset;
		}

		void Animation::SplitAnimation()
		{
			unsigned int startFrame = 0;
			double startTime = 0;
			unsigned int numKeyFrames = m_aiImporter->GetScene()->mAnimations[0]->mChannels[0]->mNumPositionKeys;
			for (unsigned int keyFrame = 0; keyFrame < numKeyFrames; keyFrame++)
			{
				bool tPose = true;
				for (unsigned int channel = 0; channel < m_aiImporter->GetScene()->mAnimations[0]->mNumChannels; channel++)
				{
					aiQuaternion rotkey = m_aiImporter->GetScene()->mAnimations[0]->mChannels[channel]->mRotationKeys[keyFrame].mValue;
					if (!(rotkey == aiQuaternion(0,0,0)))
					{
						tPose = false;
						break;
					}
				}
				if(tPose)
				{
					AnimClip clip;
					clip.m_startTime = startTime;
					clip.m_duration = m_aiImporter->GetScene()->mAnimations[0]->mChannels[0]->mRotationKeys[keyFrame - 1].mTime - startTime;
					clip.m_startFrame = startFrame;
					clip.m_stopFrame = keyFrame - 1;
					m_animClips.push_back(clip);
					std::cout << "Clip " << m_animClips.size() << ": " << "Start frame: " << clip.m_startFrame << " " << "Stop frame: " << clip.m_stopFrame << " " << "Start time: " << clip.m_startTime << " " << "Duration: " << clip.m_duration <<"\n";
					startTime = m_aiImporter->GetScene()->mAnimations[0]->mChannels[0]->mRotationKeys[keyFrame + 1].mTime;
					startFrame = keyFrame + 1;
				}
				
			}
		}

		AnimClip* Animation::GetAnimClip(unsigned int p_index)
		{
			return &m_animClips[p_index];
		}

	}
}