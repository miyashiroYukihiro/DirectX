//=============================================
// �V�F�[�_�N���X
//---------------------------------------------
// ����F�݂₵��@�䂫�Ђ�
//---------------------------------------------
// Shader.cpp
//=============================================
// ---�@�C���N���[�h���@---
#include "Shader.h"

// --------- ��{�N���X
ShaderBase::ShaderBase()
{

}

ShaderBase::~ShaderBase()
{

}

HRESULT ShaderBase::Create(const char* FileName)
{
	HRESULT hr = E_FAIL;

	// �t�@�C���̒��g��ǂݍ���
	FILE* fp;
	fp = fopen(FileName, "rb");
	if (!fp)
	{
		return hr;
	}

	// �t�@�C���̒��g���������ɓǂݍ���
	char* pData = nullptr;
	int fileSIze = 0;

	fseek(fp, 0, SEEK_END);
	fileSIze = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	pData = new char[fileSIze];
	fread_s(pData, fileSIze, fileSIze, 1, fp);

	fclose(fp);

	// �V�F�[�_�쐬
	hr = MakeShader(pData, fileSIze);

	// �I������
	if (pData) { delete[] pData; }

	return hr;
}


//// --------- ���_�V�F�[�_
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
//	// �V�F�[�_�쐬
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
//	/* �C���v�b�g���C�A�E�g
//	CPU����GPU�ɒ��_�̏��𑗂�ہAGPU���ł͂ǂ����������_���������Ă���̂����ʂł��Ȃ��B
//	�����ŁA�ǂ����������_���ɂȂ��Ă��邩���`�������"�C���v�b�g���C�A�E�g"�ƌĂ΂����̂ɂȂ�B
//	*/
//
//	// VertexShader.hlsl���󂯎�钸�_�\��
//	D3D11_INPUT_ELEMENT_DESC layout[] =
//	{
//		{
//			// �Z�}���e�B�N�X�̎w��
//			"POSITION", 0,
//			// �^�̏��
//			DXGI_FORMAT_R32G32B32_FLOAT,
//			// �ϐ��̔z�u�ʒu
//			0, 0,
//			// ���_�Ŏg�p����f�[�^
//			D3D11_INPUT_PER_VERTEX_DATA, 0
//		},
//		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0,},
//		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
//		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
//	};
//
//	// �C���v�b�g���C�A�E�g�쐬
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
//// --------- �s�N�Z���V�F�[�_
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


// --------- �萔�o�b�t�@
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
	/* �萔�o�b�t�@�쐬���̒���
	�萔�o�b�t�@�̑傫����4�o�C�g��؂�(4�o�C�g�A���C�����g)�łȂ��ƍ쐬�ł��Ȃ�
	*/
	HRESULT hr;

	// �쐬����o�b�t�@�̏��
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth = size;
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		// �ǂ������o�b�t�@�����w�肷��
	
	// �o�b�t�@�̍쐬
	ID3D11Device* pDevice/* = DX11::GetDevice()*/;
	hr = pDevice->CreateBuffer(&bufDesc, nullptr, &m_pBuffer);

	return hr;
}

void ConstantBuffer::Write(void* pData)
{
	// �萔�o�b�t�@�ւ̏�������
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
	�萔�o�b�t�@�̃f�[�^�𑗂�ہA�ǂ̈ʒu�Ɋi�[���邩���P�ڂ̈���(StartSlot)�Ɏw�肷��B
	hlsl�̃R�[�h�ł�register(bX)�Ńo�b�t�@�̊i�[�ʒu�����߂Ă����B
	*/

	ID3D11DeviceContext* pContext/* = DX11::GetContext()*/;
	pContext->VSSetConstantBuffers(
		slot,			// �ǂ̃o�b�t�@�ɓ���邩
		1,				// �o�b�t�@�̐�
		&m_pBuffer		// 
	);
}
