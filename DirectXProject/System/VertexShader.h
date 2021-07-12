#pragma once
//=============================================
// ���_�V�F�[�_
//---------------------------------------------
// ����F�݂₵��@�䂫�Ђ�
//---------------------------------------------
// VertexShader.h
//=============================================
// ---�@�C���N���[�h���@---
#include "Shader.h"

class VertexShader :public ShaderBase
{
public:
	VertexShader();
	~VertexShader();

	HRESULT Compile(LPCWSTR name, LPCSTR entryPoint = "main");
	void Bind(void);

protected:
	HRESULT MakeShader();

private:
	ID3D11VertexShader* m_pVS;
	ID3D11InputLayout* m_pInputLayout;
};

