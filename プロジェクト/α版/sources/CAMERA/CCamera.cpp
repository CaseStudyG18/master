//=============================================================================
//
// CCamera�N���X [CCamera.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MANAGER/CManager.h"
#include "CCamera.h"
#include "../INPUT/CInputKeyboard.h"
#include "../INPUT/CInputGamePad.h"
#include "../JUDGE/CJudge.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const float			NEAR_VAL = 1.0f;										// near�l �J�����ɉf��߂��̌��E
static const float			FAR_VAL = 2000.0f;										// far�l �J�����ɉf�鉓���̌��E
static const float			DEFAULT_CAMERA_MOV_R_SPD = 80.0f;						// �J�����̒����_�ύX��
static const float			DEFAULT_CAMERA_MOV_SPD = 1.8f;							// �J�����̈ړ��X�s�[�h
static const float			DEFAULT_CAMERA_ANGL_SPD = 0.03f;						// �J�����̉�]�X�s�[�h
static const float			LIMIT_UNDER_ANGLE = -1.35f;								// �J�����̉��ւ̊p�x�̌��E�l
static const float			LIMIT_UP_ANGLE = -0.014f;								// �J�����̏�ւ̊p�x�̌��E�l
static const float			BASE_HIGHT_POS = 100.0f;								// �J�����̊�{�̍���
static const float			VIEW_ANGLE = D3DX_PI / 4.f;								// ����p
static const D3DXVECTOR3	DEFAULT_CAMERA_POS = D3DXVECTOR3(0.0f, 150.0f, -600.0f);// �f�t�H���g�̃J�������W
static const D3DXVECTOR3	DEFAULT_CAMERA_POS_R = D3DXVECTOR3(0.0f, 150.0f, 0.0f);	// �f�t�H���g�̃J���������_���W
static const float			MAX_CAMERA_MOV_COEFFICIENT = 1.0f;						// �J�����ړ��W���̍ő�l
static const float			MIN_CAMERA_MOV_COEFFICIENT = 0.1f;						// �J�����ړ��W���̍ŏ��l

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
FRUSTUM		CCamera::m_Frustum;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CCamera::CCamera(void)
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CCamera::~CCamera(void)
{
}

//*****************************************************************************
// ������ ��������
//*****************************************************************************
void CCamera::Init(D3DXVECTOR3 pos, D3DXVECTOR3 posR)
{
	// ���W
	m_posCameraP = pos;
	m_destPosCameraP = pos;
	
	// �J�����̉�]�i���Ă���ꏊ ex:���̏ꍇm_posCameraP�̍��W����m_posCameraR�̍��W�����Ă���j
	m_posCameraR = posR;
	m_destPosCameraR = posR;
	
	// �J�����̕���
	m_vecCameraU = DEFAULT_UP_VECTOR;
	m_vecCameraFront = DEFAULT_FRONT_VECTOR;
	m_vecCameraRight = DEFAULT_RIGHT_VECTOR;

	// �����_�Ǝ��_�̋���
	D3DXVECTOR3 length = m_posCameraR - m_posCameraP;
	m_DistanceCamera = sqrt(length.x * length.x + length.z * length.z);
	m_fLengthInterval = sqrtf((m_posCameraR.x - m_posCameraP.x) * (m_posCameraR.x - m_posCameraP.x)
								+ (m_posCameraR.y - m_posCameraP.y) * (m_posCameraR.y - m_posCameraP.y)
								+ (m_posCameraR.z - m_posCameraP.z) * (m_posCameraR.z - m_posCameraP.z));

	// �p�x�̏�����
	m_rotCamera = D3DXVECTOR3(0,0,0);
	m_rotCamera.y = atan2f((m_posCameraR.x - m_posCameraP.x), (m_posCameraR.z - m_posCameraP.z));
	m_rotCamera.x = atan2f((m_posCameraR.y - m_posCameraP.y), m_DistanceCamera);

	// �ړ��ʂ̏�����
	m_movCamera = D3DXVECTOR3(0,0,0);

	// ������쐬
	MakeFrustum(VIEW_ANGLE, (float)SCREEN_WIDTH/SCREEN_HEIGHT, NEAR_VAL, FAR_VAL, m_Frustum);
}

//*****************************************************************************
// ������ �����Ȃ�
//*****************************************************************************
void CCamera::Init(void)
{
	// ���W
	m_posCameraP = DEFAULT_CAMERA_POS;
	m_destPosCameraP = DEFAULT_CAMERA_POS;
	
	// �J�����̉�]�i���Ă���ꏊ ex:���̏ꍇm_posCameraP�̍��W����m_posCameraR�̍��W�����Ă���j
	m_posCameraR = DEFAULT_CAMERA_POS_R;
	m_destPosCameraR = DEFAULT_CAMERA_POS_R;
	
	// �J�����̕���
	m_vecCameraU = DEFAULT_UP_VECTOR;
	m_vecCameraFront = DEFAULT_FRONT_VECTOR;
	m_vecCameraRight = DEFAULT_RIGHT_VECTOR;

	// �����_�Ǝ��_�̋���
	D3DXVECTOR3 length = m_posCameraR - m_posCameraP;
	m_DistanceCamera = sqrt(length.x * length.x + length.z * length.z);
	m_fLengthInterval = sqrtf((m_posCameraR.x - m_posCameraP.x) * (m_posCameraR.x - m_posCameraP.x)
								+ (m_posCameraR.y - m_posCameraP.y) * (m_posCameraR.y - m_posCameraP.y)
								+ (m_posCameraR.z - m_posCameraP.z) * (m_posCameraR.z - m_posCameraP.z));

	// �p�x�̏�����
	m_rotCamera = D3DXVECTOR3(0,0,0);
	m_rotCamera.y = atan2f((m_posCameraR.x - m_posCameraP.x), (m_posCameraR.z - m_posCameraP.z));
	m_rotCamera.x = atan2f((m_posCameraR.y - m_posCameraP.y), m_DistanceCamera);

	// �ړ��ʂ̏�����
	m_movCamera = D3DXVECTOR3(0,0,0);

	// ������쐬
	MakeFrustum(VIEW_ANGLE, (float)SCREEN_WIDTH/SCREEN_HEIGHT, NEAR_VAL, FAR_VAL, m_Frustum);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CCamera::Uninit(void)
{
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CCamera::Update(void)
{
}

//*****************************************************************************
// �Z�b�g�J����
//*****************************************************************************
void CCamera::SetCamera(LPDIRECT3DDEVICE9 *pDevice)
{
	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);
	
	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView, &m_posCameraP, &m_posCameraR, &m_vecCameraU);
	
	// �r���[�}�g���b�N�X�̐ݒ�
	(*pDevice)->SetTransform(D3DTS_VIEW, &m_mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection
							, VIEW_ANGLE						// ����p
							, (float)SCREEN_WIDTH/SCREEN_HEIGHT	// �A�X�y�N�g��
							, NEAR_VAL							// near�l
							, FAR_VAL);							// far�l
	
	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	(*pDevice)->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//*****************************************************************************
// 3�_���畽�ʍ쐬�֐�
//*****************************************************************************
void CCamera::MakePlaneFrom3Points(D3DXVECTOR3 &P0, D3DXVECTOR3 &P1, D3DXVECTOR3 &P2, PLANE &Plane)
{
	D3DXVECTOR3 V0, V1, V2;
	V0 = P1 - P0;
	V1 = P2 - P0;
	D3DXVec3Cross(&V2, &V0, &V1);
	D3DXVec3Normalize(&V2, &V2);
	Plane.a = V2.x;
	Plane.b = V2.y;
	Plane.c = V2.z;
	Plane.d = -((V2.x * P0.x) + (V2.y * P0.y) + (V2.z * P0.z));
}

//*****************************************************************************
// �����䐶���֐�
//*****************************************************************************
void CCamera::MakeFrustum(float Angle, float Aspect, float NearClip, float FarClip, FRUSTUM &Frustum)
{
	D3DXVECTOR3 P0, P1, P2;
	float fTan = tan(Angle * 0.5f);	// ���ʂ��甼���ɂ���������
	float fTanDivAspect = fTan / Aspect;
	float fFarMulTan = FarClip * fTan;
	float fFarMulTanDivAspect = FarClip * fTanDivAspect;

	// �����ʐ���
	P0.x = 0.f;
	P0.y = 0.f;
	P0.z = 0.f;
	P1.x = -fFarMulTanDivAspect;
	P1.y = -fFarMulTan;
	P1.z = FarClip;
	P2.x = P1.x;
	P2.y = -P1.y;
	P2.z = P1.z;
	MakePlaneFrom3Points(P0, P1, P2, Frustum.LeftPlane);

	// �E���ʐ���
	P0.x = 0.f;
	P0.y = 0.f;
	P0.z = 0.f;
	P1.x = fFarMulTanDivAspect;
	P1.y = fFarMulTan;
	P1.z = FarClip;
	P2.x = P1.x;
	P2.y = -P1.y;
	P2.z = P1.z;
	MakePlaneFrom3Points(P0, P1, P2, Frustum.RightPlane);

	// �㕽�ʐ���
	P0.x = 0.f;
	P0.y = 0.f;
	P0.z = 0.f;
	P1.x = -fFarMulTanDivAspect;
	P1.y = fFarMulTan;
	P1.z = FarClip;
	P2.x = -P1.x;
	P2.y = P1.y;
	P2.z = P1.z;
	MakePlaneFrom3Points(P0, P1, P2, Frustum.TopPlane);

	// �����ʐ���
	P0.x = 0.f;
	P0.y = 0.f;
	P0.z = 0.f;
	P1.x = fFarMulTanDivAspect;
	P1.y = -fFarMulTan;
	P1.z = FarClip;
	P2.x = -P1.x;
	P2.y = P1.y;
	P2.z = P1.z;
	MakePlaneFrom3Points(P0, P1, P2, Frustum.BottomPlane);

	Frustum.NearClip = NearClip;
	Frustum.FarClip = FarClip;
}

//-----EOF----