//=============================================================================
//
// CResultクラス [CResult.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CResult.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../CSCENE/CSceneAnime.h"
#include "../GAME/PLAYER/CPlayer.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// 勝者 敗者
enum PlayerResult{
	WIN,
	LOSE,
	DRAW,
	PLAYER_RESULT_MAX
};
// リモアニメーションの大きさ
static const D3DXVECTOR2 RESULT_RIMO_SIZE[PLAYER_RESULT_MAX] = {
	// 勝者のサイズ
	D3DXVECTOR2(150, 150),
	// 敗者のサイズ
	D3DXVECTOR2(100, 100),
	// 引き分けのサイズ
	D3DXVECTOR2(125, 125),
};
// リモアニメーションの座標
static const D3DXVECTOR3 RESULT_RIMO_POS[PLAYER_MAX] = {
	// 勝者の位置
	D3DXVECTOR3( 250, 400, 0),
	D3DXVECTOR3( 500, 400, 0),
	D3DXVECTOR3( 750, 400, 0),
	D3DXVECTOR3(1000, 400, 0),
};
// メニューの座標
static const D3DXVECTOR3 RESULT_MENU_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0);
// メニューの大きさ
static const D3DXVECTOR2 RESULT_MENU_SIZE = D3DXVECTOR2(500, 600);
// PUSH2Dの大きさ
static const D3DXVECTOR2 RESULT_PUSH_SIZE = D3DXVECTOR2(180, 80);
// PUSH2Dの調整位置
static const D3DXVECTOR3 RESULT_PUSH_ADDPOS = D3DXVECTOR3(0, 100, 0);
// プレイヤのアニメーションのスピード
static const int RESULT_RIMO_ANIME_SPEED = 10;
// プレイヤの負けアニメーションのインデックス
static const int RESULT_RIMO_LOSE_INDEX_MAX = 13;
static const int RESULT_RIMO_LOSE_INDEX_MIN = 10;
// プレイヤの勝ちアニメーションの最小インデックス
static const int RESULT_RIMO_WIN_INDEX_MAX = 9;
static const int RESULT_RIMO_WIN_INDEX_MIN = 6;
// 自動でＰＵＳＨが出るまでのカウント
static const int RESULT_PUSH_INTERVAL = 100;
// PUSHの点滅間隔
static const int RESULT_PUSH_FLASH_INTERVAL = 5;
// メニューのテクスチャ　カーソル番号ごと
static const TEXTURE_TYPE RESULT_MENU_TEXTURE[] = {
	TEXTURE_RESULT_MENU_BACK_0, TEXTURE_RESULT_MENU_BACK_1, TEXTURE_RESULT_MENU_BACK_2
};
static const TEXTURE_TYPE RESULT_MENU_MOJI_TEXTURE[] = {
	TEXTURE_RESULT_MENU_MOJI_0, TEXTURE_RESULT_MENU_MOJI_1, TEXTURE_RESULT_MENU_MOJI_2
};
// メニューの文字アニメーションの拡大スピード
static const D3DXVECTOR2 RESULT_MENU_MOJI_ADDSIZE = D3DXVECTOR2(2.5f, 2.5f);
static const float RESULT_MENU_MOJI_SIZE_MAX = 550;
static const float RESULT_MENU_MOJI_SIZE_MIN = 480;
// メニュー文字の座標
static const D3DXVECTOR3 RESULT_MENU_MOJI_POS[RESULT_MENU_MAX] = {
	D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 180),	// リトライ
	D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 360),	// キャラセレクト
	D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 550),	// ステージセレクト
};
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CResult::CResult(void)
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

	// 変数の初期化
	m_nWinPlayerNum = 0;
	m_nCount = 0;
	m_pMenuBack = NULL;
	m_nMenuCursol = 0;
	m_vMojiSize = RESULT_MENU_SIZE;
	m_bMojiScale = true;
	for (int i = 0; i < PLAYER_MAX; i++){
		m_nTextureIndex[i] = 0;
		m_nTextureCount[i] = 0;
		m_pPlayerAnime[i] = NULL;
		m_pPush2D[i] = NULL;
		m_bPush[i] = false;
		m_nPushFlashCount[i] = 0;
		m_bPushFlash[i] = true;
	}
	for (int i = 0; i < RESULT_MENU_MAX; i++){
		m_pMenuMoji[i] = NULL;
	}

	// 勝ったプレイヤ番号取得
	m_nWinPlayerNum = CManager::GetWinPlayerNum();

	// 背景
	InitializeBG();
	// リモの勝者アニメーション初期化
	InitializeWinAnimation();

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// ＢＧＭ再生
	//	CManager::PlaySoundA(SOUND_LABEL_BGM000);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CResult::Uninit(void)
{
	// 音停止
	CManager::StopSound();

	// フェイズの終了
	CPhase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CResult::Update(void)
{
	// フェイズの更新
	CPhase::Update();

	// アニメの更新
	UpdateWinAnimation();

	// プッシュがでるまでカウント
	if (m_nCount < RESULT_PUSH_INTERVAL){
		m_nCount++;
	}
	// プッシュがでる
	else if (m_nCount == RESULT_PUSH_INTERVAL){
		// ここを二度ととおらない
		m_nCount++;
		// PUSH２Ｄ初期化
		for (int i = 0; i < PLAYER_MAX; i++){
			m_pPush2D[i] = CScene2D::Create(m_pD3DDevice,
				RESULT_RIMO_POS[i] + RESULT_PUSH_ADDPOS,
				RESULT_PUSH_SIZE.x, RESULT_PUSH_SIZE.y,
				TEXTURE_RESULT_PUSH, TYPE_PRIORITY_GOAL);
			// プレイヤの色に合わせる
			m_pPush2D[i]->SetColorPolygon(PLAYER_COLOR[i]);
		}

	}
	// プッシュがでてる
	else{
		// 点滅アニメーション
		for (int i = 0; i < PLAYER_MAX; i++){
			m_nPushFlashCount[i]++;
			if (m_nPushFlashCount[i] > RESULT_PUSH_FLASH_INTERVAL){
				m_nPushFlashCount[i] = 0;
				m_bPushFlash[i] = !m_bPushFlash[i];
				m_pPush2D[i]->SetDrawFlag(m_bPushFlash[i]);
				if (m_bPush[i])	m_pPush2D[i]->SetDrawFlag(true);
			}
		}

		// 入力受け受け
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 0)){
			m_bPush[0] = !m_bPush[0];
		}
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 1)){
			m_bPush[1] = !m_bPush[1];
		}
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 2)){
			m_bPush[2] = !m_bPush[2];
		}
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 3)){
			m_bPush[3] = !m_bPush[3];
		}

		// すべてＰＵＳＨされた + メニューが出ていない状態（一回だけ通るように）
		if ((m_bPush[0] && m_bPush[1] && m_bPush[2] && m_bPush[3]) && !m_pMenuBack){
			m_pMenuBack = CScene2D::Create(m_pD3DDevice,
				RESULT_MENU_POS,
				RESULT_MENU_SIZE.x, RESULT_MENU_SIZE.y,
				RESULT_MENU_TEXTURE[m_nMenuCursol], TYPE_PRIORITY_THREAD_OF_FOOTHOLD);
			// メニューの文字
			for (int n = 0; n < RESULT_MENU_MAX; n++){
				m_pMenuMoji[n] = CScene2D::Create(m_pD3DDevice,
					RESULT_MENU_MOJI_POS[n],
					RESULT_MENU_SIZE.x, RESULT_MENU_SIZE.y,
					RESULT_MENU_MOJI_TEXTURE[n], TYPE_PRIORITY_PLAYER + n);
			}

			// つぎの選択メニューが出ている状態の分岐に連続フレームで通らないように脱出
			return;
		}
	}

	// 選択メニューが出ている状態
	if (m_pMenuBack){
		// 選択中の文字を拡大アニメーション
		if (m_bMojiScale){
			m_vMojiSize += RESULT_MENU_MOJI_ADDSIZE;
		}
		else{
			m_vMojiSize -= RESULT_MENU_MOJI_ADDSIZE;
		}
		if (m_vMojiSize.x > RESULT_MENU_MOJI_SIZE_MAX){
			m_bMojiScale = false;
		}
		else if (m_vMojiSize.x < RESULT_MENU_MOJI_SIZE_MIN){
			m_bMojiScale = true;
		}
		m_pMenuMoji[m_nMenuCursol]->SetWidth(m_vMojiSize.x);
		m_pMenuMoji[m_nMenuCursol]->SetHeight(m_vMojiSize.y);

		// 矢印キー
		if (CInputKeyboard::GetKeyboardTrigger(DIK_W) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::LEFT_STICK_UP, 0)){
			m_nMenuCursol--;
			if (m_nMenuCursol < 0){
				m_nMenuCursol = RESULT_MENU_MAX - 1;
			}
		}
		else if (CInputKeyboard::GetKeyboardTrigger(DIK_S) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::LEFT_STICK_DOWN, 0)){
			m_nMenuCursol++;
			if (m_nMenuCursol >= RESULT_MENU_MAX){
				m_nMenuCursol = 0;
			}
		}
		// 矢印の入力があったら
		if (CInputKeyboard::GetKeyboardTrigger(DIK_W) ||
			CInputKeyboard::GetKeyboardTrigger(DIK_S) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::LEFT_STICK_UP, 0) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::LEFT_STICK_DOWN, 0)){
			// メニューのテクスチャを変更
			m_pMenuBack->ChangeTexture(RESULT_MENU_TEXTURE[m_nMenuCursol]);
			// メニュー文字の大きさリセット
			m_pMenuMoji[m_nMenuCursol]->SetHeight(RESULT_MENU_SIZE.y);
			m_pMenuMoji[m_nMenuCursol]->SetWidth(RESULT_MENU_SIZE.x);
			m_bMojiScale = true;
			m_vMojiSize = RESULT_MENU_SIZE;
		}
		// 決定キー
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 0)){
			// フェードアウト開始
			m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
			// それぞれのシーンへ飛ぶ
			if (m_nMenuCursol == RESULT_MENU_RETRY){
				m_pManager->SetNextPhase(MODE_PHASE_GAME);
			}
			else if (m_nMenuCursol == RESULT_MENU_CHARA_SELECT){
				m_pManager->SetNextPhase(MODE_PHASE_CHARA_SELECT);
			}
			else
				m_pManager->SetNextPhase(MODE_PHASE_STAGE_SELECT);
		}
	}
}

//*****************************************************************************
// クリエイト
//*****************************************************************************
CResult* CResult::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CResult* pTitle = NULL;

	// 作成
	pTitle = new CResult;

	// 初期化
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//-----------------------------------------------------------------------------
// リモの勝者アニメーション初期化
//-----------------------------------------------------------------------------
void CResult::InitializeWinAnimation(void){

	// 引き分けのアニメ
	if (m_nWinPlayerNum == -1){
		for (int i = 0; i < PLAYER_MAX; i++){
			m_pPlayerAnime[i] = CSceneAnime::Create(m_pD3DDevice,
				RESULT_RIMO_POS[i],
				RESULT_RIMO_SIZE[DRAW].x, RESULT_RIMO_SIZE[DRAW].y,
				TEXTURE_PLAYER, PLAYER_WALK_TEXTURE_SEP_X, PLAYER_WALK_TEXTURE_SEP_Y,
				RESULT_RIMO_ANIME_SPEED, -1, TYPE_PRIORITY_BG);
		}

	}
	// 誰かが勝ったアニメ
	else{
		for (int i = 0; i < PLAYER_MAX; i++){
			// 勝った人のアニメ
			if (i == m_nWinPlayerNum){
				m_pPlayerAnime[i] = CSceneAnime::Create(m_pD3DDevice,
					RESULT_RIMO_POS[i],
					RESULT_RIMO_SIZE[WIN].x, RESULT_RIMO_SIZE[WIN].y,
					TEXTURE_PLAYER, PLAYER_WALK_TEXTURE_SEP_X, PLAYER_WALK_TEXTURE_SEP_Y,
					RESULT_RIMO_ANIME_SPEED, -1, TYPE_PRIORITY_BG);
			}
			// 負けた人のアニメ
			else{
				m_pPlayerAnime[i] = CSceneAnime::Create(m_pD3DDevice,
					RESULT_RIMO_POS[i],
					RESULT_RIMO_SIZE[LOSE].x, RESULT_RIMO_SIZE[LOSE].y,
					TEXTURE_PLAYER, PLAYER_WALK_TEXTURE_SEP_X, PLAYER_WALK_TEXTURE_SEP_Y,
					RESULT_RIMO_ANIME_SPEED, -1, TYPE_PRIORITY_BG);
			}
		}
	}
	// 自動で更新しない
	for (int i = 0; i < PLAYER_MAX; i++){
		m_pPlayerAnime[i]->SetAutoUpdate(false);
		// 勝った人のインデックス
		if (i == m_nWinPlayerNum){
			m_nTextureIndex[i] = RESULT_RIMO_WIN_INDEX_MIN;
		}
		// 負けた人のインデックス
		else{
			m_nTextureIndex[i] = RESULT_RIMO_LOSE_INDEX_MIN;
		}
		m_pPlayerAnime[i]->SetIndex(m_nTextureIndex[i]);
	}

}

//-----------------------------------------------------------------------------
// リモの勝者アニメーション更新
//-----------------------------------------------------------------------------
void CResult::UpdateWinAnimation(void){

	for (int i = 0; i < PLAYER_MAX; i++){
		m_nTextureCount[i]++;
		if (m_nTextureCount[i] > RESULT_RIMO_ANIME_SPEED){
			m_nTextureCount[i] = 0;

			m_nTextureIndex[i]++;
			// 勝った人のインデックス
			if (i == m_nWinPlayerNum){
				if (m_nTextureIndex[i] > RESULT_RIMO_WIN_INDEX_MAX){
					m_nTextureIndex[i] = RESULT_RIMO_WIN_INDEX_MIN;
				}
			}
			// 負けた人or引き分けのインデックス
			else{
				if (m_nTextureIndex[i] > RESULT_RIMO_LOSE_INDEX_MAX){
					m_nTextureIndex[i] = RESULT_RIMO_LOSE_INDEX_MIN;
				}
			}
		}
		m_pPlayerAnime[i]->SetIndex(m_nTextureIndex[i]);
	}

}

//-----------------------------------------------------------------------------
// 背景初期化
//-----------------------------------------------------------------------------
void CResult::InitializeBG(void){
	m_pBG = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_BG_RESULT, TYPE_PRIORITY_BG);
}

//----EOF----