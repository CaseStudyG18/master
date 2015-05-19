//=============================================================================
//
// CGameクラス [CGame.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../CAMERA/CCamera.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../CSCENE/CSceneScore.h"
#include "CTimeManager.h"
#include "PLAYER\CPlayerManager.h"
#include "TREASURE/CTreasureManager.h"
#include "GOAL/CGoalManager.h"
#include "ATTACK\CAttackManager.h"
#include "CGame.h"

//*****************************************************************************
// マクロ
//*****************************************************************************

// ゲームの制限時間
const short GAME_TIME = 300;

// 宝物の場所
const D3DXVECTOR3 TREASURE_POS = D3DXVECTOR3(200, 100, 0);

// ゴール
const short GOAL_MAX = 4;
const D3DXVECTOR3 GOAL_POS[GOAL_MAX] = {
	D3DXVECTOR3(100, 100, 0),
	D3DXVECTOR3(700, 100, 0),
	D3DXVECTOR3(100, 500, 0),
	D3DXVECTOR3(700, 500, 0),
};
const short GOAL_PLAYER_NUMBER[GOAL_MAX] = {
	0,1,2,3
};

// プレイヤ人数
const short MANUAL_PLAYER_NUM = 1;
const short CPU_PLAYER_NUM = 3;

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

	// ゲームの時間管理と残り時間表示
	m_pTimeManager = new CTimeManager(pDevice, GAME_TIME);
	m_pTimeManager->Init();

	// 攻撃マネージャ生成
	m_pAttackManager = new CAttackManager(pDevice);
	m_pAttackManager->Init();

	// プレイヤ生成
	m_pPlayerManager = new CPlayerManager(m_pAttackManager);
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
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CGame::Uninit(void)
{
	m_pAttackManager->Uninit();
	SAFE_DELETE(m_pAttackManager);

	m_pPlayerManager->Uninit();
	SAFE_DELETE(m_pPlayerManager);

	m_pGoalManager->Uninit();
	SAFE_DELETE(m_pGoalManager);

	m_pTreasureManager->Uninit();
	SAFE_DELETE(m_pTreasureManager);

	m_pTimeManager->Uninit();
	SAFE_DELETE(m_pTimeManager);

	CManager::StopSound();
	CPhase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CGame::Update(void)
{
	// マネージャー更新
	m_pPlayerManager->Update();
	m_pTimeManager->Update();
	m_pTreasureManager->Update();

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