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
#include "../../CAMERA/CCamera.h"
#include "../../MANAGER/CManager.h"
#include "../EFFECT/CEffect.h"


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

	// カメラ初期化
	m_pManager->InitCamera();

	// フェード作成
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// BGM再生
	CManager::PlaySoundA(SOUND_LABEL_BGM000);

	int num = 0;
	const TEXTURE_TYPE *tex = {};
	const int kFireMax = 8;

	for (int n = 0; n < kFireMax; n++){
		num = rand() % 10;
		for (int i = 0; i < num; i++){
			D3DXVECTOR3 pos = D3DXVECTOR3(rand() % 400, rand() % 400, 0);
			float size = 50 + rand() % 200;
			CEffect::Create(pDevice, pos, size, size, TEXTURE_FIRE_0, 10, 1, 1, -1);
		}
	}

//	m_Fire = NULL;
//	m_Fire = CEffect::Create(pDevice, D3DXVECTOR3(200, 100, 0), 200, 300, TEXTURE_FIRE_0, 10, 1, 1, -1);
}


//*****************************************************************************
// 終了
//*****************************************************************************
void CResult::Uninit(void)
{
	CManager::StopSound();
	CPhase::Uninit();

//	m_Fire->Release();
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