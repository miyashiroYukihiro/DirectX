//cbuffer global : register(b0)
//{
//    // ƒ[ƒ‹ƒh‚©‚çË‰e‚Ü‚Å‚Ì•ÏŠ·s—ñ
//    matrix g_wvp;
//};

float4 main(float4 pos : POSITION) : SV_POSITION
{
    //pos = mul(pos, g_wvp);
    return pos;
}