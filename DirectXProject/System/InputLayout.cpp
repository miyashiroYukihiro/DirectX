//=============================================
// �C���v�b�g���C�A�E�g
//---------------------------------------------
// ����F�݂₵��@�䂫�Ђ�
//---------------------------------------------
// InputLayout.h
//=============================================
// ---�@�C���N���[�h���@---
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

	// ���_�V�F�[�_���󂯎�钸�_�\��
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			// �Z�}���e�B�N�X�̎w��
			"POSITION", 0,
			// �^�̏��
			DXGI_FORMAT_R32G32B32_FLOAT,
			// �ϐ��̔z�u�ʒu
			0, 0,
			// ���_�Ŏg�p����f�[�^
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0,},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// �C���v�b�g���C�A�E�g�쐬
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
