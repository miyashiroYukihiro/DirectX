#pragma once
//=============================================
// �s�N�Z���V�F�[�_
//---------------------------------------------
// ����F�݂₵��@�䂫�Ђ�
//---------------------------------------------
// PixelShader.h
//=============================================
// ---�@�C���N���[�h���@---
#include "Shader.h"

class PixelShader :public ShaderBase
{
public:
	PixelShader();
	~PixelShader();

	HRESULT Compile(LPCWSTR name, LPCSTR entryPoint = "main");
	void Bind(void);

protected:
	HRESULT MakeShader();
private:
	ID3D11PixelShader* m_pPS;
};


