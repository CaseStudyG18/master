//=============================================================================
//
// CPlayerManager�N���X [CPlayerManager.h]
// Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CPLAYERMANAGER_H__
#define __CPLAYERMANAGER_H__

//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
class CPlayer;
class CAttackManager;
class CThreadManager;

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
// �v���C���[�̍ő吔
static const short MAXIMUM_NUMBER_OF_PLAYER = 5;

//-----------------------------------------------------------------------------
// �v���C���[�}�l�[�W���N���X��`
//-----------------------------------------------------------------------------
class CPlayerManager
{
public:
	// �R���X�g���N�^
	CPlayerManager(
		CAttackManager *pAttackManager, 
		CThreadManager *pThreadManager);

	// �f�X�g���N�^
	~CPlayerManager();

	// �N���G�C�g�֐�
	//	�����@�@���삷��v���C���[�̐��A�}�j���A�����삷��v���C���[�̐��ACPU�̐��A�U���}�l�[�W���[ ���}�l�[�W���[
	static CPlayerManager* Create(
		int nPlayerNum,
		int nManualPlayer,
		CAttackManager *pAttackPlayer,
		CThreadManager *pThreadPlayer);

	// ������
	// CPU�l���A�v���C���[�l��
	void Init(int nNumPlayer, int nManualPlayer);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �v���C���[�|�C���^�̃Q�b�^�[
	//	�����@�@�Ăяo���v���C���[���Z�b�g���Ă���z��ԍ�
	//	�߂�l�@�v���C���[�̃|�C���^
	CPlayer* GetPlayer(int nNumber){ return m_apPlayer[nNumber]; }

	// �v���C���[�|�C���^�̃Z�b�^�[
	//	�����@�@�v���C���[�|�C���^�A�Z�b�g�����̔z��ԍ�
	void SetPlayer(CPlayer* pPlayer, int nNumber){ m_apPlayer[nNumber] = pPlayer; }

private:
	// �v���C���[�̃|�C���^�i�[�p�z��
	CPlayer* m_apPlayer[MAXIMUM_NUMBER_OF_PLAYER];
	// �e�X�g�p�̃^�C�}�[
	int	m_nTimer;

	// �U�����ɕK�v�ȃ}�l�[�W���[
	CAttackManager *m_pAttackManager;
	// ���o�����ɕK�v�ȃ}�l�[�W���[
	CThreadManager *m_pThreadManager;
};

#endif //__CPLAYERMANAGER_H__

// EOF