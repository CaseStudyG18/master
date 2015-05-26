//=============================================================================
//
// CPlayerクラス [CPlayer.cpp]
// Author : 佐藤　諒一
//
//=============================================================================
#include "CPlayer.h"

#include "../../../INPUT/CInputKeyboard.h"
#include "../../../TEXTURE/CTexture.h"
#include "../ATTACK/CAttackManager.h"
#include "../THREAD/CThreadManager.h"
#include "../TREASURE/CTreasure.h"

//-----------------------------------------------------------------------------
// 定数定義
//-----------------------------------------------------------------------------
// 宝物アイコンの大きさ
static const float TREASURE_ICON_WIDTH = 40;
static const float TREASURE_ICON_HEIGHT = 30;

// 宝物アイコンの表示位置
static const D3DXVECTOR3 TREASURE_ICON_POS_BUFF = D3DXVECTOR3(0, -50, 0);


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
	m_sNumber = 0;											// マネージャーに割り振られるプレイヤー番号
	m_PlayerFacing = PLAYER_DIRECTION_UP;					// プレイヤーの初期向き
	m_PlayerFacingOld = PLAYER_DIRECTION_UP;				// プレイヤーの過去の向き

	m_nAnimTime = 0;										// プレイヤー変形時のアニメーションの時間
	m_nKnockBackTime = 0;									// ノックバック時間
	m_nDownTime = 0;										// ダウン時間
	m_nMatchlessTime = 0;									// 無敵状態の時間
	m_nKnockBackCount = 0;									// やられ状態になった回数

	m_bMatchless = false;									// 無敵状態かどうか判定

	m_pTreasure = NULL;										// 宝物ポインタ
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
//	引数　　デバイス、座標、幅、高さ、テクスチャの種類、プレイヤー操作（MANUAL or AUTO）,攻撃マネージャー , プレイヤー番号
//	戻り値　作成したプレイヤーのポインタ
//-----------------------------------------------------------------------------
CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 *pDevice,
	D3DXVECTOR3 pos,
	float fWidth,
	float fHeight,
	TEXTURE_TYPE texture,
	BOOL playerOperation,
	CAttackManager *pAttackManager,
	CThreadManager *pThreadManager,
	short sPlayerNumber)
{
	// プレイヤーポインタの作成
	CPlayer *temp = new CPlayer(pDevice);

	// 作成したプレイヤー情報の初期化
	temp->Init(pos, fWidth, fHeight, texture);

	// 操作フラグを変更
	temp->m_bOperation = playerOperation;

	// プレイヤー番号のセット
	temp->m_sNumber = sPlayerNumber;

	// 攻撃マネージャの保持
	temp->m_pAttackManager = pAttackManager;

	// 糸マネージャの保持
	temp->m_pThreadManager = pThreadManager;

	// 作成したプレイヤーのポインタを返す
	return temp;
}

//-----------------------------------------------------------------------------
// 初期化
//	引数　　座標、幅、高さ、テクスチャの種類
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::Init(D3DXVECTOR3 pos, float fWidth, float fHeight, TEXTURE_TYPE texture)
{
	CScene2D::Init(pos, fWidth, fHeight, texture);
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
	// 宝物を持っていたらアイコンの場所更新
	if (m_pTreasure){
		m_pTreasure->SetPos(m_vPos + TREASURE_ICON_POS_BUFF);
	}

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
	if (m_Action != PLAYER_ACTION_METAMORPHOSE && m_Action != PLAYER_ACTION_KNOCK_BACK && m_Action != PLAYER_ACTION_DOWN)
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
			m_PlayerFacing = PLAYER_DIRECTION_UP;
		}
		// Sで画面下方向への移動
		else if (CInputKeyboard::GetKeyboardPress(DIK_S))
		{
			m_fMoveSpeedY = 5.0f;
			m_Action = PLAYER_ACTION_WALK;
			m_PlayerFacing = PLAYER_DIRECTION_DOWN;
		}
		// Aで画面左方向への移動
		if (CInputKeyboard::GetKeyboardPress(DIK_A))
		{
			m_fMoveSpeedX = -5.0f;
			m_Action = PLAYER_ACTION_WALK;
			m_PlayerFacing = PLAYER_DIRECTION_LEFT;
		}
		// Dで画面右方向への移動
		else if (CInputKeyboard::GetKeyboardPress(DIK_D))
		{
			m_fMoveSpeedX = 5.0f;
			m_Action = PLAYER_ACTION_WALK;
			m_PlayerFacing = PLAYER_DIRECTION_RIGHT;
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

		// debug
		if (CInputKeyboard::GetKeyboardTrigger(DIK_SPACE)){
			// 宝物を落とす
			FallTreasure();
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

		// プレイヤーのアクションがやられだった際
	case PLAYER_ACTION_KNOCK_BACK:
		KnockBack();
		break;

		// プレイヤーの状態がダウンだった際
	case PLAYER_ACTION_DOWN:
		PlayerDown();
		break;

	default:
		break;
	}

	// 無敵状態だった場合は無敵状態時の処理を行う
	if (m_bMatchless == true)
	{
		Matchless();
	}
}

//-----------------------------------------------------------------------------
// 描画
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::Draw(void)
{
//	CScene2D::SetPos(m_vPos);
//	CScene2D::SetRot(m_vRot);
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

	/*
	// 上と下の移動以外の移動では向きを変える
	if (m_PlayerFacing == PLAYER_DIRECTION_UP || m_PlayerFacing == PLAYER_DIRECTION_DOWN)
	{
		// 向き設定
		m_vRot.z = DEGREE_TO_RADIAN(0.0f);

		// 角度の正規化
		NormalizeRotation(&m_vRot.z);
	}
	else
	{
		// 向き設定
		m_vRot.z = atan2f(fDiffPosY, fDiffPosX) + DEGREE_TO_RADIAN(90.0f);

		// 角度の正規化
		NormalizeRotation(&m_vRot.z);
	}
	*/

	// プレイヤーの移動方向が変わったらテクスチャのU値を変える
	if ((m_PlayerFacing == PLAYER_DIRECTION_LEFT || m_PlayerFacing == PLAYER_DIRECTION_RIGHT) &&
		m_PlayerFacing != m_PlayerFacingOld)
	{
		ChangeTextureFaceU();

		m_PlayerFacingOld = m_PlayerFacing;
	}
	//m_PlayerFacing = PLAYER_DIRECTION_UP;
}

//-----------------------------------------------------------------------------
// 攻撃マネージャを呼び出す関数
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::Attack(void)
{
	// 普通攻撃
	m_pAttackManager->CreateAttack(
		ATTACK_TYPE_NORMAL,
		m_sNumber,
		m_vPos);
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
	// 普通攻撃
	m_pThreadManager->CreateThread(
		THREAD_TYPE_NORMAL,
		m_sNumber,
		m_vPos);
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

//-----------------------------------------------------------------------------
// やられ状態での処理
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::KnockBack(void)
{
	m_nKnockBackTime++;

	// 一定回数やられ状態になったらダウン状態に移行
	if (m_nKnockBackCount > 3)
	{
		SetPlayerDown();
		m_nKnockBackCount = 0;
	}

	// 一定時間経過したら初期状態にもどす
	else if (m_nKnockBackTime > 100)
	{
		// アクションの状態を初期状態に戻す
		m_Action = PLAYER_ACTION_NONE;

		// やられになった回数を増やす
		m_nKnockBackCount++;

		// やられ時間の初期化
		m_nKnockBackTime = 0;
	}
}

//-----------------------------------------------------------------------------
// ダウン状態での処理
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::PlayerDown(void)
{
	m_nDownTime++;

	// 一定時間経過したら無敵状態へ移行
	if (m_nDownTime > 500)
	{
		m_Action = PLAYER_ACTION_NONE;

		m_bMatchless = true;

		m_nDownTime = 0;
	}
}

//-----------------------------------------------------------------------------
// 無敵状態の際の動作
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::Matchless(void)
{
	m_nMatchlessTime++;

	// 色変更
	CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(m_nMatchlessTime%10)/10.0f));

	// 一定時間経過したら初期状態へ移行
	if (m_nMatchlessTime > 500)
	{
		// 無敵状態フラグを消す
		m_bMatchless = false;

		// 色変更
		CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// 無敵時間の初期化
		m_nMatchlessTime = 0;
	}
}

//-----------------------------------------------------------------------------
// プレイヤーの状態をやられ状態に移行既にやられ状態だった場合はダウン状態へ
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::SetPlyerKnockBack(void)
{
	// 無敵状態でなければこの処理を行う
	if (m_bMatchless == false)
	{
		// 既にやられ状態だった場合にはダウン状態へ
		if (m_Action == PLAYER_ACTION_KNOCK_BACK)
		{
			m_Action = PLAYER_ACTION_DOWN;
		}
		// ダウン状態または変形状態では無かった場合はやられ状態へ
		else if (m_Action != PLAYER_ACTION_DOWN && m_Action != PLAYER_ACTION_METAMORPHOSE)
		{
			m_Action = PLAYER_ACTION_KNOCK_BACK;
		}
	}
}

//-----------------------------------------------------------------------------
// プレイヤーの状態をダウン状態へ移行
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::SetPlayerDown(void)
{
	// 無敵状態でなければこの処理を行う
	if (m_bMatchless == false)
	{
		if (m_Action != PLAYER_ACTION_METAMORPHOSE)
		{
			m_Action = PLAYER_ACTION_DOWN;
		}
	}
}

//-----------------------------------------------------------------------------
// テクスチャのUV切り替えの処理(U値の切り替え)
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::ChangeTextureFaceU(void)
{
	VERTEX_2D *pVtx;
	float fTemp;			// 仮変数

	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャの上部分のU値の入れ替え
	fTemp = pVtx[0].tex.x;
	pVtx[0].tex.x = pVtx[1].tex.x;
	pVtx[1].tex.x = fTemp;

	// テクスチャの下部分のU値の入れ替え
	fTemp = pVtx[2].tex.x;
	pVtx[2].tex.x = pVtx[3].tex.x;
	pVtx[3].tex.x = fTemp;

	m_pD3DVtxBuff->Unlock();

}

//-----------------------------------------------------------------------------
// プレイヤに宝物を入れて、アイコンにする
// プレイヤが宝物を取った時によばれる
//-----------------------------------------------------------------------------
void CPlayer::SetTreasure(CTreasure *pTreasure){
	// ポインタ保存
	m_pTreasure = pTreasure;

	// テクスチャと大きさを変更
	m_pTreasure->SetWidth(TREASURE_ICON_WIDTH);
	m_pTreasure->SetHeight(TREASURE_ICON_HEIGHT);
	m_pTreasure->ChangeTexture(TEXTURE_TREASURE_ICON);
}

//-----------------------------------------------------------------------------
// 宝物の表示を戻して、宝物の保持ポインタをNULL
// プレイヤが宝物を落としたときに呼ばれる
//-----------------------------------------------------------------------------
void CPlayer::FallTreasure(){
	
	if (m_pTreasure){
		// 宝物のテクスチャや大きさをセット
		m_pTreasure->Reset(m_vPos);

		// ポインタ削除
		m_pTreasure = NULL;
	}

}
// EOF