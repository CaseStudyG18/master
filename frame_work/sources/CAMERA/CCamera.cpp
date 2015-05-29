//=============================================================================
//
// CCameraクラス [CCamera.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MANAGER/CManager.h"
#include "CCamera.h"
#include "../INPUT/CInputKeyboard.h"
#include "../INPUT/CInputGamePad.h"
#include "../JUDGE/CJudge.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
static const float			NEAR_VAL = 1.0f;										// near値 カメラに映る近さの限界
static const float			FAR_VAL = 2000.0f;										// far値 カメラに映る遠さの限界
static const float			DEFAULT_CAMERA_MOV_R_SPD = 80.0f;						// カメラの注視点変更量
static const float			DEFAULT_CAMERA_MOV_SPD = 1.8f;							// カメラの移動スピード
static const float			DEFAULT_CAMERA_ANGL_SPD = 0.03f;						// カメラの回転スピード
static const float			LIMIT_UNDER_ANGLE = -1.35f;								// カメラの下への角度の限界値
static const float			LIMIT_UP_ANGLE = -0.014f;								// カメラの上への角度の限界値
static const float			BASE_HIGHT_POS = 100.0f;								// カメラの基本の高さ
static const float			VIEW_ANGLE = D3DX_PI / 4.f;								// 視野角
static const D3DXVECTOR3	DEFAULT_CAMERA_POS = D3DXVECTOR3(0.0f, 150.0f, -600.0f);// デフォルトのカメラ座標
static const D3DXVECTOR3	DEFAULT_CAMERA_POS_R = D3DXVECTOR3(0.0f, 150.0f, 0.0f);	// デフォルトのカメラ注視点座標
static const float			MAX_CAMERA_MOV_COEFFICIENT = 1.0f;						// カメラ移動係数の最大値
static const float			MIN_CAMERA_MOV_COEFFICIENT = 0.1f;						// カメラ移動係数の最小値

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
FRUSTUM		CCamera::m_Frustum;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CCamera::CCamera(void)
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CCamera::~CCamera(void)
{
}

//*****************************************************************************
// 初期化 引数あり
//*****************************************************************************
void CCamera::Init(D3DXVECTOR3 pos, D3DXVECTOR3 posR)
{
	// 座標
	m_posCameraP = pos;
	m_destPosCameraP = pos;
	
	// カメラの回転（見ている場所 ex:この場合m_posCameraPの座標からm_posCameraRの座標を見ている）
	m_posCameraR = posR;
	m_destPosCameraR = posR;
	
	// カメラの方向
	m_vecCameraU = DEFAULT_UP_VECTOR;
	m_vecCameraFront = DEFAULT_FRONT_VECTOR;
	m_vecCameraRight = DEFAULT_RIGHT_VECTOR;

	// 注視点と視点の距離
	D3DXVECTOR3 length = m_posCameraR - m_posCameraP;
	m_DistanceCamera = sqrt(length.x * length.x + length.z * length.z);
	m_fLengthInterval = sqrtf((m_posCameraR.x - m_posCameraP.x) * (m_posCameraR.x - m_posCameraP.x)
								+ (m_posCameraR.y - m_posCameraP.y) * (m_posCameraR.y - m_posCameraP.y)
								+ (m_posCameraR.z - m_posCameraP.z) * (m_posCameraR.z - m_posCameraP.z));

	// 角度の初期化
	m_rotCamera = D3DXVECTOR3(0,0,0);
	m_rotCamera.y = atan2f((m_posCameraR.x - m_posCameraP.x), (m_posCameraR.z - m_posCameraP.z));
	m_rotCamera.x = atan2f((m_posCameraR.y - m_posCameraP.y), m_DistanceCamera);

	// 移動量の初期化
	m_movCamera = D3DXVECTOR3(0,0,0);

	// 視錐台作成
	MakeFrustum(VIEW_ANGLE, (float)SCREEN_WIDTH/SCREEN_HEIGHT, NEAR_VAL, FAR_VAL, m_Frustum);
}

//*****************************************************************************
// 初期化 引数なし
//*****************************************************************************
void CCamera::Init(void)
{
	// 座標
	m_posCameraP = DEFAULT_CAMERA_POS;
	m_destPosCameraP = DEFAULT_CAMERA_POS;
	
	// カメラの回転（見ている場所 ex:この場合m_posCameraPの座標からm_posCameraRの座標を見ている）
	m_posCameraR = DEFAULT_CAMERA_POS_R;
	m_destPosCameraR = DEFAULT_CAMERA_POS_R;
	
	// カメラの方向
	m_vecCameraU = DEFAULT_UP_VECTOR;
	m_vecCameraFront = DEFAULT_FRONT_VECTOR;
	m_vecCameraRight = DEFAULT_RIGHT_VECTOR;

	// 注視点と視点の距離
	D3DXVECTOR3 length = m_posCameraR - m_posCameraP;
	m_DistanceCamera = sqrt(length.x * length.x + length.z * length.z);
	m_fLengthInterval = sqrtf((m_posCameraR.x - m_posCameraP.x) * (m_posCameraR.x - m_posCameraP.x)
								+ (m_posCameraR.y - m_posCameraP.y) * (m_posCameraR.y - m_posCameraP.y)
								+ (m_posCameraR.z - m_posCameraP.z) * (m_posCameraR.z - m_posCameraP.z));

	// 角度の初期化
	m_rotCamera = D3DXVECTOR3(0,0,0);
	m_rotCamera.y = atan2f((m_posCameraR.x - m_posCameraP.x), (m_posCameraR.z - m_posCameraP.z));
	m_rotCamera.x = atan2f((m_posCameraR.y - m_posCameraP.y), m_DistanceCamera);

	// 移動量の初期化
	m_movCamera = D3DXVECTOR3(0,0,0);

	// 視錐台作成
	MakeFrustum(VIEW_ANGLE, (float)SCREEN_WIDTH/SCREEN_HEIGHT, NEAR_VAL, FAR_VAL, m_Frustum);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CCamera::Uninit(void)
{
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CCamera::Update(void)
{
}

//*****************************************************************************
// セットカメラ
//*****************************************************************************
void CCamera::SetCamera(LPDIRECT3DDEVICE9 *pDevice)
{
	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);
	
	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView, &m_posCameraP, &m_posCameraR, &m_vecCameraU);
	
	// ビューマトリックスの設定
	(*pDevice)->SetTransform(D3DTS_VIEW, &m_mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection
							, VIEW_ANGLE						// 視野角
							, (float)SCREEN_WIDTH/SCREEN_HEIGHT	// アスペクト比
							, NEAR_VAL							// near値
							, FAR_VAL);							// far値
	
	// プロジェクションマトリックスの設定
	(*pDevice)->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//*****************************************************************************
// 3点から平面作成関数
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
// 視錐台生成関数
//*****************************************************************************
void CCamera::MakeFrustum(float Angle, float Aspect, float NearClip, float FarClip, FRUSTUM &Frustum)
{
	D3DXVECTOR3 P0, P1, P2;
	float fTan = tan(Angle * 0.5f);	// 正面から半分にしたいから
	float fTanDivAspect = fTan / Aspect;
	float fFarMulTan = FarClip * fTan;
	float fFarMulTanDivAspect = FarClip * fTanDivAspect;

	// 左平面生成
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

	// 右平面生成
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

	// 上平面生成
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

	// 下平面生成
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