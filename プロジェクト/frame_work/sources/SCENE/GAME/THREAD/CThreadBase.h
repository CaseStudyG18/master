//=============================================================================
//
// CThreadBaseクラス [CThreadBase.h]
// Author : 塚本　俊彦
//
// 糸の基底クラス
//
//=============================================================================
#ifndef _CTHREADBASE_H_
#define _CTHREADBASE_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../CSCENE/CScene2D.h"
#include "../PLAYER/CPlayerManager.h"
#include "CThreadManager.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CThreadBase : public CScene2D
{
	// 公開メンバ
public:

	CThreadBase(LPDIRECT3DDEVICE9 *pDevice, int priority, OBJTYPE type);
	virtual ~CThreadBase(void);

	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	short GetPlayerNum(void){ return m_nPlayerNum; };
	// 継承メンバ
protected:
	// 寿命をカウントする
	short m_nCount;
	// 寿命
	short m_nEndTime;
	// 当たり判定をする時間
	short m_nHitStartTime;
	short m_nHitEndTime;
	// この糸は誰のかプレイヤー番号
	short m_nPlayerNum;
	// 当たり判定をするフラグ
	bool m_bHitFlg;
	// 糸の種類
	ThreadType m_ThreadType;


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