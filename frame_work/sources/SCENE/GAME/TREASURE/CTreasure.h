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
#include "../../CSCENE/CScene2D.h"

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
class CTreasure : public CScene2D
{
	// 公開メンバ
public:

	CTreasure(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_4, OBJTYPE m_objType = OBJTYPE_TREASURE);
	~CTreasure(void);

	void Init(void);
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

	// 非公開メンバ
private:
	// お宝の状態
	TreasureState m_TreasureState;

};

#endif
//----EOF----