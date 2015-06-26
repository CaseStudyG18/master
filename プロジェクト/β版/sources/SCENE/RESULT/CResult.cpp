//=============================================================================
//
// CResultクラス [CResult.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CResult.h"
#include "../../MANAGER/CManager.h"
#include "../../SCENE/CSCENE/CScene2D.h"


//*****************************************************************************
// マクロ
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CResult ::CResult(void)
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CResult ::~CResult(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CResult::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// フェーズの初期化
	CPhase::Init(pDevice, mode);

	// フェード作成
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// BGM再生
	CManager::PlaySoundA(SOUND_LABEL_BGM000);

	m_pSprite = CScene2D::Create(
		pDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		SCREEN_WIDTH, SCREEN_HEIGHT, TEXTURE_BG_RESULT);

}


//*****************************************************************************
// 終了
//*****************************************************************************
void CResult::Uninit(void)
{
	CManager::StopSound();
	CPhase::Uninit();

}

//*****************************************************************************
// 更新
//*****************************************************************************
void CResult::Update(void)
{
	CPhase::Update();

	

	if(CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
	{
		// フェードアウト開始
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// タイトルへ
		m_pManager->SetNextPhase(MODE_PHASE_TITLE);
	}
}

//*****************************************************************************
// クリエイト
//*****************************************************************************
CResult* CResult::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CResult* pResult = NULL;

	// 作成
	pResult = new CResult;

	// 初期化
	pResult->Init(mode, pDevice);

	return pResult;
}
//----EOF----