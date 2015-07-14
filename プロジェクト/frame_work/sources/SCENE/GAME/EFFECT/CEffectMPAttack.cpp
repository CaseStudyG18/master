//=============================================================================
//
//	�����G�t�F�N�g
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#include "CEffectMPAttack.h"

#include "../../CSCENE/CSceneAnime.h"

// �f�o�b�O���Ă݂悤
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// �萔
//*****************************************************************************
static const int MP_ATTACK_COUNT_MAX = 1;
static const float MP_ATTACK_WIDTH = 100;
static const float MP_ATTACK_HEIGHT = 100;
static const TEXTURE_TYPE MP_ATTACK_TEXTURE = TEXTURE_TREASURE_KIRAKIRA;
static const int MP_ATTACK_TEXTURE_X = 8;
static const int MP_ATTACK_TEXTURE_Y = 1;
static const int MP_ATTACK_TEXTURE_LOOP = 100;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEffectMPAttack::CEffectMPAttack(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CEffectMPAttack ::~CEffectMPAttack(void)
{

}

//*****************************************************************************
// ������
//*****************************************************************************
void CEffectMPAttack::Init(D3DXVECTOR3 pos)
{
	m_nCount = 0;
	m_nCountMax = MP_ATTACK_COUNT_MAX;
	m_vPos = pos;
	m_vPos.y = pos.y - 30.0f;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CEffectMPAttack::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CEffectMPAttack::Update(void)
{
	if (m_nCount == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			MP_ATTACK_WIDTH, MP_ATTACK_HEIGHT,
			MP_ATTACK_TEXTURE, MP_ATTACK_TEXTURE_X, MP_ATTACK_TEXTURE_Y,
			MP_ATTACK_TEXTURE_LOOP);
	}

	// ���E�̍X�V
	CEffectBase::Update();

#ifdef _DEBUG
	CDebugProc::Print("���쐬�G�t�F�N�g�Ȃ�\n");
#endif
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CEffectMPAttack* CEffectMPAttack::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// �쐬
	CEffectMPAttack* p = new CEffectMPAttack(pDevice);

	p->Init(pos);

	return p;
}

//EOF