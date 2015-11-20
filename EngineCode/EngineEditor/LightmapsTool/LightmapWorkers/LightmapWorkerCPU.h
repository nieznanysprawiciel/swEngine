#pragma once

#include "EngineEditor/LightmapsTool/LightmapWorker.h"
#include <DirectXMath.h>

struct Triangle4
{
	DirectX::XMVECTOR	vertex1;
	DirectX::XMVECTOR	vertex2;
	DirectX::XMVECTOR	vertex3;

	Triangle4() = default;
	inline Triangle4( DirectX::XMFLOAT3* trianglePtr )
	{
		vertex1 = XMLoadFloat3( trianglePtr++ );
		vertex2 = XMLoadFloat3( trianglePtr++ );
		vertex3 = XMLoadFloat3( trianglePtr++ );
	}
};

struct Triangle3
{
	DirectX::XMFLOAT3	vertex1;
	DirectX::XMFLOAT3	vertex2;
	DirectX::XMFLOAT3	vertex3;
};

class LightmapWorkerCPU	:	public LightmapWorker
{
private:
	float			m_threshold;
protected:
public:
	LightmapWorkerCPU( SceneData* sceneData );
	~LightmapWorkerCPU() = default;

	/**@brief G��wna funkcja generuj�ca lightmapy. Zaimplementuj w klasie pochodnej.*/
	virtual void	Generate() override;
private:
	void			Prepare		( std::vector<MemoryChunk>& emisionLight, std::vector<MemoryChunk>& reachedLight, std::vector<MemoryChunk>& verticies );
	void			Radiosity	( std::vector<MemoryChunk>& emisionLight, std::vector<MemoryChunk>& reachedLight, std::vector<MemoryChunk>& verticies );
	void			BuildResult	( std::vector<MemoryChunk>& emisionLight, std::vector<MemoryChunk>& reachedLight, std::vector<MemoryChunk>& verticies );

	DirectX::XMVECTOR									HemisphereRatio		( Triangle4& emiter, Triangle4& receiver );

	std::tuple<unsigned int, unsigned int, float>		FindMaxEmision		( std::vector<MemoryChunk>& emisionLight );
};

