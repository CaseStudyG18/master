//=============================================================================
//
// CAttackBaseクラス [CAttackBase.h]
// Author : 塚本　俊彦
//
// 攻撃の基底クラス
// CSceneポインタで当たり判定を行うためCScene2Dを継承。
// 座標や半径などの大きさはCScene2Dが持っているけど、
// Baseの継承先からだとGetPosとしないと取得できないため、
// さらに自分で持つ
//
//=============================================================================
#ifndef _CATTACKBASE_H_
#define _CATTACKBASE_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../CSCENE/CScene2D.h"
#include "../PLAYER/CPlayerManager.h"
#include "CAttackManager.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CAttackBase : public CScene2D
{
	// 公開メンバ
public:

	CAttackBase(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_ATTACK, OBJTYPE objType = OBJTYPE_ATTACK);
	virtual ~CAttackBase(void);

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void){};
	short GetPlayerNumber(void){ return m_nPlayerNum; };
	// 継承メンバ
protected:
	// 寿命をカウントする
	short m_nCount;
	// 寿命
	short m_nEndTime;
	// 当たり判定をする時間
	short m_nHitStartTime;
	short m_nHitEndTime;
	// この攻撃は誰のかプレイヤー番号
	short m_nPlayerNum;
	// 当たり判定をするフラグ
	bool m_bHitFlg;
	// 当たったプレイヤー管理配列
	short m_nHitPlayer[MAXIMUM_NUMBER_OF_PLAYER];
	// 攻撃の種類
	AttackType m_AttackType;


	// ポジションアクセサ
	D3DXVECTOR3 GetPos(void){ return m_vPos; };
	void		SetPos(D3DXVECTOR3 pos){ m_vPos = pos; };
	void		SetPos(float x, float y, float z){ m_vPos.x = x; m_vPos.y = y; m_vPos.z = z; };

	// ロットアクセサ
	D3DXVECTOR3 GetRot(void){ return m_vRot; };
	void		SetRot(D3DXVECTOR3 rot){ m_vRot = rot; };
	void		SetRot(float x, float y, float z){ m_vRot.x = x; m_vRot.y = y; m_vRot.z = z; };

	// サイズアクセサ
	float GetWidth(void){ return m_fWidth; };
	void SetWidth(float width){ m_fWidth = width; };
	float GetHeight(void){ return m_fHeight; };
	void SetHeight(float height){ m_fHeight = height; };

	// 非公開メンバ
private:
	
};

#endif
//----EOF----