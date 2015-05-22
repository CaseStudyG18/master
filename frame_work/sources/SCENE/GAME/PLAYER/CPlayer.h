//=============================================================================
//
// CPlayer�N���X [CPlayer.h]
// Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CPLAYER_H__
#define __CPLAYER_H__

#include "../../CSCENE/CScene2D.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define PLAYER_DEFAULT_HP	500			// �v���C���[�̃f�t�H���g�̗̑�
#define PLAYER_DEFAULT_MP	300.0f		// �v���C���[�̃f�t�H���g�̕ό`�p�|�C���g
#define PLAYER_OPERATION	BOOL		// �v���C���[�̑���t���O
#define PLAYER_MANUAL		TRUE		// �v���C���[����}�j���A��
#define PLAYER_COMPUTER		FALSE		// �v���C���[����AUTO

//-----------------------------------------------------------------------------
// �񋓑̒�`
//-----------------------------------------------------------------------------
// �v���C���[�̌`��
typedef enum
{
	PLAYER_MODE_NONE = 0,
	PLAYER_MODE_MAX
}PLAYER_MODE;

// �v���C���[�̍s��
typedef enum
{
	PLAYER_ACTION_NONE = 0,
	PLAYER_ACTION_WALK,				// ���s
	PLAYER_ACTION_ATTACK,			// �U��
	PLAYER_ACTION_METAMORPHOSE,		// �ό`
	PLAYER_ACTION_THREAD,			// �����o��
	PLAYER_ACTION_MAX
}PLAYER_ACTION;

//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
class CAttackManager;
class CThreadManager;

//-----------------------------------------------------------------------------
// �v���C���[�N���X��`
//-----------------------------------------------------------------------------
class CPlayer : public CScene2D
{
public:
	// �R���X�g���N�^
	// �����@�f�o�C�X�A�ԍ��A�I�u�W�F�^�C�v
	CPlayer(LPDIRECT3DDEVICE9 *pDevice , int nPriority = TYPE_PRIORITY_5 , OBJTYPE objType = OBJTYPE_PLAYER);

	// �f�X�g���N�^
	~CPlayer();

	// ������
	// �����@���W�A���A�����A�e�N�X�`��
	void Init(D3DXVECTOR3 pos, float fWidth, float fHeight, TEXTURE_TYPE texture, PLAYER_OPERATION operation);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);

	// �N���G�C�g
	// �����@�f�o�C�X�A�ԍ��A���W�A���A�����A�e�N�X�`��,�v���C���[����,�U���}�l�[�W���[
	static CPlayer* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float fWidth, float fHeight, TEXTURE_TYPE texture, PLAYER_OPERATION operation, CAttackManager *pAttackManager, CThreadManager *pThreadManager);

	// ���݂̕ό`��Ԃ̎擾
	// �߂�l�@�v���C���[�̌��݂̏��
	PLAYER_MODE GetPlayerMode(void);

private:
	// �ړ�����
	void Move(void);

	// �U��
	void Attack(void);

	// �ό`
	// �����@�ό`����^�C�v
	void Metamorphose(PLAYER_MODE mode);

	// ������
	void SpidersThread(void);

	// �ό`�A�j���[�V����
	void MetamorphoseAnimation(void);

	//---------------------------------
	// �ϐ�
	//---------------------------------
	float					m_fMoveSpeedY;		// �v���C���[��X�����̈ړ���
	int						m_nHP;				// �v���C���[�̗̑�
	float					m_fMoveSpeedX;		// �v���C���[��X�����̈ړ���
	float					m_fMP;				// �v���C���[�̕ό`�p�̃|�C���g
	PLAYER_OPERATION		m_bOperation;		// �v���C���[�̑���t���O
	D3DXVECTOR3				m_vPosOld;			// �P�t���[���O�̍��W
	D3DXVECTOR3				m_vPosDest;			// �P�t���[���O�̍��W
	D3DXVECTOR3				m_vRotDest;			// �ړI�̊p�x
	PLAYER_MODE				m_Mode;				// ���݂̃v���C���[�̌`��
	PLAYER_MODE				m_ModeDest;			// �ړI�̃v���C���[�̌`��
	PLAYER_ACTION			m_Action;			// �v���C���[�����ݍs���Ă���s��

	int						m_nAnimTime;		// �ό`���̃A�j���[�V�����̎���

	CAttackManager*			m_pAttackManager;	// �U���}�l�[�W���[
	CThreadManager*			m_pThreadManager;	// ���}�l�[�W���[
};

#endif // __CPLAYER_H__

// EOF