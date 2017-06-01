/**
@file MockRenderTarget.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "MockRenderTarget.h"
#include "MockTexture.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< sw::MockRenderTarget >( "sw::MockRenderTarget" );
}


namespace sw
{


/**@brief */
MockRenderTarget::~MockRenderTarget()
{}



// ================================ //
//
MockRenderTarget::MockRenderTarget( TextureObject* colorBuffer,
									TextureObject* depthBuffer,
									TextureObject* stencilBuffer )
	: RenderTargetObject( colorBuffer, depthBuffer, stencilBuffer )
	, m_height( 0 )
	, m_width( 0 )
{}

// ================================ //
//
MockRenderTarget*		MockRenderTarget::CreateScreenRenderTarget()
{
	MockRenderTarget* newRenderTarget = new MockRenderTarget( nullptr, nullptr, nullptr );

	//auto viewPort = DX11APIObjects::get_viewport_desc();
	//newRenderTarget->SetHeight( static_cast<uint16>( viewPort.Height ) );
	//newRenderTarget->SetWidth( static_cast<uint16>( viewPort.Width ) );

	return newRenderTarget;
}

// ================================ //
//
MockRenderTarget*		MockRenderTarget::CreateRenderTarget( const std::wstring& name, const RenderTargetDescriptor& renderTargetDescriptor )
{
	if( !ValidateDescriptor( renderTargetDescriptor ) )
		return nullptr;

	TextureInfo colorBufferInfo = renderTargetDescriptor.CreateTextureInfo();
	TextureInfo depthBufferInfo = renderTargetDescriptor.CreateTextureInfo();
	TextureInfo stencilBufferInfo = renderTargetDescriptor.CreateTextureInfo();


	colorBufferInfo.FilePath = name + RENDER_TARGET_COLOR_BUFFER_NAME;
	colorBufferInfo.Format = renderTargetDescriptor.ColorBuffFormat;
	depthBufferInfo.FilePath = name + RENDER_TARGET_DEPTH_BUFFER_NAME;
	//depthBufferInfo.Format = renderTargetDescriptor.DepthStencilFormat;
	stencilBufferInfo.FilePath = name + RENDER_TARGET_STENCIL_BUFFER_NAME;
	//stencilBufferInfo.Format = renderTargetDescriptor.DepthStencilFormat;

	TextureObject* colorBufferTex = new MockTexture( std::move( colorBufferInfo ) );
	TextureObject* depthBufferTex = new MockTexture( std::move( depthBufferInfo ) );
	TextureObject* stencilBufferTex = nullptr;
	if( renderTargetDescriptor.DepthStencilFormat == DepthStencilFormat::DEPTH_STENCIL_FORMAT_D24_UNORM_S8_UINT ||
		renderTargetDescriptor.DepthStencilFormat == DepthStencilFormat::DEPTH_STENCIL_FORMAT_D32_FLOAT_S8X24_UINT )
		stencilBufferTex = new MockTexture( std::move( stencilBufferInfo ) );

	MockRenderTarget* newRenderTarget = new MockRenderTarget( colorBufferTex, depthBufferTex, stencilBufferTex );
	newRenderTarget->SetHeight( renderTargetDescriptor.TextureHeight );
	newRenderTarget->SetWidth( renderTargetDescriptor.TextureWidth );

	return newRenderTarget;
}

// ================================ //
//
bool MockRenderTarget::ValidateDescriptor( const RenderTargetDescriptor& renderTargetDescriptor )
{
	TextureType RTType = renderTargetDescriptor.TextureType;
	if( RTType == TextureType::TEXTURE_TYPE_TEXTURE1D ||
		RTType == TextureType::TEXTURE_TYPE_TEXTURE1D_ARRAY ||
		RTType == TextureType::TEXTURE_TYPE_TEXTURE3D ||
		RTType == TextureType::TEXTURE_TYPE_BUFFER )
		return false;

	return true;
}
}	// sw
