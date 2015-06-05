//==========================================================
// 定数レジスタ CPU側から変更できる数少ないもの
//==========================================================
float4 gColor;
sampler texSampler;
sampler texSampler2;
float gValue;
float3 gPointLightPos;		// 点光源の座標
float4 gPointLightColor;	// 点光源の色
float gAtteruation[3];		// 減衰率
//**********************************************************
// エントリポイント
//**********************************************************
float4 PS(float4 diffuse : COLOR0, float2 uv : TEXCOORD0, float3 posW : TEXCOORD1):COLOR0
{
	// 距離の計算
	float d = distance(gPointLightPos, posW);

	// 減衰率の計算
	float a = gAtteruation[0] + gAtteruation[1] * d + gAtteruation[2] * d * d;

	float3 c = tex2D(texSampler, uv).rgb * gPointLightColor.rgb / a;

	return float4(c, 1.0f);


	//// 指定した明るさのところだけ抜き出す
	//float4 texColor = tex2D(texSampler, uv);
	//float l = texColor.r * 0.299f + texColor.g * 0.587f + texColor * 0.114f;
	//return l > 0.98f ? (float4)l : (float4)0.0f;

	// ウェーブ
	//return tex2D(texSampler, float2(uv.x + sin(uv.y * 10 + gValue) * 0.02f, uv.y));
	//return tex2D(texSampler, float2(uv.x, uv.y + sin(uv.x * 10 + gValue) * 0.02f));
	
	//// モノクロ化したのとクロスフェード
	//float4 texColor = tex2D(texSampler, uv);
	//float l = texColor.r * 0.299f + texColor.g * 0.587f + texColor * 0.114f;
	//float4 gc =  float4(l, l, l, 1.0f);
	//return texColor * gValue + gc * (1.0 - gValue);

	//return tex2D(texSampler, uv) * gValue + tex2D(texSampler2, uv) * (1.0f - gValue);	// テクスチャのクロスフェード

	//return diffuse;
	//return float4(diffuse.x, diffuse.y, diffuse.z, 1.0f); //黄色になったらR G B A	紫になったら A R G B

	//return tex2D(texSampler2, uv);	// テクスチャ

}

//EOF