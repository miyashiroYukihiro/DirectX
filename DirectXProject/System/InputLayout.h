#pragma once
//=============================================
// インプットレイアウト
//---------------------------------------------
// 制作：みやしろ　ゆきひろ
//---------------------------------------------
// InputLayout.h
//=============================================
// ---　インクルード部　---
#include "DirectX11.h"

class InputLayout
{
private:
	ID3D11InputLayout* m_pInputLayout;

public:
	InputLayout();
	~InputLayout();

	HRESULT Create(ID3DBlob* blob);
	void Bind();

	ID3D11InputLayout* GetInputLayout() { return m_pInputLayout; }
};
