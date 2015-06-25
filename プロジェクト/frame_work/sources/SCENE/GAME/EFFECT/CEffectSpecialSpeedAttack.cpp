//=============================================================================
//
//	�ړ�������Ԃ̍U���G�t�F�N�g
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#include "CEffectSpecialSpeedAttack.h"

#include "../../CSCENE/CSceneAnime.h"

// �f�o�b�O���Ă݂悤
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// �萔
//*****************************************************************************
static const int SPEED_ATTACK_COUNT_MAX = 100;
static const float SPEED_ATTACK_WIDTH = 50;
static const float SPEED_ATTACK_HEIGHT = 50;
static const TEXTURE_TYPE SPEED_ATTACK_TEXTURE = TEXTURE_FIRE_1;
static const int SPEED_ATTACK_TEXTURE_X = 10;
static const int SPEED_ATTACK_TEXTURE_Y = 1;
static const int SPEED_ATTACK_TEXTURE_LOOP = 100;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEffectSpecialSpeedAttack::CEffectSpecialSpeedAttack(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
	m_pAnim = NULL;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CEffectSpecialSpeedAttack ::~CEffectSpecialSpeedAttack(void)
{

}

//*****************************************************************************
// ������
//*****************************************************************************
void CEffectSpecialSpeedAttack::Init(D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	m_nCount = 0;
	m_nCountMax = SPEED_ATTACK_COUNT_MAX;
	m_vPos = pos;
	m_vRot.z = 0.0f;
	m_vVelocity = velocity;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CEffectSpecialSpeedAttack::Uninit(void)
{
	m_pAnim = NULL;
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CEffectSpecialSpeedAttack::Update(void)
{
	if (m_nCount == 0){
		m_pAnim = CSceneAnime::Create(
					m_pD3DDevice,
					m_vPos,
					SPEED_ATTACK_WIDTH, SPEED_ATTACK_HEIGHT,
					SPEED_ATTACK_TEXTURE, SPEED_ATTACK_TEXTURE_X, SPEED_ATTACK_TEXTURE_Y,
					SPEED_ATTACK_TEXTURE_LOOP);
	}

	m_vPos += m_vVelocity * 1.0f;

	m_pAnim->SetPos(m_vPos);

	// ���E�̍X�V
	CEffectBase::Update();

#ifdef _DEBUG
	CDebugProc::Print("�ړ�������Ԃ̍U���̃G�t�F�N�g�Ȃ�\n");
#endif
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CEffectSpecialSpeedAttack* CEffectSpecialSpeedAttack::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	// �쐬
	CEffectSpecialSpeedAttack* p = new CEffectSpecialSpeedAttack(pDevice);

	p->Init(pos, velocity);

	return p;
}

//EOF