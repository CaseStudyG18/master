//=============================================================================
//
// CPlayer�N���X [CPlayer.cpp]
// Author : �����@�Ȉ�
//
//=============================================================================
#include "CPlayer.h"

#include "../INPUT/CInputKeyboard.h"
#include "../TEXTURE/CTexture.h"

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//	�����@�@�f�o�C�X�A�v���C�I���e�B�A�I�u�W�F�N�g�^�C�v
//-----------------------------------------------------------------------------
CPlayer::CPlayer(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE objType) :CScene2D(pDevice, nPriority, objType)
{
	m_pD3DDevice = pDevice;									// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	m_pD3DVtxBuff = NULL;									// ���_���W�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	m_pD3DTexBuff = NULL;									// �e�N�X�`�������i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �|���S���̒����̈ʒu
	m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �|���S���̊p�x
	m_vRotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �|���S���̖ړI�̊p�x
	m_fMoveSpeedX = 0.0f;									// �v���C���[�̉����̈ړ����x
	m_fMoveSpeedY = 0.0f;									// �v���C���[�̏c���̈ړ����x
	m_Action = PLAYER_ACTION_NONE;							// �v���C���[�̍s��
	m_Mode = PLAYER_MODE_NONE;								// �v���C���[�̌��݂̕ό`���
	m_ModeDest = PLAYER_MODE_NONE;							// �v���C���[�̖ړI�̕ό`���
	m_vPosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �|���S���̒����̈ʒu
	m_vPosDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �|���S���̒����̈ʒu
	m_nHP = PLAYER_DEFAULT_HP;								// �v���C���[�̗̑�
	m_fMP = PLAYER_DEFAULT_MP;								// �v���C���[�̕ό`�p�̃|�C���g
	m_bOperation = PLAYER_COMPUTER;							// �v���C���[�̑���t���O

	m_nAnimTime = 0;										// �v���C���[�ό`���̃A�j���[�V�����̎���
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//	�����@�@����
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
}

//-----------------------------------------------------------------------------
// �N���G�C�g
//	�����@�@�f�o�C�X�A���W�A���A�����A�e�N�X�`���̎�ށA�v���C���[����iMANUAL or AUTO�j
//	�߂�l�@�쐬�����v���C���[�̃|�C���^
//-----------------------------------------------------------------------------
CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float fWidth, float fHeight, TEXTURE_TYPE texture, PLAYER_OPERATION operation)
{
	// �v���C���[�|�C���^�̍쐬
	CPlayer *temp = new CPlayer(pDevice);

	// �쐬�����v���C���[���̏�����
	temp->Init(pos, fWidth, fHeight, texture, operation);

	// �쐬�����v���C���[�̃|�C���^��Ԃ�
	return temp;
}

//-----------------------------------------------------------------------------
// ������
//	�����@�@���W�A���A�����A�e�N�X�`���̎��
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::Init(D3DXVECTOR3 pos, float fWidth, float fHeight, TEXTURE_TYPE texture, PLAYER_OPERATION operation)
{
	CScene2D::Init(pos, fWidth, fHeight, texture);

	// ����t���O��ύX
	m_bOperation = operation;
}

//-----------------------------------------------------------------------------
// �I��
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::Uninit(void)
{
	CScene2D::Uninit();
}

//-----------------------------------------------------------------------------
// �X�V
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::Update(void)
{
	CScene2D::Update();

	// �����Ă邩���肷�邽�߂̃t���O��false�ɕύX
	//m_Action = PLAYER_ACTION_NONE;

	// �ړ��ʂ��O�ɂ���
	m_fMoveSpeedY = 0.0f;
	m_fMoveSpeedX = 0.0f;

	// �P�t���[���O�̍��W���p�̕ϐ��ɃZ�b�g����
	m_vPosOld = m_vPos;
	m_vPosDest = m_vPos;

	/*----------------------------------------------------------*/
	/*�v���C���[�̃A�N�V�������ό`���Ŗ��������ꍇ�̂ݑ��̍s����*/
	/*�s�����Ƃ��ł���											*/
	/*----------------------------------------------------------*/
	if (m_Action != PLAYER_ACTION_METAMORPHOSE)
	{

		/*----------------------------------------------------------*/
		/*�b��I�Ƀv���C���[�̑�����@�̓L�[�{�[�h�ł̑���Ƃ��Ă���*/
		/*�ړ��ʂ��b��I�Ȑ��l										*/
		/*----------------------------------------------------------*/

		/*----------------------------------------------------------*/
		/*�v���C���[�̈ړ�                                          */
		/*WSAD�L�[�ł��ꂼ��㉺���E�ւ̈ړ�						*/
		/*�A�N�V�����̏�Ԃ�S�ĕ��s��Ԃɕς���					*/
		/*----------------------------------------------------------*/
		// W�ŉ�ʏ�����ւ̈ړ�
		if (CInputKeyboard::GetKeyboardPress(DIK_W))
		{
			m_fMoveSpeedY = -5.0f;
			m_Action = PLAYER_ACTION_WALK;
		}
		// S�ŉ�ʉ������ւ̈ړ�
		else if (CInputKeyboard::GetKeyboardPress(DIK_S))
		{
			m_fMoveSpeedY = 5.0f;
			m_Action = PLAYER_ACTION_WALK;
		}
		// A�ŉ�ʍ������ւ̈ړ�
		if (CInputKeyboard::GetKeyboardPress(DIK_A))
		{
			m_fMoveSpeedX = -5.0f;
			m_Action = PLAYER_ACTION_WALK;
		}
		// D�ŉ�ʉE�����ւ̈ړ�
		else if (CInputKeyboard::GetKeyboardPress(DIK_D))
		{
			m_fMoveSpeedX = 5.0f;
			m_Action = PLAYER_ACTION_WALK;
		}

		/*----------------------------------------------------------*/
		/*K�L�[�Ńv���C���[�̍U��									*/
		/*----------------------------------------------------------*/
		if (CInputKeyboard::GetKeyboardTrigger(DIK_K))
		{
			// �A�N�V�����̏�Ԃ��U���ɕς���
			m_Action = PLAYER_ACTION_ATTACK;
		}

		/*----------------------------------------------------------*/
		/*L�L�[�Ŏ����o��											*/
		/*----------------------------------------------------------*/
		if (CInputKeyboard::GetKeyboardTrigger(DIK_L))
		{
			// �A�N�V�����̏�Ԃ������ˏ�Ԃɕς���
			m_Action = PLAYER_ACTION_THREAD;
		}

		/*----------------------------------------------------------*/
		/*M�L�[�Ńv���C���[�ό`�J�n									*/
		/*----------------------------------------------------------*/
		if (CInputKeyboard::GetKeyboardTrigger(DIK_M))
		{
			// �A�N�V�����̏�Ԃ�ό`�ɕς���
			m_Action = PLAYER_ACTION_METAMORPHOSE;
		}

		/*----------------------------------------------------------*/
		/*�����܂ł̂��̂��ŏI�I�ɂ̓R���g���[���[�ő���			*/
		/*----------------------------------------------------------*/
	}

	/*----------------------------------------------------------*/
	/*�v���C���[�̃A�N�V�����̏�ԂɑΉ������s������点��		*/
	/*���s�A�U���A�����ˁA�ό`									*/
	/*----------------------------------------------------------*/
	switch (m_Action)
	{
		// �v���C���[�̃A�N�V���������s�������ۂ̓���
	case PLAYER_ACTION_WALK:
		Move();
		break;

		// �v���C���[�̃A�N�V�������U���������ۂ̓���
	case PLAYER_ACTION_ATTACK:
		Attack();
		break;

		// �v���C���[�̃A�N�V�������ό`�������ۂ̓���
	case PLAYER_ACTION_METAMORPHOSE:
		/*----------------------------------------------------------*/
		/*�e�X�g�p�̏�ԕω�										*/
		/*----------------------------------------------------------*/
		if (m_Mode == PLAYER_MODE_NONE)
		{
			Metamorphose(PLAYER_MODE_MAX);
		}
		else
		{
			Metamorphose(PLAYER_MODE_NONE);
		}
		break;

		// �v���C���[�̃A�N�V�����������˂������ۂ̓���
	case PLAYER_ACTION_THREAD:
		SpidersThread();
		break;

	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// �`��
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::Draw(void)
{
	CScene2D::SetPos(m_vPos);
	CScene2D::SetRot(m_vRot);
	CScene2D::Draw();
}

//-----------------------------------------------------------------------------
// �v���C���[�̈ړ�
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::Move(void)
{
	float fDiffPosX;		// ���݂̍��W�ƖړI�̍��W�̍���(X��)
	float fDiffPosY;		// ���݂̍��W�ƖړI�̍��W�̍���(Y��)

	// �ړI�̍��W�����߂�
	m_vPosDest.y += m_fMoveSpeedY;
	m_vPosDest.x += m_fMoveSpeedX;

	// �ړI�̍��W�ƌ��݂̍��W�����߂�
	fDiffPosX = m_vPosDest.x - m_vPos.x;
	fDiffPosY = m_vPosDest.y - m_vPos.y;

	// ���݂̍��W���X�V����
	m_vPos.x += fDiffPosX * 0.5f;
	m_vPos.y += fDiffPosY * 0.5f;

	// �����ݒ�
	m_vRotDest.z = atan2f(fDiffPosY, fDiffPosX) + ((D3DX_PI * 90.0f)/180.0f);

	// �p�x�̐��K��
	NormalizeRotation(&m_vRotDest.z);

	m_vRot.z = m_vRotDest.z;
}

//-----------------------------------------------------------------------------
// �U���}�l�[�W�����Ăяo���֐�
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::Attack(void)
{
}

//-----------------------------------------------------------------------------
// �ό`
//	�����@�@�ό`������
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::Metamorphose(PLAYER_MODE mode)
{
	// �ړI�̏�Ԃ�؂�ւ���
	m_ModeDest = mode;

	MetamorphoseAnimation();
}

//-----------------------------------------------------------------------------
// ���}�l�[�W�����Ăяo���֐�
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::SpidersThread(void)
{
}

//-----------------------------------------------------------------------------
// �ό`�A�j���[�V����
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::MetamorphoseAnimation(void)
{
	m_Mode = m_ModeDest;

	// �A�j���[�V�����̎��Ԃ̑���
	m_nAnimTime++;

	if (m_nAnimTime > 60)
	{
		m_Action = PLAYER_ACTION_NONE;

		m_nAnimTime = 0;
	}
}

// EOF