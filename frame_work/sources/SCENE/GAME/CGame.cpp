//=============================================================================
//
// CGameクラス [CGame.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../CSCENE/CSceneScore.h"
#include "CTimeManager.h"
#include "PLAYER\CPlayerManager.h"
#include "TREASURE/CTreasureManager.h"
#include "GOAL/CGoalManager.h"
#include "ATTACK\CAttackManager.h"
#include "THREAD\CThreadManager.h"
#include "CGame.h"
#include "../../JUDGE/CJudgeManager.h"
#include "FIELD/CFieldManager.h"
#include "UI\CCountDown.h"

//*****************************************************************************
// マクロ
//*****************************************************************************

// ゲームの制限時間
static const short GAME_TIME = 300;

// 宝物の場所
static const D3DXVECTOR3 TREASURE_POS = D3DXVECTOR3(200, 100, 0);

// ゴール
static const short GOAL_MAX = 4;
static const D3DXVECTOR3 GOAL_POS[GOAL_MAX] = {
	D3DXVECTOR3(100, 100, 0),
	D3DXVECTOR3(700, 100, 0),
	D3DXVECTOR3(100, 500, 0),
	D3DXVECTOR3(700, 500, 0),
};
static const short GOAL_PLAYER_NUMBER[GOAL_MAX] = {
	0,1,2,3
};

// プレイヤ人数
static const short MANUAL_PLAYER_NUM = 2;
static const short CPU_PLAYER_NUM = 0;

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CGame ::CGame(void)
{
	m_pPause = NULL;
	m_pTimeManager = NULL;
	m_pPlayerManager = NULL;
	m_pAttackManager = NULL;
	m_pThreadManager = NULL;
	m_pGoalManager = NULL;
	m_pJudgeManager = NULL;
	m_pFieldManager = NULL;
	m_pCountDown = NULL;

	// プレイヤ操作可能フラグ
	m_bPlayerControl = false;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CGame ::~CGame(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CGame::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// フェーズの初期化
	CPhase::Init(pDevice, mode);

	// フェード作成
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// ポーズ作成
	m_pPause = new CPause(pDevice);
	m_pPause->Init(DEFFAULT_PAUSE_POS, (float)SCREEN_WIDTH, DEFFAULT_PAUSE_HEIGHT, TEXTURE_NULL);

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// ゲームの時間管理と残り時間表示
	m_pTimeManager = new CTimeManager(pDevice, GAME_TIME);
	m_pTimeManager->Init();

	// 攻撃マネージャ生成
	m_pAttackManager = new CAttackManager(pDevice);
	m_pAttackManager->Init();

	// 糸マネージャ生成
	m_pThreadManager = new CThreadManager(pDevice);
	m_pThreadManager->Init();

	// プレイヤ生成
	m_pPlayerManager = new CPlayerManager(m_pAttackManager, m_pThreadManager);
	m_pPlayerManager->Init(CPU_PLAYER_NUM, MANUAL_PLAYER_NUM);

	// 宝物生成
	m_pTreasureManager = new CTreasureManager(pDevice);
	m_pTreasureManager->Init();
	m_pTreasureManager->CreateTreasure(TREASURE_POS);

	// ゴール生成
	m_pGoalManager = new CGoalManager(pDevice);
	m_pGoalManager->Init();
	m_pGoalManager->CreateGoal(
		const_cast<D3DXVECTOR3*>(GOAL_POS),
		const_cast<short*>(GOAL_PLAYER_NUMBER),
		GOAL_MAX);

	// 音再生
	CManager::PlaySoundA(SOUND_LABEL_BGM000);

	// ジャッジ作成
	m_pJudgeManager = CJudgeManager::Create(m_pPlayerManager);

	// フィールド作成
	m_pFieldManager = new CFieldManager;
	m_pFieldManager->LoadField(m_pD3DDevice, CFieldManager::FIELD_TEST);
	// カウントダウン
	m_pCountDown = new CCountDown(m_pD3DDevice, &m_bPlayerControl);
	m_pCountDown->Init();

}

//*****************************************************************************
// 終了
//*****************************************************************************
void CGame::Uninit(void)
{
	if (m_pCountDown){
		m_pCountDown->Uninit();
		SAFE_DELETE(m_pCountDown);
	}

	if (m_pJudgeManager){
		m_pJudgeManager->Uninit();
		SAFE_DELETE(m_pJudgeManager);
	}
	if (m_pFieldManager){
		m_pFieldManager->Uninit();
		SAFE_DELETE(m_pFieldManager);
	}

	if (m_pThreadManager){
		m_pThreadManager->Uninit();
		SAFE_DELETE(m_pThreadManager);
	}

	if (m_pAttackManager){
		m_pAttackManager->Uninit();
		SAFE_DELETE(m_pAttackManager);
	}

	if (m_pPlayerManager){
		m_pPlayerManager->Uninit();
		SAFE_DELETE(m_pPlayerManager);
	}

	if (m_pGoalManager){
		m_pGoalManager->Uninit();
		SAFE_DELETE(m_pGoalManager);
	}

	if (m_pTreasureManager){
		m_pTreasureManager->Uninit();
		SAFE_DELETE(m_pTreasureManager);
	}

	if (m_pTimeManager){
		m_pTimeManager->Uninit();
		SAFE_DELETE(m_pTimeManager);
	}
	
	CManager::StopSound();
	CPhase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CGame::Update(void)
{
	// カウントダウンの更新
	m_pCountDown->Update();

	// Ｐが押されたら
	if (CInputKeyboard::GetKeyboardTrigger(DIK_P))
	{
		// ポーズフラグ反転
		m_pPause->ReverceFlag();
		m_pPause->SetCursolDrawFlag(false);

		// ポーズ終了なら
		if (!m_pPause->GetPauseFlag())
		{
			// カーソル位置を戻るに
			m_pPause->SetChoiceMenu(m_pPause->PAUSE_RETURN);
		}
	}

	if (!m_pPause->GetPauseFlag())
	{
		CPhase::Update();

		// プレイヤの操作ができるなら
		if (m_bPlayerControl){
			// マネージャー更新
			m_pPlayerManager->Update();
			m_pTimeManager->Update();
			m_pTreasureManager->Update();
			m_pJudgeManager->Update();
			m_pFieldManager->Update();
		}

		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
		{
			// フェードアウト開始
			m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

			// リザルトへ
			m_pManager->SetNextPhase(MODE_PHASE_RESULT);
		}
	}

	// タイトルに戻る選択なら
	if (m_pPause->GetReturnTitleFlag())
	{
		// フェードアウト開始
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// タイトルへ
		m_pManager->SetNextPhase(MODE_PHASE_TITLE);
	}

	// リトライに戻る選択なら
	if (m_pPause->GetRetryFlag())
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