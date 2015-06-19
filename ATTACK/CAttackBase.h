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

//=============================================================================
//	�O���錾
//=============================================================================
class CPlayer;

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

	// �U�����q�b�g�������ɌĂяo���֐�
	// �����@���̍U���ɓ��������v���C���[�̃v���C���[�ԍ�
	virtual void HitPlayer(CPlayer* pPlayer);

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

	// ����J�����o
private:
	
};

#endif
//----EOF----