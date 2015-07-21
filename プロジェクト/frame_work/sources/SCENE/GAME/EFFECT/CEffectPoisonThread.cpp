//=============================================================================
//
//	�ړ�������Ԃ̍U���G�t�F�N�g
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#include "CEffectPoisonThread.h"

#include "../../CSCENE/CSceneAnime.h"

// �f�o�b�O���Ă݂悤
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// �萔
//*****************************************************************************
static const int POISON_THREAD_COUNT_MAX = 70;
static const float POISON_THREAD_WIDTH = 180;
static const float POISON_THREAD_HEIGHT = 180;
static const TEXTURE_TYPE POISON_THREAD_TEXTURE = TEXTURE_THREAD_POISON;
static const int POISON_THREAD_TEXTURE_X = 2;
static const int POISON_THREAD_TEXTURE_Y = 4;
static const int POISON_THREAD_TEXTURE_LOOP = 70;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEffectPoisonThread::CEffectPoisonThread(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
	m_pAnim = NULL;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CEffectPoisonThread ::~CEffectPoisonThread(void)
{

}

//*****************************************************************************
// ������
//*****************************************************************************
void CEffectPoisonThread::Init(D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	m_nCount = 0;
	m_nCountMax = POISON_THREAD_COUNT_MAX;
	m_vPos = pos;
	m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vVelocity = velocity;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CEffectPoisonThread::Uninit(void)
{
	m_pAnim = NULL;
	CEffectBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CEffectPoisonThread::Update(void)
{
	if (m_nCount == 0){
		m_pAnim = CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			POISON_THREAD_WIDTH, POISON_THREAD_HEIGHT,
			POISON_THREAD_TEXTURE, POISON_THREAD_TEXTURE_X, POISON_THREAD_TEXTURE_Y,
			POISON_THREAD_TEXTURE_LOOP);

		if (m_vVelocity.y == -1)
		{
			m_vRot.z = D3DX_PI*0.5f;
			m_pAnim->SetRot(m_vRot);
		}
		if (m_vVelocity.y == 1)
		{
			m_vRot.z = D3DX_PI*1.5f;
			m_pAnim->SetRot(m_vRot);
		}
		if (m_vVelocity.x == 1)
		{
			m_vRot.z = 0.0f;
		}
		if (m_vVelocity.x == -1)
		{
			m_vRot.z = D3DX_PI;
			m_pAnim->SetRot(m_vRot);
		}
	}

	m_vPos += m_vVelocity * 100.0f;

	//m_pAnim->SetPos(m_vPos);

	// ���E�̍X�V
	CEffectBase::Update();
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CEffectPoisonThread* CEffectPoisonThread::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	// �쐬
	CEffectPoisonThread* p = new CEffectPoisonThread(pDevice);

	p->Init(pos, velocity);

	return p;
}

//EOF