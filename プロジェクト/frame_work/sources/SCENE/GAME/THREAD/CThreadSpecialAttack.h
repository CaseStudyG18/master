//=============================================================================
//
// CThreadSpecialAttack�N���X [CThreadSpecialAttack.h]
// Author : �����@�Ȉ�
//
// �U�������`�Ԃ̎�
//
//=============================================================================
#ifndef _CTHREAD_SPECIAL_ATTACK_H_
#define _CTHREAD_SPECIAL_ATTACK_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "CThreadBase.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CThreadSpecialAttack : public CThreadBase
{
	// ���J�����o
public:
	// �v���C�I���e�B�ƃI�u�W�F�^�C�v�̓e�X�g
	CThreadSpecialAttack(LPDIRECT3DDEVICE9 *pDevice, int priority = TYPE_PRIORITY_THREAD_OF_FOOTHOLD, OBJTYPE type = OBJTYPE_FIELD);
	~CThreadSpecialAttack(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//=======================================================================
	// �N���G�C�g�֐�
	// ����:�f�o�C�X�A�v���C���ԍ��A���W
	// �߂�l:�쐬�����A�h���X
	//=======================================================================
	static CThreadSpecialAttack *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		short nPlayerNum,
		D3DXVECTOR3 pos);

	// ����J�����o
private:

};

#endif
//----EOF----