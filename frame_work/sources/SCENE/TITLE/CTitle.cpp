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
#include "../../CAMERA/CCamera.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../CSCENE/CScene3D.h"

//*****************************************************************************
// マクロ
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTitle ::CTitle(void)
{
	m_p2D = NULL;
	m_p3D = NULL;
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

	m_p2D = m_p2D->Create(m_pD3DDevice, D3DXVECTOR3(0,0,0), 100, 100, TEXTURE_PLAYER);
	m_p3D = m_p3D->Create(m_pD3DDevice, D3DXVECTOR3(0,0,0), 100, 100, TEXTURE_PLAYER);

	// カメラ初期化
	m_pManager->InitCamera();

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// ＢＧＭ再生
	//CManager::PlaySoundA(SOUND_LABEL_BGM000);
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

	if(CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
	{
		// フェードアウト開始
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// ゲームヘ
		m_pManager->SetNextPhase(MODE_PHASE_GAME);
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