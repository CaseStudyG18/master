//=============================================================================
//
// CInputMouse�N���X [CInputMouse.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CINPUTMOUSE_H_
#define _CINPUTMOUSE_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"
#include "CInput.h"

//*****************************************************************************
//�\����
//*****************************************************************************
// �}�E�X�̏��\����
typedef struct 
{
	RECT	moveRect;		// ��ʏ�œ�����͈�
	float	x;				// X���W
	float	y;				// Y���W
	bool	lButton;		// ���{�^��
	bool	rButton;		// �E�{�^��
	bool	cButton;		// �^�񒆃{�^��
	int		moveAdd;		// �ړ���
	RECT	imgRect;		// �}�E�X�p�摜��`
	int		imgWidth;		// �}�E�X�摜��
	int		imgHeight;		// �}�E�X�摜����
} MSTATE;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CInputMouse : CInput
{
public:
	// �R���X�g���N�^
	CInputMouse(void);

	// �f�X�g���N�^
	~CInputMouse(void);

	//============================================
	// ������
	// ����: �C���X�^���X�A�E�C���h�E�n���h��
	// �߂�l: ����
	//============================================
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	//============================================
	// �}�E�X�̏��擾
	// �߂�l: �}�E�X���̃|�C���^
	//============================================
	static MSTATE* GetMouseState(void){return &m_MState;};

private:
	static MSTATE			m_MState;			// �}�E�X���\����
	static HANDLE			m_hevtMouse;		// �n���h��
	static DIMOUSESTATE2	m_diMouseState;		// �}�E�X�̏��
	static POINT			m_Point;			// ���W
	static HWND				m_hWnd;				// �E�C���h�E�n���h��

};

#endif
//----EOF----