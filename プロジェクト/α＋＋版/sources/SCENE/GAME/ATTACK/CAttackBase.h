//=============================================================================
//
// CAttackBase�N���X [CAttackBase.h]
// Author : �˖{�@�r�F
//
// �U���̊��N���X
// CScene�|�C���^�œ����蔻����s������CScene2D���p���B
// ���W�┼�a�Ȃǂ̑傫����CScene2D�������Ă��邯�ǁA
// Base�̌p���悩�炾��GetPos�Ƃ��Ȃ��Ǝ擾�ł��Ȃ����߁A
// ����Ɏ����Ŏ���
//
//=============================================================================
#ifndef _CATTACKBASE_H_
#define _CATTACKBASE_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../CSCENE/CScene2D.h"
#include "../PLAYER/CPlayerManager.h"
#include "CAttackManager.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CAttackBase : public CScene2D
{
	// ���J�����o
public:

	CAttackBase(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_ATTACK, OBJTYPE objType = OBJTYPE_ATTACK);
	virtual ~CAttackBase(void);

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void){};
	short GetPlayerNumber(void){ return m_nPlayerNum; };
	// �p�������o
protected:
	// �������J�E���g����
	short m_nCount;
	// ����
	short m_nEndTime;
	// �����蔻������鎞��
	short m_nHitStartTime;
	short m_nHitEndTime;
	// ���̍U���͒N�̂��v���C���[�ԍ�
	short m_nPlayerNum;
	// �����蔻�������t���O
	bool m_bHitFlg;
	// ���������v���C���[�Ǘ��z��
	short m_nHitPlayer[MAXIMUM_NUMBER_OF_PLAYER];
	// �U���̎��
	AttackType m_AttackType;


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