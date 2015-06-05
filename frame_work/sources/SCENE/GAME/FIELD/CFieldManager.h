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
	typedef enum
	{
		FIELD_TEST = 0,
		FIELD_MAX
	}FIELD_TYPE;


	// �R���X�g���N�^
	CFieldManager(void);

	// �f�X�g���N�^
	~CFieldManager(void);

	// �t�B�[���h�N���G�C�g
	CField* CreateField(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType);

	// ���[�h�t�B�[���h
	void LoadField(LPDIRECT3DDEVICE9 *pDevice, FIELD_TYPE fieldType);

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