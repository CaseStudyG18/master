//=============================================================================
//
// CPlayerクラス [CPlayer.h]
// Author : 佐藤　諒一
//
//=============================================================================
#ifndef __CPLAYER_H__
#define __CPLAYER_H__

#include "../../GAME/ANIMATION/CAnimation.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
static const float PLAYER_DEFAULT_HP = 500.0f; 	// プレイヤーのデフォルトの体力
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
	PLAYER_MODE_NONE = 0,		// 通常状態
	PLAYER_MODE_ATTACK,			// 攻撃特化
	PLAYER_MODE_SPEED,			// 素早さ特化
	PLAYER_MODE_TRAP,			// 妨害特化
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
	PLAYER_ACTION_STOP,				// 止まり状態
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

// プレイヤの向きに対してのテクスチャインデックスの最小と最大
static const short PLAYER_TEXTURE_INDEX_MIN[] = {
	0,	// なし
	10,	// 上
	6,	// 下
	1,	// 右
	1,	// 左
	10,	// 右上
	10,	// 左上
	6,	// 右下
	6	// 左下
};
static const short PLAYER_TEXTURE_INDEX_MAX[] = {
	0,	// なし
	13,	// 上
	9,	// 下
	4,	// 右
	4,	// 左
	13,	// 右上
	13,	// 左上
	9,	// 右下
	9,	// 左下
};

// プレイヤのHPの残り状態
enum PLAYER_HP_STATE{
	PLAYER_HP_STATE_NORMAL,
	PLAYER_HP_STATE_LOW,
	PLAYER_HP_STATE_VERY_LOW,
	PLAYER_HP_STATE_DIE,
	PLAYER_HP_STATE_MAX
};
// プレイヤのHPの残り状態を決めるHPの値
static const float PLAYER_HP_STATE_RATIO[] = {
	1.0f,	// PLAYER_NORMAL
	0.3f,	// PLAYER_LOW
	0.1f,	// PLAYER_VERY_LOW
	0.0f	// PLAYER_DIE
};
// プレイヤの残りHP状態ごとの赤く点滅するカウント数
static const short PLAYER_HP_STATE_FLASH_INTERVAL[] = {
	-1,	// PLAYER_NORMAL
	30,	// PLAYER_LOW
	12,	// PLAYER_VERY_LOW
	-1	// PLAYER_DIE
};

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CAttackManager;
class CThreadManager;
class CTreasure;
class CMp;

//-----------------------------------------------------------------------------
// プレイヤークラス定義
//-----------------------------------------------------------------------------
class CPlayer : public CAnimation
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
		short sPlayerNumber,
		bool *bPlayerControl);

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

	// 宝物を拾うときの処理 宝物をアイコンに変更
	void SetTreasure(CTreasure *pTreasure);

	// 宝物を捨てる時の処理 アイコンを宝物に変更
	void FallTreasure();

	// プレイヤの向きや状態でテクスチャを更新する
	void UpdatePlayerAnimation(void);

	// プレイヤの向きを変更する関数
	void SetFace(DIRECTION_PLAYER_FACING value);

	// 体力セッター
	void AddHp(float fPoint);

	// MP減少用関数
	void MPGainAndLoss(float changeValue);

	// HP増減関数
	void HPGainAndLoss(float changeValue);

	// 鈍足フラグ
	void SetDonashi(bool flg){
		m_bDonashi = flg;
	}

	// プレイヤーの行動状態を初期状態に変える
	void SetPlayerActionNone(void){ m_Action = PLAYER_ACTION_NONE; }

	// プレイヤーの行動状態を停止状態に変える
	void SetPlayerActionStop(void){ m_Action = PLAYER_ACTION_STOP; }

	// プレイヤ番号ゲッター
	short GetPlayerNum(){
		return m_sNumber;
	}

	// プレイヤ保持宝箱インスタンスゲッター
	CTreasure *GetTreasure(){
		return m_pTreasure;
	}

	// オールドポジションアクセサ
	D3DXVECTOR3 GetOldPos(void) { return m_vPosOld; }

private:
	//---------------------------------
	// 関数
	//---------------------------------
	// 移動する
	void Move(void);

	// 攻撃
	void Attack(void);

	// 変形
	// 引数　変形するタイプ
	void Metamorphose(void);

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

	// プレイヤーが移動形態の際の攻撃での移動
	void Rush(void);

	// プレイヤのHP残りステート管理
	void UpdatePlayerHpState(void);
	void UpdatePlayerRed(void);

	//---------------------------------
	// 変数
	//---------------------------------
	float					m_fMoveSpeedY;		// プレイヤーのX方向の移動量
	float					m_fHP;				// プレイヤーの体力
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
	short					m_nTextureIndex;	// プレイヤテクスチャの現在のインデックス
	short					m_nTextureCount;	// テクスチャアニメーションのカウント
	short					m_sAnimTime;		// 変形時のアニメーションの時間
	short					m_sKnockBackTime;	// ノックバック時の時間
	short					m_sDownTime;		// ダウン時の時間
	short					m_sMatchlessTime;	// 無敵状態の時間
	short					m_sKnockBackCount;	// やられ状態になった回数
	short					m_sRushTime;		// 移動形態での攻撃中のタイマー
	bool					m_bMatchless;		// 無敵状態かどうかの判定
	bool					m_bMetamorphose;	// 変形している状態かのフラグ
	bool					m_bSpeedAttack;		// 移動形態での攻撃中かどうかのフラグ
	bool*					m_bPlayerControl;	// プレイヤがコントロールできるかフラグ
	bool					m_bDonashi;			// 鈍足フラグ
	CAttackManager*			m_pAttackManager;	// 攻撃マネージャー
	CThreadManager*			m_pThreadManager;	// 糸マネージャー
	CTreasure*				m_pTreasure;		// 宝物を拾った時の宝物ポインタ
	CMp*					m_pMp;				// MPゲージ

	// 赤くする系
	short					m_nRedCount;		// 赤くするためにカウント
	bool					m_bRed;				// 赤い状態かフラグ
	PLAYER_HP_STATE			m_HpState;			// プレイヤの残りHP状態
	// シェーダー
	IDirect3DPixelShader9 *m_pPS;
	LPD3DXCONSTANTTABLE m_pPSC;

};

#endif // __CPLAYER_H__

// EOF