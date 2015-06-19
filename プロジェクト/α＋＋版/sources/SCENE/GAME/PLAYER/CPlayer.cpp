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
#include "../UI/CMp.h"

//-----------------------------------------------------------------------------
// �萔��`
//-----------------------------------------------------------------------------
// �v���C���[�̈ړ����x(��)
static const float PLAYER_SPEED = 8.0f;

// �󕨃A�C�R���̑傫��
static const float TREASURE_ICON_WIDTH = 40;
static const float TREASURE_ICON_HEIGHT = 30;

// �󕨃A�C�R���̕\���ʒu
static const D3DXVECTOR3 TREASURE_ICON_POS_BUFF = D3DXVECTOR3(0, -50, 0);


//-----------------------------------------------------------------------------
// �R���X�g���N�^
//	�����@�@�f�o�C�X�A�v���C�I���e�B�A�I�u�W�F�N�g�^�C�v
//-----------------------------------------------------------------------------
CPlayer::CPlayer(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE objType) :CAnimation(pDevice, nPriority, objType)
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
	m_fHP = PLAYER_DEFAULT_HP;								// �v���C���[�̗̑�
	m_fMP = PLAYER_DEFAULT_MP;								// �v���C���[�̕ό`�p�̃|�C���g
	m_bOperation = PLAYER_COMPUTER;							// �v���C���[�̑���t���O
	m_sNumber = 0;											// �}�l�[�W���[�Ɋ���U����v���C���[�ԍ�
	m_PlayerFacing = PLAYER_DIRECTION_UP;					// �v���C���[�̏�������
	m_PlayerFacingOld = PLAYER_DIRECTION_UP;				// �v���C���[�̉ߋ��̌���
	m_nRedCount = 0;										// �Ԃ��_�ł��邽�߂̃J�E���g
	m_bRed = false;											// �Ԃ��_�ł��Ă��邩
	m_HpState = PLAYER_HP_STATE_NORMAL;						// HP�̎c����
	m_nTextureIndex = 0;									// �v���C���̍ŏ��̃C���f�b�N�X
	m_nTextureCount = 0;									// �e�N�X�`����ύX���邽�߂̃J�E���g

	// MP���
	m_pMp = new CMp(pDevice, PLAYER_DEFAULT_MP);
	m_pMp->Init();

	m_sAnimTime = 0;										// �v���C���[�ό`���̃A�j���[�V�����̎���
	m_sKnockBackTime = 0;									// �m�b�N�o�b�N����
	m_sDownTime = 0;										// �_�E������
	m_sMatchlessTime = 0;									// ���G��Ԃ̎���
	m_sKnockBackCount = 0;									// �����ԂɂȂ�����
	m_sRushTime = 0;

	m_bMatchless = false;									// ���G��Ԃ��ǂ�������
	m_bMetamorphose = false;								// �ό`������
	m_bSpeedAttack = false;									// �ړ��`�Ԃ̍U����������

	m_pTreasure = NULL;										// �󕨃|�C���^

	// �V�F�[�_�[�̏�����
	LPD3DXBUFFER code;
	LPD3DXBUFFER error;
	HRESULT hr = D3DXCompileShaderFromFile(
		"data/SHADER/redPS.hlsl", NULL, NULL, "main",
		"ps_2_0", 0, &code, &error, &m_pPSC);
	if (FAILED(hr)){
		MessageBox(nullptr, (LPSTR)error->GetBufferPointer(), "�G���[", 0);
	}

	(*pDevice)->CreatePixelShader((DWORD*)code->GetBufferPointer(), &m_pPS);

}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//	�����@�@����
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
	// MP�폜
	if (m_pMp){
		m_pMp->Uninit();
	}
	SAFE_DELETE(m_pMp);
}

//-----------------------------------------------------------------------------
// �N���G�C�g
//	�����@�@�f�o�C�X�A���W�A���A�����A�e�N�X�`���̎�ށA�v���C���[����iMANUAL or AUTO�j,�U���}�l�[�W���[ , �v���C���[�ԍ�, �v���C���R���g���[���t���O
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
	short sPlayerNumber,
	bool *bPlayerControl)
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

	// �v���C�����R���g���[���ł��邩�t���O
	temp->m_bPlayerControl = bPlayerControl;

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
	// �e�N�X�`���A�j���[�V�����̏�����
	CAnimation::Init(pos, fWidth, fHeight, texture, 6, 3);

	// �����𐳖ʂ̃e�N�X�`����
	SetIndex(5);
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
	// �v���C�����R���g���[���ł��Ȃ��Ȃ�X�V���Ȃ�
	if (!(*m_bPlayerControl)){
		return;
	}

	// �󕨂������Ă�����A�C�R���̏ꏊ�X�V
	if (m_pTreasure){
		m_pTreasure->SetPos(m_vPos + TREASURE_ICON_POS_BUFF);
	}

	CScene2D::Update();

	// MP�X�V
	m_pMp->Update(m_vPos, m_fMP);
	// �A�j���[�V�����X�V
	UpdatePlayerAnimation();
	// �Ԃ��_�ł���Ԋu�X�V
	UpdatePlayerRed();

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
		/*�v���C���[No.�ňړ��̃L�[��ς���							*/
		/*----------------------------------------------------------*/

		/*----------------------------------------------------------*/
		/*�v���C���[�̈ړ�                                          */
		/*WSAD�L�[�ł��ꂼ��㉺���E�ւ̈ړ�						*/
		/*�A�N�V�����̏�Ԃ�S�ĕ��s��Ԃɕς���					*/
		/*----------------------------------------------------------*/
		if (m_sNumber == 0)
		{
			// W�ŉ�ʏ�����ւ̈ړ�
			if (CInputKeyboard::GetKeyboardPress(DIK_W))
			{
				// �ړ����x�̕ύX
				// �v���C���[�̌`�Ԃ����x�d���������ꍇ�����ړ�����
				if (m_Mode == PLAYER_MODE_SPEED)
				{
					m_fMoveSpeedY = -PLAYER_SPEED * 1.5f;
				}
				else
				{
					m_fMoveSpeedY = -PLAYER_SPEED;
				}

				// �s�����ړ���Ԃɕς���
				m_Action = PLAYER_ACTION_WALK;

				// �v���C���[�̌����Ă��������ς���
				SetFace(PLAYER_DIRECTION_UP);
			}
			// S�ŉ�ʉ������ւ̈ړ�
			else if (CInputKeyboard::GetKeyboardPress(DIK_S))
			{
				// �ړ����x�̕ύX
				// �v���C���[�̌`�Ԃ����x�d���������ꍇ�����ړ�����
				if (m_Mode == PLAYER_MODE_SPEED)
				{
					m_fMoveSpeedY = PLAYER_SPEED * 1.5f;
				}
				else
				{
					m_fMoveSpeedY = PLAYER_SPEED;
				}

				// �s�����ړ���Ԃɕς���
				m_Action = PLAYER_ACTION_WALK;

				// �v���C���[�̌����Ă��������ς���
				SetFace(PLAYER_DIRECTION_DOWN);
			}
			// A�ŉ�ʍ������ւ̈ړ�
			if (CInputKeyboard::GetKeyboardPress(DIK_A))
			{
				// �ړ����x�̕ύX
				// �v���C���[�̌`�Ԃ����x�d���������ꍇ�����ړ�����
				if (m_Mode == PLAYER_MODE_SPEED)
				{
					m_fMoveSpeedX = -PLAYER_SPEED * 1.5f;
				}
				else
				{
					m_fMoveSpeedX = -PLAYER_SPEED;
				}

				// �s�����ړ���Ԃɕς���
				m_Action = PLAYER_ACTION_WALK;

				// �v���C���[�̌����Ă��������ς���
				SetFace(PLAYER_DIRECTION_LEFT);
			}
			// D�ŉ�ʉE�����ւ̈ړ�
			else if (CInputKeyboard::GetKeyboardPress(DIK_D))
			{
				// �ړ����x�̕ύX
				// �v���C���[�̌`�Ԃ����x�d���������ꍇ�����ړ�����
				if (m_Mode == PLAYER_MODE_SPEED)
				{
					m_fMoveSpeedX = PLAYER_SPEED * 1.5f;
				}
				else
				{
					m_fMoveSpeedX = PLAYER_SPEED;
				}

				// �s�����ړ���Ԃɕς���
				m_Action = PLAYER_ACTION_WALK;

				// �v���C���[�̌����Ă��������ς���
				SetFace(PLAYER_DIRECTION_RIGHT);
			}

			/*----------------------------------------------------------*/
			/*4�L�[�Ńv���C���[�̍U��									*/
			/*----------------------------------------------------------*/
			if (CInputKeyboard::GetKeyboardTrigger(DIK_4))
			{
				// �A�N�V�����̏�Ԃ��U���ɕς���
				m_Action = PLAYER_ACTION_ATTACK;
			}

			/*----------------------------------------------------------*/
			/*5�L�[�Ŏ����o��											*/
			/*----------------------------------------------------------*/
			if (CInputKeyboard::GetKeyboardTrigger(DIK_5))
			{
				// �A�N�V�����̏�Ԃ������ˏ�Ԃɕς���
				m_Action = PLAYER_ACTION_THREAD;
			}

			/*----------------------------------------------------------*/
			/*6�L�[�Ńv���C���[�ό`�J�n	(Attack)						*/
			/*----------------------------------------------------------*/
			if (CInputKeyboard::GetKeyboardTrigger(DIK_6) && m_fMP>0.0f)
			{
				// �A�N�V�����̏�Ԃ�ό`�ɕς���
				m_Action = PLAYER_ACTION_METAMORPHOSE;
				// ���ɕό`�����Ԃ�ύX����
				m_ModeDest = PLAYER_MODE_ATTACK;
			}
			/*----------------------------------------------------------*/
			/*7�L�[�Ńv���C���[�ό`�J�n	(Speed)						*/
			/*----------------------------------------------------------*/
			if (CInputKeyboard::GetKeyboardTrigger(DIK_7) && m_fMP>0.0f)
			{
				// �A�N�V�����̏�Ԃ�ό`�ɕς���
				m_Action = PLAYER_ACTION_METAMORPHOSE;
				// ���ɕό`�����Ԃ�ύX����
				m_ModeDest = PLAYER_MODE_SPEED;
			}
			/*----------------------------------------------------------*/
			/*8�L�[�Ńv���C���[�ό`�J�n	(Trap)						*/
			/*----------------------------------------------------------*/
			if (CInputKeyboard::GetKeyboardTrigger(DIK_8) && m_fMP>0.0f)
			{
				// �A�N�V�����̏�Ԃ�ό`�ɕς���
				m_Action = PLAYER_ACTION_METAMORPHOSE;
				// ���ɕό`�����Ԃ�ύX����
				m_ModeDest = PLAYER_MODE_TRAP;
			}
			/*----------------------------------------------------------*/
			/*9�L�[�Ńv���C���[�ό`�J�n	(Normal)						*/
			/*----------------------------------------------------------*/
			if (CInputKeyboard::GetKeyboardTrigger(DIK_9) && m_fMP>0.0f)
			{
				// �A�N�V�����̏�Ԃ�ό`�ɕς���
				m_Action = PLAYER_ACTION_METAMORPHOSE;
				// ���ɕό`�����Ԃ�ύX����
				m_ModeDest = PLAYER_MODE_NONE;
			}

			// debug
			if (CInputKeyboard::GetKeyboardTrigger(DIK_SPACE)){
				// �󕨂𗎂Ƃ�
				FallTreasure();
			}
		}
		/*----------------------------------------------------------*/
		/*�Q�̖ڂ̓G�̓���											*/
		/*----------------------------------------------------------*/
		else if (m_sNumber == 1)
		{
			// I�ŉ�ʏ�����ւ̈ړ�
			if (CInputKeyboard::GetKeyboardPress(DIK_I))
			{
				m_fMoveSpeedY = -5.0f;
				m_Action = PLAYER_ACTION_WALK;
				SetFace(PLAYER_DIRECTION_UP);
			}
			// K�ŉ�ʉ������ւ̈ړ�
			else if (CInputKeyboard::GetKeyboardPress(DIK_K))
			{
				m_fMoveSpeedY = 5.0f;
				m_Action = PLAYER_ACTION_WALK;
				SetFace(PLAYER_DIRECTION_DOWN);
			}
			// J�ŉ�ʍ������ւ̈ړ�
			if (CInputKeyboard::GetKeyboardPress(DIK_J))
			{
				m_fMoveSpeedX = -5.0f;
				m_Action = PLAYER_ACTION_WALK;
				SetFace(PLAYER_DIRECTION_LEFT);
			}
			// L�ŉ�ʉE�����ւ̈ړ�
			else if (CInputKeyboard::GetKeyboardPress(DIK_L))
			{
				m_fMoveSpeedX = 5.0f;
				m_Action = PLAYER_ACTION_WALK;
				SetFace(PLAYER_DIRECTION_RIGHT);
			}

			/*----------------------------------------------------------*/
			/*0�L�[�Ńv���C���[�̍U��									*/
			/*----------------------------------------------------------*/
			if (CInputKeyboard::GetKeyboardTrigger(DIK_0))
			{
				// �A�N�V�����̏�Ԃ��U���ɕς���
				m_Action = PLAYER_ACTION_ATTACK;
			}
		}


		/*----------------------------------------------------------*/
		/*�����܂ł̂��̂��ŏI�I�ɂ̓R���g���[���[�ő���			*/
		/*----------------------------------------------------------*/
	}

	// ���G��Ԃł̏���
	if (m_bMatchless)
	{
		Matchless();
	}

	// �ό`���Ă���ꍇMP�����������Ă���
	if (m_bMetamorphose)
	{
		MPReduce();

		// MP���O�ɂȂ�����ʏ��Ԃɖ߂�
		if (m_fMP <= 0.0f)
		{
			m_ModeDest = PLAYER_MODE_NONE;

			m_Action = PLAYER_ACTION_METAMORPHOSE;

			m_fMP = 0.0f;
		}
	}
	else
	{
		// �f�t�H���gMP�܂ŉ񕜂�����
		if (m_fMP < PLAYER_DEFAULT_MP)
		{
			m_fMP += 0.5f;

			// �����߂������ǉ�
			if (m_fMP > PLAYER_DEFAULT_MP){
				m_fMP = PLAYER_DEFAULT_MP;
			}
		}
	}

	/*----------------------------------------------------------*/
	/*�ړ��`�Ԃł̍U�����������ꍇ�A�N�V������NONE�ɕς���		*/
	/*�ړ��`�Ԃ̍U���̃G�t�F�N�g�ɍ��킹�Ĉړ�����				*/
	/*----------------------------------------------------------*/
	if (m_bSpeedAttack)
	{
		Rush();
		m_Action = PLAYER_ACTION_NONE;
		return;
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
		// �ό`��ƕό`�����������ό`�J�n
		if (m_Mode != m_ModeDest)
		{
			Metamorphose();
		}
		// �ό`�\���ƌ��݂̌`�Ԃ�������������s����߂�
		else
		{
			m_Action = PLAYER_ACTION_NONE;
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

	// ���݂�MP��\������
#ifdef _DEBUG
	CDebugProc::Print("%d�v���C���[�c��MP %f\n", m_sNumber , m_fMP);
#endif

}

//-----------------------------------------------------------------------------
// �`��
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::Draw(void)
{
	// �Ԃ�����t���O
	if (m_bRed){
		// �s�N�Z���V�F�[�_�[�̃Z�b�g
		(*m_pD3DDevice)->SetPixelShader(m_pPS);
	}

	CScene2D::Draw();

	// �s�N�Z���V�F�[�_�[NULL�Z�b�g
	(*m_pD3DDevice)->SetPixelShader(nullptr);
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
}

//-----------------------------------------------------------------------------
// �U���}�l�[�W�����Ăяo���֐�
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::Attack(void)
{
	switch (m_Mode)
	{
		// ���ʍU��
	case PLAYER_MODE_NONE:
		m_pAttackManager->CreateAttack(
			ATTACK_TYPE_NORMAL,
			m_sNumber,
			m_vPos,
			PLAYER_DIRECTION_VECTOR[m_PlayerFacing]);
		break;
		// �U�������̍U��
	case PLAYER_MODE_ATTACK:
		m_pAttackManager->CreateAttack(
			ATTACK_TYPE_ATTACK,
			m_sNumber,
			m_vPos,
			PLAYER_DIRECTION_VECTOR[m_PlayerFacing]);
		break;
		// �ړ������̍U��
	case PLAYER_MODE_SPEED:
		m_pAttackManager->CreateAttack(
			ATTACK_TYPE_SPEED,
			m_sNumber,
			m_vPos,
			PLAYER_DIRECTION_VECTOR[m_PlayerFacing]);
		m_bSpeedAttack = true;
		break;
		// 㩌^�̍U��
	case PLAYER_MODE_TRAP:
		m_pAttackManager->CreateAttack(
			ATTACK_TYPE_TRAP,
			m_sNumber,
			m_vPos,
			PLAYER_DIRECTION_VECTOR[m_PlayerFacing]);
		break;
	default:
		break;
	}

	// �A�N�V�����̏�Ԃ�߂�
	m_Action = PLAYER_ACTION_NONE;

}

//-----------------------------------------------------------------------------
// �ό`
//	�����@�@�ό`������
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::Metamorphose(void)
{
	MetamorphoseAnimation();
}

//-----------------------------------------------------------------------------
// �ό`�A�j���[�V����
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::MetamorphoseAnimation(void)
{
	// �A�j���[�V�����̎��Ԃ̑���
	m_sAnimTime++;

	if (m_sAnimTime > 60)
	{
		switch (m_ModeDest)
		{
			// �U���`�Ԃ�������Ԃ�����
		case PLAYER_MODE_ATTACK:
			// �F�ύX
			CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			// �t���O�ύX
			m_bMetamorphose = true;
			break;
			// �ړ��`�Ԃ������������
		case PLAYER_MODE_SPEED:
			// �F�ύX
			CScene2D::SetColorPolygon(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
			// �t���O�ύX
			m_bMetamorphose = true;
			break;
			// 㩌`�Ԃ�������΂ɂ���
		case PLAYER_MODE_TRAP:
			// �F�ύX
			CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
			// �t���O�ύX
			m_bMetamorphose = true;
			break;
			// �ʏ��Ԃ�������ʏ�̐F�ɂ���
		case PLAYER_MODE_NONE:
			// �F�ύX
			CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			// �t���O�ύX
			m_bMetamorphose = false;
			break;
		default:
			break;
		}

		// �A�j���[�V�������Ԃ����Z�b�g���Ă���
		m_sAnimTime = 0;

		// �ړI�̌`�Ԃ����݂̌`�Ԃɂ���
		m_Mode = m_ModeDest;

		// �v���C���[�̍s����������Ԃɂ��ǂ�
		m_Action = PLAYER_ACTION_NONE;
	}
}

//-----------------------------------------------------------------------------
// MP������������֐�
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::MPReduce(void)
{
	// MP�����炵�Ă���
	// ���l�͉�
	m_fMP -= 1.5f;
}

//-----------------------------------------------------------------------------
// ���}�l�[�W�����Ăяo���֐�
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::SpidersThread(void)
{
	switch (m_Mode)
	{
		// �ʏ��Ԃ̎�
	case PLAYER_MODE_NONE:
		m_pThreadManager->CreateThread(
			THREAD_TYPE_NORMAL,
			m_sNumber,
			m_vPos);
		break;
		// �U�������`�Ԃ̎�
	case PLAYER_MODE_ATTACK:
		m_pThreadManager->CreateThread(
			THREAD_TYPE_ATTACK,
			m_sNumber,
			m_vPos);
		break;
		// �ړ������`�Ԃ̎�
	case PLAYER_MODE_SPEED:
		m_pThreadManager->CreateThread(
			THREAD_TYPE_SPEED,
			m_sNumber,
			m_vPos);
		break;
		// 㩓����`�Ԃ̎�
	case PLAYER_MODE_TRAP:
		m_pThreadManager->CreateThread(
			THREAD_TYPE_TRAP,
			m_sNumber,
			m_vPos);
		break;
	default:
		break;
	}

	// �s����������Ԃɖ߂�
	m_Action = PLAYER_ACTION_NONE;
}

//-----------------------------------------------------------------------------
// �����Ԃł̏���
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::KnockBack(void)
{
	m_sKnockBackTime++;

	// ���񐔂����ԂɂȂ�����_�E����ԂɈڍs
	if (m_sKnockBackCount > 3)
	{
		SetPlayerDown();
		m_sKnockBackCount = 0;
	}

	// ��莞�Ԍo�߂����珉����Ԃɂ��ǂ�
	else if (m_sKnockBackTime > 100)
	{
		// �A�N�V�����̏�Ԃ�������Ԃɖ߂�
		m_Action = PLAYER_ACTION_NONE;

		// ����ɂȂ����񐔂𑝂₷
		m_sKnockBackCount++;

		// ���ꎞ�Ԃ̏�����
		m_sKnockBackTime = 0;

		// �F�ύX
		CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

//-----------------------------------------------------------------------------
// �_�E����Ԃł̏���
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::PlayerDown(void)
{
	m_sDownTime++;

	if (m_sDownTime > 300)
	{
		m_Action = PLAYER_ACTION_NONE;

		CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		m_bMatchless = true;

		m_sDownTime = 0;
	}
}

//-----------------------------------------------------------------------------
// ���G��Ԃ̍ۂ̓���
//	�����@�@����
//	�߂�l�@����
//-----------------------------------------------------------------------------
void CPlayer::Matchless(void)
{
	m_sMatchlessTime++;

	// �F�ύX
	CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(m_sMatchlessTime % 10) / 10.0f));

	// ��莞�Ԍo�߂����珉����Ԃֈڍs
	if (m_sMatchlessTime > 500)
	{
		// ���G��ԃt���O������
		m_bMatchless = false;

		// �F�ύX
		CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// ���G���Ԃ̏�����
		m_sMatchlessTime = 0;
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

			// �F�ύX
			CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 0.6f, 0.6f, 1.0f));

			// ��������Ă����痎�Ƃ�
			if (m_pTreasure)
			{
				FallTreasure();
			}
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

			CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f));

			// ��������Ă����痎�Ƃ�
			if (m_pTreasure)
			{
				FallTreasure();
			}
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


//-----------------------------------------------------------------------------
// �v���C���̌������Z�b�g����֐�
//-----------------------------------------------------------------------------
void CPlayer::SetFace(DIRECTION_PLAYER_FACING value){

	// �����l�͘A���ŃZ�b�g���Ȃ�
	if (m_PlayerFacing == value){
		return;
	}

	// �v���C���̌����X�e�[�g�ύX
	m_PlayerFacing = value;

	// �v���C���̌����ɑΉ������e�N�X�`�����Z�b�g
	m_nTextureIndex = PLAYER_TEXTURE_INDEX_MIN[value];

	// �v���C���̃e�N�X�`���A�j���[�V�����p�J�E���g�����Z�b�g
	m_nTextureCount = 0;
}

//-----------------------------------------------------------------------------
// �v���C���̌����ɂ���ăe�N�X�`���ԍ�����
// �X�V�ň��Ă�
//-----------------------------------------------------------------------------
void CPlayer::UpdatePlayerAnimation(void){

	if (m_PlayerFacing == PLAYER_DIRECTION_UP){
		m_nTextureCount++;
		if (m_nTextureCount > 10){
			m_nTextureCount = 0;

			m_nTextureIndex++;
			if (m_nTextureIndex > PLAYER_TEXTURE_INDEX_MAX[PLAYER_DIRECTION_UP]){
				m_nTextureIndex = PLAYER_TEXTURE_INDEX_MIN[PLAYER_DIRECTION_UP];
			}
		}
		SetIndex(m_nTextureIndex);
	}
	else if (m_PlayerFacing == PLAYER_DIRECTION_DOWN){
		m_nTextureCount++;
		if (m_nTextureCount > 10){
			m_nTextureCount = 0;

			m_nTextureIndex++;
			if (m_nTextureIndex > PLAYER_TEXTURE_INDEX_MAX[PLAYER_DIRECTION_DOWN]){
				m_nTextureIndex = PLAYER_TEXTURE_INDEX_MIN[PLAYER_DIRECTION_DOWN];
			}
		}
		SetIndex(m_nTextureIndex);
	}
	else if (m_PlayerFacing == PLAYER_DIRECTION_RIGHT){
		m_nTextureCount++;
		if (m_nTextureCount > 10){
			m_nTextureCount = 0;

			m_nTextureIndex++;
			if (m_nTextureIndex > PLAYER_TEXTURE_INDEX_MAX[PLAYER_DIRECTION_RIGHT]){
				m_nTextureIndex = PLAYER_TEXTURE_INDEX_MIN[PLAYER_DIRECTION_RIGHT];
			}
		}
		SetIndex(m_nTextureIndex, true);
	}
	else if (m_PlayerFacing == PLAYER_DIRECTION_LEFT){
		m_nTextureCount++;
		if (m_nTextureCount > 10){
			m_nTextureCount = 0;

			m_nTextureIndex++;
			if (m_nTextureIndex > PLAYER_TEXTURE_INDEX_MAX[PLAYER_DIRECTION_LEFT]){
				m_nTextureIndex = PLAYER_TEXTURE_INDEX_MIN[PLAYER_DIRECTION_LEFT];
			}
		}
		SetIndex(m_nTextureIndex, false);
	}

}

//-----------------------------------------------------------------------------
// �ړ��`�Ԃł̍U���ł̃^�b�N���ړ�
//-----------------------------------------------------------------------------
void CPlayer::Rush(void)
{
	m_vPos += PLAYER_DIRECTION_VECTOR[m_PlayerFacing] * 4.0f;

	m_sRushTime++;

	if (m_sRushTime > 50)
	{
		m_bSpeedAttack = false;
		m_sRushTime = 0;
	}
}

//-----------------------------------------------------------------------------
// �̗̓Z�b�^�[
//-----------------------------------------------------------------------------
void CPlayer::AddHp(float fPoint){

	// HP���v
	m_fHP += fPoint;

	// �N�����v
	if (m_fHP > PLAYER_DEFAULT_HP){
		m_fHP = PLAYER_DEFAULT_HP;
	}
	else if (m_fHP < 0.0f){
		m_fHP = 0.0f;
	}

	// HP�c���Ԃ��X�V
	UpdatePlayerHpState();
}

//-----------------------------------------------------------------------------
// �v���C����HP�c���Ԃ��Ǘ��@�X�V�ň��Ăԁ@��������HP�ύX�̂������Ƃ��݂̂�OK 
//-----------------------------------------------------------------------------
void CPlayer::UpdatePlayerHpState(void){

	// ���݂�HP�̊���
	float ratio = m_fHP / PLAYER_DEFAULT_HP;

	// �����ɉ����ăX�e�[�g��ύX
	for (int i = 0; i < PLAYER_HP_STATE_MAX; i++){
		if (ratio >= PLAYER_HP_STATE_RATIO[i]){
			m_HpState = (PLAYER_HP_STATE)i;
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// �v���C����HP�c���ԂŐԂ��_�ł���Ǘ��@�X�V�ň��Ă�
//-----------------------------------------------------------------------------
void CPlayer::UpdatePlayerRed(void){

	if (CInputKeyboard::GetKeyboardPress(DIK_RCONTROL)){
		AddHp(-2);
	}
	if (CInputKeyboard::GetKeyboardPress(DIK_RSHIFT)){
		AddHp(PLAYER_DEFAULT_HP);
	}
#ifdef _DEBUG
	if (m_HpState == PLAYER_HP_STATE_NORMAL){
		CDebugProc::Print("�v���C��HP��� = NORMAL\n");
	}
	else if (m_HpState == PLAYER_HP_STATE_LOW){
		CDebugProc::Print("�v���C��HP��� = LOW\n");
	}
	else if (m_HpState == PLAYER_HP_STATE_VERY_LOW){
		CDebugProc::Print("�v���C��HP��� = VERY_LOW\n");
	}
	else if (m_HpState == PLAYER_HP_STATE_DIE){
		CDebugProc::Print("�v���C��HP��� = DIE\n");
	}
	CDebugProc::Print("count = %d\n", m_nRedCount);
#endif
	// ����ł��� or HP���\������Ȃ�X�V���Ȃ�
	if (m_HpState == PLAYER_HP_STATE_DIE ||
		m_HpState == PLAYER_HP_STATE_NORMAL){
		m_nRedCount = 0;
		m_bRed = false;
		return;
	}

	// �Ԃ��_�ł���Ԋu�Ńt���O�؂�ւ�
	m_nRedCount++;
	if (m_nRedCount > PLAYER_HP_STATE_FLASH_INTERVAL[m_HpState]){
		m_bRed = !m_bRed;
		m_nRedCount = 0;
	}
}
// EOF