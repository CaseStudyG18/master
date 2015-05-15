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
//-----------------------------------------------------------------------------
CPlayer::CPlayer(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE objType) :CScene2D(pDevice, nPriority, objType)
{
	m_pD3DDevice = pDevice;                                 // �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	m_pD3DVtxBuff = NULL;                                   // ���_���W�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	m_pD3DTexBuff = NULL;                                   // �e�N�X�`�������i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);                 // �|���S���̒����̈ʒu
	m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);                 // �|���S���̉�]
	m_bMove = false;
	m_fMoveSpeedX = 0.0f;
	m_fMoveSpeedY = 0.0f;
	m_Action = PLAYER_ACTION_NONE;
	m_Mode = PLAYER_MODE_NONE;
	m_vPosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);              // �|���S���̒����̈ʒu
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
}

//-----------------------------------------------------------------------------
// �N���G�C�g
//-----------------------------------------------------------------------------
CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float fWidth, float fHeight, TEXTURE_TYPE texture)
{
	CPlayer *temp = new CPlayer(pDevice);

	temp->Init(pos, fWidth, fHeight, texture);

	return temp;
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
void CPlayer::Init(D3DXVECTOR3 pos, float fWidth, float fHeight, TEXTURE_TYPE texture)
{
	CScene2D::Init(pos, fWidth, fHeight, texture);
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CPlayer::Uninit(void)
{
	CScene2D::Uninit();
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CPlayer::Update(void)
{
	CScene2D::Update();

	// �����Ă邩���肷�邽�߂̃t���O��false�ɕύX
	m_Action = PLAYER_ACTION_NONE;

	// �ړ��ʂ��O�ɂ���
	m_fMoveSpeedY = 0.0f;
	m_fMoveSpeedX = 0.0f;

	// �P�t���[���O�̍��W���p�̕ϐ��ɃZ�b�g����
	m_vPosOld = m_vPos;
	m_vRotOld = m_vRot;

	/*----------------------------------------------------------*/
	/*�b��I�Ƀv���C���[�̈ړ����@�̓L�[�{�[�h�ł̑���Ƃ��Ă���*/
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
	/*�b��I�Ƀv���C���[�̈ړ����@�̓L�[�{�[�h�ł̑���Ƃ��Ă���*/
	/*----------------------------------------------------------*/

	// �ړ����̏���
	if (m_Action == PLAYER_ACTION_WALK)
	{
		//�ړ�
		m_vPos.y += m_fMoveSpeedY;
		m_vPos.x += m_fMoveSpeedX;

		// �����ݒ�
		m_vRot.z = ((180.0f * atan2f(m_vPos.y - m_vPosOld.y, m_vPos.x - m_vPosOld.x)) / D3DX_PI);

		// �p�x�̐��K��
		NormalizeRotation(&m_vRot.z);
	}
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void CPlayer::Draw(void)
{
	CScene2D::SetPos(m_vPos);
	CScene2D::SetRot(m_vRot);
	CScene2D::Draw();
}

// EOF