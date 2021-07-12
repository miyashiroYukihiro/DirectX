#pragma once
//=============================================
// ピクセルシェーダ
//---------------------------------------------
// 制作：みやしろ　ゆきひろ
//---------------------------------------------
// PixelShader.h
//=============================================
// ---　インクルード部　---
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


