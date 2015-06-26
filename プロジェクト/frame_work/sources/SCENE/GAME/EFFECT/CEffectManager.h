//=============================================================================
//
// CEffectManagerクラス [CEffectManager.h]
// Author : 塚本　俊彦
//
// 宝物マネージャー　こいつを介して宝物を生成する
//
//=============================================================================
#ifndef _CEFFECTMANAGER_H_
#define _CEFFECTMANAGER_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../../RENDERER/CRenderer.h"

//=============================================================================
// 定数
//=============================================================================
enum EFFECT_TYPE{
	EFFECT_ATTACK_HIT = 0,
	EFFECT_EXPLOSION,
	EFFECT_SPECIAL_ATTACK_ATTACK,
	EFFECT_SPECIAL_ATTACK_SPEED,
	EFFECT_SPECIAL_THREAD_ATTACK,
	EFFECT_MAX,
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEffectManager
{
	// 公開メンバ
public:
	CEffectManager(LPDIRECT3DDEVICE9 *pDevice);
	~CEffectManager(void);

	void Init(void);
	void Uninit(void);
	void Update(void);

	// エフェクト生成関数
	// 座標とタイプを設定
	void CreateEffect(EFFECT_TYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 velocity);

	// 非公開メンバ
private:

	// デバイス
	LPDIRECT3DDEVICE9 *m_pDevice;
};

#endif
//----EOF----