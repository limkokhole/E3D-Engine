﻿//
//  E3DVertexBufferManager.cpp
//
//  Created by 潘庆云 on 2017/7/19.
//

#include "E3DRenderManager.hpp"
#include "src/Source/EngineDelegate.h"
#include "../E3DRenderSystem.hpp"

namespace E3DEngine
{
	BatchRenderer * RendererManager::GetRenderer(int materialID, int vertexSize, RENDER_TYPE type, bool isStatic)
	{
		GetRenderSystem()->UseShareContext();
		BatchRenderer * buffer = nullptr;
		if (!isStatic)
		{
			buffer = newRenderer(type, materialID);
		}
		else
		{
			int rendererID = -1;
			if (m_mapMaterialID2RendererID.find(materialID) != m_mapMaterialID2RendererID.end())
			{
				rendererID = m_mapMaterialID2RendererID[materialID];
			}
			if (m_mapVertexBuffers.find(rendererID) == m_mapVertexBuffers.end())
			{
				buffer = newRenderer(type, materialID);
			}
			else
			{
				buffer = m_mapVertexBuffers[rendererID];
				if (buffer->GetVertextCount() + vertexSize > MAX_VERTEX_NUMBER || !buffer->IsStaticDraw)
				{
					buffer = newRenderer(type, materialID);
				}
			}
			m_mapMaterialID2RendererID[materialID] = buffer->ID;
		}
		buffer->IsStaticDraw = isStatic;
		GetRenderSystem()->UseRenderContext();
		return buffer;
	}
}
