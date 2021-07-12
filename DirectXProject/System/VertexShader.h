#pragma once
//=============================================
// 頂点シェーダ
//---------------------------------------------
// 制作：みやしろ　ゆきひろ
//---------------------------------------------
// VertexShader.h
//=============================================
// ---　インクルード部　---
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

