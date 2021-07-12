//=============================================
// �s�N�Z���V�F�[�_
//---------------------------------------------
// ����F�݂₵��@�䂫�Ђ�
//---------------------------------------------
// PixelShader.h
//=============================================
// ---�@�C���N���[�h���@---
#include "PixelShader.h"

PixelShader::PixelShader()
{

}

PixelShader::~PixelShader()
{

}

HRESULT PixelShader::Compile(LPCWSTR name, LPCSTR entryPoint)
{
	HRESULT hr;
	ID3D11Device* pDevice/* = DX11::GetDevice()*/;
	// �s�N�Z���V�F�[�_�̃R�[�h���R���p�C��
	hr = D3DCompileFromFile(
		/*L"Shader/PixelShader.hlsl"*/name,
		NULL,
		NULL,
		entryPoint,
		"ps_5_0",
		/*D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR*/NULL,
		0,
		&m_pBlob,
		NULL
	);
	if (FAILED(hr))
	{
		// ���b�Z�[�W��\��
		MessageBox(NULL, "�s�N�Z���E�V�F�[�_�̃R���p�C���Ɏ��s���܂����B", "Error", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	return hr;
}

void PixelShader::Bind(void)
{
	ID3D11DeviceContext* pContext /*= DX11::GetContext()*/;
	pContext->PSSetShader(m_pPS, NULL, 0);
}

HRESULT PixelShader::MakeShader()
{
	HRESULT hr;
	ID3D11Device* pDevice /*= DX11::GetDevice()*/;

	// �s�N�Z���E�V�F�[�_�̍쐬
	hr = pDevice->CreatePixelShader(
		m_pBlob->GetBufferPointer(),
		m_pBlob->GetBufferSize(),
		NULL,
		&m_pPS
	);
	if (FAILED(hr))
	{
		// ���b�Z�[�W��\��
		MessageBox(NULL, "�s�N�Z���V�F�[�_�̍쐬�Ɏ��s���܂����B", "Error", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	return hr;
}
