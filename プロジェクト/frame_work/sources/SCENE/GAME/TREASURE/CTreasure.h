//=============================================================================
//
// CTreasureクラス [CTreasure.h]
// Author : 塚本　俊彦
//
// 奪い合う宝物（仮）
//
//=============================================================================
#ifndef _CTREASURE_H_
#define _CTREASURE_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../../MAIN/main.h"
#include "../../EFFECT/CEffect.h"

//=============================================================================
// 定数
//=============================================================================
enum TreasureState{
	TREASURE_STATE_OWNER_NONE,
	TREASURE_STATE_OWNED,
	TREASURE_STATE_TRANS_READY,
	TREASURE_STATE_TRANS_END,
	TREASURE_STATE_MAX,
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTreasure : public CEffect
{
	// 公開メンバ
public:

	CTreasure(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_TRESURE, OBJTYPE m_objType = OBJTYPE_TREASURE);
	~CTreasure(void);

	void Uninit(void);
	void Update(void);

	//=======================================================================
	// クリエイト関数
	// 引数:デバイス、座標、幅、高さ、テクスチャの種類
	// 戻り値:作成したアドレス
	//=======================================================================
	static CTreasure *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos);

	// プレイヤが宝物を落とすときの処理
	// テクスチャと大きさをリセットする
	void Reset(D3DXVECTOR3 pos);

	// 宝箱を持っているプレイヤーのIDをセット
	void SetOwnerPlayerNumber(short sPlayerNumber){ m_sOwnerPlayer = sPlayerNumber; m_TreasureState = TREASURE_STATE_OWNED; }

	// 宝箱を持っているプレイヤーのIDをゲット
	short GetOwnerPlayerNumber(void){ return m_sOwnerPlayer; }

	// 宝箱の状態を取得
	TreasureState GetTreasureState(void){ return m_TreasureState; }

	// 宝箱の状態をセット
	void SetTreasureState(TreasureState State){ m_TreasureState = State; }

	// 非公開メンバ
private:

	// 初期化
	void Init(D3DXVECTOR3 pos);

	// お宝の状態
	TreasureState m_TreasureState;
	short m_sOwnerPlayer;
};

#endif
//----EOF----