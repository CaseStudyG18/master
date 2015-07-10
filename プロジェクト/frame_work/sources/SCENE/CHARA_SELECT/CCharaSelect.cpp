//=============================================================================
//
// CCharaSelectクラス [CCharaSelect.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CCharaSelect.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../CSCENE/CSceneAnime.h"
#include "../GAME/PLAYER/CPlayer.h"

//*****************************************************************************
// 定数
//*****************************************************************************


//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CCharaSelect::CCharaSelect(void)
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CCharaSelect ::~CCharaSelect(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CCharaSelect::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// フェーズの初期化
	CPhase::Init(pDevice, mode);

	// フェード作成
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// 背景
	InitializeBG();

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// ＢＧＭ再生
	//	CManager::PlaySoundA(SOUND_LABEL_BGM000);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CCharaSelect::Uninit(void)
{
	// 音停止
	CManager::StopSound();

	// フェイズの終了
	CPhase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CCharaSelect::Update(void)
{
	// フェイズの更新
	CPhase::Update();

	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) || CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 0))
	{
		// フェードアウト開始
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// ゲームヘ
		m_pManager->SetNextPhase(MODE_PHASE_STAGE_SELECT);
	}

}

//*****************************************************************************
// クリエイト
//*****************************************************************************
CCharaSelect* CCharaSelect::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CCharaSelect* pTitle = NULL;

	// 作成
	pTitle = new CCharaSelect;

	// 初期化
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//-----------------------------------------------------------------------------
// 背景初期化
//-----------------------------------------------------------------------------
void CCharaSelect::InitializeBG(void){
	m_pBG = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_BG_CHARA_SELECT, TYPE_PRIORITY_BG);
}

//----EOF----