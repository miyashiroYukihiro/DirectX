#pragma once
//=============================================
// DirectX�N���X
//---------------------------------------------
// ����F�݂₵��@�䂫�Ђ�
//---------------------------------------------
// DirectX11.h
//=============================================
// ---�@�C���N���[�h���@---
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <d3dcompiler.h>

// ���C�u�����̒ǉ�
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// �����΍�
using namespace DirectX;

// �}�N����`
#define SAFE_RELEASE(x)	if(x){ x = nullptr; }

// �萔�o�b�t�@�̒�`
struct cbNeverChanges
{
	XMFLOAT4X4 Projection;		// �����ϊ��s��
};

struct cbChangesEveryFrame
{
	XMFLOAT4X4 View;	// �r���[�ϊ��s��
	XMFLOAT3 Light;		// �������W
	FLOAT dummy;		// �萔�o�b�t�@�̃T�C�Y��16�̔{���ɂ��邽�߂̃_�~�[
};

struct cbChangesEveryObject
{
	XMFLOAT4X4 World;
};


// ==== DirectX11�N���X ====
class DX11
{
public:
	// �萔
	const float SCREEN_WIDTH = 1024.0f;
	const float SCREEN_HEIGHT = 576.0f;

private:
	HWND hWnd;
	LPCSTR ErrorWndName;

	//D3D_FEATURE_LEVEL m_pFeatureLevels[3] =
	//{
	//	D3D_FEATURE_LEVEL_11_0,
	//	D3D_FEATURE_LEVEL_10_1,
	//	D3D_FEATURE_LEVEL_10_0,
	//};
	UINT m_FeatureLevels = 3;		// �z��̗v�f��
	D3D_FEATURE_LEVEL m_FeatureLevelsSupported;		// �f�o�C�X�쐬���ɕԂ����@�\���x��

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;

	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11Texture2D* m_pDepthStencil;
	ID3D11DepthStencilView* m_pDepthStencilView;

	ID3D11Buffer* m_pVertexBuffer[2] =		// ���_�o�b�t�@�̃C���^�[�t�F�C�X
	{
		NULL,
		NULL
	};

	ID3D11Buffer* m_pIdxBuffer = NULL;
	ID3D11VertexShader* m_pVertexShader;		// ���_�V�F�[�_
	//ID3D11GeometryShader* m_pGeometryShader;	// �W�I���g���V�F�[�_
	ID3D11PixelShader* m_pPixelShader;			// �s�N�Z���V�F�[�_

	struct cbNeverChanges m_cbNeverChanges;
	struct cbChangesEveryFrame m_cbChangesEveryFrame;
	struct cbChangesEveryObject m_cbChangesEveryObject;

	ID3D11Buffer* m_pCBuffer[3] =
	{
		NULL,
		NULL,
		NULL
	};


protected:


public:
	DX11();
	~DX11();

	HRESULT Init(HWND wnd);		// ����������
	void	Uninit();				// �I������
	HRESULT Update();
	void	Draw();					// �`�揈��

	HRESULT InitDevice(HWND wnd);	// �f�o�C�X�֘A�̏�����
	HRESULT	InitView();				// �r���[�֘A������
	HRESULT InitShader();			// �V�F�[�_�[�֘A������
	HRESULT InitBuffer();			// �o�b�t�@�֘A������
	void	InitMatrix();			// �}�g���b�N�X�֘A������

	ID3D11Device*		GetDevice(void) { return m_pDevice; }		// �f�o�C�X�̎擾
	ID3D11DeviceContext*	GetContext(void) { return m_pDeviceContext; }		// �f�o�C�X�R���e�L�X�g�̎擾
	IDXGISwapChain*			GetSwapChain(void) { return m_pSwapChain; }

};