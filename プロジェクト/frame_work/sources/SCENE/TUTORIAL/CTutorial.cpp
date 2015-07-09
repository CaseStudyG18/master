//=============================================================================
//
// CTutorialクラス [CTutorial.h]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CTutorial.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../../CONTROLLER/CControllerManager.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// ゲームロゴ
static const float TITLE_LOGO_WIDTH = 900;
static const float TITLE_LOGO_HEIGHT = 370;
static const D3DXVECTOR3 TITLE_LOGO_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 180, 0);

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTutorial::CTutorial(void)
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTutorial ::~CTutorial(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CTutorial::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// フェーズの初期化
	CPhase::Init(pDevice, mode);

	// 変数初期化
	m_pBackGround = NULL;

	// フェード作成
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// 背景
	m_pBackGround = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_BG);
	m_pBackGround->Init(
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_BG_TUTORIAL);

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// ＢＧＭ再生
	//	CManager::PlaySoundA(SOUND_LABEL_BGM000);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CTutorial::Uninit(void)
{
	// 音停止
	CManager::StopSound();

	// フェイズの終了
	CPhase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CTutorial::Update(void)
{
	// フェイズの更新
	CPhase::Update();

	// フェードアウトしてタイトルヘもどる
	// 入力されたらPUSH START実行
	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
		CControllerManager::GetTriggerKey(CInputGamePad::KEY_A, 0) ||
		CControllerManager::GetTriggerKey(CInputGamePad::KEY_B, 0) ||
		CControllerManager::GetTriggerKey(CInputGamePad::KEY_X, 0) ||
		CControllerManager::GetTriggerKey(CInputGamePad::KEY_Y, 0)){

		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
		m_pManager->SetNextPhase(MODE_PHASE_TITLE);
	}

}

//*****************************************************************************
// クリエイト
//*****************************************************************************
CTutorial* CTutorial::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CTutorial* pTitle = NULL;

	// 作成
	pTitle = new CTutorial;

	// 初期化
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//----EOF----