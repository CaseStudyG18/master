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
// �e�N�X�`���֘A
static const TEXTURE_TYPE TREASURE_TEXTURE = TEXTURE_TREASURE;
static const int TREASURE_TEXTURE_X = 13;
static const int TREASURE_TEXTURE_Y = 1;
// ����Q�c
static const float TREASURE_WIDTH = 70;
static const float TREASURE_HEIGHT = 70;
static const int TREASURE_ANIME_SPEED = 5;
// ����A�C�R���Q�c
static const float TREASURE_ICON_WIDTH = 40;
static const float TREASURE_ICON_HEIGHT = 40;
static const int TREASURE_ICON_ANIME_SPEED = 3;

// ����̏����ԍ��i�N�������Ă��Ȃ��Ƃ���-1�j
static const short OWNER_NONE = -1;

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTreasure::CTreasure(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE m_objType) :CSceneAnime(pDevice, nPriority, m_objType)
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
	CSceneAnime::Init(pos, TREASURE_WIDTH, TREASURE_HEIGHT, TREASURE_TEXTURE,
		TREASURE_TEXTURE_X, TREASURE_TEXTURE_Y, TREASURE_ANIME_SPEED, -1);

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
	CSceneAnime::Update();
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

	// �G�̕ύX
//	CScene2D::ChangeTexture(TREASURE_TEXTURE);
	CScene2D::SetWidth(TREASURE_WIDTH);
	CScene2D::SetHeight(TREASURE_HEIGHT);
	SetAnimeSpeed(TREASURE_ANIME_SPEED);

	// ���Ƃ�������
	CScene2D::SetPos(pos + D3DXVECTOR3(100, 100, 0));

	m_TreasureState = TREASURE_STATE_OWNER_NONE;
}

//*****************************************************************************
// �傫����e�N�X�`�����ŏ��̏�Ԃɖ߂�
// �󕨂��v���C�������Ƃ����ۂɌĂ΂��
//*****************************************************************************
void CTreasure::SetIcon(){

	// �G�̕ύX
//	CScene2D::ChangeTexture(TEXTURE_TREASURE_ICON);
	CScene2D::SetWidth(TREASURE_ICON_WIDTH);
	CScene2D::SetHeight(TREASURE_ICON_HEIGHT);
	SetAnimeSpeed(TREASURE_ICON_ANIME_SPEED);
}

//----EOF-------