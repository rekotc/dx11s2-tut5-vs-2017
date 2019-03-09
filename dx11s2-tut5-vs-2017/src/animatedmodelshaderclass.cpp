////////////////////////////////////////////////////////////////////////////////
// Filename: animatedmodelshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "animatedmodelshaderclass.h"


AnimatedModelShaderClass::AnimatedModelShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
}


AnimatedModelShaderClass::AnimatedModelShaderClass(const AnimatedModelShaderClass& other)
{
}


AnimatedModelShaderClass::~AnimatedModelShaderClass()
{
}


bool AnimatedModelShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, L"Engine/data/shaders/model.vs", L"Engine/data/shaders/model.ps");
	if(!result)
	{
		return false;
	}

	return true;
}


void AnimatedModelShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}


bool AnimatedModelShaderClass::Render(ID3D11DeviceContext* deviceContext, AnimatedModelClass* model, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
							  XMMATRIX projectionMatrix)
{
	bool result;


	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if(!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, model);

	return true;
}


bool AnimatedModelShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	//D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	//unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT numElements = ARRAYSIZE(layout);


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

    // Compile the vertex shader code.
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "ModelVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
								&vertexShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

    // Compile the pixel shader code.
	result = D3DCompileFromFile(psFilename, NULL, NULL, "ModelPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
								&pixelShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

    // Create the vertex shader from the buffer.
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(result))
	{
		return false;
	}

    // Create the pixel shader from the buffer.
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if(FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout.
	result = device->CreateInputLayout(layout, numElements, vertexShaderBuffer->GetBufferPointer(), 
									   vertexShaderBuffer->GetBufferSize(), &m_layout);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

    // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}


void AnimatedModelShaderClass::ShutdownShader()
{
	// Release the matrix constant buffer.
	if(m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the layout.
	if(m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if(m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if(m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	return;
}


void AnimatedModelShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}


bool AnimatedModelShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
										   XMMATRIX projectionMatrix)
{
	HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;


	// Transpose the matrices to prepare them for the shader.
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// Lock the matrix constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the matrix constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the matrix constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;
	
	// Unlock the matrix constant buffer.
    deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the matrix constant buffer in the vertex shader.
	bufferNumber = 0;
	// Finanly set the matrix constant buffer in the vertex shader with the updated values.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	bufferNumber = 1;
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	//d3d11DevCon->PSSetConstantBuffers(1, 1, &cbPerObjectBuffer);

	return true;
}


void AnimatedModelShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, AnimatedModelClass* model)
{



	for (int i = 0; i < model->GetNumSubsets(); i++)
	{

	AnimatedModelClass::ModelSubset temp = model->GetSubset(i);
	////Set the WVP matrix and send it to the constant buffer in effect file
	//WVP = smilesWorld * camView * camProjection;
	//cbPerObj.WVP = XMMatrixTranspose(WVP);
	//cbPerObj.World = XMMatrixTranspose(smilesWorld);
	//cbPerObj.hasTexture = false;        // We'll assume all md5 subsets have textures
	//cbPerObj.hasNormMap = false;    // We'll also assume md5 models have no normal map (easy to change later though)
	////deviceContext->UpdateSubresource(m_texture, 0, NULL, m_targaData, rowPitch, 0);
	////d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	////d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	////d3d11DevCon->PSSetConstantBuffers(1, 1, &cbPerObjectBuffer);
	////d3d11DevCon->PSSetShaderResources(0, 1, &meshSRV[NewMD5Model.subsets[i].texArrayIndex]);
	////d3d11DevCon->PSSetSamplers(0, 1, &CubesTexSamplerState);

	////d3d11DevCon->RSSetState(RSCullNone);
	///*ID3D11Buffer* cbPerObjectBuffer;*/


	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

    // Set the vertex and pixel shaders that will be used to do the rendering.
    deviceContext->VSSetShader(m_vertexShader, NULL, 0);
    deviceContext->PSSetShader(m_pixelShader, NULL, 0);
	deviceContext->DrawIndexed(	temp.indices->size(), 0, 0);

	}



	//// Set the vertex input layout.
	//deviceContext->IASetInputLayout(m_layout);

 //   // Set the vertex and pixel shaders that will be used to do the rendering.
 //   deviceContext->VSSetShader(m_vertexShader, NULL, 0);
 //   deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	//// Render the data.
	//deviceContext->DrawIndexed(indexCount, 0, 0);



	return;
}


//
//
/////////////////**************new**************////////////////////
/////***Draw MD5 Model***///
//for (int i = 0; i < NewMD5Model.numSubsets; i++)
//{
//	//Set the grounds index buffer
//	d3d11DevCon->IASetIndexBuffer(NewMD5Model.subsets[i].indexBuff, DXGI_FORMAT_R32_UINT, 0);
//	//Set the grounds vertex buffer
//	d3d11DevCon->IASetVertexBuffers(0, 1, &NewMD5Model.subsets[i].vertBuff, &stride, &offset);
//
//	//Set the WVP matrix and send it to the constant buffer in effect file
//	WVP = smilesWorld * camView * camProjection;
//	cbPerObj.WVP = XMMatrixTranspose(WVP);
//	cbPerObj.World = XMMatrixTranspose(smilesWorld);
//	cbPerObj.hasTexture = true;        // We'll assume all md5 subsets have textures
//	cbPerObj.hasNormMap = false;    // We'll also assume md5 models have no normal map (easy to change later though)
//	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
//	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
//	d3d11DevCon->PSSetConstantBuffers(1, 1, &cbPerObjectBuffer);
//	d3d11DevCon->PSSetShaderResources(0, 1, &meshSRV[NewMD5Model.subsets[i].texArrayIndex]);
//	d3d11DevCon->PSSetSamplers(0, 1, &CubesTexSamplerState);
//
//	d3d11DevCon->RSSetState(RSCullNone);
//	d3d11DevCon->DrawIndexed(NewMD5Model.subsets[i].indices.size(), 0, 0);
//
//}
/////////////////**************new**************////////////////////
//
///////Draw our model's NON-transparent subsets/////
//for (int i = 0; i < meshSubsets; ++i)
//{
//	//Set the grounds index buffer
//	d3d11DevCon->IASetIndexBuffer(meshIndexBuff, DXGI_FORMAT_R32_UINT, 0);
//	//Set the grounds vertex buffer
//	d3d11DevCon->IASetVertexBuffers(0, 1, &meshVertBuff, &stride, &offset);
//
//	//Set the WVP matrix and send it to the constant buffer in effect file
//	WVP = meshWorld * camView * camProjection;
//	cbPerObj.WVP = XMMatrixTranspose(WVP);
//	cbPerObj.World = XMMatrixTranspose(meshWorld);
//	cbPerObj.difColor = material[meshSubsetTexture[i]].difColor;
//	cbPerObj.hasTexture = material[meshSubsetTexture[i]].hasTexture;
//	cbPerObj.hasNormMap = material[meshSubsetTexture[i]].hasNormMap;
//	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
//	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
//	d3d11DevCon->PSSetConstantBuffers(1, 1, &cbPerObjectBuffer);
//	if (material[meshSubsetTexture[i]].hasTexture)
//		d3d11DevCon->PSSetShaderResources(0, 1, &meshSRV[material[meshSubsetTexture[i]].texArrayIndex]);
//	if (material[meshSubsetTexture[i]].hasNormMap)
//		d3d11DevCon->PSSetShaderResources(1, 1, &meshSRV[material[meshSubsetTexture[i]].normMapTexArrayIndex]);
//	d3d11DevCon->PSSetSamplers(0, 1, &CubesTexSamplerState);
//
//	d3d11DevCon->RSSetState(RSCullNone);
//	int indexStart = meshSubsetIndexStart[i];
//	int indexDrawAmount = meshSubsetIndexStart[i + 1] - meshSubsetIndexStart[i];
//	if (!material[meshSubsetTexture[i]].transparent)
//		d3d11DevCon->DrawIndexed(indexDrawAmount, indexStart, 0);
//}
//
			///////Draw the Sky's Sphere//////
			////Set the spheres index buffer
			//d3d11DevCon->IASetIndexBuffer(sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			////Set the spheres vertex buffer
			//d3d11DevCon->IASetVertexBuffers(0, 1, &sphereVertBuffer, &stride, &offset);
			//
			////Set the WVP matrix and send it to the constant buffer in effect file
			//WVP = sphereWorld * camView * camProjection;
			//cbPerObj.WVP = XMMatrixTranspose(WVP);
			//cbPerObj.World = XMMatrixTranspose(sphereWorld);
			//d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
			//d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
			////Send our skymap resource view to pixel shader
			//d3d11DevCon->PSSetShaderResources(0, 1, &smrv);
			//d3d11DevCon->PSSetSamplers(0, 1, &CubesTexSamplerState);
			//
			////Set the new VS and PS shaders
			//d3d11DevCon->VSSetShader(SKYMAP_VS, 0, 0);
			//d3d11DevCon->PSSetShader(SKYMAP_PS, 0, 0);
			////Set the new depth/stencil and RS states
			//d3d11DevCon->OMSetDepthStencilState(DSLessEqual, 0);
			//d3d11DevCon->RSSetState(RSCullNone);
			//d3d11DevCon->DrawIndexed(NumSphereFaces * 3, 0, 0);
			//
			////Set the default VS, PS shaders and depth/stencil state
			//d3d11DevCon->VSSetShader(VS, 0, 0);
			//d3d11DevCon->PSSetShader(PS, 0, 0);
			//d3d11DevCon->OMSetDepthStencilState(NULL, 0);
			//
///////Draw our model's TRANSPARENT subsets now/////
////Set our blend state
//d3d11DevCon->OMSetBlendState(Transparency, NULL, 0xffffffff);
//
//for (int i = 0; i < meshSubsets; ++i)
//{
//	//Set the grounds index buffer
//	d3d11DevCon->IASetIndexBuffer(meshIndexBuff, DXGI_FORMAT_R32_UINT, 0);
//	//Set the grounds vertex buffer
//	d3d11DevCon->IASetVertexBuffers(0, 1, &meshVertBuff, &stride, &offset);
//
//	//Set the WVP matrix and send it to the constant buffer in effect file
//	WVP = meshWorld * camView * camProjection;
//	cbPerObj.WVP = XMMatrixTranspose(WVP);
//	cbPerObj.World = XMMatrixTranspose(meshWorld);
//	cbPerObj.difColor = material[meshSubsetTexture[i]].difColor;
//	cbPerObj.hasTexture = material[meshSubsetTexture[i]].hasTexture;
//	cbPerObj.hasNormMap = material[meshSubsetTexture[i]].hasNormMap;
//	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
//	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
//	d3d11DevCon->PSSetConstantBuffers(1, 1, &cbPerObjectBuffer);
//	if (material[meshSubsetTexture[i]].hasTexture)
//		d3d11DevCon->PSSetShaderResources(0, 1, &meshSRV[material[meshSubsetTexture[i]].texArrayIndex]);
//	if (material[meshSubsetTexture[i]].hasNormMap)
//		d3d11DevCon->PSSetShaderResources(1, 1, &meshSRV[material[meshSubsetTexture[i]].normMapTexArrayIndex]);
//	d3d11DevCon->PSSetSamplers(0, 1, &CubesTexSamplerState);
//
//	d3d11DevCon->RSSetState(RSCullNone);
//	int indexStart = meshSubsetIndexStart[i];
//	int indexDrawAmount = meshSubsetIndexStart[i + 1] - meshSubsetIndexStart[i];
//	if (material[meshSubsetTexture[i]].transparent)
//		d3d11DevCon->DrawIndexed(indexDrawAmount, indexStart, 0);
//}