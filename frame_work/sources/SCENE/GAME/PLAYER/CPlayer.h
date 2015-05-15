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
	PLAYER_ACTION_MAX
}PLAYER_ACTION;

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
	void Init(D3DXVECTOR3 pos , float fWidth , float fHeight , TEXTURE_TYPE texture);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);

	// �N���G�C�g
	// �����@�f�o�C�X�A�ԍ��A���W�A���A�����A�e�N�X�`��
	static CPlayer* Create(LPDIRECT3DDEVICE9 *pDevice , D3DXVECTOR3 pos , float fWidth , float fHeight , TEXTURE_TYPE texture);

private:
	float			m_fMoveSpeedX;		// �v���C���[��X�����̈ړ���
	float			m_fMoveSpeedY;		// �v���C���[��X�����̈ړ���
	bool			m_bMove;			// �������Ă���̂�
	D3DXVECTOR3		m_vPos;				// ���W
	D3DXVECTOR3		m_vPosOld;			// �P�t���[���O�̍��W
	D3DXVECTOR3		m_vRot;				// �p�x
	D3DXVECTOR3		m_vRotOld;			// �P�t���[���O�̊p�x
	PLAYER_MODE		m_Mode;				// ���݂̃v���C���[�̌`��
	PLAYER_ACTION	m_Action;			// �v���C���[�����ݍs���Ă���s��
};

#endif // __CPLAYER_H__

// EOF