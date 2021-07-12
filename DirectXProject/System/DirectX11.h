#pragma once
//=============================================
// DirectXクラス
//---------------------------------------------
// 制作：みやしろ　ゆきひろ
//---------------------------------------------
// DirectX11.h
//=============================================
// ---　インクルード部　---
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <d3dcompiler.h>

// ライブラリの追加
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// 長文対策
using namespace DirectX;

// マクロ定義
#define SAFE_RELEASE(x)	if(x){ x = nullptr; }

// 定数バッファの定義
struct cbNeverChanges
{
	XMFLOAT4X4 Projection;		// 透視変換行列
};

struct cbChangesEveryFrame
{
	XMFLOAT4X4 View;	// ビュー変換行列
	XMFLOAT3 Light;		// 光源座標
	FLOAT dummy;		// 定数バッファのサイズを16の倍数にするためのダミー
};

struct cbChangesEveryObject
{
	XMFLOAT4X4 World;
};


// ==== DirectX11クラス ====
class DX11
{
public:
	// 定数
	const float SCREEN_WIDTH = 1024.0f;
	const float SCREEN_HEIGHT = 576.0f;

private:
	HWND hWnd;
	LPCSTR ErrorWndName;

	//D3D_FEATURE_LEVEL m_pFeatureLevels[3] =
	//{
	//	D3D_FEATURE_LEVEL_11_0,
	//	D3D_FEATURE_LEVEL_10_1,
	//	D3D_FEATURE_LEVEL_10_0,
	//};
	UINT m_FeatureLevels = 3;		// 配列の要素数
	D3D_FEATURE_LEVEL m_FeatureLevelsSupported;		// デバイス作成時に返される機能レベル

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;

	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11Texture2D* m_pDepthStencil;
	ID3D11DepthStencilView* m_pDepthStencilView;

	ID3D11Buffer* m_pVertexBuffer[2] =		// 頂点バッファのインターフェイス
	{
		NULL,
		NULL
	};

	ID3D11Buffer* m_pIdxBuffer = NULL;
	ID3D11VertexShader* m_pVertexShader;		// 頂点シェーダ
	//ID3D11GeometryShader* m_pGeometryShader;	// ジオメトリシェーダ
	ID3D11PixelShader* m_pPixelShader;			// ピクセルシェーダ

	struct cbNeverChanges m_cbNeverChanges;
	struct cbChangesEveryFrame m_cbChangesEveryFrame;
	struct cbChangesEveryObject m_cbChangesEveryObject;

	ID3D11Buffer* m_pCBuffer[3] =
	{
		NULL,
		NULL,
		NULL
	};


protected:


public:
	DX11();
	~DX11();

	HRESULT Init(HWND wnd);		// 初期化処理
	void	Uninit();				// 終了処理
	HRESULT Update();
	void	Draw();					// 描画処理

	HRESULT InitDevice(HWND wnd);	// デバイス関連の初期化
	HRESULT	InitView();				// ビュー関連初期化
	HRESULT InitShader();			// シェーダー関連初期化
	HRESULT InitBuffer();			// バッファ関連初期化
	void	InitMatrix();			// マトリックス関連初期化

	ID3D11Device*		GetDevice(void) { return m_pDevice; }		// デバイスの取得
	ID3D11DeviceContext*	GetContext(void) { return m_pDeviceContext; }		// デバイスコンテキストの取得
	IDXGISwapChain*			GetSwapChain(void) { return m_pSwapChain; }

};