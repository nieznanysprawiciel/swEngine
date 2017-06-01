#pragma once
/**
@file MockBuffer.h
@author nieznanysprawiciel
@copyright Plik jest częścią silnika graficznego SWEngine.
*/


#include "swGraphicAPI/Resources/MeshResources.h"


namespace sw
{

/**@brief 
@ingroup MockAPI*/
class MockBuffer : public BufferObject
{
	RTTR_ENABLE( BufferObject );
private:

	BufferInfo					m_descriptor;

protected:
	~MockBuffer();
public:
	MockBuffer( const std::wstring& name, const BufferInfo& descriptor );


	static MockBuffer*			CreateFromMemory( const std::wstring& name, const uint8* data, const BufferInfo& bufferInfo );

	virtual MemoryChunk			CopyData		() override;
	virtual const BufferInfo&	GetDescriptor	() const { return m_descriptor; }
};

}	// sw
