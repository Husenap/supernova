#pragma once

#include "WindowHandler.h"

#include "../../utils/math/Vector.h"
#include "../../utils/math/Matrix.h"

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct IDXGIAdapter;
struct ID3D11Texture2D;
struct D3D11_VIEWPORT;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;
struct ID3D11DepthStencilState;
struct ID3D11BlendState;
struct ID3D11RasterizerState;

class CDirectXFramework {
public:
	CDirectXFramework();
	~CDirectXFramework();

	bool Init(CWindowHandler& aWindowHandler);

	void Present();

	ID3D11Device* GetDevice() { return mDevice; }
	ID3D11DeviceContext* GetContext() { return mContext; }

private:
	struct ConstantBufferData {
		float mTime, t0, t1, t2;
	} mConstantBufferData;

void UpdateConstantBuffer();

private:
	void CreateDeviceAndSwapChain(CWindowHandler& aWindowHandler);
	void CreateBackbuffer();
	void CreateQuad();
	void CreateConstantBuffers();
	void CreateShaders();

	IDXGISwapChain* mySwapChain;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mContext;
	ID3D11Texture2D* m_backbuffer;
	ID3D11RenderTargetView* m_rendertarget;
	D3D11_VIEWPORT* m_viewport;

	ID3D11VertexShader* myVertexShader;
	ID3D11InputLayout* myInputLayout;
	ID3D11PixelShader* myPixelShader;
	ID3D11Buffer* mIndexBuffer;
	ID3D11Buffer* mVertexBuffer;
	unsigned int mStride;
	unsigned int mOffset;

	ID3D11Buffer* mConstantBuffer;
};
