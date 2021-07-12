//=============================================
// 頂点シェーダ
//---------------------------------------------
// 制作：みやしろ　ゆきひろ
//---------------------------------------------
// VertexShader.h
//=============================================
// ---　インクルード部　---
#include "VertexShader.h"

VertexShader::VertexShader()
{

}

VertexShader::~VertexShader()
{

}

HRESULT VertexShader::Compile(LPCWSTR name, LPCSTR entryPoint)
{
	HRESULT hr;
	ID3D11Device* pDevice/* = DX11::GetDevice()*/;

	// 頂点シェーダのコードコンパイル
	hr = D3DCompileFromFile(
		/*L"Shader/VertexShader.hlsl"*/name,
		NULL,
		NULL,
		entryPoint,
		"vs_5_0",
		/*D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR*/NULL,
		0,
		&m_pBlob,
		NULL
	);
	if (FAILED(hr))
	{
		// メッセージを表示
		MessageBox(NULL, "頂点シェーダのコンパイルに失敗しました。", "Error", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// 頂点シェーダが受け取る頂点構造
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			// セマンティクスの指定
			"POSITION", 0,
			// 型の情報
			DXGI_FORMAT_R32G32B32_FLOAT,
			// 変数の配置位置
			0, 0,
			// 頂点で使用するデータ
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0,},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// インプットレイアウト作成
	hr = pDevice->CreateInputLayout(
		layout,
		_countof(layout),
		m_pBlob->GetBufferPointer(),
		m_pBlob->GetBufferSize(),
		&m_pInputLayout
	);

	return hr;
}

void VertexShader::Bind(void)
{
	ID3D11DeviceContext* pContext/* = DX11::GetContext()*/;
	pContext->VSSetShader(m_pVS, NULL, 0);
	pContext->IASetInputLayout(m_pInputLayout);
}

HRESULT VertexShader::MakeShader()
{
	HRESULT hr;
	ID3D11Device* pDevice /*= DX11::GetDevice()*/;

	// 頂点シェーダの作成
	hr = pDevice->CreateVertexShader(
		m_pBlob->GetBufferPointer(),
		m_pBlob->GetBufferSize(),
		NULL,
		&m_pVS
	);
	m_pBlob->Release();
	if (FAILED(hr))
	{
		// メッセージを表示
		MessageBox(NULL, "頂点シェーダの作成に失敗しました。", "Error", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	return hr;
}
