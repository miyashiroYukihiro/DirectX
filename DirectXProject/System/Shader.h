#pragma once
//=============================================
// シェーダクラス
//---------------------------------------------
// 制作：みやしろ　ゆきひろ
//---------------------------------------------
// Shader.h
//=============================================
// ---　インクルード部　---
#include <WinMain.h>
#include "DirectX11.h"

// シェーダの基本クラス
class ShaderBase
{
public:
	ShaderBase();
	virtual ~ShaderBase();

	// シェーダをコンパイル
	virtual HRESULT Compile(LPCWSTR name, LPCSTR entryPoint = "main") = 0;
	// シェーダファイル(*.cso)を読み込む処理
	HRESULT Create(const char* FileName);
	// シェーダを描画に使用
	virtual void Bind(void) = 0;

protected:
	ID3DBlob* m_pBlob;

protected:
	// シェーダファイルを読み込んだ後、
	// シェーダの種類別に処理を行う
	virtual HRESULT MakeShader(void* pData, UINT size) = 0;

private:


};

//// 頂点シェーダ
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

//// ピクセルシェーダ
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

// 定数バッファ
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

