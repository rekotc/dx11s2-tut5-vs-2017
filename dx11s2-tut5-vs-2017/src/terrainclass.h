////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
#include <stdio.h>
using namespace DirectX;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////
class TerrainClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		//non pi� utilizzato TODO remove
		/*XMFLOAT4 color;*/
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct HeightMapType
	{
		float x, y, z;
		float nx, ny, nz;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType
	{
		float x, y, z;
	};

public:
	TerrainClass();
	TerrainClass(const TerrainClass&);
	~TerrainClass();

	bool Initialize(ID3D11Device*, const std::string&);
	void Shutdown();
	bool Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:

	bool LoadSetupFile(const std::string& filename);
	bool LoadBitmapHeightMap();
	void ShutdownHeightMap();
	void SetTerrainCoordinates();

	bool CalculateNormals();
	bool BuildTerrainModel();
	void ShutdownTerrainModel();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	int m_terrainHeight, m_terrainWidth;
	float m_heightScale;
	char* m_terrainFilename;
	HeightMapType* m_heightMap;
	ModelType* m_terrainModel;
};

#endif