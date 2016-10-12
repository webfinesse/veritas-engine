#ifndef H_EXTERNALDIRECTXVARIABLES
#define H_EXTERNALDIRECTXVARIABLES

#include <wrl\client.h>
#include <d3d11.h>

using namespace Microsoft::WRL;

extern ComPtr<ID3D11Device> g_device;
extern ComPtr<IDXGISwapChain> g_swapChain;
extern ComPtr<ID3D11DeviceContext> g_context;
extern ComPtr<ID3D11RenderTargetView> g_renderTargetView;
extern ComPtr<ID3D11DepthStencilView> g_depthStencilView;

#endif
