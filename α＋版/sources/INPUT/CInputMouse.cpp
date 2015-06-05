//=============================================================================
//
// CInputMouse�N���X [CInputMouse.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CInputMouse.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const RECT	RECT_MOV_MOUSE = {10, 10, 500, 700};		// �`��͈�
static const int	DEFAULT_MOV_SPD = 2;						// �}�E�X�̃f�t�H���g�ړ����x
static const RECT	RECT_IMAGE_MOUSE = {400, 0, 420, 20};		// �}�E�X�̉摜�͈�

//*****************************************************************************
// �X�^�e�B�b�N�����o�ϐ�
//*****************************************************************************
MSTATE			CInputMouse::m_MState;
HANDLE			CInputMouse::m_hevtMouse;
DIMOUSESTATE2	CInputMouse::m_diMouseState;
POINT			CInputMouse::m_Point;
HWND			CInputMouse::m_hWnd;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CInputMouse::CInputMouse(void)
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CInputMouse::~CInputMouse(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �e�N���X�̏�����
	CInput ::Init(hInstance, hWnd);

	m_hWnd = hWnd;
	HRESULT hr;
	//------------------------------------------------------- 
	// �}�E�X�|�C���^���̏�����
	// �}�E�X�J�[�\���̓����͈�
	SetRect(&m_MState.moveRect
			, RECT_MOV_MOUSE.left
			, RECT_MOV_MOUSE.top
			, RECT_MOV_MOUSE.right
			, RECT_MOV_MOUSE.bottom);

	// �}�E�X�J�[�\���̍��W��������
	m_MState.x = (float)m_MState.moveRect.left;
	m_MState.y = (float)m_MState.moveRect.top;
	m_diMouseState.lX = m_MState.moveRect.left;
	m_diMouseState.lY = m_MState.moveRect.top;

	// ���{�^���̏���������
	m_MState.lButton = false;

	// �E�{�^���̏���������
	m_MState.rButton = false;

	// �����{�^���̏���������
	m_MState.cButton = false;

	// �}�E�X�J�[�\���̈ړ��ʂ�ݒ�
	m_MState.moveAdd = DEFAULT_MOV_SPD;

	// �}�E�X�J�[�\���摜�̋�`��ݒ�
	SetRect(&m_MState.imgRect
			, RECT_IMAGE_MOUSE.left
			, RECT_IMAGE_MOUSE.top
			, RECT_IMAGE_MOUSE.right
			, RECT_IMAGE_MOUSE.bottom);

	// �摜�̕����v�Z
	m_MState.imgWidth = m_MState.imgRect.right - m_MState.imgRect.left;

	// �摜�̍������v�Z
	m_MState.imgHeight = m_MState.imgRect.bottom - m_MState.imgRect.top;

	// DirectInput�I�u�W�F�N�g�̍쐬
	if(m_pDInput==NULL)
	{
		hr = DirectInput8Create(hInstance
							, DIRECTINPUT_VERSION
							, IID_IDirectInput8
							, (void**)&m_pDInput
							, NULL);
		if(FAILED(hr))
		{
			MessageBox(NULL,"DirectInput�I�u�W�F�N�g�̍쐬���s","DirectInput�I�u�W�F�N�g�̍쐬���s",MB_OK);
			return hr;
		}
	}

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = m_pDInput->CreateDevice(GUID_SysMouse
								, &m_pDIDevice
								, NULL);
	if(FAILED(hr))
	{
		MessageBox(NULL,"�f�o�C�X�I�u�W�F�N�g�̍쐬���s","�f�o�C�X�I�u�W�F�N�g�̍쐬���s",MB_OK);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_pDIDevice->SetDataFormat(&c_dfDIMouse2);
	if(FAILED(hr))
	{
		MessageBox(NULL,"�f�[�^�t�H�[�}�b�g�̐ݒ莸�s","�f�[�^�t�H�[�}�b�g�̐ݒ莸�s",MB_OK);
		return hr;
	}

	//�o�b�t�@�T�C�Y�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;
	hr = m_pDIDevice->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph);
	if ( FAILED( hr ) )
	{
		return false;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(NULL,"�������[�h�̐ݒ莸�s","�������[�h�̐ݒ莸�s",MB_OK);
		return hr;
	}

	// �}�E�X�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDIDevice->Acquire();

	return S_OK;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CInputMouse::Uninit(void)
{
	if(m_pDIDevice != NULL)
	{
		m_pDIDevice->Release();
		m_pDIDevice = NULL;
	}
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CInputMouse::Update(void)
{
	// �J�[�\���̍��W�Q�b�g
	GetCursorPos(&m_Point);

	// �X�N���[�����W�ɍ��킹��
	ScreenToClient(m_hWnd, &m_Point);

	// �f�[�^���擾
	if(SUCCEEDED(m_pDIDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_diMouseState)))
	{
		// �擾�����������Ƀ}�E�X�̏����X�V
		if(m_diMouseState.lX)
		{
			m_MState.x = (float)m_Point.x;
		}
		if(m_diMouseState.lY)
		{
			m_MState.y = (float)m_Point.y;
		}
		(m_diMouseState.rgbButtons[0] & 0x80) ? m_MState.lButton = true : m_MState.lButton = false;
		(m_diMouseState.rgbButtons[1] & 0x80) ? m_MState.rButton = true : m_MState.rButton = false;
		(m_diMouseState.rgbButtons[2] & 0x80) ? m_MState.cButton = true : m_MState.cButton = false;
	}
	// ���s������
	else
	{
		// �����擾
		m_pDIDevice->Acquire();
	}
}

//----EOF----