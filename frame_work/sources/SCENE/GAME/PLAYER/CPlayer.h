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
static const int PLAYER_DEFAULT_HP = 500;	  	// プレイヤーのデフォルトの体力
static const float PLAYER_DEFAULT_MP = 300.0f;	// プレイヤーのデフォルトの変形用ポイント
static const bool PLAYER_MANUAL = TRUE;		  	// プレイヤー操作マニュアル
static const bool PLAYER_COMPUTER = FALSE;	  	// プレイヤー操作AUTO

#define PLAYER_OPERATION	BOOL				// プレイヤーの操作フラグ

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

// プレイヤの向いてる方向に対してのベクトル
static const D3DXVECTOR3 PLAYER_DIRECTION_VECTOR[] = {
	D3DXVECTOR3(0, 0, 0),
	D3DXVECTOR3(0, -1, 0),
	D3DXVECTOR3(0, +1, 0),
	D3DXVECTOR3(+1, 0, 0),
	D3DXVECTOR3(-1, 0, 0),
	D3DXVECTOR3(+0.5f, -0.5f, 0),
	D3DXVECTOR3(-0.5f, -0.5f, 0),
	D3DXVECTOR3(+0.5f, +0.5f, 0),
	D3DXVECTOR3(-0.5f, +0.5f, 0)
};

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CAttackManager;
class CThreadManager;
class CTreasure;

//-----------------------------------------------------------------------------
// プレイヤークラス定義
//-----------------------------------------------------------------------------
class CPlayer : public CScene2D
{
public:
	// コンストラクタ
	// 引数　デバイス、番号、オブジェタイプ
	CPlayer(LPDIRECT3DDEVICE9 *pDevice , int nPriority = TYPE_PRIORITY_PLAYER , OBJTYPE objType = OBJTYPE_PLAYER);

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
	void SetPlyerKnockBack(void);

	// ダウン状態へ移行
	//  引数、戻り値　無し
	//  プレイヤーの行動状態をダウンに変更するだけ
	void SetPlayerDown(void);

	// プレイヤーのIDの取得
	short GetPlayerNumber(void){ return m_sNumber; }

	// 宝物を拾うときの処理 宝物をアイコンに変更
	void SetTreasure(CTreasure *pTreasure);

	// 宝物を捨てる時の処理 アイコンを宝物に変更
	void FallTreasure();

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

	// 無敵状態での処理
	void Matchless(void);

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
	int						m_nMatchlessTime;	// 無敵状態の時間
	int						m_nKnockBackCount;	// やられ状態になった回数
	bool					m_bMatchless;		// 無敵状態かどうかの判定

	CAttackManager*			m_pAttackManager;	// 攻撃マネージャー
	CThreadManager*			m_pThreadManager;	// 糸マネージャー

	CTreasure*				m_pTreasure;		// 宝物を拾った時の宝物ポインタ
};

#endif // __CPLAYER_H__

// EOF