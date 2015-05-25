//=============================================================================
//
// CSceneCrinder�N���X [CSceneCrinder.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CSCENECRINDER_H_
#define _CSCENECRINDER_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../MAIN/main.h"
#include "CScene.h"

//=============================================================================
// �}�N��
//=============================================================================
static const D3DXVECTOR3	DEFAULT_CRINDER_POS(0.f, -50.f,0.f);	// �f�t�H���g�̍��W
static const float			DEFAULT_CRINDER_RADIUS = 1900.f;		// �f�t�H���g�̔��a
static const float			DEFAULT_CRINDER_HEIGHT = 150.f;			// �f�t�H���g�̍���
static const int			DEFAULT_CRINDER_SEPLATE_NUM = 20;		// �f�t�H���g�̕�����

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSceneCrinder : public CScene
{
public:
	//======================================================
	// �R���X�g���N�^
	// ����: �f�o�C�X�A�v���C�I���e�B�A�I�u�W�F�^�C�v
	//======================================================
	CSceneCrinder(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_3);

	// �f�X�g���N�^
	~CSceneCrinder();

	//======================================================
	// ������
	// ����: ���W�A���A�����A�e�N�X�`���[�^�C�v�A�������Ƀe�N�X�`�����͂邩�A(���������A�c������)
	// �߂�l: ����
	//======================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, bool In, int wblock = 1, int hblock = 1);
	
	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);

	// �|�W�V�����A�N�Z�T
	D3DXVECTOR3 GetPos(void){ return m_vPos;};
	void		SetPos(D3DXVECTOR3 pos){ m_vPos = pos;};
	void		SetPos(float x,float y, float z){ m_vPos.x = x; m_vPos.y = y; m_vPos.z = z;};

	// ���b�g�A�N�Z�T
	D3DXVECTOR3 GetRot(void){return m_vRot;};
	void		SetRot(D3DXVECTOR3 rot){m_vRot = rot;};
	void		SetRot(float x,float y,float z){ m_vRot.x = x; m_vRot.y = y; m_vRot.z = z;};

	//=================================================
	// �N���G�C�g�֐�
	// ����: �f�o�C�X�A���W�A���a�A�����A���������A�c�������A�e�N�X�`���[�^�C�v�A�������Ƀe�N�X�`�����͂邩�A�p�x
	// �߂�l: �������̃A�h���X
	//=================================================
	static CSceneCrinder* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float r, float height, int wblock, int hblock, TEXTURE_TYPE texType, bool In, D3DXVECTOR3 rot = D3DXVECTOR3(0,0,0));

private:
	// �������@�g��Ȃ�
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1);
	
	// ���_�o�^�֐�
	void SetVertexPolygon(void);

	// �F�ύX�֐�
	void SetColorPolygon(D3DXCOLOR color);

	LPDIRECT3DDEVICE9		*m_pD3DDevice;			// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;			// ���_���W�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	LPDIRECT3DINDEXBUFFER9	m_pD3DIndexBuff;		// �C���f�b�N�X�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;			// �e�N�X�`�������i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	D3DXVECTOR3				m_vPos;					// �|���S���̒����̈ʒu
	D3DXVECTOR3				m_vRot;					// �|���S���̉�]
	D3DXVECTOR3				m_vScl;					// �傫���i�X�P�[���j
	float					m_fAngle;				// �|���S���̑Ίp���̊p�x
	float					m_fRadius;				// ���a
	float					m_fHeight;				// ����
	int						m_nNumPolygon;			// �|���S����
	int						m_nNumVertexIndex;		// �C���f�b�N�X��
	int						m_nNumVtxNum;			// ���_��
	int						m_nNumWidthBlock;		// ���̕�����
	int						m_nNumHeightBlock;		// �c�̕�����
	D3DXMATRIX				m_mtxWorld;				// ���[���h�}�g���b�N�X
};
#endif
//----EOF----