//=============================================================================
//
// CTreasureManager�N���X [CTreasureManager.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CTreasure.h"


//*****************************************************************************
// �}�N��
//*****************************************************************************
// �����2D�֘A
static const float TREASURE_WIDTH = 70;
static const float TREASURE_HEIGHT = 70;
static const TEXTURE_TYPE TREASURE_TEXTURE = TEXTURE_TREASURE;
static const int TREASURE_TEXTURE_ANIME_SPEED = 5;
static const int TREASURE_TEXTURE_X = 3;
static const int TREASURE_TEXTURE_Y = 1;

// ����̏����ԍ��i�N�������Ă��Ȃ��Ƃ���-1�j
static const short OWNER_NONE = -1;

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTreasure::CTreasure(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE m_objType) :CEffect(pDevice, nPriority, m_objType)
{
	// ����̏�ԏ�����
	m_TreasureState = TREASURE_STATE_OWNER_NONE;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTreasure ::~CTreasure(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CTreasure::Init(D3DXVECTOR3 pos)
{
	// ������
	CEffect::Init(pos, TREASURE_WIDTH, TREASURE_HEIGHT, TREASURE_TEXTURE,
		TREASURE_TEXTURE_X, TREASURE_TEXTURE_Y, TREASURE_TEXTURE_ANIME_SPEED, -1);

	SetAutoUpdate(true);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CTreasure::Uninit(void)
{
	CScene2D::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CTreasure::Update(void)
{
	CEffect::Update();
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CTreasure* CTreasure::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// �쐬
	CTreasure* p = new CTreasure(pDevice);

	p->Init(pos);

	return p;
}

//*****************************************************************************
// �傫����e�N�X�`�����ŏ��̏�Ԃɖ߂�
// �󕨂��v���C�������Ƃ����ۂɌĂ΂��
//*****************************************************************************
void CTreasure::Reset(D3DXVECTOR3 pos){

	// �e�N�X�`��
	CScene2D::ChangeTexture(TREASURE_TEXTURE);
	// �e�N�X�`���̑傫�����Z�b�g
	CEffect::Init(pos, TREASURE_WIDTH, TREASURE_HEIGHT, TREASURE_TEXTURE,
		TREASURE_TEXTURE_X, TREASURE_TEXTURE_Y, TREASURE_TEXTURE_ANIME_SPEED, 1);
	// �傫��
	CScene2D::SetWidth(TREASURE_WIDTH);
	CScene2D::SetHeight(TREASURE_HEIGHT);
	// ���W(��)
	// ����̓A�j���[�V����������
	CScene2D::SetPos(pos + D3DXVECTOR3(100, 100, 0));

	m_TreasureState = TREASURE_STATE_OWNER_NONE;
}
//----EOF-------