////////////////////////////////////////////////////////////////////////////////
// Filename: animatedmodelshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _ANIMATEDMODELSHADERCLASS_H_
#define _ANIMATEDMODELSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
using namespace DirectX;
using namespace std;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "animatedmodelclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: AnimatedModelShaderClass
////////////////////////////////////////////////////////////////////////////////
class AnimatedModelShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	AnimatedModelShaderClass();
	AnimatedModelShaderClass(const AnimatedModelShaderClass&);
	~AnimatedModelShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, AnimatedModelClass*, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	void RenderShader(ID3D11DeviceContext*, AnimatedModelClass*);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
};

#endif