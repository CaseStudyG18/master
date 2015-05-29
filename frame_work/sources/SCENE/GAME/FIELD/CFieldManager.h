//=============================================================================
//
// CFieldManager�N���X [CFieldManager.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CFIELDMANAGER_H_
#define _CFIELDMANAGER_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../MAIN/main.h"
#include "../../../TEXTURE/CTexture.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CField;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CFieldManager
{
public:
	// �R���X�g���N�^
	CFieldManager(void);

	// �f�X�g���N�^
	~CFieldManager(void);

	// �t�B�[���h�N���G�C�g
	CField* CreateField(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType);

	// ������
	void Init(void);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

private:

};

#endif
//----EOF----