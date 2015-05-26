//=============================================================================
//
// CPlayer�N���X [CPlayer.cpp]
// Author : �����@�Ȉ�
//
//=============================================================================
#include "CPlayer.h"

#include "../../../INPUT/CInputKeyboard.h"
#include "../../../TEXTURE/CTexture.h"
#include "../ATTACK/CAttackManager.h"
#include "../THREAD/CThreadManager.h"
#include "../TREASURE/CTreasure.h"

//-----------------------------------------------------------------------------
// �萔��`
//-----------------------------------------------------------------------------
// �󕨃A�C�R���̑傫��
static const float TREASURE_ICON_WIDTH = 40;
static const float TREASURE_ICON_HEIGHT = 30;

// �󕨃A�C�R���̕\���ʒu
static const D3DXVECTOR3 TREASURE_ICON_POS_BUFF = D3DXVECTOR3(0, -50, 0);


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
	m_sNumber = 0;											// �}�l�[�W���[�Ɋ���U����v���C���[�ԍ�
	m_PlayerFacing = PLAYER_DIRECTION_UP;					// �v���C���[�̏�������
	m_PlayerFacingOld = PLAYER_DIRECTION_UP;				// �v���C���[�̉ߋ��̌���

	m_nAnimTime = 0;										// �v���C���[�ό`���̃A�j���[�V�����̎���
	m_nKnockBackTime = 0;									// �m�b�N�o�b�N����
	m_nDownTime = 0;										// �_�E������
	m_nMatchlessTime = 0;									// ���G��Ԃ̎���
	m_nKnockBackCount = 0;									// �����ԂɂȂ�����

	m_bMatchless = false;									// ���G��Ԃ��ǂ�������

	m_pTreasure = NULL;										// �󕨃|�C���^
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
//	�����@�@�f�o�C�X�A���W�A���A�����A�e�N�X�`���̎�ށA�v���C���[����iMANUAL or AUTO�j,�U���}�l�[�W���[ , �v���C���[�ԍ�
//	�߂�l�@�쐬�����v���C���[�̃|�C���^
//-----------------------------------------------------------------------------
CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 *pDevice,
	D3DXVECTOR3 pos,
	float fWidth,
	float fHeight,
	TEXTURE_TYPE texture,
	BOOL playerOperation,
	CAttackManager *pAttackManager,
	CThreadManager *pThreadManager,
	short sPlayerNumber)
{
	// �v���C���[�|�C���^�̍쐬
	CPlayer *temp = new CPlayer(pDevice);

	// �쐬�����v���C���[���̏�����
	temp->Init(pos, fWidth, fHeight, texture);

	// ����t���O��ύX
	temp->m_bOperation = playerOperation;

	// �v���C���[�ԍ��̃Z�b�g
	temp->m_sNumber = sPlayerNumber;

	// �U���}�l�[�W���̕ێ�
	temp->m_pAttackManager = pAttackManager;

	// ���}�l�[�W���̕ێ�
	temp->m_pThreadManager = pThreadManager;

	// �쐬�����v���C���[�̃|�C���^��Ԃ�
	return temp;
}

//-----------------------------------------------------------------------------
// ������
//	�����@�@���W�A���A�����A�e�N�X�`���̎��
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::Init(D3DXVECTOR3 pos, float fWidth, float fHeight, TEXTURE_TYPE texture)
{
	CScene2D::Init(pos, fWidth, fHeight, texture);
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
	// �󕨂������Ă�����A�C�R���̏ꏊ�X�V
	if (m_pTreasure){
		m_pTreasure->SetPos(m_vPos + TREASURE_ICON_POS_BUFF);
	}

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
	if (m_Action != PLAYER_ACTION_METAMORPHOSE && m_Action != PLAYER_ACTION_KNOCK_BACK && m_Action != PLAYER_ACTION_DOWN)
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
			m_PlayerFacing = PLAYER_DIRECTION_UP;
		}
		// S�ŉ�ʉ������ւ̈ړ�
		else if (CInputKeyboard::GetKeyboardPress(DIK_S))
		{
			m_fMoveSpeedY = 5.0f;
			m_Action = PLAYER_ACTION_WALK;
			m_PlayerFacing = PLAYER_DIRECTION_DOWN;
		}
		// A�ŉ�ʍ������ւ̈ړ�
		if (CInputKeyboard::GetKeyboardPress(DIK_A))
		{
			m_fMoveSpeedX = -5.0f;
			m_Action = PLAYER_ACTION_WALK;
			m_PlayerFacing = PLAYER_DIRECTION_LEFT;
		}
		// D�ŉ�ʉE�����ւ̈ړ�
		else if (CInputKeyboard::GetKeyboardPress(DIK_D))
		{
			m_fMoveSpeedX = 5.0f;
			m_Action = PLAYER_ACTION_WALK;
			m_PlayerFacing = PLAYER_DIRECTION_RIGHT;
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

		// debug
		if (CInputKeyboard::GetKeyboardTrigger(DIK_SPACE)){
			// �󕨂𗎂Ƃ�
			FallTreasure();
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

		// �v���C���[�̃A�N�V���������ꂾ������
	case PLAYER_ACTION_KNOCK_BACK:
		KnockBack();
		break;

		// �v���C���[�̏�Ԃ��_�E����������
	case PLAYER_ACTION_DOWN:
		PlayerDown();
		break;

	default:
		break;
	}

	// ���G��Ԃ������ꍇ�͖��G��Ԏ��̏������s��
	if (m_bMatchless == true)
	{
		Matchless();
	}
}

//-----------------------------------------------------------------------------
// �`��
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::Draw(void)
{
//	CScene2D::SetPos(m_vPos);
//	CScene2D::SetRot(m_vRot);
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

	/*
	// ��Ɖ��̈ړ��ȊO�̈ړ��ł͌�����ς���
	if (m_PlayerFacing == PLAYER_DIRECTION_UP || m_PlayerFacing == PLAYER_DIRECTION_DOWN)
	{
		// �����ݒ�
		m_vRot.z = DEGREE_TO_RADIAN(0.0f);

		// �p�x�̐��K��
		NormalizeRotation(&m_vRot.z);
	}
	else
	{
		// �����ݒ�
		m_vRot.z = atan2f(fDiffPosY, fDiffPosX) + DEGREE_TO_RADIAN(90.0f);

		// �p�x�̐��K��
		NormalizeRotation(&m_vRot.z);
	}
	*/

	// �v���C���[�̈ړ��������ς������e�N�X�`����U�l��ς���
	if ((m_PlayerFacing == PLAYER_DIRECTION_LEFT || m_PlayerFacing == PLAYER_DIRECTION_RIGHT) &&
		m_PlayerFacing != m_PlayerFacingOld)
	{
		ChangeTextureFaceU();

		m_PlayerFacingOld = m_PlayerFacing;
	}
	//m_PlayerFacing = PLAYER_DIRECTION_UP;
}

//-----------------------------------------------------------------------------
// �U���}�l�[�W�����Ăяo���֐�
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::Attack(void)
{
	// ���ʍU��
	m_pAttackManager->CreateAttack(
		ATTACK_TYPE_NORMAL,
		m_sNumber,
		m_vPos);
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
	// ���ʍU��
	m_pThreadManager->CreateThread(
		THREAD_TYPE_NORMAL,
		m_sNumber,
		m_vPos);
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

//-----------------------------------------------------------------------------
// �����Ԃł̏���
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::KnockBack(void)
{
	m_nKnockBackTime++;

	// ���񐔂����ԂɂȂ�����_�E����ԂɈڍs
	if (m_nKnockBackCount > 3)
	{
		SetPlayerDown();
		m_nKnockBackCount = 0;
	}

	// ��莞�Ԍo�߂����珉����Ԃɂ��ǂ�
	else if (m_nKnockBackTime > 100)
	{
		// �A�N�V�����̏�Ԃ�������Ԃɖ߂�
		m_Action = PLAYER_ACTION_NONE;

		// ����ɂȂ����񐔂𑝂₷
		m_nKnockBackCount++;

		// ���ꎞ�Ԃ̏�����
		m_nKnockBackTime = 0;
	}
}

//-----------------------------------------------------------------------------
// �_�E����Ԃł̏���
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::PlayerDown(void)
{
	m_nDownTime++;

	// ��莞�Ԍo�߂����疳�G��Ԃֈڍs
	if (m_nDownTime > 500)
	{
		m_Action = PLAYER_ACTION_NONE;

		m_bMatchless = true;

		m_nDownTime = 0;
	}
}

//-----------------------------------------------------------------------------
// ���G��Ԃ̍ۂ̓���
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::Matchless(void)
{
	m_nMatchlessTime++;

	// �F�ύX
	CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(m_nMatchlessTime%10)/10.0f));

	// ��莞�Ԍo�߂����珉����Ԃֈڍs
	if (m_nMatchlessTime > 500)
	{
		// ���G��ԃt���O������
		m_bMatchless = false;

		// �F�ύX
		CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// ���G���Ԃ̏�����
		m_nMatchlessTime = 0;
	}
}

//-----------------------------------------------------------------------------
// �v���C���[�̏�Ԃ������ԂɈڍs���ɂ����Ԃ������ꍇ�̓_�E����Ԃ�
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::SetPlyerKnockBack(void)
{
	// ���G��ԂłȂ���΂��̏������s��
	if (m_bMatchless == false)
	{
		// ���ɂ����Ԃ������ꍇ�ɂ̓_�E����Ԃ�
		if (m_Action == PLAYER_ACTION_KNOCK_BACK)
		{
			m_Action = PLAYER_ACTION_DOWN;
		}
		// �_�E����Ԃ܂��͕ό`��Ԃł͖��������ꍇ�͂����Ԃ�
		else if (m_Action != PLAYER_ACTION_DOWN && m_Action != PLAYER_ACTION_METAMORPHOSE)
		{
			m_Action = PLAYER_ACTION_KNOCK_BACK;
		}
	}
}

//-----------------------------------------------------------------------------
// �v���C���[�̏�Ԃ��_�E����Ԃֈڍs
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::SetPlayerDown(void)
{
	// ���G��ԂłȂ���΂��̏������s��
	if (m_bMatchless == false)
	{
		if (m_Action != PLAYER_ACTION_METAMORPHOSE)
		{
			m_Action = PLAYER_ACTION_DOWN;
		}
	}
}

//-----------------------------------------------------------------------------
// �e�N�X�`����UV�؂�ւ��̏���(U�l�̐؂�ւ�)
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::ChangeTextureFaceU(void)
{
	VERTEX_2D *pVtx;
	float fTemp;			// ���ϐ�

	// �|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`���̏㕔����U�l�̓���ւ�
	fTemp = pVtx[0].tex.x;
	pVtx[0].tex.x = pVtx[1].tex.x;
	pVtx[1].tex.x = fTemp;

	// �e�N�X�`���̉�������U�l�̓���ւ�
	fTemp = pVtx[2].tex.x;
	pVtx[2].tex.x = pVtx[3].tex.x;
	pVtx[3].tex.x = fTemp;

	m_pD3DVtxBuff->Unlock();

}

//-----------------------------------------------------------------------------
// �v���C���ɕ󕨂����āA�A�C�R���ɂ���
// �v���C�����󕨂���������ɂ�΂��
//-----------------------------------------------------------------------------
void CPlayer::SetTreasure(CTreasure *pTreasure){
	// �|�C���^�ۑ�
	m_pTreasure = pTreasure;

	// �e�N�X�`���Ƒ傫����ύX
	m_pTreasure->SetWidth(TREASURE_ICON_WIDTH);
	m_pTreasure->SetHeight(TREASURE_ICON_HEIGHT);
	m_pTreasure->ChangeTexture(TEXTURE_TREASURE_ICON);
}

//-----------------------------------------------------------------------------
// �󕨂̕\����߂��āA�󕨂̕ێ��|�C���^��NULL
// �v���C�����󕨂𗎂Ƃ����Ƃ��ɌĂ΂��
//-----------------------------------------------------------------------------
void CPlayer::FallTreasure(){
	
	if (m_pTreasure){
		// �󕨂̃e�N�X�`����傫�����Z�b�g
		m_pTreasure->Reset(m_vPos);

		// �|�C���^�폜
		m_pTreasure = NULL;
	}

}
// EOF