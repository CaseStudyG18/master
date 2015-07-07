//=============================================================================
//
// CTitleクラス [CTitle.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CTitle.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// ゲームロゴ
static const float TITLE_LOGO_WIDTH = 500;
static const float TITLE_LOGO_HEIGHT = 200;
static const D3DXVECTOR3 TITLE_LOGO_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 280, 0);
// PUSH START
static const float TITLE_PUSHSTART_WIDTH = 300;
static const float TITLE_PUSHSTART_HEIGHT = 100;
static const D3DXVECTOR3 TITLE_PUSHSTART_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 500, 0);
// MENU
static const float TITLE_MENU_WIDTH = 500;
static const float TITLE_MENU_HEIGHT = 300;
static const D3DXVECTOR3 TITLE_MENU_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0);
// MENUのカーソル
static const int TITLE_MENU_MAX = 3;
static const float TITLE_CURSOL_WIDTH = 500;
static const float TITLE_CURSOL_HEIGHT = 100;
static const D3DXVECTOR3 TITLE_CURSOL_POS[TITLE_MENU_MAX] = {
	D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 210, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 310, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 410, 0),
};

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTitle ::CTitle(void)
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTitle ::~CTitle(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CTitle::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// フェーズの初期化
	CPhase::Init(pDevice, mode);

	// フェード作成
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// 背景
	m_pBackGround = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_BG);
	m_pBackGround->Init(
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_BG_STAGE_SELECT);
	// Logo
	m_pLogo = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_UI);
	m_pLogo->Init(
		TITLE_LOGO_POS, TITLE_LOGO_WIDTH, TITLE_LOGO_HEIGHT,
		TEXTURE_BG_STAGE_SELECT);
	// PushStart
	m_pPushStart = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_UI);
	m_pPushStart->Init(
		TITLE_PUSHSTART_POS, TITLE_PUSHSTART_WIDTH, TITLE_PUSHSTART_HEIGHT,
		TEXTURE_BG_STAGE_SELECT);
	// メニュー
	m_pMenu = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_UI);
	m_pMenu->Init(
		TITLE_MENU_POS, TITLE_MENU_WIDTH, TITLE_MENU_HEIGHT,
		TEXTURE_BG_STAGE_SELECT);
	// カーソル
	m_nCursol = 0;
	m_pCursol = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_UI);
	m_pCursol->Init(
		TITLE_CURSOL_POS[m_nCursol], TITLE_CURSOL_WIDTH, TITLE_CURSOL_HEIGHT,
		TEXTURE_BG_STAGE_SELECT);

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// ＢＧＭ再生
//	CManager::PlaySoundA(SOUND_LABEL_BGM000);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CTitle::Uninit(void)
{
	// 音停止
	CManager::StopSound();

	// フェイズの終了
	CPhase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CTitle::Update(void)
{
	// フェイズの更新
	CPhase::Update();

	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) || CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 0))
	{
		// フェードアウト開始
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// ゲームヘ
		m_pManager->SetNextPhase(MODE_PHASE_CHARA_SELECT);
	}
}

//*****************************************************************************
// クリエイト
//*****************************************************************************
CTitle* CTitle::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CTitle* pTitle = NULL;

	// 作成
	pTitle = new CTitle;

	// 初期化
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//----EOF----