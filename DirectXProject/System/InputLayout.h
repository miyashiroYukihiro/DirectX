#pragma once
//=============================================
// �C���v�b�g���C�A�E�g
//---------------------------------------------
// ����F�݂₵��@�䂫�Ђ�
//---------------------------------------------
// InputLayout.h
//=============================================
// ---�@�C���N���[�h���@---
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
