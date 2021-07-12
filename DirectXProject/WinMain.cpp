//=============================================
// メイン関数
//---------------------------------------------
// 制作：みやしろ　ゆきひろ
//---------------------------------------------
// WinMain.cpp
//=============================================
// ---　インクルード部　---
#include "WinMain.h"

// ===== ライブラリのリンク =====
#pragma comment (lib,"winmm.lib")

// マクロ・定数定義
#define WINDOW_TITLE		("ウィンドウの表示")
#define WINDOW_CLASS		("DX")
#define FRAME_TIME			(500)
#define FPS_CNT				(60)

// ===== プロトタイプ宣言 =====
LRESULT CALLBACK WindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

// グローバル宣言
DX11 dx;				// DirectXクラス
int FrameCnt;			// フレームカウンタ
int FPS;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ウィンドウクラスを初期化
	WNDCLASSEX window_class =
	{
		sizeof(WNDCLASSEX),					// 構造体のサイズ
		CS_HREDRAW | CS_VREDRAW,			// クラスのスタイル
		WindowProc,							// ウィンドウプロシージャ
		0,									// 補助メモリ
		0,									// 補助メモリ
		hInstance,							// このアプリのインスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),	// アイコン画像
		LoadCursor(NULL, IDC_ARROW),		// カーソル画像
		NULL,								// 背景ブラシ
		NULL,								// メニュー名
		WINDOW_CLASS,						// クラス名
		NULL								// 小さいアイコン
	};
	// ウィンドウクラスの登録
	if (!RegisterClassEx(&window_class))
	{
		// 終了
		return 0;
	}

	// ウィンドウサイズを求める
	RECT rect_Wind
	{
		0,
		0,
		dx.SCREEN_WIDTH,
		dx.SCREEN_HEIGHT
	};
	AdjustWindowRect(&rect_Wind, WS_OVERLAPPED, FALSE);

	// フレームサイズ算出
	int frameSizeX = (rect_Wind.right - rect_Wind.left);
	int frameSizeY = (rect_Wind.bottom - rect_Wind.top);

	// ウィンドウの作成
	HWND hWnd;
	hWnd = CreateWindow(
		WINDOW_CLASS,								// ウィンドウクラスの名前
		WINDOW_TITLE,								// ウィンドウ名(タイトルバーに表示)
		(WS_OVERLAPPEDWINDOW/* ^ WS_THICKFRAME*/),		// ウィンドウスタイル ※全スタイルから画面サイズの可変機能をオフにしている(画面サイズを変えられない)
		CW_USEDEFAULT,								// 表示位置
		CW_USEDEFAULT,								// 表示位置
		frameSizeX,									// ウィンドウのサイズX
		frameSizeY,									// ウィンドウのサイズY
		nullptr,									// 親ハンドル
		nullptr,									// メニューハンドル
		hInstance,									// インスタンスハンドル
		nullptr										// WM_CREATEメッセージでlpparamに渡す値
	);
	// エラー処理
	if (hWnd == NULL)
	{
		return 0;
	}

	// 計測時間保存用
	DWORD time_current = 0;		// 現在時間保存用
	DWORD time_prev = 0;		// 過去時間保存用

	// 精度を1msに変更
	timeBeginPeriod(1);

	// 過去時間として保存
	time_prev = timeGetTime();

	// メッセージの初期化
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	// DirectXの初期化
	if (FAILED(dx.Init(hWnd)))
	{
		return -1;
	}

	// ウィンドウ表示
	ShowWindow(hWnd, SW_SHOW);

	// メインループ
	while (msg.message != WM_QUIT)
	{
		// メッセージの取得
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// メッセージ翻訳
			TranslateMessage(&msg);
			// プロシージャにメッセージ送信
			DispatchMessage(&msg);
		}
		else
		{
			// 現在時間として保存
			time_current = timeGetTime();

			// フレーム時間超過判定
			if (time_current - time_prev >= FRAME_TIME)
			{
				FPS = FrameCnt * 1000 / (time_current - time_prev);
				time_prev = time_current;
				FrameCnt = 0;
			}

			if ((time_current - time_prev) >= (1000 / FPS_CNT))
			{
				// 現在の時間を過去時間に代入
				time_prev = time_current;
				dx.Update();			// DirectX更新処理
				dx.Draw();				// DirectX描画処理
				// フレームカウント
				FrameCnt++;
			}
		}
	}

	// メッセージの部分をWindowsに返す
	return msg.wParam;
}


// ウィンドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	// メッセージに対応する
	switch (nMsg)
	{
		// ×が押された
	case WM_CLOSE:
		// DirectXの終了処理
		dx.Uninit();
		// 精度を戻す
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

	// switchで処理しなかったメッセージの処理
	return DefWindowProc(hWnd, nMsg, wParam, lParam);
}