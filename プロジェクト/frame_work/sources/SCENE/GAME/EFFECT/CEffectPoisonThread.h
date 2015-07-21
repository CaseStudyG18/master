//=============================================================================
//
//	妨害形態の攻撃エフェクト
//
//	Author : 佐藤　諒一
//
//=============================================================================
#ifndef __CEFFECT_POISON_THREAD_H__
#define __CEFFECT_POISON_THREAD_H__

//=============================================================================
// インクルード
//=============================================================================
#include "../../../MAIN/main.h"
#include "../../GAME/EFFECT/CEffectBase.h"

//=============================================================================
// 定数
//=============================================================================

//=============================================================================
//	前方宣言
//=============================================================================
class CSceneAnime;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEffectPoisonThread : public CEffectBase
{
	// 公開メンバ
public:
	CEffectPoisonThread(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectPoisonThread(void);

	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 velocity);
	void Uninit(void);
	void Update(void);

	// クリエイト関数
	static CEffectPoisonThread *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 velocity);

	// 非公開メンバ
private:
	CSceneAnime *m_pAnim;
};

#endif