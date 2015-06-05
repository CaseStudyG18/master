//=============================================================================
//
// CCountDownクラス [CCountDown.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include "CCountDown.h"
#include "../ANIMATION/CAnimation.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// 数字の生成位置(visible)、出現位置、大きさ、テクスチャ、テクスチャの分割数
static const D3DXVECTOR3 COUNTDOWN_FIGURE_CREATE_POS = D3DXVECTOR3(0, -100, 0);
static const D3DXVECTOR3 COUNTDOWN_FIGURE_APPEAR_POS = D3DXVECTOR3(SCREEN_WIDTH*0.5f, 0, 0);
static const float COUNTDOWN_FIGURE_WIDTH = 100;
static const float COUNTDOWN_FIGURE_HEIGHT = 100;
static const TEXTURE_TYPE COUNTDOWN_FIGURE_TEXTURE = TEXTURE_NUMBER;
static const int COUNTDOWN_FIGURE_TEXTURE_SEPALATE_X = 10;
static const int COUNTDOWN_FIGURE_TEXTURE_SEPALATE_Y = 1;
// 数字の落ちる座標
static const float COUNTDOWN_FIGURE_FALL_POSY = SCREEN_HEIGHT * 0.5f;
// 数字のフェードスピード
static const float COUNTDOWN_FIGURE_FADE_SPEED = 0.03f;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CCountDown::CCountDown(LPDIRECT3DDEVICE9 *pDevice, bool *bPlayerControl)
{
	m_pDevice = pDevice;
	m_pFigure = NULL;
	m_nCount = 0;
	m_bPlayerControl = bPlayerControl;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CCountDown ::~CCountDown(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CCountDown::Init()
{
	m_pFigure = CAnimation::Create(
		m_pDevice,
		TYPE_PRIORITY_UI,
		COUNTDOWN_FIGURE_CREATE_POS,
		COUNTDOWN_FIGURE_WIDTH,
		COUNTDOWN_FIGURE_HEIGHT,
		COUNTDOWN_FIGURE_TEXTURE,
		COUNTDOWN_FIGURE_TEXTURE_SEPALATE_X,
		COUNTDOWN_FIGURE_TEXTURE_SEPALATE_Y);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CCountDown::Uninit(void)
{
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CCountDown::Update(void){

	m_nCount++;

	// ゲームが始まってからカウントダウンが始まるまでのカウント数
	short TIME_START = 20;
	// 数字が落ちてからフェードアウトをし始める間のカウント数
	short TIME_START_TO_FADE = 60;
	// フェードアウトし始めてから次の数字が落ちる間のカウント数
	short TIME_NEXT_COUNT = 20;

#define COUNT_MAX 5

	short TIME[COUNT_MAX] = { 0 };

	for (int i = COUNT_MAX - 1; i >= 0; i--){
		TIME[i] = TIME_START + (TIME_START + TIME_START_TO_FADE + TIME_NEXT_COUNT) * (3 - i);
	}

	for (int i = COUNT_MAX - 1; i >= 0; i--){
		if (m_nCount == TIME[i]){
			m_pFigure->SetIndex(i);
			m_pFigure->SetNonFall(COUNTDOWN_FIGURE_CREATE_POS);
			m_pFigure->SetNonFadeOut();
			m_pFigure->SetFall(COUNTDOWN_FIGURE_APPEAR_POS, COUNTDOWN_FIGURE_FALL_POSY);
		}
		if (m_nCount == TIME[i] + TIME_START_TO_FADE){
			m_pFigure->SetFadeOut(COUNTDOWN_FIGURE_FADE_SPEED);
			
			if (i == 0){
				*m_bPlayerControl = true;
			}
		}
	}
}

//----EOF-------