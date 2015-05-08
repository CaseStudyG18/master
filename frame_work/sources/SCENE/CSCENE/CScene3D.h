//=============================================================================
//
// CScene3D�N���X [CScene3D.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CSCENE3D_H_
#define _CSCENE3D_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../MAIN/main.h"
#include "CScene.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScene3D : public CScene
{
public:
	//======================================================
	// �R���X�g���N�^
	// ����: �f�o�C�X�A�v���C�I���e�B�A�I�u�W�F�^�C�v
	//======================================================
	CScene3D(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_3, OBJTYPE m_objType = OBJTYPE_3D);

	// �f�X�g���N�^
	~CScene3D();

	//======================================================
	// ������
	// ����: ���W�A���A�����A�e�N�X�`���[�^�C�v�A(���������A�c������)
	// �߂�l: ����
	//======================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);
	
	// �|�W�V�����A�N�Z�T
	D3DXVECTOR3 GetPos(void){ return m_vPos;};
	void SetPos(D3DXVECTOR3 pos){ m_vPos = pos;};
	void SetPos(float x,float y, float z){ m_vPos.x = x; m_vPos.y = y; m_vPos.z = z;};
	
	// ���b�g�A�N�Z�T
	D3DXVECTOR3 GetRot(void){return m_vRot;};
	void SetRot(D3DXVECTOR3 rot){m_vRot = rot;};
	void SetRot(float x,float y,float z){ m_vRot.x = x; m_vRot.y = y; m_vRot.z = z;};
	
	//=================================================
	// �����擾
	// ����: �m�肽���ꏊ�A(out)�@��
	// �߂�l: ����
	//=================================================
	float GetHeight(const D3DXVECTOR3& pos, D3DXVECTOR3* pNormal);

	//=================================================
	// �N���G�C�g�֐�
	// ����: �f�o�C�X�A���W�A���A�����A�e�N�X�`���^�C�v
	// �߂�l: �������̃A�h���X
	//=================================================
	static CScene3D* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType);

private:
	// ���_���f�֐�
	void SetVertexPolygon(void);

	//=================================================
	// �F�ύX�֐�
	// ����: �F
	//=================================================
	void SetColorPolygon(D3DXCOLOR color);
	
	LPDIRECT3DDEVICE9		*m_pD3DDevice;			// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;			// ���_���W�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;			// �e�N�X�`�������i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	D3DXVECTOR3				m_vScl;					// �n�ʂ̑傫���i�X�P�[���j
	static D3DXMATRIX		m_mtxWorld;				// ���[���h�}�g���b�N�X

protected:
	D3DXVECTOR3				m_vPos;					// �|���S���̒����̈ʒu
	D3DXVECTOR3				m_vRot;					// �|���S���̉�]
	float					m_fWidth;				// �|���S���̑Ίp���̒���
	float					m_fHeight;				// �|���S���̑Ίp���̊p�x
};
#endif
//----EOF----