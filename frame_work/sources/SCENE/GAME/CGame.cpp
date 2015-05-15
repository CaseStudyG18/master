//=============================================================================
//
// CGameクラス [CGame.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CGame.h"
#include "../../CAMERA/CCamera.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "PLAYER\CPlayerManager.h"

//*****************************************************************************
// マクロ
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CGame ::CGame(void)
{
	m_pPause = NULL;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CGame ::~CGame(void)
{
	m_pPause = NULL;
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CGame::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// フェーズの初期化
	CPhase::Init(pDevice, mode);

	// カメラ初期化
	m_pManager->InitCamera();

	// フェード作成
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// ポーズ作成
	m_pPause = new CPause(pDevice);
	m_pPause->Init(DEFFAULT_PAUSE_POS, (float)SCREEN_WIDTH, DEFFAULT_PAUSE_HEIGHT, TEXTURE_NULL);

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// 音再生
	CManager::PlaySoundA(SOUND_LABEL_BGM000);

	// プレイヤーマネージャー
	m_pPlayerManager = new CPlayerManager();
	m_pPlayerManager->Init(2);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CGame::Uninit(void)
{
	m_pPlayerManager->Uninit();
	SAFE_DELETE(m_pPlayerManager);
	CManager::StopSound();
	CPhase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CGame::Update(void)
{

	m_pPlayerManager->Update();

	// Ｐが押されたら
	if(CInputKeyboard::GetKeyboardTrigger(DIK_P))
	{
		// ポーズフラグ反転
		m_pPause->ReverceFlag();
		m_pPause->SetCursolDrawFlag(false);

		// ポーズ終了なら
		if(!m_pPause->GetPauseFlag())
		{
			// カーソル位置を戻るに
			m_pPause->SetChoiceMenu(m_pPause->PAUSE_RETURN);
		}
	}

	if(!m_pPause->GetPauseFlag())
	{
		CPhase::Update();

		if(CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
		{
			// フェードアウト開始
			m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

			// リザルトへ
			m_pManager->SetNextPhase(MODE_PHASE_RESULT);
		}
	}

	// タイトルに戻る選択なら
	if(m_pPause->GetReturnTitleFlag())
	{
		// フェードアウト開始
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// タイトルへ
		m_pManager->SetNextPhase(MODE_PHASE_TITLE);
	}

	// リトライに戻る選択なら
	if(m_pPause->GetRetryFlag())
	{
		// フェードアウト開始
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
	}
}

//*****************************************************************************
// クリエイト
//*****************************************************************************
CGame* CGame::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CGame* pGame = NULL;

	// 作成
	pGame = new CGame;

	// 初期化
	pGame->Init(mode, pDevice);

	return pGame;
}
//----EOF-------