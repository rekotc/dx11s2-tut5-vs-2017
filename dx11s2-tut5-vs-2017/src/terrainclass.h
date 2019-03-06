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

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "terraincellclass.h"
#include "frustumclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////
class TerrainClass
{
private:
	//struct VertexType
	//{
	//	XMFLOAT3 position;
	//	//non più utilizzato TODO remove
	//	/*XMFLOAT4 color;*/
	//	XMFLOAT3 color;
	//	XMFLOAT2 texture;
	//	XMFLOAT3 normal;
	//	XMFLOAT3 tangent;
	//	XMFLOAT3 binormal;
	//};

	struct HeightMapType
	{
		float x, y, z;
		float nx, ny, nz;
		float r, g, b;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		float tx, ty, tz;
		float bx, by, bz;
		float r, g, b;
	};

	struct VectorType
	{
		float x, y, z;
	};

	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	TerrainClass();
	TerrainClass(const TerrainClass&);
	~TerrainClass();

	bool Initialize(ID3D11Device*, const std::string&);
	void Shutdown();
	//bool Render(ID3D11DeviceContext*);
	void Frame();

	//int GetIndexCount();

	/*bool RenderCell(ID3D11DeviceContext*, int);*/
	bool RenderCell(ID3D11DeviceContext*, int, FrustumClass*);
	void RenderCellLines(ID3D11DeviceContext*, int);

	int GetCellIndexCount(int);
	int GetCellLinesIndexCount(int);
	int GetCellCount();

	int GetRenderCount();
	int GetCellsDrawn();
	int GetCellsCulled();

private:

	bool LoadSetupFile(const std::string& filename);
	bool LoadBitmapHeightMap();
	void ShutdownHeightMap();
	void SetTerrainCoordinates();
	bool CalculateNormals();

	bool LoadColorMap();
	bool BuildTerrainModel();
	void ShutdownTerrainModel();

	void CalculateTerrainVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);

	//bool InitializeBuffers(ID3D11Device*);
	//void ShutdownBuffers();
	//void RenderBuffers(ID3D11DeviceContext*);

	bool LoadRawHeightMap();

	bool LoadTerrainCells(ID3D11Device*);
	void ShutdownTerrainCells();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	int m_terrainHeight, m_terrainWidth;
	float m_heightScale;
	HeightMapType* m_heightMap;
	ModelType* m_terrainModel;

	char *m_terrainFilename, *m_colorMapFilename;
	TerrainCellClass* m_TerrainCells;

	int m_cellCount, m_renderCount, m_cellsDrawn, m_cellsCulled;
};

#endif