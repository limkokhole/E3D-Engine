﻿//
//  E3DVertexBuffer.cpp
//
//  Created by 潘庆云 on 2017/7/17.
//

#include "E3DGLESRenderManager.hpp"
#include <src/Camera/E3DCamera.h>
#include <src/Object/E3DTransform.hpp>
#include <src/RenderSystem/E3DRenderSystem.hpp>
#include <src/Scene/E3DSceneManager.hpp>
#include <src/Light/E3DLight.hpp>

namespace E3DEngine
{
	
	void GLES_Renderer::prepareRender(float deltaTime)
	{
		if (pMaterial == nullptr)
		{
			return;
		}
		pMaterial->UseProgram();

		m_nIndexSize = (uint)Indices.size();
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		
		if (EnableDepthTest)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
		}
		if (pMaterial->blendType == eBlendType_One)
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		}
        else if(pMaterial->blendType == eBlendType_Text)
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
		else
		{
#ifdef __IOS__
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
#else
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif // __IOS__
			
		}
//
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_ALPHA);

	}

	void GLES_Renderer::TransformChange()
	{
		UINT drawState = GL_STREAM_DRAW;
		if (IsStaticDraw)
		{
			drawState = GL_STATIC_DRAW;
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* Vertices.size(), Vertices.data(), drawState);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)* Indices.size(), Indices.data(), drawState);
	}

	void GLES_Renderer::ClearVertexIndexBuffer()
	{
		UINT drawState = GL_STREAM_DRAW;
		if (IsStaticDraw)
		{
			drawState = GL_STATIC_DRAW;
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		if (!m_bIsBufferData)
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* Vertices.size(), nullptr, drawState);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* Indices.size(), nullptr, drawState);
		}
	}


	void GLES_Renderer::SetDrawModule(DWORD module)
	{
		switch (module)
		{
		case eDM_LINE_STRIP:
			m_nDrawModule = GL_LINE_STRIP;
			break;
		case eDM_TRIANGLE_STRIP:
			m_nDrawModule = GL_TRIANGLE_STRIP;
			break;
		case eDM_TRIANGLES:
			m_nDrawModule = GL_TRIANGLES;
			break;
		case  eDM_LINES:
			m_nDrawModule = GL_LINES;
			break;
		case eDM_POINTS:
			m_nDrawModule = GL_POINTS;
			break;
		default:
			m_nDrawModule = GL_TRIANGLES;
			break;
		}
	}

	void GLES_Renderer::Render(float deltaTime)
	{
		prepareRender(deltaTime);
		if (pMaterial == nullptr)
		{
			return;
		}

		if (Vertices.empty())
		{
			return;
		}
		updateEngineDefineShaderValue();

		pMaterial->UseMaterial();
		// 绘制图形
		glDrawElements(m_nDrawModule, (int)m_nIndexSize, GL_UNSIGNED_SHORT, nullptr);
		int err = glGetError();
		afterRender(deltaTime);
	}

	void GLES_Renderer::updateEngineDefineShaderValue()
	{
		pMaterial->pShader->UpdateMatrix4Value(PROJ_MATRIX, pCamera->GetProjectionMatrix());
		pMaterial->pShader->UpdateMatrix4Value(VIEW_MATRIX, pCamera->GetViewMatrix());

		pMaterial->pShader->UpdateMatrix4Value(MODEL_MATRIX, GetTransform()->WorldMatrix);
		pMaterial->pShader->UpdateFloatValue(ROTATION_VEC, GetTransform()->RotationEuler.x  * M_PI / 180, GetTransform()->RotationEuler.y * M_PI / 180, GetTransform()->RotationEuler.z * M_PI / 180);

		DirectionLight * dlight = (DirectionLight *)SceneManager::GetInstance().GetCurrentScene()->GetDirectionalLight();
		if (dlight != nullptr)
		{
			pMaterial->pShader->UpdateFloatValue(LIGHT_COLOR, dlight->Color.r, dlight->Color.g, dlight->Color.b, dlight->Color.a);
			pMaterial->pShader->UpdateFloatValue(LIGHT_DIR, dlight->Transform->Position.x, dlight->Transform->Position.y, dlight->Transform->Position.z);
		}

		std::map<UINT, Light*>& plights = SceneManager::GetInstance().GetCurrentScene()->GetPointLights();
		if (plights.size() != 0)
		{
			std::vector<float> lightPos;
			std::vector<float> lightColor;
			std::vector<float> lightRange;
			for (auto & pl : plights)
			{
				lightPos.emplace_back(pl.second->Transform->Position.x);
				lightPos.emplace_back(pl.second->Transform->Position.y);
				lightPos.emplace_back(pl.second->Transform->Position.z);
				lightColor.emplace_back(pl.second->Color.r);
				lightColor.emplace_back(pl.second->Color.g);
				lightColor.emplace_back(pl.second->Color.b);
				lightColor.emplace_back(pl.second->Color.a);
				lightRange.emplace_back(static_cast<PointLight*>(pl.second)->Range);
			}
			pMaterial->pShader->UpdataFloat3ArrayUniform(POINT_LIGHT_POS, lightPos);
			pMaterial->pShader->UpdataFloat4ArrayUniform(POINT_LIGHT_COLOR, lightColor);
			pMaterial->pShader->UpdataFloat1ArrayUniform(POINT_LIGHT_RANGE, lightRange);
		}
		if (pCamera != nullptr)
		{
			vec3f &pos = pCamera->Transform->Position;
			pMaterial->pShader->UpdateFloatValue(CAMERA_POS, pos.x, pos.y, pos.z);
		}
	}

	void GLES_Renderer::afterRender(float deltaTime)
	{
		if (pMaterial == nullptr)
		{
			return;
		}
		pMaterial->InvalidMaterial();
		glDisable(GL_STENCIL_TEST);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}


}
