//cbuffer global : register(b0)
//{
//    // ワールドから射影までの変換行列
//    matrix g_wvp;
//};

float4 main(float4 pos : POSITION) : SV_POSITION
{
    //pos = mul(pos, g_wvp);
    return pos;
}