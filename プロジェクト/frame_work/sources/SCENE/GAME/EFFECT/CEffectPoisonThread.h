//=============================================================================
//
//	�W�Q�`�Ԃ̍U���G�t�F�N�g
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CEFFECT_POISON_THREAD_H__
#define __CEFFECT_POISON_THREAD_H__

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../MAIN/main.h"
#include "../../GAME/EFFECT/CEffectBase.h"

//=============================================================================
// �萔
//=============================================================================

//=============================================================================
//	�O���錾
//=============================================================================
class CSceneAnime;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEffectPoisonThread : public CEffectBase
{
	// ���J�����o
public:
	CEffectPoisonThread(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectPoisonThread(void);

	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 velocity);
	void Uninit(void);
	void Update(void);

	// �N���G�C�g�֐�
	static CEffectPoisonThread *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 velocity);

	// ����J�����o
private:
	CSceneAnime *m_pAnim;
};

#endif