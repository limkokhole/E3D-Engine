﻿//
//  Material.cpp
//
//  Created by 潘庆云 on 2017/3/30.
//

#include "E3DGLESMaterial.hpp"
#include <src/Source/Application.h>
#include <src/Source/E3DVertex.h>
#include <src/Source/Helpers.h>

namespace E3DEngine
{
	const char * lightShader = "";

	GLES_Material::GLES_Material()
	{
		
	}

	void GLES_Material::Destory()
	{
		for (std::map<UINT, Texture*>::iterator it = Textures.begin(); it != Textures.end(); ++it)
		{
			SAFE_DELETE(it->second);
		}
		SAFE_DELETE(pShader);
		Textures.clear();
	}

	void GLES_Material::SetTexture(Texture * texture, int index)
	{
		if (Textures.find(index) != Textures.end() && texture != Textures[index])
		{
			SAFE_DELETE(Textures[index]);
		}
		UINT textureIndex = GL_TEXTURE0 + index;
		Textures[index] = texture;
		static_cast<GLES_Texture*>(texture)->SetTextureUniformName(static_cast<GLES_Texture*>(texture)->m_strTextureUniformName);
		static_cast<GLES_Texture*>(texture)->SetTextureEnum(textureIndex);
		static_cast<GLES_Texture*>(texture)->SetTextureUniformIndex(index, static_cast<GLES_Shader*>(pShader)->ShaderProgram);
		
	}

	void GLES_Material::createTexture(TextureData& data)
	{
		GLES_Texture * texture = new GLES_Texture();
		std::string path = filePath;
		path = path + data.fileName;
		int textureSum = Textures.size();
		texture->Create(path, data);
		texture->SetTextureEnum(GL_TEXTURE0 + textureSum);
		texture->SetTextureUniformName(data.uniformName);
		texture->SetTextureUniformIndex(textureSum, static_cast<GLES_Shader*>(pShader)->ShaderProgram);
		Textures[textureSum] = ((E3DEngine::Texture*)texture);
	}

	void GLES_Material::createTexture(Texture *texture, std::string textureUniform)
	{
		int textureSum = Textures.size();
		texture->SetTextureEnum(GL_TEXTURE0 + textureSum);
		static_cast<GLES_Texture*>(texture)->SetTextureUniformName(textureUniform);
		static_cast<GLES_Texture*>(texture)->SetTextureUniformIndex(textureSum, static_cast<GLES_Shader*>(pShader)->ShaderProgram);
		Textures[textureSum] = ((E3DEngine::Texture*)texture);
	}

	void GLES_Material::BindTexture()
	{
		for (auto & it : Textures)
		{
			if (it.second == nullptr)
			{
				continue;
			}
			it.second->ActiveBindTexture();
		}
	}

	void GLES_Material::UseProgram()
	{
		if (pShader == nullptr)
			return;
		static_cast<GLES_Shader*>(pShader)->UseProgram();
	}

	void GLES_Material::UseNullProgram()
	{
		if (pShader == nullptr)
			return;
		static_cast<GLES_Shader*>(pShader)->UseNullProgram();
	}

	void GLES_Material::CreateCubeTexture(std::string dirPath, std::string xPName, std::string xNName, std::string yPName, std::string yNName, std::string zPName, std::string ZNName)
	{

	}

	void GLES_Material::UseMaterial()
	{
		UseProgram();
		openState();
		for(auto & it : Textures)
		{
			if (it.second == nullptr)
			{
				continue;
			}
			it.second->ActiveBindTexture();
		}
		if (pShader != nullptr)
		{
			static_cast<GLES_Shader*>(pShader)->UpdateProgramUniformValue();
		}
	}

    void GLES_Material::openState()
	{
		if (enableDepthTest)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
        //深度测试
        if(enablewriteDepth)
		{
			glDepthMask(GL_TRUE);
        }
        else
		{
			glDepthMask(GL_FALSE);
        }       

        if(enableDoubleSide)
        {
            glDisable(GL_CULL_FACE);
        }
        else
        {
			glEnable(GL_CULL_FACE);
        }		

		if (blendType == BLEND_TYPE::ONE_BLEND_ONE)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		}
		else if(blendType == BLEND_TYPE::SRCALPHA_ONEMINUSSRCALPHA)
		{
			glEnable(GL_BLEND);
#ifdef __IOS__
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
#else
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif // __IOS__
		}	 
		else
		{
			glDisable(GL_BLEND);
		}
		glEnable(GL_ALPHA);

        glEnable(GL_STENCIL_TEST);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        
        if(enableStencilTest)
        {
            enableStencil();
        }
    }

    void GLES_Material::enableStencil()
    {
        glDepthMask(GL_TRUE);//启用写入深度值
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
        glStencilMask(0xFF);
    }


	void GLES_Material::InvalidMaterial()
	{
		for (auto & it : Textures)
		{
			if (it.second == nullptr)
			{
				continue;
			}
			it.second->InvalidTexture();
		}
		UseNullProgram();
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	void GLES_Material::UpdateShader(unsigned int vertexType)
	{
		static_cast<GLES_Shader*>(pShader)->UpdateAttribPointerValue(vertexType);
	}

}
