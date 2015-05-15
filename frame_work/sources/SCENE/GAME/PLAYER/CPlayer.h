//=============================================================================
//
// CPlayerクラス [CPlayer.h]
// Author : 佐藤　諒一
//
//=============================================================================
#ifndef __CPLAYER_H__
#define __CPLAYER_H__

#include "../../CSCENE/CScene2D.h"

//-----------------------------------------------------------------------------
// 列挙体定義
//-----------------------------------------------------------------------------
// プレイヤーの形態
typedef enum
{
	PLAYER_MODE_NONE = 0,
	PLAYER_MODE_MAX
}PLAYER_MODE;

// プレイヤーの行動
typedef enum
{
	PLAYER_ACTION_NONE = 0,
	PLAYER_ACTION_WALK,				// 歩行
	PLAYER_ACTION_ATTACK,			// 攻撃
	PLAYER_ACTION_METAMORPHOSE,		// 変形
	PLAYER_ACTION_MAX
}PLAYER_ACTION;

//-----------------------------------------------------------------------------
// プレイヤークラス定義
//-----------------------------------------------------------------------------
class CPlayer : public CScene2D
{
public:
	// コンストラクタ
	// 引数　デバイス、番号、オブジェタイプ
	CPlayer(LPDIRECT3DDEVICE9 *pDevice , int nPriority = TYPE_PRIORITY_5 , OBJTYPE objType = OBJTYPE_PLAYER);

	// デストラクタ
	~CPlayer();

	// 初期化
	// 引数　座標、幅、高さ、テクスチャ
	void Init(D3DXVECTOR3 pos , float fWidth , float fHeight , TEXTURE_TYPE texture);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// クリエイト
	// 引数　デバイス、番号、座標、幅、高さ、テクスチャ
	static CPlayer* Create(LPDIRECT3DDEVICE9 *pDevice , D3DXVECTOR3 pos , float fWidth , float fHeight , TEXTURE_TYPE texture);

private:
	float			m_fMoveSpeedX;		// プレイヤーのX方向の移動量
	float			m_fMoveSpeedY;		// プレイヤーのX方向の移動量
	bool			m_bMove;			// 動かしているのか
	D3DXVECTOR3		m_vPos;				// 座標
	D3DXVECTOR3		m_vPosOld;			// １フレーム前の座標
	D3DXVECTOR3		m_vRot;				// 角度
	D3DXVECTOR3		m_vRotOld;			// １フレーム前の角度
	PLAYER_MODE		m_Mode;				// 現在のプレイヤーの形態
	PLAYER_ACTION	m_Action;			// プレイヤーが現在行っている行動
};

#endif // __CPLAYER_H__

// EOF