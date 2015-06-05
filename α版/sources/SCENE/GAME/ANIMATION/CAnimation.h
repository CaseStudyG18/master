//=============================================================================
//
// �A�j���[�V�����N���X [CAnimation.h]
// Author : �˖{�@�r�F
//
//=============================================================================
#ifndef _CANIMATION_H_
#define _CANIMATION_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../MAIN/main.h"
#include "../../CSCENE/CScene.h"
#include "../../CSCENE/CScene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CAnimation : public CScene2D
{
public:
	//======================================================
	// �R���X�g���N�^
	// ����: �f�o�C�X�A�v���C�I���e�B
	//======================================================
	CAnimation(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE m_objType = OBJTYPE_2D);

	// �f�X�g���N�^
	~CAnimation();

	//======================================================
	// ������
	// ����: ���W�A���A�����A�e�N�X�`���[�^�C�v�A�e�N�X�`�����������A�e�N�X�`���c������
	// �߂�l: ����
	//======================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX,int sepalateY);
	
	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);
	
	//======================================================
	// �A�j���[�V�����ԍ��ݒ�
	// ����: �A�j���[�V�����ԍ�,���]���邩
	//======================================================
	void SetIndex(int nIdx);
	void SetIndex(int nIdx, bool reverse);

	//======================================================
	// �`�悷�邩
	// ����: �t���O
	//======================================================
	void SetDrawFlag(bool flag){m_bDraw = flag;};

	//======================================================
	// �N���G�C�g�֐�
	// ����: �f�o�C�X�A���W�A���A�����A�e�N�X�`���[�^�C�v�A�e�N�X�`�����������A�e�N�X�`���c������
	// �߂�l: �������̃A�h���X
	//======================================================
	static CAnimation* Create(LPDIRECT3DDEVICE9 *pDevice, int nPriority, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX, int sepalateY);

protected:
	// ���[�v�Ȃ��X�V
	void Update_Normal(void);

	// ���[�v����X�V
	void Update_Loop(void);
	
	LPDIRECT3DDEVICE9	*m_pD3DDevice;			// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	float				m_fLeft;				// UV�l(��)
	float				m_fRight;				// UV�l(�E)
	float				m_fTop;					// UV�l(��)
	float				m_fBottom;				// UV�l(��)
	int					m_nWidth;				// ����
	int					m_nHeight;				// �c��
	int					m_nSepalateNumX;		// �摜������(��)�E�E�Eex:���ɂT����ł�Ȃ�T
	int					m_nSepalateNumY;		// �摜������(�c)
	int					m_nIdx;					// �����ԍ�
	int					m_nMaxIdx;				// idx�̍ő�l
	bool				m_bDraw;				// �`�悷�邩
};
#endif
//----EOF----