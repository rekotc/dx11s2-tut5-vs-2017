////////////////////////////////////////////////////////////////////////////////
// Filename: animatedmodelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _ANIMATEDMODELCLASS_H_
#define _ANIMATEDMODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <d3d11.h>
#include <directxmath.h>
//#include <fstream>
#include<string>
#include<vector>
#include<fstream>
#include<istream>

using namespace DirectX;
using namespace std;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
//#include "textureclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: AnimatedModelClass
////////////////////////////////////////////////////////////////////////////////
class AnimatedModelClass
{
private:

	struct Joint
	{
		std::wstring name;
		int parentID;

		XMFLOAT3 pos;
		XMFLOAT4 orientation;
	};

	struct Weight
	{
		int jointID;
		float bias;
		XMFLOAT3 pos;
	};

	struct Vertex    //Overloaded Vertex Structure
	{
		Vertex() {}
		Vertex(float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz,
			float tx, float ty, float tz)
			: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz),
			tangent(tx, ty, tz) {}

		XMFLOAT3 pos;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 biTangent;

		///////////////**************new**************////////////////////
			// Will not be sent to shader
		int StartWeight;
		int WeightCount;
		///////////////**************new**************////////////////////
	};

public:
	  
	struct ModelSubset
	{
		int texArrayIndex;
		int numTriangles;

		std::vector<Vertex>* vertices = new std::vector<Vertex>;
		std::vector<DWORD>* indices = new std::vector<DWORD>;
		std::vector<Weight>* weights = new std::vector<Weight>;

		std::vector<XMFLOAT3> positions;

		ID3D11Buffer* vertBuff;
		ID3D11Buffer* indexBuff;
	};

	AnimatedModelClass();
	AnimatedModelClass(const AnimatedModelClass&);
	~AnimatedModelClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const char*, const char*, float, int);
	void Shutdown();
	bool Render(ID3D11DeviceContext*);
	int GetIndexCount();
	int GetNumSubsets();
	AnimatedModelClass::ModelSubset GetSubset(int);

private:

	struct Model3D
	{
		int numSubsets;
		int numJoints;

		std::vector<Joint> joints;
		std::vector<ModelSubset>* subsets = new std::vector<ModelSubset>;
	};

	bool LoadMD5Model(std::wstring filename,
		Model3D* MD5Model,
		//std::vector<ID3D11ShaderResourceView*>& shaderResourceViewArray,
		std::vector<std::wstring> texFileNameArray);
	bool InitializeBuffers(ID3D11Device*);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
	//int m_numSubsets;
	//int m_numJoints;
	int m_indexCount;

	std::vector<std::wstring> m_textureNameArray;

	//std::vector<Joint> m_joints;
	//std::vector<ModelSubset> m_subsets;

	Model3D* m_MD5Model;


};
#endif