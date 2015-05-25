//=============================================================================
//
// CSceneMeshWall�N���X [CSceneMeshWall.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CSCENEMESHWALL_H_
#define _CSCENEMESHWALL_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../MAIN/main.h"
#include "CScene.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const int SCENE_MAX_WALL = 4;				// �ǂ̍ő吔
static const int DEFAULT_WALL_SEPALATE_NUM = 5;		// �ǂ̃f�t�H���g������
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSceneMeshWall : public CScene
{
public:
	//======================================================
	// �R���X�g���N�^
	// ����: �f�o�C�X�A�v���C�I���e�B�A�I�u�W�F�^�C�v
	//======================================================
	CSceneMeshWall(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_3);

	// �f�X�g���N�^
	~CSceneMeshWall();

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
	void		SetPos(D3DXVECTOR3 pos){ m_vPos = pos;};
	void		SetPos(float x,float y, float z){ m_vPos.x = x; m_vPos.y = y; m_vPos.z = z;};
	
	// ���b�g�A�N�Z�T
	D3DXVECTOR3 GetRot(void){return m_vRot;};
	void		SetRot(D3DXVECTOR3 rot){m_vRot = rot;};
	void		SetRot(float x,float y,float z){ m_vRot.x = x; m_vRot.y = y; m_vRot.z = z;};
	
	//======================================================
	// �p�̍��W�Q�b�g
	// ����: �p�̔ԍ�
	// �߂�l: ���W
	//======================================================
	D3DXVECTOR3 GetCornerPos(int idx){ return m_vCornerPos[idx];};


	//=================================================
	// �N���G�C�g�֐�
	// ����: �f�o�C�X�A���W�A���A�����A���������A�c�������A�e�N�X�`���^�C�v�A�p�x
	// �߂�l: �������̃A�h���X
	//=================================================
	static CSceneMeshWall* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, int wblock, int hblock, TEXTURE_TYPE texType, D3DXVECTOR3 rot = D3DXVECTOR3(0,0,0));

	//=================================================
	// �A�h���X�擾
	// ����: �ǂ̔ԍ�
	// �߂�l: �A�h���X
	//=================================================
	static CSceneMeshWall* GetAddress(int id){return m_apMeshWall[id];};

private:
	// ���_���f�֐�
	void SetVertexPolygon(void);

	//=================================================
	// �F�ύX�֐�
	// ����: �F
	//=================================================
	void SetColorPolygon(D3DXCOLOR color);

	LPDIRECT3DDEVICE9		*m_pD3DDevice;					// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;					// ���_���W�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	LPDIRECT3DINDEXBUFFER9	m_pD3DIndexBuff;				// �C���f�b�N�X�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;					// �e�N�X�`�������i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	D3DXVECTOR3				m_vPos;							// �|���S���̒����̈ʒu
	D3DXVECTOR3				m_vRot;							// �|���S���̉�]
	D3DXVECTOR3				m_vScl;							// �n�ʂ̑傫���i�X�P�[���j
	D3DXVECTOR3				m_vCornerPos[4];				// �p�̍��W  �l�p�`�̊p�Ȃ̂�4��
	float					m_fAngle;						// �|���S���̑Ίp���̊p�x
	float					m_fWidth;						// ��
	float					m_fHeight;						// ����
	int						m_nNumPolygon;					// �|���S����
	int						m_nNumVertexIndex;				// �C���f�b�N�X��
	int						m_nNumVtxNum;					// ���_��
	int						m_nNumWidthBlock;				// ���̕�����
	int						m_nNumHeightBlock;				// �c�̕�����
	int						m_nID;							// ID
	static D3DXMATRIX		m_mtxWorld;						// ���[���h�}�g���b�N�X
	static CSceneMeshWall	*m_apMeshWall[SCENE_MAX_WALL];	// �ǂ̃o�b�t�@
	static int				m_nNum;							// ����
};
#endif
//----EOF----