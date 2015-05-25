//==========================================================
// �萔���W�X�^ CPU������ύX�ł��鐔���Ȃ�����
//==========================================================
float4 gColor;
sampler texSampler;
sampler texSampler2;
float gValue;
float3 gPointLightPos;		// �_�����̍��W
float4 gPointLightColor;	// �_�����̐F
float gAtteruation[3];		// ������
//**********************************************************
// �G���g���|�C���g
//**********************************************************
float4 PS(float4 diffuse : COLOR0, float2 uv : TEXCOORD0, float3 posW : TEXCOORD1):COLOR0
{
	// �����̌v�Z
	float d = distance(gPointLightPos, posW);

	// �������̌v�Z
	float a = gAtteruation[0] + gAtteruation[1] * d + gAtteruation[2] * d * d;

	float3 c = tex2D(texSampler, uv).rgb * gPointLightColor.rgb / a;

	return float4(c, 1.0f);


	//// �w�肵�����邳�̂Ƃ��낾�������o��
	//float4 texColor = tex2D(texSampler, uv);
	//float l = texColor.r * 0.299f + texColor.g * 0.587f + texColor * 0.114f;
	//return l > 0.98f ? (float4)l : (float4)0.0f;

	// �E�F�[�u
	//return tex2D(texSampler, float2(uv.x + sin(uv.y * 10 + gValue) * 0.02f, uv.y));
	//return tex2D(texSampler, float2(uv.x, uv.y + sin(uv.x * 10 + gValue) * 0.02f));
	
	//// ���m�N���������̂ƃN���X�t�F�[�h
	//float4 texColor = tex2D(texSampler, uv);
	//float l = texColor.r * 0.299f + texColor.g * 0.587f + texColor * 0.114f;
	//float4 gc =  float4(l, l, l, 1.0f);
	//return texColor * gValue + gc * (1.0 - gValue);

	//return tex2D(texSampler, uv) * gValue + tex2D(texSampler2, uv) * (1.0f - gValue);	// �e�N�X�`���̃N���X�t�F�[�h

	//return diffuse;
	//return float4(diffuse.x, diffuse.y, diffuse.z, 1.0f); //���F�ɂȂ�����R G B A	���ɂȂ����� A R G B

	//return tex2D(texSampler2, uv);	// �e�N�X�`��

}

//EOF