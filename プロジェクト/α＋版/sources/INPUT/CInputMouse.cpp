//=============================================================================
//
// CInputMouseクラス [CInputMouse.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CInputMouse.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
static const RECT	RECT_MOV_MOUSE = {10, 10, 500, 700};		// 描画範囲
static const int	DEFAULT_MOV_SPD = 2;						// マウスのデフォルト移動速度
static const RECT	RECT_IMAGE_MOUSE = {400, 0, 420, 20};		// マウスの画像範囲

//*****************************************************************************
// スタティックメンバ変数
//*****************************************************************************
MSTATE			CInputMouse::m_MState;
HANDLE			CInputMouse::m_hevtMouse;
DIMOUSESTATE2	CInputMouse::m_diMouseState;
POINT			CInputMouse::m_Point;
HWND			CInputMouse::m_hWnd;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CInputMouse::CInputMouse(void)
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CInputMouse::~CInputMouse(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 親クラスの初期化
	CInput ::Init(hInstance, hWnd);

	m_hWnd = hWnd;
	HRESULT hr;
	//------------------------------------------------------- 
	// マウスポインタ情報の初期化
	// マウスカーソルの動く範囲
	SetRect(&m_MState.moveRect
			, RECT_MOV_MOUSE.left
			, RECT_MOV_MOUSE.top
			, RECT_MOV_MOUSE.right
			, RECT_MOV_MOUSE.bottom);

	// マウスカーソルの座標を初期化
	m_MState.x = (float)m_MState.moveRect.left;
	m_MState.y = (float)m_MState.moveRect.top;
	m_diMouseState.lX = m_MState.moveRect.left;
	m_diMouseState.lY = m_MState.moveRect.top;

	// 左ボタンの情報を初期化
	m_MState.lButton = false;

	// 右ボタンの情報を初期化
	m_MState.rButton = false;

	// 中央ボタンの情報を初期化
	m_MState.cButton = false;

	// マウスカーソルの移動量を設定
	m_MState.moveAdd = DEFAULT_MOV_SPD;

	// マウスカーソル画像の矩形を設定
	SetRect(&m_MState.imgRect
			, RECT_IMAGE_MOUSE.left
			, RECT_IMAGE_MOUSE.top
			, RECT_IMAGE_MOUSE.right
			, RECT_IMAGE_MOUSE.bottom);

	// 画像の幅を計算
	m_MState.imgWidth = m_MState.imgRect.right - m_MState.imgRect.left;

	// 画像の高さを計算
	m_MState.imgHeight = m_MState.imgRect.bottom - m_MState.imgRect.top;

	// DirectInputオブジェクトの作成
	if(m_pDInput==NULL)
	{
		hr = DirectInput8Create(hInstance
							, DIRECTINPUT_VERSION
							, IID_IDirectInput8
							, (void**)&m_pDInput
							, NULL);
		if(FAILED(hr))
		{
			MessageBox(NULL,"DirectInputオブジェクトの作成失敗","DirectInputオブジェクトの作成失敗",MB_OK);
			return hr;
		}
	}

	// デバイスオブジェクトを作成
	hr = m_pDInput->CreateDevice(GUID_SysMouse
								, &m_pDIDevice
								, NULL);
	if(FAILED(hr))
	{
		MessageBox(NULL,"デバイスオブジェクトの作成失敗","デバイスオブジェクトの作成失敗",MB_OK);
		return hr;
	}

	// データフォーマットを設定
	hr = m_pDIDevice->SetDataFormat(&c_dfDIMouse2);
	if(FAILED(hr))
	{
		MessageBox(NULL,"データフォーマットの設定失敗","データフォーマットの設定失敗",MB_OK);
		return hr;
	}

	//バッファサイズの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;
	hr = m_pDIDevice->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph);
	if ( FAILED( hr ) )
	{
		return false;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(NULL,"協調モードの設定失敗","協調モードの設定失敗",MB_OK);
		return hr;
	}

	// マウスへのアクセス権を獲得(入力制御開始)
	m_pDIDevice->Acquire();

	return S_OK;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CInputMouse::Uninit(void)
{
	if(m_pDIDevice != NULL)
	{
		m_pDIDevice->Release();
		m_pDIDevice = NULL;
	}
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CInputMouse::Update(void)
{
	// カーソルの座標ゲット
	GetCursorPos(&m_Point);

	// スクリーン座標に合わせる
	ScreenToClient(m_hWnd, &m_Point);

	// データを取得
	if(SUCCEEDED(m_pDIDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_diMouseState)))
	{
		// 取得した情報を元にマウスの情報を更新
		if(m_diMouseState.lX)
		{
			m_MState.x = (float)m_Point.x;
		}
		if(m_diMouseState.lY)
		{
			m_MState.y = (float)m_Point.y;
		}
		(m_diMouseState.rgbButtons[0] & 0x80) ? m_MState.lButton = true : m_MState.lButton = false;
		(m_diMouseState.rgbButtons[1] & 0x80) ? m_MState.rButton = true : m_MState.rButton = false;
		(m_diMouseState.rgbButtons[2] & 0x80) ? m_MState.cButton = true : m_MState.cButton = false;
	}
	// 失敗したら
	else
	{
		// 権利取得
		m_pDIDevice->Acquire();
	}
}

//----EOF----