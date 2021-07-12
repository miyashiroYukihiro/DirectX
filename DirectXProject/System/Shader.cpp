//=============================================
// シェーダクラス
//---------------------------------------------
// 制作：みやしろ　ゆきひろ
//---------------------------------------------
// Shader.cpp
//=============================================
// ---　インクルード部　---
#include "Shader.h"

// --------- 基本クラス
ShaderBase::ShaderBase()
{

}

ShaderBase::~ShaderBase()
{

}

HRESULT ShaderBase::Create(const char* FileName)
{
	HRESULT hr = E_FAIL;

	// ファイルの中身を読み込む
	FILE* fp;
	fp = fopen(FileName, "rb");
	if (!fp)
	{
		return hr;
	}

	// ファイルの中身をメモリに読み込み
	char* pData = nullptr;
	int fileSIze = 0;

	fseek(fp, 0, SEEK_END);
	fileSIze = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	pData = new char[fileSIze];
	fread_s(pData, fileSIze, fileSIze, 1, fp);

	fclose(fp);

	// シェーダ作成
	hr = MakeShader(pData, fileSIze);

	// 終了処理
	if (pData) { delete[] pData; }

	return hr;
}


//// --------- 頂点シェーダ
//VertexShader::VertexShader()
//	:m_pVS(nullptr)
//	, m_pInputLayout(nullptr)
//{
//
//}
//
//VertexShader::~VertexShader()
//{
//	SAFE_RELEASE(m_pVS);
//	SAFE_RELEASE(m_pInputLayout)
//}
//
//HRESULT VertexShader::Compile(LPCWSTR name)
//{
//	return E_NOTIMPL;
//}
//
//void VertexShader::Bind(void)
//{
//	ID3D11DeviceContext* pContext = GetContext();
//	pContext->VSSetShader(m_pVS, NULL, 0);
//	pContext->IASetInputLayout(m_pInputLayout);
//}
//
//HRESULT VertexShader::MakeShader(void* pData, UINT size)
//{
//	HRESULT hr;
//	ID3D11Device* pDevice = GetDevice();
//
//	// シェーダ作成
//	hr = pDevice->CreateVertexShader(
//		pData,
//		size,
//		NULL,
//		&m_pVS
//	);
//
//	if (FAILED(hr))
//	{
//		return hr;
//	}
//
//	/* インプットレイアウト
//	CPUからGPUに頂点の情報を送る際、GPU側ではどういった頂点情報を持っているのか判別できない。
//	そこで、どういった頂点情報になっているかを定義した情報が"インプットレイアウト"と呼ばれるものになる。
//	*/
//
//	// VertexShader.hlslが受け取る頂点構造
//	D3D11_INPUT_ELEMENT_DESC layout[] =
//	{
//		{
//			// セマンティクスの指定
//			"POSITION", 0,
//			// 型の情報
//			DXGI_FORMAT_R32G32B32_FLOAT,
//			// 変数の配置位置
//			0, 0,
//			// 頂点で使用するデータ
//			D3D11_INPUT_PER_VERTEX_DATA, 0
//		},
//		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0,},
//		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
//		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
//	};
//
//	// インプットレイアウト作成
//	hr = pDevice->CreateInputLayout(
//		layout,
//		_countof(layout),
//		pData,
//		size,
//		&m_pInputLayout
//	);
//
//	return hr;
//}
//
//
//// --------- ピクセルシェーダ
//PixelShader::PixelShader()
//	:m_pPS(nullptr)
//{
//
//}
//
//PixelShader::~PixelShader()
//{
//	SAFE_RELEASE(m_pPS);
//}
//
//HRESULT PixelShader::Compile(LPCWSTR name)
//{
//	return E_NOTIMPL;
//}
//
//void PixelShader::Bind(void)
//{
//	ID3D11DeviceContext* pContext = GetContext();
//	pContext->PSSetShader(m_pPS, NULL, 0);
//}
//
//HRESULT PixelShader::MakeShader(void* pData, UINT size)
//{
//	HRESULT hr;
//	ID3D11Device* pDevice = GetDevice();
//	hr = pDevice->CreatePixelShader(
//		pData,
//		size,
//		NULL,
//		&m_pPS
//	);
//
//	return hr;
//}


// --------- 定数バッファ
ConstantBuffer::ConstantBuffer()
	:m_pBuffer(nullptr)
{

}
ConstantBuffer::~ConstantBuffer()
{
	if (m_pBuffer) { delete m_pBuffer; }
}

HRESULT ConstantBuffer::Create(UINT size)
{
	/* 定数バッファ作成時の注意
	定数バッファの大きさは4バイト区切り(4バイトアライメント)でないと作成できない
	*/
	HRESULT hr;

	// 作成するバッファの情報
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth = size;
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		// どういうバッファかを指定する
	
	// バッファの作成
	ID3D11Device* pDevice/* = DX11::GetDevice()*/;
	hr = pDevice->CreateBuffer(&bufDesc, nullptr, &m_pBuffer);

	return hr;
}

void ConstantBuffer::Write(void* pData)
{
	// 定数バッファへの書き込み
	ID3D11DeviceContext* pContext/* = DX11::GetContext()*/;
	pContext->UpdateSubresource(
		m_pBuffer,
		0,
		nullptr,
		pData,
		0,
		0
	);
}

void ConstantBuffer::Bind(UINT slot)
{
	/*
	定数バッファのデータを送る際、どの位置に格納するかを１つ目の引数(StartSlot)に指定する。
	hlslのコードではregister(bX)でバッファの格納位置を決めておく。
	*/

	ID3D11DeviceContext* pContext/* = DX11::GetContext()*/;
	pContext->VSSetConstantBuffers(
		slot,			// どのバッファに入れるか
		1,				// バッファの数
		&m_pBuffer		// 
	);
}
