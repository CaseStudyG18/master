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
//-----------------------------------------------------------------------------
CPlayer::CPlayer(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE objType) :CScene2D(pDevice, nPriority, objType)
{
	m_pD3DDevice = pDevice;                                 // デバイスオブジェクト(描画に必要)
	m_pD3DVtxBuff = NULL;                                   // 頂点座標情報を格納する場所のアドレスを確保する場所
	m_pD3DTexBuff = NULL;                                   // テクスチャ情報を格納する場所のアドレスを確保する場所
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);                 // ポリゴンの中央の位置
	m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);                 // ポリゴンの回転
	m_bMove = false;
	m_fMoveSpeedX = 0.0f;
	m_fMoveSpeedY = 0.0f;
	m_Action = PLAYER_ACTION_NONE;
	m_Mode = PLAYER_MODE_NONE;
	m_vPosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);              // ポリゴンの中央の位置
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
}

//-----------------------------------------------------------------------------
// クリエイト
//-----------------------------------------------------------------------------
CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float fWidth, float fHeight, TEXTURE_TYPE texture)
{
	CPlayer *temp = new CPlayer(pDevice);

	temp->Init(pos, fWidth, fHeight, texture);

	return temp;
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
void CPlayer::Init(D3DXVECTOR3 pos, float fWidth, float fHeight, TEXTURE_TYPE texture)
{
	CScene2D::Init(pos, fWidth, fHeight, texture);
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CPlayer::Uninit(void)
{
	CScene2D::Uninit();
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CPlayer::Update(void)
{
	CScene2D::Update();

	// 動いてるか判定するためのフラグをfalseに変更
	m_Action = PLAYER_ACTION_NONE;

	// 移動量を０にする
	m_fMoveSpeedY = 0.0f;
	m_fMoveSpeedX = 0.0f;

	// １フレーム前の座標を専用の変数にセットする
	m_vPosOld = m_vPos;
	m_vRotOld = m_vRot;

	/*----------------------------------------------------------*/
	/*暫定的にプレイヤーの移動方法はキーボードでの操作としておく*/
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
	/*暫定的にプレイヤーの移動方法はキーボードでの操作としておく*/
	/*----------------------------------------------------------*/

	// 移動時の処理
	if (m_Action == PLAYER_ACTION_WALK)
	{
		//移動
		m_vPos.y += m_fMoveSpeedY;
		m_vPos.x += m_fMoveSpeedX;

		// 向き設定
		m_vRot.z = ((180.0f * atan2f(m_vPos.y - m_vPosOld.y, m_vPos.x - m_vPosOld.x)) / D3DX_PI);

		// 角度の正規化
		NormalizeRotation(&m_vRot.z);
	}
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void CPlayer::Draw(void)
{
	CScene2D::SetPos(m_vPos);
	CScene2D::SetRot(m_vRot);
	CScene2D::Draw();
}

// EOF