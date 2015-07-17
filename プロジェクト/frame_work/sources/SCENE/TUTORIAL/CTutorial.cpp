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
// 絵の移動量
static const D3DXVECTOR3 TUTORIAL_PICT_VELO = D3DXVECTOR3(100, 0, 0);
// 絵の慣性調整位置
static const float TUTORIAL_PICT_RESIST = 0.98f;
// 絵の位置
static const D3DXVECTOR3 TUTORIAL_PICT_POS[TUTORIAL_PICT_MAX] = {
	D3DXVECTOR3(SCREEN_WIDTH * 0.5f - SCREEN_WIDTH, SCREEN_HEIGHT * 0.5f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.5f + SCREEN_WIDTH, SCREEN_HEIGHT * 0.5f, 0),
};
// ページ数に対応したテクスチャ
static const TEXTURE_TYPE TUTORIAL_PAGE_TEXTURE[TUTORIAL_PAGE_MAX] = {
//	TEXTURE_TUTORIAL_0,
//	TEXTURE_TUTORIAL_1,
//	TEXTURE_TUTORIAL_2,
//	TEXTURE_TUTORIAL_3,
	TEXTURE_STAGE_1,
	TEXTURE_STAGE_2,
	TEXTURE_STAGE_3,
	TEXTURE_STAGE_4,
};

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
	m_vVelo = D3DXVECTOR3(0, 0, 0);
	m_nPage = 0;
	m_bRight = true;

	// フェード作成
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// 背景
	m_pBackGround = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_BG_TUTORIAL, TYPE_PRIORITY_BG);

	// チュートリアルの絵
	for (int n = 0; n < TUTORIAL_PICT_MAX; n++){
		m_p2D[n] = CScene2D::Create(m_pD3DDevice,
			TUTORIAL_PICT_POS[n],
			static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
			TEXTURE_TUTORIAL_0, TYPE_PRIORITY_FIELD);
	}

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
		CControllerManager::GetTriggerKey(CInputGamePad::KEY_DECIDE, 0)){

		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
		m_pManager->SetNextPhase(MODE_PHASE_TITLE);
	}


	// 絵が止まっているときのみ動かせる
	if (m_vVelo.x == 0){
		// 左右キーで次のページへ移動
		for (int i = 0; i < PLAYER_MAX; i++){
			if (CInputKeyboard::GetKeyboardTrigger(DIK_A) ||
				CInputGamePad::GetGamePadTrigger(CInputGamePad::LEFT_STICK_RIGHT, i)){
				// 絵の移動量を入れる
				m_vVelo = TUTORIAL_PICT_VELO;
				// フラグ切り替え
				m_bRight = true;
				// ページ切り替え
				m_nPage--;
				if (m_nPage <= -1){
					m_nPage = TUTORIAL_PAGE_MAX - 1;
	  			}
				// テクスチャ切り替え
				m_p2D[0]->ChangeTexture(TUTORIAL_PAGE_TEXTURE[m_nPage]);
				// 一回だけ通る
				break;
			}
			else if (CInputKeyboard::GetKeyboardTrigger(DIK_D) ||
				CInputGamePad::GetGamePadTrigger(CInputGamePad::LEFT_STICK_LEFT, i)){

				// 絵の移動量を入れる
				m_vVelo = -TUTORIAL_PICT_VELO;
				// フラグ切り替え
				m_bRight = false;
				// ページ切り替え
				m_nPage++;
				if (m_nPage > TUTORIAL_PAGE_MAX - 1){
					m_nPage = 0;
				}
				// テクスチャ切り替え
				m_p2D[2]->ChangeTexture(TUTORIAL_PAGE_TEXTURE[m_nPage]);
				// 一回だけ通る
				break;
			}
		}
	}

	// 絵の動き
	for (int n = 0; n < TUTORIAL_PICT_MAX; n++){
		m_vVelo *= TUTORIAL_PICT_RESIST;
		m_p2D[n]->AddPos(m_vVelo);
	}

	// 端っこ当たり判定
	if (m_bRight){
		if (m_p2D[0]->GetPos().x >= SCREEN_WIDTH * 0.5f){
			// 移動量初期化
			m_vVelo = D3DXVECTOR3(0, 0, 0);

			// 絵のポインタの入れ替え
			CScene2D *t;
			t = m_p2D[0];
			m_p2D[0] = m_p2D[1];
			m_p2D[1] = m_p2D[2];
			m_p2D[2] = t;

			// 全位置調整
			for (int n = 0; n < TUTORIAL_PICT_MAX; n++){
				m_p2D[n]->SetPos(TUTORIAL_PICT_POS[n]);
			}
		}
	}
	else{
		if (m_p2D[2]->GetPos().x <= SCREEN_WIDTH * 0.5f){
			// 移動量初期化
			m_vVelo = D3DXVECTOR3(0, 0, 0);

			// 絵のポインタの入れ替え
			CScene2D *t;
			t = m_p2D[2];
			m_p2D[2] = m_p2D[1];
			m_p2D[1] = m_p2D[0];
			m_p2D[0] = t;

			// 全位置調整
			for (int n = 0; n < TUTORIAL_PICT_MAX; n++){
				m_p2D[n]->SetPos(TUTORIAL_PICT_POS[n]);
			}
		}
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