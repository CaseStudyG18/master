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
class CAttackManager;
class CThreadManager;
class CJudgeManager;
class CFieldManager;
class CBackGroundManager;
class CCountDown;

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

	// �Q�[���I���Z�b�g
	void SetGameOver(){
		m_bGameOver = true;
	}

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
	// �U���}�l�[�W��
	CAttackManager * m_pAttackManager;
	// ���}�l�[�W��
	CThreadManager * m_pThreadManager;
	// �W���b�W�}�l�[�W���[
	CJudgeManager* m_pJudgeManager;
	// �t�B�[���h�}�l�[�W���[
	CFieldManager* m_pFieldManager;
	// �w�i�}�l�[�W���[
	CBackGroundManager *m_BackGroundManager;
	// �J�E���g�_�E��
	CCountDown* m_pCountDown;
	// �Q�[���J�n�t���O(�v���C���𑀍�ł���t���O)
	bool m_bPlayerControl;
	// �Q�[���N���A�t���O
	bool m_bGameOver;
};

#endif
//----EOF----