//cbuffer global : register(b0)
//{
//    // ���[���h����ˉe�܂ł̕ϊ��s��
//    matrix g_wvp;
//};

float4 main(float4 pos : POSITION) : SV_POSITION
{
    //pos = mul(pos, g_wvp);
    return pos;
}