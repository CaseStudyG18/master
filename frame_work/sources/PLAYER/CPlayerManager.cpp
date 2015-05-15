//=============================================================================
//
// CPlayerManager�N���X [CPlayerManager.cpp]
// Author : �����@�Ȉ�
//
//=============================================================================
#include "CPlayerManager.h"

#include "CPlayer.h"
#include "../RENDERER/CRenderer.h"
#include "../TEXTURE/CTexture.h"

#ifdef _DEBUG
#define PLAYER_TEXTURE TEXTURE_PLAYER	// �f�o�b�O���̃v���C���[�̃e�N�X�`��
#else
#define PLAYER_TEXTURE TEXTURE_NULL		// 
#endif

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CPlayerManager::CPlayerManager()
{
	for (int i = 0; i < MAXIMUM_NUMBER_OF_PLAYER; i++)
	{
		m_apPlayer[i] = NULL;
	}
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CPlayerManager::~CPlayerManager()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
void CPlayerManager::Init(int nNumPlayer)
{
	for (int i = 0; i < nNumPlayer; i++)
	{
		m_apPlayer[i] = CPlayer::Create(CRenderer::GetDevice(), D3DXVECTOR3(50.0f, 20.0f, 0), 50.0f, 50.0f, PLAYER_TEXTURE);
	}
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void CPlayerManager::Update(void)
{
}

//-----------------------------------------------------------------------------
// �I��
//-----------------------------------------------------------------------------
void CPlayerManager::Uninit(void)
{
	for (int i = 0; i < MAXIMUM_NUMBER_OF_PLAYER; i++)
	{
		m_apPlayer[i] = NULL;
	}
}

// EOF