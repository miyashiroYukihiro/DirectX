//=============================================
// ���C���֐�
//---------------------------------------------
// ����F�݂₵��@�䂫�Ђ�
//---------------------------------------------
// WinMain.cpp
//=============================================
// ---�@�C���N���[�h���@---
#include "WinMain.h"

// ===== ���C�u�����̃����N =====
#pragma comment (lib,"winmm.lib")

// �}�N���E�萔��`
#define WINDOW_TITLE		("�E�B���h�E�̕\��")
#define WINDOW_CLASS		("DX")
#define FRAME_TIME			(500)
#define FPS_CNT				(60)

// ===== �v���g�^�C�v�錾 =====
LRESULT CALLBACK WindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

// �O���[�o���錾
DX11 dx;				// DirectX�N���X
int FrameCnt;			// �t���[���J�E���^
int FPS;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �E�B���h�E�N���X��������
	WNDCLASSEX window_class =
	{
		sizeof(WNDCLASSEX),					// �\���̂̃T�C�Y
		CS_HREDRAW | CS_VREDRAW,			// �N���X�̃X�^�C��
		WindowProc,							// �E�B���h�E�v���V�[�W��
		0,									// �⏕������
		0,									// �⏕������
		hInstance,							// ���̃A�v���̃C���X�^���X�n���h��
		LoadIcon(NULL, IDI_APPLICATION),	// �A�C�R���摜
		LoadCursor(NULL, IDC_ARROW),		// �J�[�\���摜
		NULL,								// �w�i�u���V
		NULL,								// ���j���[��
		WINDOW_CLASS,						// �N���X��
		NULL								// �������A�C�R��
	};
	// �E�B���h�E�N���X�̓o�^
	if (!RegisterClassEx(&window_class))
	{
		// �I��
		return 0;
	}

	// �E�B���h�E�T�C�Y�����߂�
	RECT rect_Wind
	{
		0,
		0,
		dx.SCREEN_WIDTH,
		dx.SCREEN_HEIGHT
	};
	AdjustWindowRect(&rect_Wind, WS_OVERLAPPED, FALSE);

	// �t���[���T�C�Y�Z�o
	int frameSizeX = (rect_Wind.right - rect_Wind.left);
	int frameSizeY = (rect_Wind.bottom - rect_Wind.top);

	// �E�B���h�E�̍쐬
	HWND hWnd;
	hWnd = CreateWindow(
		WINDOW_CLASS,								// �E�B���h�E�N���X�̖��O
		WINDOW_TITLE,								// �E�B���h�E��(�^�C�g���o�[�ɕ\��)
		(WS_OVERLAPPEDWINDOW/* ^ WS_THICKFRAME*/),		// �E�B���h�E�X�^�C�� ���S�X�^�C�������ʃT�C�Y�̉ϋ@�\���I�t�ɂ��Ă���(��ʃT�C�Y��ς����Ȃ�)
		CW_USEDEFAULT,								// �\���ʒu
		CW_USEDEFAULT,								// �\���ʒu
		frameSizeX,									// �E�B���h�E�̃T�C�YX
		frameSizeY,									// �E�B���h�E�̃T�C�YY
		nullptr,									// �e�n���h��
		nullptr,									// ���j���[�n���h��
		hInstance,									// �C���X�^���X�n���h��
		nullptr										// WM_CREATE���b�Z�[�W��lpparam�ɓn���l
	);
	// �G���[����
	if (hWnd == NULL)
	{
		return 0;
	}

	// �v�����ԕۑ��p
	DWORD time_current = 0;		// ���ݎ��ԕۑ��p
	DWORD time_prev = 0;		// �ߋ����ԕۑ��p

	// ���x��1ms�ɕύX
	timeBeginPeriod(1);

	// �ߋ����ԂƂ��ĕۑ�
	time_prev = timeGetTime();

	// ���b�Z�[�W�̏�����
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	// DirectX�̏�����
	if (FAILED(dx.Init(hWnd)))
	{
		return -1;
	}

	// �E�B���h�E�\��
	ShowWindow(hWnd, SW_SHOW);

	// ���C�����[�v
	while (msg.message != WM_QUIT)
	{
		// ���b�Z�[�W�̎擾
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// ���b�Z�[�W�|��
			TranslateMessage(&msg);
			// �v���V�[�W���Ƀ��b�Z�[�W���M
			DispatchMessage(&msg);
		}
		else
		{
			// ���ݎ��ԂƂ��ĕۑ�
			time_current = timeGetTime();

			// �t���[�����Ԓ��ߔ���
			if (time_current - time_prev >= FRAME_TIME)
			{
				FPS = FrameCnt * 1000 / (time_current - time_prev);
				time_prev = time_current;
				FrameCnt = 0;
			}

			if ((time_current - time_prev) >= (1000 / FPS_CNT))
			{
				// ���݂̎��Ԃ��ߋ����Ԃɑ��
				time_prev = time_current;
				dx.Update();			// DirectX�X�V����
				dx.Draw();				// DirectX�`�揈��
				// �t���[���J�E���g
				FrameCnt++;
			}
		}
	}

	// ���b�Z�[�W�̕�����Windows�ɕԂ�
	return msg.wParam;
}


// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	// ���b�Z�[�W�ɑΉ�����
	switch (nMsg)
	{
		// �~�������ꂽ
	case WM_CLOSE:
		// DirectX�̏I������
		dx.Uninit();
		// ���x��߂�
		timeBeginPeriod(0);
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			return 0;
		}
		break;
	}

	// switch�ŏ������Ȃ��������b�Z�[�W�̏���
	return DefWindowProc(hWnd, nMsg, wParam, lParam);
}