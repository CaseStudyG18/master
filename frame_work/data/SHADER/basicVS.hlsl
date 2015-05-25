//==========================================================
// �萔���W�X�^ CPU������ύX�ł��鐔���Ȃ�����
//==========================================================
float gValue;
float4x4 gWorld;			// ���[���h�}�g���N�X
float4x4 gView;				// �r���[�}�g���N�X
float4x4 gProj;				// �v���W�F�N�V�����}�g���N�X
float3 gLightDir;			// ���C�g�̕���
float4 gLightColor;			// ���C�g�̐F

//**********************************************************
// �G���g���|�C���g
//**********************************************************
void VS(in float3 inPos:POSITION0			// ���� FVF���ɍ��킹��
		, in float4 inDiffuse : COLOR0		// ���� FVF���ɍ��킹��	FVF_VERTEX_2D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
		, in float2 inUV : TEXCOORD0		// ���� FVF���ɍ��킹��
		, in float3 inNormal : NORMAL0		// ���� �@��
		, out float4 outPos : POSITION		// �Ԃ��l �K��float4��POSITON��Ԃ��Ȃ���΂Ȃ�Ȃ� �V�X�e��(GPU)�ɍs�� �s�N�Z���V�F�[�_�[�ɓn�������Ƃ���TEXCOORD~�����R�Ɏg����̂ł�����g��
		, out float4 outDiffuse : COLOR0	// �Ԃ��l �s�N�Z���V�F�[�_�[�ɐ��`��Ԃ���ē͂�
		, out float2 outUV : TEXCOORD0		// �Ԃ��l �s�N�Z���V�F�[�_�[�ɐ��`��Ԃ���ē͂�
		, out float3 posW : TEXCOORD1)		// �Ԃ��l �s�N�Z���V�F�[�_�[�ɑ��郏�[���h���W
{
	// �s�N�Z���V�F�[�_�[�ɑ��郏�[���h���W
	posW = mul(float4(inPos, 1.0f), gWorld).xyz;

	// �O�Ń��C�g�x�N�g�������[�J���ɕϊ����Ă���̂ŃJ�b�g�ł���
	//float4 normal = mul(float4(inNormal, 0.f), gWorld);	// �@���̃��[���h���W�ϊ��i��]�̂݁j
	//float light = max(dot(-gLightDir, normal), 0);	// ���C�g���������܂ɂ���̂�Y�ꂸ��

	float light = max(dot(-gLightDir, inNormal), 0);	// ���C�g���������܂ɂ���̂�Y�ꂸ��
	outDiffuse = gLightColor * light;				// �e�X�g�p

	float4x4 wv = mul(gWorld, gView);
	float4x4 wvp = mul(wv, gProj);
	outPos = mul(float4(inPos, 1.0f), wvp);
	//outDiffuse = inDiffuse;
	outUV = inUV;

	/*
	outPos.x = (inPos.x + gValue) / 400.0f - 1.0f;		// ��ʕ��̔�����z�肵�Ă���
	outPos.y = -(inPos.y / 300.0f - 1.0f);	// ��ʍ����̔�����z�肵�Ă���
	outPos.z = 0.0f;
	outPos.w = 1.0f;						// w�ł��ׂĐ��K�������̂ł����1�ɂ��Ă����Ȃ��Ƃق��̒l�����������Ȃ�
	outDiffuse = inDiffuse;					// ��������Ȃ��ƃs�N�Z���V�F�[�_�[�ɕs��l���͂��i�t�Ɍ����ƒl���͂��Ȃ��j�̂ŕK������
	outUV = inUV;							// ��������Ȃ��ƃs�N�Z���V�F�[�_�[�ɕs��l���͂��i�t�Ɍ����ƒl���͂��Ȃ��j�̂ŕK������
	*/
}
//EOF