//=============================================================================
//
// CPauseクラス [CPause.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CPAUSE_H_
#define _CPAUSE_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../MAIN/main.h"
#include "../CSCENE/CScene2D.h"
#include "../UI/CCursol.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CCursol;

//*****************************************************************************
// マクロ
//*****************************************************************************
static const D3DXVECTOR3	DEFFAULT_PAUSE_POS((float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2, 0.f);	// デフォルトのポーズの初期座標
static const float			DEFFAULT_PAUSE_WIDTH = (float)SCREEN_WIDTH;								// デフォルトのポーズの幅
static const float			DEFFAULT_PAUSE_HEIGHT = (float)SCREEN_HEIGHT;							// デフォルトのポーズの高さ

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPause : public CScene2D
{
public:
	// ポーズメニューの選択中の種類
	typedef enum
	{
		PAUSE_RETRY,
		PAUSE_TITLE,
		PAUSE_RETURN,
		PAUSE_MAX
	}PAUSE_MENU;

	//======================================================
	// コンストラクタ
	// 引数: デバイス、プライオリティ
	//======================================================
	CPause(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_PAUSE);

	// デストラクタ
	~CPause();

	//======================================================
	// 初期化
	// 引数: デバイス、座標、回転、幅、テクスチャタイプ、（横分割数、縦分割数）
	// 戻り値: 成否
	//======================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	//======================================================
	// タイトルに戻るフラグゲッター
	// 戻り値: 真偽
	//======================================================
	bool GetReturnTitleFlag(void){return m_bReturnTititle;};

	//======================================================
	// リトライするかフラグゲッター
	// 戻り値: 真偽
	//======================================================
	static bool GetRetryFlag(void){return m_bRetry;};

	//======================================================
	// ポーズ中かフラグゲッター
	// 戻り値: 真偽
	//======================================================
	static bool GetPauseFlag(void){return m_bPause;};

	//======================================================
	// カーソル描画フラグセッター
	// 引数: 真偽
	//======================================================
	void SetCursolDrawFlag(bool flag){m_pCursol->SetDrawFlag(flag);};

	//======================================================
	// ポーズフラグセッター
	// 引数: 真偽
	//======================================================
	static void SetPauseFlag(bool flag){m_bPause = flag;};

	//======================================================
	// 選択中ポーズメニューセッター
	// 引数: ポーズメニューの種類
	//======================================================
	void SetChoiceMenu(PAUSE_MENU type){m_nChoiceMenu = type;};

	//======================================================
	// リトライフラグセッター
	// 引数: 真偽
	//======================================================
	static void SetRetryFlag(bool flag){m_bRetry = flag;};

	// ポーズフラグ反転
	static void ReverceFlag(void){m_bPause = !m_bPause;};

private:
	static bool	m_bPause;			// ポーズフラグ
	static bool	m_bRetry;			// リトライフラグ
	CCursol*	m_pCursol;			// カーソル
	int			m_nChoiceMenu;		// 選択中のメニュー
	bool		m_bReturnTititle;	// タイトルに戻るかのフラグ
};
#endif
//----EOF----