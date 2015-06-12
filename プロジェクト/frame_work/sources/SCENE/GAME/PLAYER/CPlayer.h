//=============================================================================
//
// CPlayer�N���X [CPlayer.h]
// Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CPLAYER_H__
#define __CPLAYER_H__

#include "../../GAME/ANIMATION/CAnimation.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
static const float PLAYER_DEFAULT_HP = 500.0f; 	// �v���C���[�̃f�t�H���g�̗̑�
static const float PLAYER_DEFAULT_MP = 300.0f;	// �v���C���[�̃f�t�H���g�̕ό`�p�|�C���g
static const bool PLAYER_MANUAL = TRUE;		  	// �v���C���[����}�j���A��
static const bool PLAYER_COMPUTER = FALSE;	  	// �v���C���[����AUTO

#define PLAYER_OPERATION	BOOL				// �v���C���[�̑���t���O

//-----------------------------------------------------------------------------
// �񋓑̒�`
//-----------------------------------------------------------------------------
// �v���C���[�̌`��
typedef enum
{
	PLAYER_MODE_NONE = 0,		// �ʏ���
	PLAYER_MODE_ATTACK,			// �U������
	PLAYER_MODE_SPEED,			// �f��������
	PLAYER_MODE_TRAP,			// �W�Q����
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
	PLAYER_ACTION_KNOCK_BACK,		// ������
	PLAYER_ACTION_DOWN,				// �_�E�����
	PLAYER_ACTION_MAX
}PLAYER_ACTION;

// �v���C���[�̌����Ă������
typedef enum
{
	PLAYER_DIRECTION_NONE = 0,
	PLAYER_DIRECTION_UP,			// ��
	PLAYER_DIRECTION_DOWN,			// ��
	PLAYER_DIRECTION_RIGHT,			// �E
	PLAYER_DIRECTION_LEFT,			// ��
	PLAYER_DIRECTION_UPPER_RIGHT,	// �E��
	PLAYER_DIRECTION_UPPER_LEFT,	// ����
	PLAYER_DIRECTION_DOWNER_RIGHT,	// �E��
	PLAYER_DIRECTION_DOWNER_LEFT,	// ����
	PLAYER_DIRECTION_MAX
}DIRECTION_PLAYER_FACING;

// �v���C���̌����Ă�����ɑ΂��Ẵx�N�g��
static const D3DXVECTOR3 PLAYER_DIRECTION_VECTOR[] = {
	D3DXVECTOR3(0, 0, 0),
	D3DXVECTOR3(0, -1, 0),
	D3DXVECTOR3(0, +1, 0),
	D3DXVECTOR3(+1, 0, 0),
	D3DXVECTOR3(-1, 0, 0),
	D3DXVECTOR3(+0.5f, -0.5f, 0),
	D3DXVECTOR3(-0.5f, -0.5f, 0),
	D3DXVECTOR3(+0.5f, +0.5f, 0),
	D3DXVECTOR3(-0.5f, +0.5f, 0)
};

//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
class CAttackManager;
class CThreadManager;
class CTreasure;
class CMp;

//-----------------------------------------------------------------------------
// �v���C���[�N���X��`
//-----------------------------------------------------------------------------
class CPlayer : public CAnimation
{
public:
	// �R���X�g���N�^
	// �����@�f�o�C�X�A�ԍ��A�I�u�W�F�^�C�v
	CPlayer(LPDIRECT3DDEVICE9 *pDevice , int nPriority = TYPE_PRIORITY_PLAYER , OBJTYPE objType = OBJTYPE_PLAYER);

	// �f�X�g���N�^
	~CPlayer();

	// ������
	// �����@���W�A���A�����A�e�N�X�`��
	void Init(D3DXVECTOR3 pos, float fWidth, float fHeight, TEXTURE_TYPE texture);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);

	// �N���G�C�g
	// �����@�f�o�C�X�A�ԍ��A���W�A���A�����A�e�N�X�`��,�v���C���[����t���O,�U���}�l�[�W���[,�v���C���[�ԍ�
	static CPlayer* Create(LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos,
		float fWidth,
		float fHeight,
		TEXTURE_TYPE texture,
		BOOL playerOperation,
		CAttackManager *pAttackManager,
		CThreadManager *pThreadManager,
		short sPlayerNumber);

	// ���݂̕ό`��Ԃ̎擾
	// �߂�l�@�v���C���[�̌��݂̏��
	PLAYER_MODE GetPlayerMode(void);

	// �����Ԃֈڍs
	//  �����A�߂�l�@����
	//  �v���C���[�̍s����Ԃ�����ɕύX���邾��
	void SetPlyerKnockBack(void);

	// �_�E����Ԃֈڍs
	//  �����A�߂�l�@����
	//  �v���C���[�̍s����Ԃ��_�E���ɕύX���邾��
	void SetPlayerDown(void);

	// �󕨂��E���Ƃ��̏��� �󕨂��A�C�R���ɕύX
	void SetTreasure(CTreasure *pTreasure);

	// �󕨂��̂Ă鎞�̏��� �A�C�R����󕨂ɕύX
	void FallTreasure();

	// �v���C���̌������ԂŃe�N�X�`�����X�V����
	// �X�V�ň��Ă�ł������� by �˖{
	void UpdatePlayerAnimation(void);

	// �̗͉񕜊֐�
	//	�����@�񕜂��镪�̗̑̓|�C���g
	void HPRepair(float fPoint){ m_fHP += fPoint; }

	// MP�����p�֐�
	void MPReduce(void);

	// �v���C���ԍ��Q�b�^�[
	short GetPlayerNum(){
		return m_sNumber;
	}

	// �v���C���ێ��󔠃C���X�^���X�Q�b�^�[
	CTreasure *GetTreasure(){
		return m_pTreasure;
	}

private:
	//---------------------------------
	// �֐�
	//---------------------------------
	// �ړ�����
	void Move(void);

	// �U��
	void Attack(void);

	// �ό`
	// �����@�ό`����^�C�v
	void Metamorphose(void);

	// ������
	void SpidersThread(void);

	// �ό`�A�j���[�V����
	void MetamorphoseAnimation(void);

	// �����Ԃ̏���
	void KnockBack(void);

	// �_�E����Ԃ̏���
	void PlayerDown(void);

	// ���G��Ԃł̏���
	void Matchless(void);

	// �v���C���[�̃e�N�X�`����UV�l�̐؂�ւ�(U�̐؂�ւ�)
	void ChangeTextureFaceU(void);

	// �v���C���[���ړ��`�Ԃ̍ۂ̍U���ł̈ړ�
	void Rush(void);

	//---------------------------------
	// �ϐ�
	//---------------------------------
	float					m_fMoveSpeedY;		// �v���C���[��X�����̈ړ���
	float					m_fHP;				// �v���C���[�̗̑�
	float					m_fMoveSpeedX;		// �v���C���[��X�����̈ړ���
	float					m_fMP;				// �v���C���[�̕ό`�p�̃|�C���g
	BOOL					m_bOperation;		// �v���C���[�̑���t���O
	D3DXVECTOR3				m_vPosOld;			// �P�t���[���O�̍��W
	D3DXVECTOR3				m_vPosDest;			// �P�t���[���O�̍��W
	D3DXVECTOR3				m_vRotDest;			// �ړI�̊p�x
	PLAYER_MODE				m_Mode;				// ���݂̃v���C���[�̌`��
	PLAYER_MODE				m_ModeDest;			// �ړI�̃v���C���[�̌`��
	PLAYER_ACTION			m_Action;			// �v���C���[�����ݍs���Ă���s��
	short					m_sNumber;			// �}�l�[�W���Ɋ���U��ꂽ�v���C���[�ԍ�
	DIRECTION_PLAYER_FACING	m_PlayerFacing;		// �v���C���[�̌����Ă������
	DIRECTION_PLAYER_FACING	m_PlayerFacingOld;	// �v���C���[�̌����Ă������

	short					m_sAnimTime;		// �ό`���̃A�j���[�V�����̎���
	short					m_sKnockBackTime;	// �m�b�N�o�b�N���̎���
	short					m_sDownTime;		// �_�E�����̎���
	short					m_sMatchlessTime;	// ���G��Ԃ̎���
	short					m_sKnockBackCount;	// �����ԂɂȂ�����
	short					m_sRushTime;		// �ړ��`�Ԃł̍U�����̃^�C�}�[
	bool					m_bMatchless;		// ���G��Ԃ��ǂ����̔���
	bool					m_bMetamorphose;	// �ό`���Ă����Ԃ��̃t���O
	bool					m_bSpeedAttack;		// �ړ��`�Ԃł̍U�������ǂ����̃t���O

	CAttackManager*			m_pAttackManager;	// �U���}�l�[�W���[
	CThreadManager*			m_pThreadManager;	// ���}�l�[�W���[
	CTreasure*				m_pTreasure;		// �󕨂��E�������̕󕨃|�C���^
	CMp*					m_pMp;				// MP�Q�[�W

};

#endif // __CPLAYER_H__

// EOF