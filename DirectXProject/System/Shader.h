#pragma once
//=============================================
// �V�F�[�_�N���X
//---------------------------------------------
// ����F�݂₵��@�䂫�Ђ�
//---------------------------------------------
// Shader.h
//=============================================
// ---�@�C���N���[�h���@---
#include <WinMain.h>
#include "DirectX11.h"

// �V�F�[�_�̊�{�N���X
class ShaderBase
{
public:
	ShaderBase();
	virtual ~ShaderBase();

	// �V�F�[�_���R���p�C��
	virtual HRESULT Compile(LPCWSTR name, LPCSTR entryPoint = "main") = 0;
	// �V�F�[�_�t�@�C��(*.cso)��ǂݍ��ޏ���
	HRESULT Create(const char* FileName);
	// �V�F�[�_��`��Ɏg�p
	virtual void Bind(void) = 0;

protected:
	ID3DBlob* m_pBlob;

protected:
	// �V�F�[�_�t�@�C����ǂݍ��񂾌�A
	// �V�F�[�_�̎�ޕʂɏ������s��
	virtual HRESULT MakeShader(void* pData, UINT size) = 0;

private:


};

//// ���_�V�F�[�_
//class VertexShader :public ShaderBase
//{
//public:
//	VertexShader();
//	~VertexShader();
//
//	HRESULT Compile(LPCWSTR name);
//	void Bind(void);
//
//protected:
//	HRESULT MakeShader(void* pData, UINT size);
//
//private:
//	ID3D11VertexShader* m_pVS;
//	ID3D11InputLayout* m_pInputLayout;
//
//};

//// �s�N�Z���V�F�[�_
//class PixelShader :public ShaderBase
//{
//public:
//	PixelShader();
//	~PixelShader();
//
//	HRESULT Compile(LPCWSTR name);
//	void Bind(void);
//
//protected:
//	HRESULT MakeShader(void* pData, UINT size);
//private:
//	ID3D11PixelShader* m_pPS;
//};

// �萔�o�b�t�@
class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	HRESULT Create(UINT size);
	void Write(void* pData);
	void Bind(UINT slot);

private:
	ID3D11Buffer* m_pBuffer;

};

