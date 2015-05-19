//=============================================================================
//
// CGame�N���X [CGame.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CGAME_H_
#define _CGAME_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../MAIN/main.h"
#include "../../PHASE/CPhase.h"
#include "../../TEXTURE/CTexture.h"
#include "../PAUSE/CPause.h"
#include "CTimeManager.h"

//=============================================================================
// �O���錾
//=============================================================================
class CTimeManager;
class CTreasureManager;
class CGoalManager;
class CPlayerManager;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGame : public CPhase
{
	// ���J�����o
public:
	// �R���X�g���N�^
	CGame(void);

	// �f�X�g���N�^
	~CGame(void);

	//======================================================
	// ������
	// ����: �t�F�[�Y���[�h�A�f�o�C�X
	//======================================================
	void Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	//======================================================
	// �N���G�C�g
	// ����: �t�F�[�Y���[�h�A�f�o�C�X
	// �߂�l: �������̃|�C���^
	//======================================================
	static CGame* Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

	// ����J�����o
private:
	// �|�[�Y
	CPause *m_pPause;
	// �Q�[���̃^�C���Ǘ���
	CTimeManager *m_pTimeManager;
	// �󕨊Ǘ���
	CTreasureManager *m_pTreasureManager;
	// �S�[���Ǘ���
	CGoalManager *m_pGoalManager;
	// �v���C���}�l�[�W��
	CPlayerManager *m_pPlayerManager;
};

#endif
//----EOF----