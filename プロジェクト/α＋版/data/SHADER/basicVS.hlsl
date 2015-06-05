//==========================================================
// 定数レジスタ CPU側から変更できる数少ないもの
//==========================================================
float gValue;
float4x4 gWorld;			// ワールドマトリクス
float4x4 gView;				// ビューマトリクス
float4x4 gProj;				// プロジェクションマトリクス
float3 gLightDir;			// ライトの方向
float4 gLightColor;			// ライトの色

//**********************************************************
// エントリポイント
//**********************************************************
void VS(in float3 inPos:POSITION0			// 引数 FVF等に合わせる
		, in float4 inDiffuse : COLOR0		// 引数 FVF等に合わせる	FVF_VERTEX_2D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
		, in float2 inUV : TEXCOORD0		// 引数 FVF等に合わせる
		, in float3 inNormal : NORMAL0		// 引数 法線
		, out float4 outPos : POSITION		// 返す値 必ずfloat4のPOSITONを返さなければならない システム(GPU)に行く ピクセルシェーダーに渡したいときはTEXCOORD~が自由に使えるのでそれを使う
		, out float4 outDiffuse : COLOR0	// 返す値 ピクセルシェーダーに線形補間されて届く
		, out float2 outUV : TEXCOORD0		// 返す値 ピクセルシェーダーに線形補間されて届く
		, out float3 posW : TEXCOORD1)		// 返す値 ピクセルシェーダーに送るワールド座標
{
	// ピクセルシェーダーに送るワールド座標
	posW = mul(float4(inPos, 1.0f), gWorld).xyz;

	// 外でライトベクトルをローカルに変換しているのでカットできる
	//float4 normal = mul(float4(inNormal, 0.f), gWorld);	// 法線のワールド座標変換（回転のみ）
	//float light = max(dot(-gLightDir, normal), 0);	// ライトをさかさまにするのを忘れずに

	float light = max(dot(-gLightDir, inNormal), 0);	// ライトをさかさまにするのを忘れずに
	outDiffuse = gLightColor * light;				// テスト用

	float4x4 wv = mul(gWorld, gView);
	float4x4 wvp = mul(wv, gProj);
	outPos = mul(float4(inPos, 1.0f), wvp);
	//outDiffuse = inDiffuse;
	outUV = inUV;

	/*
	outPos.x = (inPos.x + gValue) / 400.0f - 1.0f;		// 画面幅の半分を想定している
	outPos.y = -(inPos.y / 300.0f - 1.0f);	// 画面高さの半分を想定している
	outPos.z = 0.0f;
	outPos.w = 1.0f;						// wですべて正規化されるのでこれを1にしておかないとほかの値がおかしくなる
	outDiffuse = inDiffuse;					// これをしないとピクセルシェーダーに不定値が届く（逆に言うと値が届かない）ので必ずする
	outUV = inUV;							// これをしないとピクセルシェーダーに不定値が届く（逆に言うと値が届かない）ので必ずする
	*/
}
//EOF