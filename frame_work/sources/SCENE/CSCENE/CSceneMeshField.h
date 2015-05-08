//=============================================================================
//
// CSceneMeshField�N���X [CSceneMeshField.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CSCENEMESHFIELD_H_
#define _CSCENEMESHFIELD_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../MAIN/main.h"
#include "CScene.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const int SCENE_MAX_FIELD = 1;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSceneMeshField : public CScene
{
public:
	//======================================================
	// �R���X�g���N�^
	// ����: �f�o�C�X�A�v���C�I���e�B�A�I�u�W�F�^�C�v
	//======================================================
	CSceneMeshField(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_3, OBJTYPE m_objType = OBJTYPE_FIELD);

	// �f�X�g���N�^
	~CSceneMeshField();

	//======================================================
	// ������
	// ����: ���W�A���A�����A�e�N�X�`���[�t�@�C���l�[���A(���������A�c������)
	// �߂�l: ����
	//======================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, char* texFileName, int wblock = 1, int hblock = 1, bool useMap = true);
	
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
	// �����擾
	// ����: �m�肽���ꏊ�A(out)�@��
	// �߂�l: ����
	//=================================================
	float GetHeight(const D3DXVECTOR3& pos, D3DXVECTOR3* pNormal);

	//=================================================
	// �T�C�Y�Q�b�g
	// ����: (out)�w�T�C�Y�A(out)�y�T�C�Y
	//=================================================
	void GetSize(float* pOutXSize, float* pOutZSize){*pOutXSize = m_fWidth; *pOutZSize = m_fHeight;};

	//=================================================
	// �p�̍��W�Q�b�g
	// ����: �p�̔ԍ�
	// �߂�l: �p�̍��W
	//=================================================
	D3DXVECTOR3 GetCornerPos(int idx){ return m_vCornerPos[idx];};

	//=================================================
	// �N���G�C�g�֐�
	// ����: �f�o�C�X�A�t�B�[���h�t�@�C���p�X
	// �߂�l: �������̃|�C���^
	//=================================================
	static CSceneMeshField* Create(LPDIRECT3DDEVICE9 *pDevice, char* fieldFileName);
	
	//=================================================
	// �A�h���X�擾
	// ����: �t�B�[���h�ԍ�
	// �߂�l: �A�h���X
	//=================================================
	static CSceneMeshField* GetAddress(int id){return m_apMeshField[id];};

private:
	// �g���ĂȂ�������
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1){return S_OK;};
	
	// ���_�Z�b�g
	void SetVertexPolygon(void);

	//=================================================
	// �F�ύX�֐�
	// ����: �F
	//=================================================
	void SetColorPolygon(D3DXCOLOR color);

	//=================================================
	// �|���S���̍����擾
	// ����: ���_���W0�A���_���W1�A���_���W2�A���ׂ������W�A�iout�j�@���|�C���^
	// �߂�l: ����
	//=================================================
	float GetHeightPolygon(const D3DXVECTOR3& p0, const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, const D3DXVECTOR3& pos, D3DXVECTOR3* pNormal);

	LPDIRECT3DDEVICE9		*m_pD3DDevice;						// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;						// ���_���W�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	LPDIRECT3DINDEXBUFFER9	m_pD3DIndexBuff;					// �C���f�b�N�X�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;						// �e�N�X�`�������i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	D3DXVECTOR3				m_vPos;								// �|���S���̒����̈ʒu
	D3DXVECTOR3				m_vRot;								// �|���S���̉�]
	D3DXVECTOR3				m_vScl;								// �n�ʂ̑傫���i�X�P�[���j
	D3DXVECTOR3				m_vCornerPos[4];					// �p�̍��W
	D3DXVECTOR3*			m_vFaceNormalBuf;					// �ʖ@���o�b�t�@
	D3DXVECTOR3*			m_vVtxNormalBuf;					// ���_�@���o�b�t�@
	float					m_fAngle;							// �|���S���̑Ίp���̊p�x
	float					m_fWidth;							// ��
	float					m_fHeight;							// ����
	float					m_fGridWidth;						// �O���b�h�̕�
	float					m_fGridHeight;						// �O���b�h�̍���
	int						m_nNumPolygon;						// �|���S����
	int						m_nNumVertexIndex;					// �C���f�b�N�X��
	int						m_nNumVtxNum;						// ���_��
	int						m_nNumWidthBlock;					// ���̕�����
	int						m_nNumHeightBlock;					// �c�̕�����
	int						m_nID;								// ID
	static D3DXMATRIX		m_mtxWorld;							// ���[���h�}�g���b�N�X
	static CSceneMeshField	*m_apMeshField[SCENE_MAX_FIELD];	// �t�B�[���h�o�b�t�@
	static int				m_nNum;								// ����
};
#endif
//----EOF----