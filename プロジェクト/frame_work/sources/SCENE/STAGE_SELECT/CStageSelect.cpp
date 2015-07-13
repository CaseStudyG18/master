//=============================================================================
//
// CStageSelectクラス [CStageSelect.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CStageSelect.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../../CONTROLLER/CControllerManager.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// ステージ絵の大きさ
static const float STAGE_SELECT_WIDTH = 400;
static const float STAGE_SELECT_HEIGHT = 250;
// ステージ絵の座標（左上、右上、左下、右下）
static const float STAGE_SELECT_WIDTH_ONE = SCREEN_WIDTH / 4;
static const D3DXVECTOR3 STAGE_SELECT_POS[] = {
	D3DXVECTOR3(STAGE_SELECT_WIDTH_ONE * 1, 200, 0),
	D3DXVECTOR3(STAGE_SELECT_WIDTH_ONE * 3, 200, 0),
	D3DXVECTOR3(STAGE_SELECT_WIDTH_ONE * 1, 500, 0),
	D3DXVECTOR3(STAGE_SELECT_WIDTH_ONE * 3, 500, 0),
};
// ステージ絵のテクスチャ（左上、右上、左下、右下）
static const TEXTURE_TYPE STAGE_SELECT_TEX[] = {
	TEXTURE_STAGE_0,
	TEXTURE_STAGE_1,
	TEXTURE_STAGE_2,
	TEXTURE_STAGE_3,
};
// 選択枠の大きさ、色
static const float STAGE_SELECT_FRAME_WIDTH = 420;
static const float STAGE_SELECT_FRAME_HEIGHT = 270;
static const D3DXCOLOR STAGE_SELECT_FRAME_COLOR = D3DXCOLOR(0.9f, 0.1f, 0.0f, 0.0f);
// 選択枠のアルファ変更スピード
static const float STAGE_SELECT_FRAME_AALPHA_SPEED = 0.05f;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CStageSelect::CStageSelect(void)
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CStageSelect::~CStageSelect(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CStageSelect::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// フェーズの初期化
	CPhase::Init(pDevice, mode);

	// フェード作成
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);
	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// 背景
	m_pBG = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_BG);
	m_pBG->Init(
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_BG_STAGE_SELECT);

	// 各ステージの絵
	for (int i = 0; i < STAGE_MAX; i++){
		m_pStage2D[i] = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_UI);
		m_pStage2D[i]->Init(
			STAGE_SELECT_POS[i],
			STAGE_SELECT_WIDTH, STAGE_SELECT_HEIGHT,
			STAGE_SELECT_TEX[i]);
	}

	// 選択中の枠 PRIORITY_UIよりも前に表示したいからEFFECTにした
	m_pFrame2D = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_EFFECT);
	m_pFrame2D->Init(
		STAGE_SELECT_POS[0],
		STAGE_SELECT_FRAME_WIDTH, STAGE_SELECT_FRAME_HEIGHT,
		TEXTURE_NULL);

	// 選択中の枠に色設定
	m_FrameColor = STAGE_SELECT_FRAME_COLOR;
	m_pFrame2D->SetColorPolygon(m_FrameColor);

	// 使用変数初期化
	m_nSelectNum = 0;
	m_bFrameAlphaAdd = true;

	// ＢＧＭ再生
//	CManager::PlaySoundA(SOUND_LABEL_BGM000);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CStageSelect::Uninit(void)
{
	// 音停止
	CManager::StopSound();

	// フェイズの終了
	CPhase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CStageSelect::Update(void)
{
	// フェイズの更新
	CPhase::Update();

	// 選択の更新
	UpdateSelect();

	// 決定ボタンを押したら
	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
		CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 0)){

		// 選択したステージ番号をセット
		CManager::SetStageNum(m_nSelectNum);

		// フェードアウト開始
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
		// ゲームヘ
		m_pManager->SetNextPhase(MODE_PHASE_GAME);
	}
}

//*****************************************************************************
// クリエイト
//*****************************************************************************
CStageSelect* CStageSelect::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CStageSelect* pTitle = NULL;

	// 作成
	pTitle = new CStageSelect;

	// 初期化
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//*****************************************************************************
// 選択中の更新
//*****************************************************************************
void CStageSelect::UpdateSelect(){
	
	// 入力で選択を変更
	if (CInputKeyboard::GetKeyboardTrigger(DIK_A) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_LEFT, 0)){
		if (m_nSelectNum == 0)
			m_nSelectNum = 1;
		else if (m_nSelectNum == 2)
			m_nSelectNum = 3;
		else
			m_nSelectNum--;
	}
	else if (CInputKeyboard::GetKeyboardTrigger(DIK_D) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_RIGHT, 0)){
		if (m_nSelectNum == 1)
			m_nSelectNum = 0;
		else if (m_nSelectNum == 3)
			m_nSelectNum = 2;
		else
			m_nSelectNum++;
	}
	if (CInputKeyboard::GetKeyboardTrigger(DIK_W) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_UP, 0)){
		if (m_nSelectNum == 0)
			m_nSelectNum = 2;
		else if (m_nSelectNum == 1)
			m_nSelectNum = 3;
		else
			m_nSelectNum -= 2;
	}
	else if (CInputKeyboard::GetKeyboardTrigger(DIK_S) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_DOWN, 0)){
		if (m_nSelectNum == 2)
			m_nSelectNum = 0;
		else if (m_nSelectNum == 3)
			m_nSelectNum = 1;
		else
			m_nSelectNum += 2;
	}

	// 移動の入力があったら
	if (CInputKeyboard::GetKeyboardTrigger(DIK_A) ||
		CInputKeyboard::GetKeyboardTrigger(DIK_D) ||
		CInputKeyboard::GetKeyboardTrigger(DIK_W) ||
		CInputKeyboard::GetKeyboardTrigger(DIK_S) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_RIGHT, 0) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_LEFT, 0) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_UP, 0) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_DOWN, 0)){
		// 選択枠の移動
		m_pFrame2D->SetPos(STAGE_SELECT_POS[m_nSelectNum]);
		// なんか不自然だったから色も初期化
		m_bFrameAlphaAdd = true;
		m_FrameColor.a = 0.0f;
	}

	// 選択枠の色変更
	if (m_bFrameAlphaAdd){
		m_FrameColor.a += STAGE_SELECT_FRAME_AALPHA_SPEED;
		if (m_FrameColor.a > 1.0f){
			m_FrameColor.a = 1.0f;
			m_bFrameAlphaAdd = !m_bFrameAlphaAdd;
		}
	}
	else {
		m_FrameColor.a -= STAGE_SELECT_FRAME_AALPHA_SPEED;
		if (m_FrameColor.a < 0.0f){
			m_FrameColor.a = 0.0f;
			m_bFrameAlphaAdd = !m_bFrameAlphaAdd;
		}
	}
	m_pFrame2D->SetColorPolygon(m_FrameColor);
}
//----EOF----