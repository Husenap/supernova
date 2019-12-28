#include "DirectXFramework.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include "../precompiled.h"

CDirectXFramework::CDirectXFramework() {
	mContext = NULL;
	mDevice = NULL;
	mySwapChain = NULL;
	m_backbuffer = NULL;
	m_rendertarget = NULL;
	m_viewport = NULL;

}

CDirectXFramework::~CDirectXFramework() {
	if (mContext) mContext->Release();
	if (mDevice) mDevice->Release();
	if (mySwapChain) mySwapChain->Release();
	if (m_backbuffer) m_backbuffer->Release();
	if (m_rendertarget) m_rendertarget->Release();
	delete m_viewport;
}

bool CDirectXFramework::Init(CWindowHandler& aWindowHandler) {
	CreateDeviceAndSwapChain(aWindowHandler);
	CreateBackbuffer();
	CreateQuad();
	CreateConstantBuffers();
	CreateShaders();

	return true;
}

void CDirectXFramework::CreateDeviceAndSwapChain(CWindowHandler& aWindowHandler) {
	DXGI_SWAP_CHAIN_DESC swapchainDesc;
	ZeroMemory(&swapchainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapchainDesc.BufferCount = 1;
	swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.BufferDesc.Width = WIDTH;
	swapchainDesc.BufferDesc.Height = HEIGHT;
	swapchainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.OutputWindow = aWindowHandler.GetWindowHandle();
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.Windowed = true;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifndef _RETAIL
	// If the project is in a debug build, enable the debug layer.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Create swapchain, device and context
	D3D11CreateDeviceAndSwapChain(NULL,
								  D3D_DRIVER_TYPE_HARDWARE,
								  NULL,
								  creationFlags,
								  NULL,
								  NULL,

								  D3D11_SDK_VERSION,
								  &swapchainDesc,
								  &mySwapChain,
								  &mDevice,
								  nullptr,
								  &mContext);
}

void CDirectXFramework::CreateBackbuffer() {
	mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_backbuffer);

	mDevice->CreateRenderTargetView(m_backbuffer, nullptr, &m_rendertarget);

	D3D11_TEXTURE2D_DESC textureDesc;
	m_backbuffer->GetDesc(&textureDesc);

	m_viewport = new D3D11_VIEWPORT();
	m_viewport->TopLeftX = 0.f;
	m_viewport->TopLeftY = 0.f;
	m_viewport->Width = static_cast<float>(textureDesc.Width);
	m_viewport->Height = static_cast<float>(textureDesc.Height);
	m_viewport->MinDepth = 0.f;
	m_viewport->MaxDepth = 1.0f;
}

void CDirectXFramework::CreateQuad() {
	struct Vertex {
		float x, y, z, w;
		float u, v;
	} vertices[4] = {{-1.f, +1.f, 0.f, 1.f, 0.f, 0.f},
					 {+1.f, +1.f, 0.f, 1.f, 1.f, 0.f},
					 {-1.f, -1.f, 0.f, 1.f, 0.f, 1.f},
					 {+1.f, -1.f, 0.f, 1.f, 1.f, 1.f}};
	unsigned int indices[6] = {0, 1, 2, 1, 3, 2};

	// Creating Vertex Buffer
	D3D11_BUFFER_DESC vertexBufferDesc = {0};
	vertexBufferDesc.ByteWidth = sizeof(vertices);
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexBufferData = {0};
	vertexBufferData.pSysMem = vertices;

	mDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &mVertexBuffer);

	// Creating Index Buffer
	D3D11_BUFFER_DESC indexBufferDesc = {0};
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufferData = {0};
	indexBufferData.pSysMem = indices;

	mDevice->CreateBuffer(&indexBufferDesc, &indexBufferData, &mIndexBuffer);

	mStride = sizeof(Vertex);
	mOffset = 0;
}

void CDirectXFramework::CreateConstantBuffers() {
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(mConstantBufferData);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	mDevice->CreateBuffer(&bufferDesc, nullptr, &mConstantBuffer);
}

void CDirectXFramework::Present() {
	mContext->RSSetViewports(1, m_viewport);
	mContext->OMSetRenderTargets(1, &m_rendertarget, NULL);

	mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &mStride, &mOffset);
	mContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	mContext->VSSetShader(myVertexShader, NULL, NULL);
	mContext->IASetInputLayout(myInputLayout);
	mContext->PSSetShader(myPixelShader, NULL, NULL);

	UpdateConstantBuffer();

	mContext->DrawIndexed(6, 0, 0);

	mySwapChain->Present(1, 0);
}

void CDirectXFramework::UpdateConstantBuffer() {
	static __int64 startCounter;
	static BOOL _startCounterResult = QueryPerformanceCounter((LARGE_INTEGER*)&startCounter);
	__int64 counter, frequency;
	QueryPerformanceCounter((LARGE_INTEGER*)&counter);
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	float t = float((counter - startCounter) / double(frequency));

	mConstantBufferData.mTime = t;

	D3D11_MAPPED_SUBRESOURCE data;
	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	mContext->Map(mConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	memcpy_s(data.pData, sizeof(ConstantBufferData), &mConstantBufferData, sizeof(ConstantBufferData));
	mContext->Unmap(mConstantBuffer, 0);

	mContext->VSSetConstantBuffers(0, 1, &mConstantBuffer);
	mContext->PSSetConstantBuffers(0, 1, &mConstantBuffer);
}

enum EShaderProfile { EShaderProfile_Vertex, EShaderProfile_Pixel, EShaderProfile_Count };
const char* shaderProfiles[EShaderProfile_Count] = {"vs_5_0", "ps_5_0"};
ID3DBlob* _CompileShader(const std::wstring& aShaderFile,
						 EShaderProfile aProfile,
						 D3D_SHADER_MACRO* aMacros = NULL) {
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

	LPCSTR profile;
	profile = shaderProfiles[aProfile];

	ID3DBlob* shaderBlob = NULL;
	ID3DBlob* errorBlob = NULL;
	HRESULT result = D3DCompileFromFile(aShaderFile.c_str(),
										aMacros,
										D3D_COMPILE_STANDARD_FILE_INCLUDE,
										"main",
										profile,
										flags,
										0,
										&shaderBlob,
										&errorBlob);

	if (FAILED(result)) {
		std::string shaderFile(aShaderFile.begin(), aShaderFile.end());
		if (errorBlob) {
			std::string errorText(static_cast<char*>(errorBlob->GetBufferPointer()));
			ERROR_LOG("%s", errorText.c_str());
			errorBlob->Release();
		}

		if (shaderBlob) {
			shaderBlob->Release();
			shaderBlob = NULL;
		}

		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND) ||
			result == HRESULT_FROM_WIN32(ERROR_PATH_NOT_FOUND)) {
			ERROR_LOG("Failed to find shader file: %s", shaderFile.c_str());
		}

		ERROR_LOG("Failed to compile shader: &s", shaderFile.c_str());
		// Fail
	}

	return shaderBlob;
}
void CDirectXFramework::CreateShaders() {
	ID3DBlob* vertexBlob = _CompileShader(L"assets/shaders/Main.vs", EShaderProfile_Vertex);

	// clang-format off
	mDevice->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), NULL, &myVertexShader);
	D3D11_INPUT_ELEMENT_DESC layout[2] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	// clang-format on

	mDevice->CreateInputLayout(layout,
							   sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC),
							   vertexBlob->GetBufferPointer(),
							   vertexBlob->GetBufferSize(),
							   &myInputLayout);

	ID3DBlob* pixelBlob = _CompileShader(L"assets/shaders/Main.ps", EShaderProfile_Pixel);
	mDevice->CreatePixelShader(
		pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), NULL, &myPixelShader);

	vertexBlob->Release();
	pixelBlob->Release();
}
