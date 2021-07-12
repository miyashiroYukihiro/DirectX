//=============================================
// DirectX�N���X
//---------------------------------------------
// ����F�݂₵��@�䂫�Ђ�
//---------------------------------------------
// DirectX11.cpp
//=============================================
// ---�@�C���N���[�h���@---
#include "DirectX11.h"

//// �ÓI�����o�錾
//ID3D11Device* DX11::m_pDevice;
//ID3D11DeviceContext* DX11::m_pDeviceContext;

// === �\���̒�` ===
struct XYZBuffer
{
	XMFLOAT3 Pos;
};

struct ColBuffer
{
	XMFLOAT3 Color;
};

DX11::DX11()
{
	ErrorWndName = "DX_Init";

	m_FeatureLevels = 3;		// �z��̗v�f��

	m_pDevice = nullptr;
	m_pDeviceContext = nullptr;
	m_pSwapChain = nullptr;

	m_pRenderTargetView = nullptr;
	m_pDepthStencil = nullptr;
	m_pDepthStencilView = nullptr;

	m_pIdxBuffer = nullptr;
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
}

DX11::~DX11()
{
	Uninit();
}

// ===== ���������� =====
HRESULT DX11::Init(HWND wnd)
{
	HRESULT hr;

	hWnd = wnd;

	// �h���C�o�[��ʂ��`
	std::vector<D3D_DRIVER_TYPE> driverTypes
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
		D3D_DRIVER_TYPE_SOFTWARE
	};

	// �X���b�v�`�F�C���̍쐬
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));							// �X���b�v�`�F�C���̏�����
	scd.BufferDesc.Width = SCREEN_WIDTH;					// �o�b�t�@�̉���
	scd.BufferDesc.Height = SCREEN_HEIGHT;					// �o�b�t�@�̏c��
	scd.BufferDesc.RefreshRate.Numerator = 60;				// ���t���b�V�����[�g�̕���
	scd.BufferDesc.RefreshRate.Denominator = 1;				// ���t���b�V�����[�g�̕��q
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// �o�b�t�@�̃t�H�[�}�b�g
	scd.SampleDesc.Count = 1;								// 1�s�N�Z���̐F�����߂�T���v�����O��
	scd.SampleDesc.Quality = 0;								// �T���v�����O�̕i��
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// 
	scd.BufferCount = 1;									// 
	scd.OutputWindow = hWnd;								// 
	scd.Windowed = TRUE;									// 

	// �h���C�o��ʂ��ォ�猟�؂��I��
	// �I�������f�o�C�X��p���ĕ`�悷��
	for (size_t i = 0; i < driverTypes.size(); ++i)
	{
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,								// �g�p����f�B�X�v���C�A�_�v�^�̎w��
			driverTypes[i],							// �`����s���h���C�o�̎w��
			nullptr,								// �h���C�o�̎w����\�t�g�E�F�A�ɂ����ꍇ�ɂ̂ݎw�肪�K�v
			0,										// �����^�C�����C���̃t���O�w��
			nullptr,								// FeatureLevel�̎w��
			0,										// D3D_FEATURE_LEVEL���w�肵���ꍇ�̔z��̗v�f��
			D3D11_SDK_VERSION,						// DirectSDK�̃o�[�W����
			&scd,									// �ݒ���s����DXGI_SWAP_CHAIN_DESC�̃A�h���X
			&m_pSwapChain,							// ����������������IDXGISwapChain���󂯎�邽�߂̃|�C���^�A�h���X
			&m_pDevice,								// ����������������ID3D11Device���󂯎�邽�߂̃|�C���^�A�h���X
			nullptr,								// ���܂���D3D_FEATURE_LEVEL���擾���邽�߂̃A�h���X
			&m_pDeviceContext						// �쐬���ꂽID3D11DeviceContext���󂯎�邽�߂̃|�C���^�A�h���X
		);
		// ���������烋�[�v�𔲂���
		if (SUCCEEDED(hr))
		{
			break;
		}
	}
	// ���s������I������
	if (FAILED(hr))
	{
		// ���b�Z�[�W��\��
		MessageBox(NULL, "DirectX11�ɑΉ����Ă��Ȃ��f�o�C�X�ł��B", ErrorWndName, MB_OK | MB_ICONERROR);

		return E_FAIL;
	}

	//---------------------------------------------------------------------------------------------------
	// �X���b�v�E�`�F�C������ŏ��̃o�b�N�E�o�b�t�@���擾����
	ID3D11Texture2D *pBackBuffer = nullptr;	// �o�b�t�@�Ɂ��̃A�N�Z�X�Ɏg���C���^�[�t�F�C�X
	hr = m_pSwapChain->GetBuffer(
		0,									// �o�b�N�o�b�t�@�̔ԍ�
		__uuidof(ID3D11Texture2D),			// �o�b�t�@�ɃA�N�Z�X����C���^�[�t�F�C�X
		(LPVOID*)&pBackBuffer				// �o�b�t�@���󂯎��ϐ�
	);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// �o�b�N�E�o�b�t�@�̕`��^�[�Q�b�g�E�r���[�����
	hr = m_pDevice->CreateRenderTargetView(
		pBackBuffer,
		NULL,
		&m_pRenderTargetView
	);
	
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	
	m_pDeviceContext->OMSetRenderTargets(
		1,
		&m_pRenderTargetView,
		nullptr
	);

	D3D11_VIEWPORT vp;
	// �r���[�|�[�g�̐ݒ�
	vp.TopLeftX = 0.0f;			// �r���[�|�[�g�̈�̍���X���W
	vp.TopLeftY = 0.0f;			// �r���[�|�[�g�̈�̍���Y���W
	vp.Width = SCREEN_WIDTH;		// �r���[�|�[�g�̈�̕�
	vp.Height = SCREEN_HEIGHT;	// �r���[�|�[�g�̈�̍���
	vp.MinDepth = 0.0f;			// �r���[�|�[�g�̈�̐[�x�l�̍ŏ��l
	vp.MaxDepth = 1.0f;			// �r���[�|�[�g�̈�̐[�x�l�̍ő�l
	m_pDeviceContext->RSSetViewports(1, &vp);
	//---------------------------------------------------------------------------------------------------
	// �[�x/�X�e���V���E�e�N�X�`���̍쐬
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = SCREEN_WIDTH;
	descDepth.Height = SCREEN_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = m_pDevice->CreateTexture2D(
		&descDepth,
		NULL,
		&m_pDepthStencil
	);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	//---------------------------------------------------------------------------------------------------
	// �[�x/�X�e���V���E�r���[�̍쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;

	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_pDevice->CreateDepthStencilView(
		m_pDepthStencil,
		&descDSV,
		&m_pDepthStencilView
	);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	m_pDeviceContext->OMSetRenderTargets(
		1,
		&m_pRenderTargetView,
		m_pDepthStencilView
	);

	//------------------------------------------------------------------------
	//IDXGIOutput* pOutput;
	//hr=m_pSwapChain->GetContainingOutput(&pOutput);
	//if (FAILED(hr))
	//{
	//	// ���b�Z�[�W��\��
	//	MessageBox(NULL, "IDXXGIOutput�C���^�[�t�F�C�X�̎擾�Ɏ��s���܂���", ErrorWndName, MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}

	//DXGI_GAMMA_CONTROL_CAPABILITIES gammaCap;
	//hr = pOutput->GetGammaControlCapabilities(&gammaCap);
	//if (FAILED(hr))
	//{
	//	// ���b�Z�[�W��\��
	//	MessageBox(NULL, "DirectX11�ɑΉ����Ă��Ȃ��f�o�C�X�ł��B", ErrorWndName, MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}

	//float gamma = 1.0f / 2.2f;

	//// �g�[���J�[�u��ݒ�
	//DXGI_GAMMA_CONTROL gammaControl;
	//gammaControl.Scale.Red = 1.0f;
	//gammaControl.Scale.Green = 1.0f;
	//gammaControl.Scale.Blue = 1.0f;
	//gammaControl.Offset.Red = 0.0f;
	//gammaControl.Offset.Green = 0.0f;
	//gammaControl.Offset.Blue = 0.0f;
	//for (UINT i = 0; i < gammaCap.NumGammaControlPoints; ++i)
	//{
	//	float L0 = gammaCap.ControlPointPositions[i];
	//	float L1 = pow(L0, gamma);
	//	gammaControl.GammaCurve[i].Red = L1;
	//	gammaControl.GammaCurve[i].Green = L1;
	//	gammaControl.GammaCurve[i].Blue = L1;
	//}
	//hr = pOutput->SetGammaControl(&gammaControl);
	//if (FAILED(hr))
	//{
	//	return E_FAIL;
	//}

	//------------------------------------------------------------------------
	// ���W�p���_�o�b�t�@���쐬
	D3D11_BUFFER_DESC xyzBufferDesc;
	xyzBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	xyzBufferDesc.ByteWidth = sizeof(XYZBuffer) * 8;
	xyzBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	xyzBufferDesc.CPUAccessFlags = 0;
	xyzBufferDesc.MiscFlags = 0;
	xyzBufferDesc.StructureByteStride = 0;

	// ���W�p���_�o�b�t�@�̃T�u���\�[�X�̏����l�ݒ�
	struct XYZBuffer posVertex[] =
	{
		XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(1.0f,  1.0f, -1.0f),
		XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f),
		XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(1.0f,  1.0f,  1.0f),
		XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(-1.0f, -1.0f,  1.0f),
	};

	// ���W�p���_�o�b�t�@�̃T�u���\�[�X�̒�`
	D3D11_SUBRESOURCE_DATA xyzSubData;
	xyzSubData.pSysMem = posVertex;
	xyzSubData.SysMemPitch = 0;
	xyzSubData.SysMemSlicePitch = 0;

	// ���W�p���_�o�b�t�@�̍쐬
	hr = m_pDevice->CreateBuffer(
		&xyzBufferDesc,
		&xyzSubData,
		&m_pVertexBuffer[0]
	);
	if (FAILED(hr))
	{
		// ���b�Z�[�W��\��
		MessageBox(NULL, "���W�p���_�o�b�t�@�̍쐬�Ɏ��s���܂����B", ErrorWndName, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// �F���p���_�o�b�t�@�̒�`
	D3D11_BUFFER_DESC colorBufferDesc;
	colorBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	colorBufferDesc.ByteWidth = sizeof(ColBuffer) * 8;
	colorBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	colorBufferDesc.CPUAccessFlags = 0;
	colorBufferDesc.MiscFlags = 0;
	colorBufferDesc.StructureByteStride = 0;

	// �F���p���_�o�b�t�@�̃T�u���\�[�X�̏����l�ݒ�
	struct ColBuffer ColorVertex[] =
	{
		XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
	};

	// �F���p���_�o�b�t�@�̃T�u���\�[�X�̒�`
	D3D11_SUBRESOURCE_DATA ColorSubData;
	ColorSubData.pSysMem = ColorVertex;
	ColorSubData.SysMemPitch = 0;
	ColorSubData.SysMemSlicePitch = 0;

	// �F���p���_�o�b�t�@�̃T�u���\�[�X�̍쐬
	hr = m_pDevice->CreateBuffer(
		&colorBufferDesc,
		&ColorSubData,
		&m_pVertexBuffer[1]
	);
	if (FAILED(hr))
	{
		// ���b�Z�[�W��\��
		MessageBox(NULL, "�F���p���_�o�b�t�@�̍쐬�Ɏ��s���܂����B", ErrorWndName, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// �C���f�b�N�X�E�o�b�t�@�̒�`
	D3D11_BUFFER_DESC idxBufferDesc;
	idxBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	idxBufferDesc.ByteWidth = sizeof(UINT) * 36;
	idxBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	idxBufferDesc.CPUAccessFlags = 0;
	idxBufferDesc.MiscFlags = 0;
	idxBufferDesc.StructureByteStride = 0;

	// �C���f�b�N�X�E�o�b�t�@�̃T�u���\�[�X�̏����l�ݒ�
	UINT idxVertexID[] =
	{
		0,1,3,
		1,2,3,
		1,5,2,
		5,6,2,
		5,4,6,
		4,7,6,
		4,5,0,
		5,1,0,
		4,0,7,
		0,3,7,
		3,2,7,
		2,6,7,
	};

	// �C���f�b�N�X�E�o�b�t�@�̃T�u���\�[�X�̒�`
	D3D11_SUBRESOURCE_DATA idxSubData;
	idxSubData.pSysMem = idxVertexID;
	idxSubData.SysMemPitch = 0;
	idxSubData.SysMemSlicePitch = 0;

	// �C���f�b�N�X�E�o�b�t�@�̍쐬
	hr = m_pDevice->CreateBuffer(
		&idxBufferDesc,
		&idxSubData,
		&m_pIdxBuffer
	);
	if (FAILED(hr))
	{
		// ���b�Z�[�W��\��
		MessageBox(NULL, "�C���f�b�N�X�E�o�b�t�@�̍쐬�Ɏ��s���܂����B", ErrorWndName, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	////------------------------------------------------------------------------
	//// ���_�V�F�[�_�̃R�[�h�R���p�C��
	//ID3DBlob* pBlobVS = nullptr;
	//hr = D3DCompileFromFile(
	//	/*L"VSample.hlsl"*/L"Shader/VertexShader.hlsl",
	//	NULL,
	//	NULL,
	//	"main",
	//	"vs_5_0",
	//	/*D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR*/NULL,
	//	0,
	//	&pBlobVS,
	//	NULL
	//);
	//if (FAILED(hr))
	//{
	//	// ���b�Z�[�W��\��
	//	MessageBox(NULL, "���_�V�F�[�_�̃R���p�C���Ɏ��s���܂����B", ErrorWndName, MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}
	//// ���_�V�F�[�_�̍쐬
	//hr = m_pDevice->CreateVertexShader(
	//	pBlobVS->GetBufferPointer(),
	//	pBlobVS->GetBufferSize(),
	//	NULL,
	//	&m_pVertexShader
	//);
	//pBlobVS->Release();
	//if (FAILED(hr))
	//{
	//	// ���b�Z�[�W��\��
	//	MessageBox(NULL, "���_�V�F�[�_�̍쐬�Ɏ��s���܂����B", ErrorWndName, MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}



	//// �s�N�Z���V�F�[�_�̃R�[�h���R���p�C��
	//ID3DBlob* pBlobPS = nullptr;
	//hr = D3DCompileFromFile(
	//	/*L"PSample.hlsl"*/L"Shader/PixelShader.hlsl",
	//	NULL,
	//	NULL,
	//	"main",
	//	"ps_5_0",
	//	/*D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR*/NULL,
	//	0,
	//	&pBlobPS,
	//	NULL
	//);
	//if (FAILED(hr))
	//{
	//	// ���b�Z�[�W��\��
	//	MessageBox(NULL, "�s�N�Z���E�V�F�[�_�̃R���p�C���Ɏ��s���܂����B", ErrorWndName, MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}
	//// �s�N�Z���E�V�F�[�_�̍쐬
	//hr = m_pDevice->CreatePixelShader(
	//	pBlobPS->GetBufferPointer(),
	//	pBlobPS->GetBufferSize(),
	//	NULL,
	//	&m_pPixelShader
	//);
	//if (FAILED(hr))
	//{
	//	// ���b�Z�[�W��\��
	//	MessageBox(NULL, "�s�N�Z���V�F�[�_�̍쐬�Ɏ��s���܂����B", ErrorWndName, MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}

	////// �W�I���g���V�F�[�_�̃R�[�h���R���p�C��
	////ID3DBlob* pBlobGS = NULL;
	////hr = D3DCompileFromFile(
	////	L"GSample.hlsl",
	////	NULL,
	////	NULL,
	////	"GS",
	////	"gs_4_0",
	////	D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR,
	////	0,
	////	NULL,
	////	&pBlobGS
	////);
	////if (FAILED(hr))
	////{
	////	// ���b�Z�[�W��\��
	////	MessageBox(NULL, "�W�I���g���V�F�[�_�̃R���p�C���Ɏ��s���܂����B", ErrorWndName, MB_OK | MB_ICONERROR);
	////	return E_FAIL;
	////}
	////// �W�I���g���E�V�F�[�_�̍쐬
	////hr = m_pDevice->CreateGeometryShader(
	////	pBlobGS->GetBufferPointer(),
	////	pBlobGS->GetBufferSize(),
	////	NULL,
	////	&
	////	m_pGeometryShader
	////);
	////if (FAILED(hr))
	////{
	////	// ���b�Z�[�W��\��
	////	MessageBox(NULL, "�W�I���g���V�F�[�_�̍쐬�Ɏ��s���܂����B", ErrorWndName, MB_OK | MB_ICONERROR);
	////	return E_FAIL;
	////}

	//// ���_�V�F�[�_�̐ݒ�
	//m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	////// �W�I���g���V�F�[�_�̐ݒ�
	////m_pDeviceContext->GSSetShader(m_pGeometryShader, NULL, 0);
	//// �s�N�Z���V�F�[�_�̐ݒ�
	//m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	////------------------------------------------------------------------------
	//// �萔�o�b�t�@�̒�`
	//D3D11_BUFFER_DESC cBufferDesc;
	//cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//cBufferDesc.MiscFlags = 0;

	//// �萔�o�b�t�@�̍쐬
	//cBufferDesc.ByteWidth = sizeof(cbNeverChanges);
	//hr = m_pDevice->CreateBuffer(&cBufferDesc, NULL, &m_pCBuffer[0]);
	//if (FAILED(hr))
	//{
	//	// ���b�Z�[�W��\��
	//	MessageBox(NULL, "�萔�o�b�t�@�@�̍쐬�Ɏ��s���܂����B", ErrorWndName, MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}

	//// �萔�o�b�t�@�̍쐬
	//cBufferDesc.ByteWidth = sizeof(cbChangesEveryFrame);
	//hr = m_pDevice->CreateBuffer(&cBufferDesc, NULL, &m_pCBuffer[1]);
	//if (FAILED(hr))
	//{
	//	// ���b�Z�[�W��\��
	//	MessageBox(NULL, "�萔�o�b�t�@�A�̍쐬�Ɏ��s���܂����B", ErrorWndName, MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}

	//// �萔�o�b�t�@�̍쐬
	//cBufferDesc.ByteWidth = sizeof(cbChangesEveryObject);
	//hr = m_pDevice->CreateBuffer(&cBufferDesc, NULL, &m_pCBuffer[2]);
	//if (FAILED(hr))
	//{
	//	// ���b�Z�[�W��\��
	//	MessageBox(NULL, "�萔�o�b�t�@�B�̍쐬�Ɏ��s���܂����B", ErrorWndName, MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}

	//// �萔�o�b�t�@�̃}�b�v�擾
	//D3D11_MAPPED_SUBRESOURCE MappedResource;
	//hr = m_pDeviceContext->Map(
	//	m_pCBuffer[0],
	//	0,
	//	D3D11_MAP_WRITE_DISCARD,
	//	0,
	//	&MappedResource
	//);
	//if (FAILED(hr))
	//{
	//	// ���b�Z�[�W��\��
	//	MessageBox(NULL, "�}�b�v�̎擾�Ɏ��s���܂����B", ErrorWndName, MB_OK | MB_ICONERROR);
	//	return E_FAIL;
	//}

	//// �f�[�^�̏�������
	//CopyMemory(MappedResource.pData, &m_cbNeverChanges, sizeof(cbNeverChanges));

	//// �}�b�v����
	//m_pDeviceContext->Unmap(m_pCBuffer[0], 0);

	//// ���_�o�b�t�@�ɒ萔�o�b�t�@��ݒ�
	//m_pDeviceContext->VSSetConstantBuffers(0, 3, m_pCBuffer);
	////// �W�I���g���E�V�F�[�_�ɒ萔�o�b�t�@��ݒ�
	////m_pDeviceContext->GSSetConstantBuffers(0, 3, m_pCBuffer);
	//// �s�N�Z���E�V�F�[�_�ɒ萔�o�b�t�@��ݒ�
	//m_pDeviceContext->PSSetConstantBuffers(0, 3, m_pCBuffer);

	//// ���_�C���v�b�g���C�A�E�g��`
	//D3D11_INPUT_ELEMENT_DESC layout[] =
	//{
	//	{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},

	//};
	//UINT numElements = sizeof(layout) / sizeof(layout[0]);
	////// ���_�C���v�b�g���C�A�E�g���쐬
	////m_pDevice->CreateInputLayout(
	////	layout,
	////	numElements,
	////	
	////)

	return S_OK;
}

// ===== �I������ =====
void DX11::Uninit()
{
	// �f�o�C�X�E�X�e�[�g�̃N���A
	if (m_pDeviceContext)
	{
		m_pDeviceContext->ClearState();
	}

	// �擾�����C���^�[�t�F�C�X�̊J��
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}

HRESULT DX11::Update()
{
	HRESULT hr;

	hr = m_pDevice->GetDeviceRemovedReason();

	switch (hr)
	{
	case S_OK:
		break;
	case DXGI_ERROR_DEVICE_HUNG:
	case DXGI_ERROR_DEVICE_RESET:
		Uninit();
		hr = Init(hWnd);
		if (FAILED(hr))
		{
			return E_FAIL;
		}
		break;
	case DXGI_ERROR_DEVICE_REMOVED:
	case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
	case DXGI_ERROR_INVALID_CALL:
	default:
		return E_FAIL;
	}

	return S_OK;
}


// ===== �`�揈�� =====
void DX11::Draw()
{
	HRESULT hr;

	// �����_�[�^�[�Q�b�g�r���[���w�肵���F�ŃN���A
	FLOAT clearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(
		m_pRenderTargetView,
		clearColor
	);

	// �[�x/�X�e���V���l�̃N���A
	m_pDeviceContext->ClearDepthStencilView(
		m_pDepthStencilView,
		D3D11_CLEAR_DEPTH,
		1.0f,
		0
	);

	// �`�揈��


	// �o�b�N�E�o�b�t�@�̕\��
	hr = m_pSwapChain->Present(0, 0);
}

HRESULT DX11::InitDevice(HWND wnd)
{
	HRESULT hr;



	return S_OK;
}

HRESULT DX11::InitView()
{
	HRESULT hr;




	return S_OK;
}

HRESULT DX11::InitShader()
{
	HRESULT hr;

	return S_OK;
}

HRESULT DX11::InitBuffer()
{
	HRESULT hr;

	return S_OK;
}

void DX11::InitMatrix()
{

}