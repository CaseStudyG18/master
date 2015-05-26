//=============================================================================
//
// CTreasure�N���X [CTreasure.h]
// Author : �˖{�@�r�F
//
// �D�������󕨁i���j
//
//=============================================================================
#ifndef _CTREASURE_H_
#define _CTREASURE_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../MAIN/main.h"
#include "../../CSCENE/CScene2D.h"

//=============================================================================
// �萔
//=============================================================================
enum TreasureState{
	TREASURE_STATE_OWNER_NONE,
	TREASURE_STATE_OWNED,
	TREASURE_STATE_TRANS_READY,
	TREASURE_STATE_TRANS_END,
	TREASURE_STATE_MAX,
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTreasure : public CScene2D
{
	// ���J�����o
public:

	CTreasure(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_4, OBJTYPE m_objType = OBJTYPE_TREASURE);
	~CTreasure(void);

	void Init(void);
	void Uninit(void);
	void Update(void);

	//=======================================================================
	// �N���G�C�g�֐�
	// ����:�f�o�C�X�A���W�A���A�����A�e�N�X�`���̎��
	// �߂�l:�쐬�����A�h���X
	//=======================================================================
	static CTreasure *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos);

	// �v���C�����󕨂𗎂Ƃ��Ƃ��̏���
	// �e�N�X�`���Ƒ傫�������Z�b�g����
	void Reset(D3DXVECTOR3 pos);

	// ����J�����o
private:
	// ����̏��
	TreasureState m_TreasureState;

};

#endif
//----EOF----