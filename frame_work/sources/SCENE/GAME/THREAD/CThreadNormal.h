//=============================================================================
//
// CThreadNormal�N���X [CThreadNoaml.h]
// Author : �˖{�@�r�F
//
// ���ʎ�
//
//=============================================================================
#ifndef _CTHREADNORMAL_H_
#define _CTHREADNORMAL_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "CThreadBase.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CThreadNormal : public CThreadBase
{
	// ���J�����o
public:

	CThreadNormal(LPDIRECT3DDEVICE9 *pDevice);
	~CThreadNormal(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//=======================================================================
	// �N���G�C�g�֐�
	// ����:�f�o�C�X�A�v���C���ԍ��A���W
	// �߂�l:�쐬�����A�h���X
	//=======================================================================
	static CThreadNormal *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		short nPlayerNum,
		D3DXVECTOR3 pos);

	// ����J�����o
private:

};

#endif
//----EOF----