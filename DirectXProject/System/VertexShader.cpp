//=============================================
// ���_�V�F�[�_
//---------------------------------------------
// ����F�݂₵��@�䂫�Ђ�
//---------------------------------------------
// VertexShader.h
//=============================================
// ---�@�C���N���[�h���@---
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

	// ���_�V�F�[�_�̃R�[�h�R���p�C��
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
		// ���b�Z�[�W��\��
		MessageBox(NULL, "���_�V�F�[�_�̃R���p�C���Ɏ��s���܂����B", "Error", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

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

	// ���_�V�F�[�_�̍쐬
	hr = pDevice->CreateVertexShader(
		m_pBlob->GetBufferPointer(),
		m_pBlob->GetBufferSize(),
		NULL,
		&m_pVS
	);
	m_pBlob->Release();
	if (FAILED(hr))
	{
		// ���b�Z�[�W��\��
		MessageBox(NULL, "���_�V�F�[�_�̍쐬�Ɏ��s���܂����B", "Error", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	return hr;
}
