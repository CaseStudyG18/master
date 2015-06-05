//=============================================================================
//
// CInputMouseクラス [CInputMouse.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CINPUTMOUSE_H_
#define _CINPUTMOUSE_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"
#include "CInput.h"

//*****************************************************************************
//構造体
//*****************************************************************************
// マウスの情報構造体
typedef struct 
{
	RECT	moveRect;		// 画面上で動ける範囲
	float	x;				// X座標
	float	y;				// Y座標
	bool	lButton;		// 左ボタン
	bool	rButton;		// 右ボタン
	bool	cButton;		// 真ん中ボタン
	int		moveAdd;		// 移動量
	RECT	imgRect;		// マウス用画像矩形
	int		imgWidth;		// マウス画像幅
	int		imgHeight;		// マウス画像高さ
} MSTATE;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CInputMouse : CInput
{
public:
	// コンストラクタ
	CInputMouse(void);

	// デストラクタ
	~CInputMouse(void);

	//============================================
	// 初期化
	// 引数: インスタンス、ウインドウハンドル
	// 戻り値: 成否
	//============================================
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	//============================================
	// マウスの情報取得
	// 戻り値: マウス情報のポインタ
	//============================================
	static MSTATE* GetMouseState(void){return &m_MState;};

private:
	static MSTATE			m_MState;			// マウス情報構造体
	static HANDLE			m_hevtMouse;		// ハンドル
	static DIMOUSESTATE2	m_diMouseState;		// マウスの情報
	static POINT			m_Point;			// 座標
	static HWND				m_hWnd;				// ウインドウハンドル

};

#endif
//----EOF----