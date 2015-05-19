//=============================================================================
//
// CPlayerクラス [CPlayer.cpp]
// Author : 佐藤　諒一
//
//=============================================================================
#include "CPlayer.h"

#include "../INPUT/CInputKeyboard.h"
#include "../TEXTURE/CTexture.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//	引数　　デバイス、プライオリティ、オブジェクトタイプ
//-----------------------------------------------------------------------------
CPlayer::CPlayer(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE objType) :CScene2D(pDevice, nPriority, objType)
{
	m_pD3DDevice = pDevice;									// デバイスオブジェクト(描画に必要)
	m_pD3DVtxBuff = NULL;									// 頂点座標情報を格納する場所のアドレスを確保する場所
	m_pD3DTexBuff = NULL;									// テクスチャ情報を格納する場所のアドレスを確保する場所
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// ポリゴンの中央の位置
	m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// ポリゴンの角度
	m_vRotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ポリゴンの目的の角度
	m_fMoveSpeedX = 0.0f;									// プレイヤーの横軸の移動速度
	m_fMoveSpeedY = 0.0f;									// プレイヤーの縦軸の移動速度
	m_Action = PLAYER_ACTION_NONE;							// プレイヤーの行動
	m_Mode = PLAYER_MODE_NONE;								// プレイヤーの現在の変形状態
	m_ModeDest = PLAYER_MODE_NONE;							// プレイヤーの目的の変形状態
	m_vPosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ポリゴンの中央の位置
	m_vPosDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ポリゴンの中央の位置
	m_nHP = PLAYER_DEFAULT_HP;								// プレイヤーの体力
	m_fMP = PLAYER_DEFAULT_MP;								// プレイヤーの変形用のポイント
	m_bOperation = PLAYER_COMPUTER;							// プレイヤーの操作フラグ

	m_nAnimTime = 0;										// プレイヤー変形時のアニメーションの時間
}

//-----------------------------------------------------------------------------
// デストラクタ
//	引数　　無し
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
}

//-----------------------------------------------------------------------------
// クリエイト
//	引数　　デバイス、座標、幅、高さ、テクスチャの種類、プレイヤー操作（MANUAL or AUTO）
//	戻り値　作成したプレイヤーのポインタ
//-----------------------------------------------------------------------------
CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float fWidth, float fHeight, TEXTURE_TYPE texture, PLAYER_OPERATION operation)
{
	// プレイヤーポインタの作成
	CPlayer *temp = new CPlayer(pDevice);

	// 作成したプレイヤー情報の初期化
	temp->Init(pos, fWidth, fHeight, texture, operation);

	// 作成したプレイヤーのポインタを返す
	return temp;
}

//-----------------------------------------------------------------------------
// 初期化
//	引数　　座標、幅、高さ、テクスチャの種類
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::Init(D3DXVECTOR3 pos, float fWidth, float fHeight, TEXTURE_TYPE texture, PLAYER_OPERATION operation)
{
	CScene2D::Init(pos, fWidth, fHeight, texture);

	// 操作フラグを変更
	m_bOperation = operation;
}

//-----------------------------------------------------------------------------
// 終了
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::Uninit(void)
{
	CScene2D::Uninit();
}

//-----------------------------------------------------------------------------
// 更新
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::Update(void)
{
	CScene2D::Update();

	// 動いてるか判定するためのフラグをfalseに変更
	//m_Action = PLAYER_ACTION_NONE;

	// 移動量を０にする
	m_fMoveSpeedY = 0.0f;
	m_fMoveSpeedX = 0.0f;

	// １フレーム前の座標を専用の変数にセットする
	m_vPosOld = m_vPos;
	m_vPosDest = m_vPos;

	/*----------------------------------------------------------*/
	/*プレイヤーのアクションが変形中で無かった場合のみ他の行動を*/
	/*行うことができる											*/
	/*----------------------------------------------------------*/
	if (m_Action != PLAYER_ACTION_METAMORPHOSE)
	{

		/*----------------------------------------------------------*/
		/*暫定的にプレイヤーの操作方法はキーボードでの操作としておく*/
		/*移動量も暫定的な数値										*/
		/*----------------------------------------------------------*/

		/*----------------------------------------------------------*/
		/*プレイヤーの移動                                          */
		/*WSADキーでそれぞれ上下左右への移動						*/
		/*アクションの状態を全て歩行状態に変える					*/
		/*----------------------------------------------------------*/
		// Wで画面上方向への移動
		if (CInputKeyboard::GetKeyboardPress(DIK_W))
		{
			m_fMoveSpeedY = -5.0f;
			m_Action = PLAYER_ACTION_WALK;
		}
		// Sで画面下方向への移動
		else if (CInputKeyboard::GetKeyboardPress(DIK_S))
		{
			m_fMoveSpeedY = 5.0f;
			m_Action = PLAYER_ACTION_WALK;
		}
		// Aで画面左方向への移動
		if (CInputKeyboard::GetKeyboardPress(DIK_A))
		{
			m_fMoveSpeedX = -5.0f;
			m_Action = PLAYER_ACTION_WALK;
		}
		// Dで画面右方向への移動
		else if (CInputKeyboard::GetKeyboardPress(DIK_D))
		{
			m_fMoveSpeedX = 5.0f;
			m_Action = PLAYER_ACTION_WALK;
		}

		/*----------------------------------------------------------*/
		/*Kキーでプレイヤーの攻撃									*/
		/*----------------------------------------------------------*/
		if (CInputKeyboard::GetKeyboardTrigger(DIK_K))
		{
			// アクションの状態を攻撃に変える
			m_Action = PLAYER_ACTION_ATTACK;
		}

		/*----------------------------------------------------------*/
		/*Lキーで糸を出す											*/
		/*----------------------------------------------------------*/
		if (CInputKeyboard::GetKeyboardTrigger(DIK_L))
		{
			// アクションの状態を糸発射状態に変える
			m_Action = PLAYER_ACTION_THREAD;
		}

		/*----------------------------------------------------------*/
		/*Mキーでプレイヤー変形開始									*/
		/*----------------------------------------------------------*/
		if (CInputKeyboard::GetKeyboardTrigger(DIK_M))
		{
			// アクションの状態を変形に変える
			m_Action = PLAYER_ACTION_METAMORPHOSE;
		}

		/*----------------------------------------------------------*/
		/*ここまでのものを最終的にはコントローラーで操作			*/
		/*----------------------------------------------------------*/
	}

	/*----------------------------------------------------------*/
	/*プレイヤーのアクションの状態に対応した行動を取らせる		*/
	/*歩行、攻撃、糸発射、変形									*/
	/*----------------------------------------------------------*/
	switch (m_Action)
	{
		// プレイヤーのアクションが歩行だった際の動き
	case PLAYER_ACTION_WALK:
		Move();
		break;

		// プレイヤーのアクションが攻撃だった際の動き
	case PLAYER_ACTION_ATTACK:
		Attack();
		break;

		// プレイヤーのアクションが変形だった際の動き
	case PLAYER_ACTION_METAMORPHOSE:
		/*----------------------------------------------------------*/
		/*テスト用の状態変化										*/
		/*----------------------------------------------------------*/
		if (m_Mode == PLAYER_MODE_NONE)
		{
			Metamorphose(PLAYER_MODE_MAX);
		}
		else
		{
			Metamorphose(PLAYER_MODE_NONE);
		}
		break;

		// プレイヤーのアクションが糸発射だった際の動き
	case PLAYER_ACTION_THREAD:
		SpidersThread();
		break;

	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 描画
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::Draw(void)
{
	CScene2D::SetPos(m_vPos);
	CScene2D::SetRot(m_vRot);
	CScene2D::Draw();
}

//-----------------------------------------------------------------------------
// プレイヤーの移動
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::Move(void)
{
	float fDiffPosX;		// 現在の座標と目的の座標の差分(X軸)
	float fDiffPosY;		// 現在の座標と目的の座標の差分(Y軸)

	// 目的の座標を決める
	m_vPosDest.y += m_fMoveSpeedY;
	m_vPosDest.x += m_fMoveSpeedX;

	// 目的の座標と現在の座標を求める
	fDiffPosX = m_vPosDest.x - m_vPos.x;
	fDiffPosY = m_vPosDest.y - m_vPos.y;

	// 現在の座標を更新する
	m_vPos.x += fDiffPosX * 0.5f;
	m_vPos.y += fDiffPosY * 0.5f;

	// 向き設定
	m_vRotDest.z = atan2f(fDiffPosY, fDiffPosX) + ((D3DX_PI * 90.0f)/180.0f);

	// 角度の正規化
	NormalizeRotation(&m_vRotDest.z);

	m_vRot.z = m_vRotDest.z;
}

//-----------------------------------------------------------------------------
// 攻撃マネージャを呼び出す関数
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::Attack(void)
{
}

//-----------------------------------------------------------------------------
// 変形
//	引数　　変形する状態
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::Metamorphose(PLAYER_MODE mode)
{
	// 目的の状態を切り替える
	m_ModeDest = mode;

	MetamorphoseAnimation();
}

//-----------------------------------------------------------------------------
// 糸マネージャを呼び出す関数
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::SpidersThread(void)
{
}

//-----------------------------------------------------------------------------
// 変形アニメーション
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::MetamorphoseAnimation(void)
{
	m_Mode = m_ModeDest;

	// アニメーションの時間の増加
	m_nAnimTime++;

	if (m_nAnimTime > 60)
	{
		m_Action = PLAYER_ACTION_NONE;

		m_nAnimTime = 0;
	}
}

// EOF