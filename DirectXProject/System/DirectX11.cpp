//=============================================
// DirectXクラス
//---------------------------------------------
// 制作：みやしろ　ゆきひろ
//---------------------------------------------
// DirectX11.cpp
//=============================================
// ---　インクルード部　---
#include "DirectX11.h"

//// 静的メンバ宣言
//ID3D11Device* DX11::m_pDevice;
//ID3D11DeviceContext* DX11::m_pDeviceContext;

// === 構造体定義 ===
struct XYZBuffer
{
	XMFLOAT3 Pos;
};

struct ColBuffer
{
	XMFLOAT3 Color;
};

DX11::DX11()
{
	ErrorWndName = "DX_Init";

	m_FeatureLevels = 3;		// 配列の要素数

	m_pDevice = nullptr;
	m_pDeviceContext = nullptr;
	m_pSwapChain = nullptr;

	m_pRenderTargetView = nullptr;
	m_pDepthStencil = nullptr;
	m_pDepthStencilView = nullptr;

	m_pIdxBuffer = nullptr;
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
}

DX11::~DX11()
{
	Uninit();
}

// ===== 初期化処理 =====
HRESULT DX11::Init(HWND wnd)
{
	HRESULT hr;

	hWnd = wnd;

	// ドライバー種別を定義
	std::vector<D3D_DRIVER_TYPE> driverTypes
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
		D3D_DRIVER_TYPE_SOFTWARE
	};

	// スワップチェインの作成
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));							// スワップチェインの初期化
	scd.BufferDesc.Width = SCREEN_WIDTH;					// バッファの横幅
	scd.BufferDesc.Height = SCREEN_HEIGHT;					// バッファの縦幅
	scd.BufferDesc.RefreshRate.Numerator = 60;				// リフレッシュレートの分母
	scd.BufferDesc.RefreshRate.Denominator = 1;				// リフレッシュレートの分子
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// バッファのフォーマット
	scd.SampleDesc.Count = 1;								// 1ピクセルの色を決めるサンプリング数
	scd.SampleDesc.Quality = 0;								// サンプリングの品質
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// 
	scd.BufferCount = 1;									// 
	scd.OutputWindow = hWnd;								// 
	scd.Windowed = TRUE;									// 

	// ドライバ種別を上から検証し選択
	// 選択したデバイスを用いて描画する
	for (size_t i = 0; i < driverTypes.size(); ++i)
	{
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,								// 使用するディスプレイアダプタの指定
			driverTypes[i],							// 描画を行うドライバの指定
			nullptr,								// ドライバの指定をソフトウェアにした場合にのみ指定が必要
			0,										// ランタイムレイヤのフラグ指定
			nullptr,								// FeatureLevelの指定
			0,										// D3D_FEATURE_LEVELを指定した場合の配列の要素数
			D3D11_SDK_VERSION,						// DirectSDKのバージョン
			&scd,									// 設定を行ったDXGI_SWAP_CHAIN_DESCのアドレス
			&m_pSwapChain,							// 初期化が完了したIDXGISwapChainを受け取るためのポインタアドレス
			&m_pDevice,								// 初期化が完了したID3D11Deviceを受け取るためのポインタアドレス
			nullptr,								// 決まったD3D_FEATURE_LEVELを取得するためのアドレス
			&m_pDeviceContext						// 作成されたID3D11DeviceContextを受け取るためのポインタアドレス
		);
		// 成功したらループを抜ける
		if (SUCCEEDED(hr))
		{
			break;
		}
	}
	// 失敗したら終了する
	if (FAILED(hr))
	{
		// メッセージを表示
		MessageBox(NULL, "DirectX11に対応していないデバイスです。", ErrorWndName, MB_OK | MB_ICONERROR);

		return E_FAIL;
	}

	//---------------------------------------------------------------------------------------------------
	// スワップ・チェインから最初のバック・バッファを取得する
	ID3D11Texture2D *pBackBuffer = nullptr;	// バッファに￥のアクセスに使うインターフェイス
	hr = m_pSwapChain->GetBuffer(
		0,									// バックバッファの番号
		__uuidof(ID3D11Texture2D),			// バッファにアクセスするインターフェイス
		(LPVOID*)&pBackBuffer				// バッファを受け取る変数
	);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// バック・バッファの描画ターゲット・ビューを作る
	hr = m_pDevice->CreateRenderTargetView(
		pBackBuffer,
		NULL,
		&m_pRenderTargetView
	);
	
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	
	m_pDeviceContext->OMSetRenderTargets(
		1,
		&m_pRenderTargetView,
		nullptr
	);

	D3D11_VIEWPORT vp;
	// ビューポートの設定
	vp.TopLeftX = 0.0f;			// ビューポート領域の左上X座標
	vp.TopLeftY = 0.0f;			// ビューポート領域の左上Y座標
	vp.Width = SCREEN_WIDTH;		// ビューポート領域の幅
	vp.Height = SCREEN_HEIGHT;	// ビューポート領域の高さ
	vp.MinDepth = 0.0f;			// ビューポート領域の深度値の最小値
	vp.MaxDepth = 1.0f;			// ビューポート領域の深度値の最大値
	m_pDeviceContext->RSSetViewports(1, &vp);
	//---------------------------------------------------------------------------------------------------
	// 深度/ステンシル・テクスチャの作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = SCREEN_WIDTH;
	descDepth.Height = SCREEN_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = m_pDevice->CreateTexture2D(
		&descDepth,
		NULL,
		&m_pDepthStencil
	);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	//---------------------------------------------------------------------------------------------------
	// 深度/ステンシル・ビューの作成
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;

	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_pDevice->CreateDepthStencilView(
		m_pDepthStencil,
		&descDSV,
		&m_pDepthStencilView
	);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_pDeviceContext->OMSetRenderTargets(
		1,
		&m_pRenderTargetView,
		m_pDepthStencilView
	);

	//------------------------------------------------------------------------
	//IDXGIOutput* pOutput;
	//hr=m_pSwapChain->GetContainingOutput(&pOutput);
	//if (FAILED(hr))
	//{
	//	// メッセージを表示
	//	MessageBox(NULL, "IDXXGIOutputインターフェイスの取得に失敗しました", ErrorWndName, MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}

	//DXGI_GAMMA_CONTROL_CAPABILITIES gammaCap;
	//hr = pOutput->GetGammaControlCapabilities(&gammaCap);
	//if (FAILED(hr))
	//{
	//	// メッセージを表示
	//	MessageBox(NULL, "DirectX11に対応していないデバイスです。", ErrorWndName, MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}

	//float gamma = 1.0f / 2.2f;

	//// トーンカーブを設定
	//DXGI_GAMMA_CONTROL gammaControl;
	//gammaControl.Scale.Red = 1.0f;
	//gammaControl.Scale.Green = 1.0f;
	//gammaControl.Scale.Blue = 1.0f;
	//gammaControl.Offset.Red = 0.0f;
	//gammaControl.Offset.Green = 0.0f;
	//gammaControl.Offset.Blue = 0.0f;
	//for (UINT i = 0; i < gammaCap.NumGammaControlPoints; ++i)
	//{
	//	float L0 = gammaCap.ControlPointPositions[i];
	//	float L1 = pow(L0, gamma);
	//	gammaControl.GammaCurve[i].Red = L1;
	//	gammaControl.GammaCurve[i].Green = L1;
	//	gammaControl.GammaCurve[i].Blue = L1;
	//}
	//hr = pOutput->SetGammaControl(&gammaControl);
	//if (FAILED(hr))
	//{
	//	return E_FAIL;
	//}

	//------------------------------------------------------------------------
	// 座標用頂点バッファを作成
	D3D11_BUFFER_DESC xyzBufferDesc;
	xyzBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	xyzBufferDesc.ByteWidth = sizeof(XYZBuffer) * 8;
	xyzBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	xyzBufferDesc.CPUAccessFlags = 0;
	xyzBufferDesc.MiscFlags = 0;
	xyzBufferDesc.StructureByteStride = 0;

	// 座標用頂点バッファのサブリソースの初期値設定
	struct XYZBuffer posVertex[] =
	{
		XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(1.0f,  1.0f, -1.0f),
		XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f),
		XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(1.0f,  1.0f,  1.0f),
		XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(-1.0f, -1.0f,  1.0f),
	};

	// 座標用頂点バッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA xyzSubData;
	xyzSubData.pSysMem = posVertex;
	xyzSubData.SysMemPitch = 0;
	xyzSubData.SysMemSlicePitch = 0;

	// 座標用頂点バッファの作成
	hr = m_pDevice->CreateBuffer(
		&xyzBufferDesc,
		&xyzSubData,
		&m_pVertexBuffer[0]
	);
	if (FAILED(hr))
	{
		// メッセージを表示
		MessageBox(NULL, "座標用頂点バッファの作成に失敗しました。", ErrorWndName, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// 色情報用頂点バッファの定義
	D3D11_BUFFER_DESC colorBufferDesc;
	colorBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	colorBufferDesc.ByteWidth = sizeof(ColBuffer) * 8;
	colorBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	colorBufferDesc.CPUAccessFlags = 0;
	colorBufferDesc.MiscFlags = 0;
	colorBufferDesc.StructureByteStride = 0;

	// 色情報用頂点バッファのサブリソースの初期値設定
	struct ColBuffer ColorVertex[] =
	{
		XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
	};

	// 色情報用頂点バッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA ColorSubData;
	ColorSubData.pSysMem = ColorVertex;
	ColorSubData.SysMemPitch = 0;
	ColorSubData.SysMemSlicePitch = 0;

	// 色情報用頂点バッファのサブリソースの作成
	hr = m_pDevice->CreateBuffer(
		&colorBufferDesc,
		&ColorSubData,
		&m_pVertexBuffer[1]
	);
	if (FAILED(hr))
	{
		// メッセージを表示
		MessageBox(NULL, "色情報用頂点バッファの作成に失敗しました。", ErrorWndName, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// インデックス・バッファの定義
	D3D11_BUFFER_DESC idxBufferDesc;
	idxBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	idxBufferDesc.ByteWidth = sizeof(UINT) * 36;
	idxBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	idxBufferDesc.CPUAccessFlags = 0;
	idxBufferDesc.MiscFlags = 0;
	idxBufferDesc.StructureByteStride = 0;

	// インデックス・バッファのサブリソースの初期値設定
	UINT idxVertexID[] =
	{
		0,1,3,
		1,2,3,
		1,5,2,
		5,6,2,
		5,4,6,
		4,7,6,
		4,5,0,
		5,1,0,
		4,0,7,
		0,3,7,
		3,2,7,
		2,6,7,
	};

	// インデックス・バッファのサブリソースの定義
	D3D11_SUBRESOURCE_DATA idxSubData;
	idxSubData.pSysMem = idxVertexID;
	idxSubData.SysMemPitch = 0;
	idxSubData.SysMemSlicePitch = 0;

	// インデックス・バッファの作成
	hr = m_pDevice->CreateBuffer(
		&idxBufferDesc,
		&idxSubData,
		&m_pIdxBuffer
	);
	if (FAILED(hr))
	{
		// メッセージを表示
		MessageBox(NULL, "インデックス・バッファの作成に失敗しました。", ErrorWndName, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	////------------------------------------------------------------------------
	//// 頂点シェーダのコードコンパイル
	//ID3DBlob* pBlobVS = nullptr;
	//hr = D3DCompileFromFile(
	//	/*L"VSample.hlsl"*/L"Shader/VertexShader.hlsl",
	//	NULL,
	//	NULL,
	//	"main",
	//	"vs_5_0",
	//	/*D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR*/NULL,
	//	0,
	//	&pBlobVS,
	//	NULL
	//);
	//if (FAILED(hr))
	//{
	//	// メッセージを表示
	//	MessageBox(NULL, "頂点シェーダのコンパイルに失敗しました。", ErrorWndName, MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}
	//// 頂点シェーダの作成
	//hr = m_pDevice->CreateVertexShader(
	//	pBlobVS->GetBufferPointer(),
	//	pBlobVS->GetBufferSize(),
	//	NULL,
	//	&m_pVertexShader
	//);
	//pBlobVS->Release();
	//if (FAILED(hr))
	//{
	//	// メッセージを表示
	//	MessageBox(NULL, "頂点シェーダの作成に失敗しました。", ErrorWndName, MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}



	//// ピクセルシェーダのコードをコンパイル
	//ID3DBlob* pBlobPS = nullptr;
	//hr = D3DCompileFromFile(
	//	/*L"PSample.hlsl"*/L"Shader/PixelShader.hlsl",
	//	NULL,
	//	NULL,
	//	"main",
	//	"ps_5_0",
	//	/*D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR*/NULL,
	//	0,
	//	&pBlobPS,
	//	NULL
	//);
	//if (FAILED(hr))
	//{
	//	// メッセージを表示
	//	MessageBox(NULL, "ピクセル・シェーダのコンパイルに失敗しました。", ErrorWndName, MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}
	//// ピクセル・シェーダの作成
	//hr = m_pDevice->CreatePixelShader(
	//	pBlobPS->GetBufferPointer(),
	//	pBlobPS->GetBufferSize(),
	//	NULL,
	//	&m_pPixelShader
	//);
	//if (FAILED(hr))
	//{
	//	// メッセージを表示
	//	MessageBox(NULL, "ピクセルシェーダの作成に失敗しました。", ErrorWndName, MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}

	////// ジオメトリシェーダのコードをコンパイル
	////ID3DBlob* pBlobGS = NULL;
	////hr = D3DCompileFromFile(
	////	L"GSample.hlsl",
	////	NULL,
	////	NULL,
	////	"GS",
	////	"gs_4_0",
	////	D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR,
	////	0,
	////	NULL,
	////	&pBlobGS
	////);
	////if (FAILED(hr))
	////{
	////	// メッセージを表示
	////	MessageBox(NULL, "ジオメトリシェーダのコンパイルに失敗しました。", ErrorWndName, MB_OK | MB_ICONERROR);
	////	return E_FAIL;
	////}
	////// ジオメトリ・シェーダの作成
	////hr = m_pDevice->CreateGeometryShader(
	////	pBlobGS->GetBufferPointer(),
	////	pBlobGS->GetBufferSize(),
	////	NULL,
	////	&
	////	m_pGeometryShader
	////);
	////if (FAILED(hr))
	////{
	////	// メッセージを表示
	////	MessageBox(NULL, "ジオメトリシェーダの作成に失敗しました。", ErrorWndName, MB_OK | MB_ICONERROR);
	////	return E_FAIL;
	////}

	//// 頂点シェーダの設定
	//m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	////// ジオメトリシェーダの設定
	////m_pDeviceContext->GSSetShader(m_pGeometryShader, NULL, 0);
	//// ピクセルシェーダの設定
	//m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	////------------------------------------------------------------------------
	//// 定数バッファの定義
	//D3D11_BUFFER_DESC cBufferDesc;
	//cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//cBufferDesc.MiscFlags = 0;

	//// 定数バッファの作成
	//cBufferDesc.ByteWidth = sizeof(cbNeverChanges);
	//hr = m_pDevice->CreateBuffer(&cBufferDesc, NULL, &m_pCBuffer[0]);
	//if (FAILED(hr))
	//{
	//	// メッセージを表示
	//	MessageBox(NULL, "定数バッファ①の作成に失敗しました。", ErrorWndName, MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}

	//// 定数バッファの作成
	//cBufferDesc.ByteWidth = sizeof(cbChangesEveryFrame);
	//hr = m_pDevice->CreateBuffer(&cBufferDesc, NULL, &m_pCBuffer[1]);
	//if (FAILED(hr))
	//{
	//	// メッセージを表示
	//	MessageBox(NULL, "定数バッファ②の作成に失敗しました。", ErrorWndName, MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}

	//// 定数バッファの作成
	//cBufferDesc.ByteWidth = sizeof(cbChangesEveryObject);
	//hr = m_pDevice->CreateBuffer(&cBufferDesc, NULL, &m_pCBuffer[2]);
	//if (FAILED(hr))
	//{
	//	// メッセージを表示
	//	MessageBox(NULL, "定数バッファ③の作成に失敗しました。", ErrorWndName, MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}

	//// 定数バッファのマップ取得
	//D3D11_MAPPED_SUBRESOURCE MappedResource;
	//hr = m_pDeviceContext->Map(
	//	m_pCBuffer[0],
	//	0,
	//	D3D11_MAP_WRITE_DISCARD,
	//	0,
	//	&MappedResource
	//);
	//if (FAILED(hr))
	//{
	//	// メッセージを表示
	//	MessageBox(NULL, "マップの取得に失敗しました。", ErrorWndName, MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}

	//// データの書き込み
	//CopyMemory(MappedResource.pData, &m_cbNeverChanges, sizeof(cbNeverChanges));

	//// マップ解除
	//m_pDeviceContext->Unmap(m_pCBuffer[0], 0);

	//// 頂点バッファに定数バッファを設定
	//m_pDeviceContext->VSSetConstantBuffers(0, 3, m_pCBuffer);
	////// ジオメトリ・シェーダに定数バッファを設定
	////m_pDeviceContext->GSSetConstantBuffers(0, 3, m_pCBuffer);
	//// ピクセル・シェーダに定数バッファを設定
	//m_pDeviceContext->PSSetConstantBuffers(0, 3, m_pCBuffer);

	//// 頂点インプットレイアウト定義
	//D3D11_INPUT_ELEMENT_DESC layout[] =
	//{
	//	{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},

	//};
	//UINT numElements = sizeof(layout) / sizeof(layout[0]);
	////// 頂点インプットレイアウトを作成
	////m_pDevice->CreateInputLayout(
	////	layout,
	////	numElements,
	////	
	////)

	return S_OK;
}

// ===== 終了処理 =====
void DX11::Uninit()
{
	// デバイス・ステートのクリア
	if (m_pDeviceContext)
	{
		m_pDeviceContext->ClearState();
	}

	// 取得したインターフェイスの開放
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}

HRESULT DX11::Update()
{
	HRESULT hr;

	hr = m_pDevice->GetDeviceRemovedReason();

	switch (hr)
	{
	case S_OK:
		break;
	case DXGI_ERROR_DEVICE_HUNG:
	case DXGI_ERROR_DEVICE_RESET:
		Uninit();
		hr = Init(hWnd);
		if (FAILED(hr))
		{
			return E_FAIL;
		}
		break;
	case DXGI_ERROR_DEVICE_REMOVED:
	case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
	case DXGI_ERROR_INVALID_CALL:
	default:
		return E_FAIL;
	}

	return S_OK;
}


// ===== 描画処理 =====
void DX11::Draw()
{
	HRESULT hr;

	// レンダーターゲットビューを指定した色でクリア
	FLOAT clearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(
		m_pRenderTargetView,
		clearColor
	);

	// 深度/ステンシル値のクリア
	m_pDeviceContext->ClearDepthStencilView(
		m_pDepthStencilView,
		D3D11_CLEAR_DEPTH,
		1.0f,
		0
	);

	// 描画処理


	// バック・バッファの表示
	hr = m_pSwapChain->Present(0, 0);
}

HRESULT DX11::InitDevice(HWND wnd)
{
	HRESULT hr;



	return S_OK;
}

HRESULT DX11::InitView()
{
	HRESULT hr;




	return S_OK;
}

HRESULT DX11::InitShader()
{
	HRESULT hr;

	return S_OK;
}

HRESULT DX11::InitBuffer()
{
	HRESULT hr;

	return S_OK;
}

void DX11::InitMatrix()
{

}