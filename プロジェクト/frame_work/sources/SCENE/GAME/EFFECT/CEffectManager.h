//=============================================================================
//
// CEffectManager�N���X [CEffectManager.h]
// Author : �˖{�@�r�F
//
// �󕨃}�l�[�W���[�@��������ĕ󕨂𐶐�����
//
//=============================================================================
#ifndef _CEFFECTMANAGER_H_
#define _CEFFECTMANAGER_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../RENDERER/CRenderer.h"

//=============================================================================
// �萔
//=============================================================================
enum EFFECT_TYPE{
	EFFECT_ATTACK_HIT = 0,
	EFFECT_EXPLOSION,
	EFFECT_SPECIAL_ATTACK_ATTACK,
	EFFECT_SPECIAL_ATTACK_SPEED,
	EFFECT_SPECIAL_THREAD_ATTACK,
	EFFECT_MAX,
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEffectManager
{
	// ���J�����o
public:
	CEffectManager(LPDIRECT3DDEVICE9 *pDevice);
	~CEffectManager(void);

	void Init(void);
	void Uninit(void);
	void Update(void);

	// �G�t�F�N�g�����֐�
	// ���W�ƃ^�C�v��ݒ�
	void CreateEffect(EFFECT_TYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 velocity);

	// ����J�����o
private:

	// �f�o�C�X
	LPDIRECT3DDEVICE9 *m_pDevice;
};

#endif
//----EOF----