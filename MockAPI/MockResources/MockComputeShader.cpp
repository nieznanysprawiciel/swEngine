/**
@file MockComputeShader.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/


#include "MockComputeShader.h"




RTTR_REGISTRATION
{
	rttr::registration::class_< sw::MockComputeShader >( "sw::MockComputeShader" );
}


namespace sw
{

// ================================ //
//
MockComputeShader::MockComputeShader()
{}

// ================================ //
//
MockComputeShader::~MockComputeShader()
{}

// ================================ //
//
bool MockComputeShader::ReloadFromFile()
{
	return false;
}

// ================================ //
//
bool MockComputeShader::ReloadFromBinFile()
{
	return false;
}

// ================================ //
//
void MockComputeShader::SaveShaderBinFile( const std::wstring& fileName )
{

}

// ================================ //
//
MockComputeShader* MockComputeShader::CreateFromFile( const std::wstring& fileName, const std::string& shaderName, const char* shaderModel )
{
	MockComputeShader* newShader = new MockComputeShader();
	return newShader;
}

MockComputeShader* MockComputeShader::CreateFromBinFile( const std::wstring& file_name )
{

	return nullptr;
}


}	// sw
