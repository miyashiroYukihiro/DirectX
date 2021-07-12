//=============================================
// インプットレイアウト
//---------------------------------------------
// 制作：みやしろ　ゆきひろ
//---------------------------------------------
// InputLayout.h
//=============================================
// ---　インクルード部　---
#include "InputLayout.h"

InputLayout::InputLayout()
{

}

InputLayout::~InputLayout()
{

}

HRESULT InputLayout::Create(ID3DBlob * blob)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice /*= DX11::GetDevice()*/;

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
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		&m_pInputLayout
	);

	return hr;
}

void InputLayout::Bind()
{
	ID3D11DeviceContext* pContext/* = DX11::GetContext()*/;
	pContext->IASetInputLayout(m_pInputLayout);
}
