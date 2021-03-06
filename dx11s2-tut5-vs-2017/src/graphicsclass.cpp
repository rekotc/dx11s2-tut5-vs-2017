//////////////////////////////////////////////////////////////////////////////////
//// Filename: graphicsclass.cpp
//////////////////////////////////////////////////////////////////////////////////
//#include "graphicsclass.h"
//
//
//GraphicsClass::GraphicsClass()
//{
//	m_Direct3D = 0;
//	m_Camera = 0;
//	m_Model = 0;
//	m_TextureShader = 0;
//	m_LightShader = 0;
//	m_Light = 0;
//}
//
//
//GraphicsClass::GraphicsClass(const GraphicsClass& other)
//{
//}
//
//
//GraphicsClass::~GraphicsClass()
//{
//}
//
//
//bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
//{
//	bool result;
//
//
//	// Create the Direct3D object.
//	m_Direct3D = new D3DClass;
//	if(!m_Direct3D)
//	{
//		return false;
//	}
//
//	// Initialize the Direct3D object.
//	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
//	if(!result)
//	{
//		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
//		return false;
//	}
//
//	// Create the camera object.
//	m_Camera = new CameraClass;
//	if (!m_Camera)
//	{
//		return false;
//	}
//
//	// Set the initial position of the camera.
//	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
//
//	// Create the model object.
//	m_Model = new ModelClass;
//	if (!m_Model)
//	{
//		return false;
//	}
//
//	//std::string buf(config::project_directory);
//	//buf.append("/stone01.tga");
//	std::string filename = "/stone01.tga";
//	std::string path = config::textures_folder + filename;
//	// Initialize the model object.	
//	result = m_Model->InitializeS(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), path );
//	if (!result)
//	{
//		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
//		return false;
//	}
//
//	// create the texture shader object.
//	//m_textureshader = new textureshaderclass;
//	//if (!m_textureshader)
//	//{
//	//	return false;
//	//}
//
//	// initialize the color shader object.
//	//result = m_textureshader->initialize(m_direct3d->getdevice(), hwnd);
//	//if (!result)
//	//{
//	//	messagebox(hwnd, l"could not initialize the color shader object.", l"error", mb_ok);
//	//	return false;
//	//}
//
//		// Create the light shader object.
//	m_LightShader = new LightShaderClass;
//	if (!m_LightShader)
//	{
//		return false;
//	}
//
//	// Initialize the light shader object.
//	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
//	if (!result)
//	{
//		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
//		return false;
//	}
//
//	//The new light object is created here.
//
//		// Create the light object.
//		m_Light = new LightClass;
//	if (!m_Light)
//	{
//		return false;
//	}
//
//	//The color of the light is set to purple and the light direction is set to point down the positive Z axis.
//
//		// Initialize the light object.
//		m_Light->SetDiffuseColor(1.0f, 0.0f, 1.0f, 1.0f);
//	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
//
//	return true;
//}
//
//
//void GraphicsClass::Shutdown()
//{
//	// Release the light object.
//	if (m_Light)
//	{
//		delete m_Light;
//		m_Light = 0;
//	}
//
//	// Release the light shader object.
//	if (m_LightShader)
//	{
//		m_LightShader->Shutdown();
//		delete m_LightShader;
//		m_LightShader = 0;
//	}
//
//	// Release the texture shader object.
//	if (m_TextureShader)
//	{
//		m_TextureShader->Shutdown();
//		delete m_TextureShader;
//		m_TextureShader = 0;
//	}
//
//	// Release the model object.
//	if (m_Model)
//	{
//		m_Model->Shutdown();
//		delete m_Model;
//		m_Model = 0;
//	}
//
//	// Release the camera object.
//	if (m_Camera)
//	{
//		delete m_Camera;
//		m_Camera = 0;
//	}
//
//	// Release the D3D object.
//	if(m_Direct3D)
//	{
//		m_Direct3D->Shutdown();
//		delete m_Direct3D;
//		m_Direct3D = 0;
//	}
//
//	return;
//}
//
//
//bool GraphicsClass::Frame()
//{
//	static float rotation = 0.0f;
//
//
//	// Update the rotation variable each frame.
//	//rotation += (float)XM_PI * 0.01f;
//
//	rotation += 1*(XM_PI / 180);
//	if (rotation > 360.0f)
//	{
//		rotation -= 360.0f;
//	}
//
//	bool result;
//
//
//	// Render the graphics scene.
//	result = Render(rotation);
//	if(!result)
//	{
//		return false;
//	}
//
//	return true;
//}
//
//
//bool GraphicsClass::Render(float rotation)
//{
//	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
//	bool result;
//
//
//	// Clear the buffers to begin the scene.
//	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
//
//	// Generate the view matrix based on the camera's position.
//	m_Camera->Render();
//
//	// Get the world, view, and projection matrices from the camera and d3d objects.
//	m_Direct3D->GetWorldMatrix(worldMatrix);
//	m_Camera->GetViewMatrix(viewMatrix);
//	m_Direct3D->GetProjectionMatrix(projectionMatrix);
//
//	// Rotate the world matrix by the rotation value so that the triangle will spin.
//	//XMMatrixRotationY(&worldMatrix, rotation);
//	worldMatrix = worldMatrix * XMMatrixRotationY(rotation);
//
//	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
//	m_Model->Render(m_Direct3D->GetDeviceContext());
//
//	//// Render the model using the texture shader.
//	//result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture());
//	//if (!result)
//	//{
//	//	return false;
//	//}
//
//	// Render the model using the light shader.
//	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
//		m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());
//	if (!result)
//	{
//		return false;
//	}
//
//	// Present the rendered scene to the screen.
//	m_Direct3D->EndScene();
//
//	return true;
//}