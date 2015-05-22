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
// マクロ定義
//-----------------------------------------------------------------------------
#define PLAYER_DEFAULT_HP	500			// プレイヤーのデフォルトの体力
#define PLAYER_DEFAULT_MP	300.0f		// プレイヤーのデフォルトの変形用ポイント
#define PLAYER_OPERATION	BOOL		// プレイヤーの操作フラグ
#define PLAYER_MANUAL		TRUE		// プレイヤー操作マニュアル
#define PLAYER_COMPUTER		FALSE		// プレイヤー操作AUTO

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
	PLAYER_ACTION_THREAD,			// 糸を出す
	PLAYER_ACTION_KNOCK_BACK,		// やられ状態
	PLAYER_ACTION_DOWN,				// ダウン状態
	PLAYER_ACTION_MAX
}PLAYER_ACTION;

// プレイヤーの向いている方向
typedef enum
{
	PLAYER_DIRECTION_NONE = 0,
	PLAYER_DIRECTION_UP,			// 上
	PLAYER_DIRECTION_DOWN,			// 下
	PLAYER_DIRECTION_RIGHT,			// 右
	PLAYER_DIRECTION_LEFT,			// 左
	PLAYER_DIRECTION_UPPER_RIGHT,	// 右上
	PLAYER_DIRECTION_UPPER_LEFT,	// 左上
	PLAYER_DIRECTION_DOWNER_RIGHT,	// 右下
	PLAYER_DIRECTION_DOWNER_LEFT,	// 左下
	PLAYER_DIRECTION_MAX
}DIRECTION_PLAYER_FACING;

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CAttackManager;
class CThreadManager;

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
	void Init(D3DXVECTOR3 pos, float fWidth, float fHeight, TEXTURE_TYPE texture);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// クリエイト
	// 引数　デバイス、番号、座標、幅、高さ、テクスチャ,プレイヤー操作フラグ,攻撃マネージャー,プレイヤー番号
	static CPlayer* Create(LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos,
		float fWidth,
		float fHeight,
		TEXTURE_TYPE texture,
		BOOL playerOperation,
		CAttackManager *pAttackManager,
		CThreadManager *pThreadManager,
		short sPlayerNumber);

	// 現在の変形状態の取得
	// 戻り値　プレイヤーの現在の状態
	PLAYER_MODE GetPlayerMode(void);

	// やられ状態へ移行
	//  引数、戻り値　無し
	//  プレイヤーの行動状態をやられに変更するだけ
	void SetPlyerKnockBack(void){ if (m_Action != PLAYER_ACTION_DOWN && m_Action != PLAYER_ACTION_METAMORPHOSE){ m_Action = PLAYER_ACTION_KNOCK_BACK; } }

	// ダウン状態へ移行
	//  引数、戻り値　無し
	//  プレイヤーの行動状態をダウンに変更するだけ
	void SetPlayerDown(void){ if (m_Action != PLAYER_ACTION_KNOCK_BACK && m_Action != PLAYER_ACTION_METAMORPHOSE){ m_Action = PLAYER_ACTION_DOWN; } }

private:
	// 移動する
	void Move(void);

	// 攻撃
	void Attack(void);

	// 変形
	// 引数　変形するタイプ
	void Metamorphose(PLAYER_MODE mode);

	// 糸発射
	void SpidersThread(void);

	// 変形アニメーション
	void MetamorphoseAnimation(void);

	// やられ状態の処理
	void KnockBack(void);

	// ダウン状態の処理
	void PlayerDown(void);

	// プレイヤーのテクスチャのUV値の切り替え(Uの切り替え)
	void ChangeTextureFaceU(void);

	//---------------------------------
	// 変数
	//---------------------------------
	float					m_fMoveSpeedY;		// プレイヤーのX方向の移動量
	int						m_nHP;				// プレイヤーの体力
	float					m_fMoveSpeedX;		// プレイヤーのX方向の移動量
	float					m_fMP;				// プレイヤーの変形用のポイント
	BOOL					m_bOperation;		// プレイヤーの操作フラグ
	D3DXVECTOR3				m_vPosOld;			// １フレーム前の座標
	D3DXVECTOR3				m_vPosDest;			// １フレーム前の座標
	D3DXVECTOR3				m_vRotDest;			// 目的の角度
	PLAYER_MODE				m_Mode;				// 現在のプレイヤーの形態
	PLAYER_MODE				m_ModeDest;			// 目的のプレイヤーの形態
	PLAYER_ACTION			m_Action;			// プレイヤーが現在行っている行動
	short					m_sNumber;			// マネージャに割り振られたプレイヤー番号
	DIRECTION_PLAYER_FACING	m_PlayerFacing;		// プレイヤーの向いている方向
	DIRECTION_PLAYER_FACING	m_PlayerFacingOld;	// プレイヤーの向いている方向

	int						m_nAnimTime;		// 変形時のアニメーションの時間
	int						m_nKnockBackTime;	// ノックバック時の時間
	int						m_nDownTime;		// ダウン時の時間

	CAttackManager*			m_pAttackManager;	// 攻撃マネージャー
	CThreadManager*			m_pThreadManager;	// 糸マネージャー
};

#endif // __CPLAYER_H__

// EOF