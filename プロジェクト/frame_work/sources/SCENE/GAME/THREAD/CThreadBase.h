//=============================================================================
//
// CThreadBase�N���X [CThreadBase.h]
// Author : �˖{�@�r�F
//
// ���̊��N���X
//
//=============================================================================
#ifndef _CTHREADBASE_H_
#define _CTHREADBASE_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../CSCENE/CScene2D.h"
#include "../PLAYER/CPlayerManager.h"
#include "CThreadManager.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CThreadBase : public CScene2D
{
	// ���J�����o
public:

	CThreadBase(LPDIRECT3DDEVICE9 *pDevice, int priority, OBJTYPE type);
	virtual ~CThreadBase(void);

	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	short GetPlayerNum(void){ return m_nPlayerNum; };
	// �p�������o
protected:
	// �������J�E���g����
	short m_nCount;
	// ����
	short m_nEndTime;
	// �����蔻������鎞��
	short m_nHitStartTime;
	short m_nHitEndTime;
	// ���̎��͒N�̂��v���C���[�ԍ�
	short m_nPlayerNum;
	// �����蔻�������t���O
	bool m_bHitFlg;
	// ���̎��
	ThreadType m_ThreadType;


	// �|�W�V�����A�N�Z�T
	D3DXVECTOR3 GetPos(void){ return m_vPos; };
	void		SetPos(D3DXVECTOR3 pos){ m_vPos = pos; };
	void		SetPos(float x, float y, float z){ m_vPos.x = x; m_vPos.y = y; m_vPos.z = z; };

	// ���b�g�A�N�Z�T
	D3DXVECTOR3 GetRot(void){ return m_vRot; };
	void		SetRot(D3DXVECTOR3 rot){ m_vRot = rot; };
	void		SetRot(float x, float y, float z){ m_vRot.x = x; m_vRot.y = y; m_vRot.z = z; };

	// �T�C�Y�A�N�Z�T
	float GetWidth(void){ return m_fWidth; };
	void SetWidth(float width){ m_fWidth = width; };
	float GetHeight(void){ return m_fHeight; };
	void SetHeight(float height){ m_fHeight = height; };

	// ����J�����o
private:
	
};

#endif
//----EOF----