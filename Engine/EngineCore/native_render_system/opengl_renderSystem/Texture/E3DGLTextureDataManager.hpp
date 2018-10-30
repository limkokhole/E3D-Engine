﻿//
//  TextureDataManager.hpp
//
//  Created by 潘庆云 on 2017/5/24.
//

#ifndef GL_TextureDataManager_HPP
#define GL_TextureDataManager_HPP

#include <src/RenderSystem/Texture/E3DTextureDataManager.hpp>
#include "../Include/Include.h"

namespace E3DEngine
{
	class GL_TextureDataManager : public TextureDataManager
	{
	public:
		virtual unsigned int GetTextureBuffer(std::string imageName) override;
		virtual Render2Texture* CreateRender2Texture(float width, float height) override;
		virtual void Cleanup() override;
	};
}

#endif /* TextureDataManager_hpp */