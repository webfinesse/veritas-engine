#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 6.3.9600.16384
//
//
// Buffer Definitions: 
//
// cbuffer cbCamera
// {
//
//   float4x4 viewMatrix;               // Offset:    0 Size:    64
//   float4x4 projMatrix;               // Offset:   64 Size:    64
//   float3 eyePosition;                // Offset:  128 Size:    12 [unused]
//   float pad;                         // Offset:  140 Size:     4 [unused]
//
// }
//
// cbuffer cbPerObject
// {
//
//   float4x4 worldTransform;           // Offset:    0 Size:    64
//   float4x4 worldInverseTranspose;    // Offset:   64 Size:    64
//   
//   struct
//   {
//       
//       float4 ambient;                // Offset:  128
//       float4 diffuse;                // Offset:  144
//       float4 specular;               // Offset:  160
//       float4 emissive;               // Offset:  176
//
//   } material;                        // Offset:  128 Size:    64 [unused]
//   bool hasDiffuseMap;                // Offset:  192 Size:     4 [unused]
//   bool hasNormalMap;                 // Offset:  196 Size:     4 [unused]
//   bool hasSpecularMap;               // Offset:  200 Size:     4 [unused]
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// cbCamera                          cbuffer      NA          NA    0        1
// cbPerObject                       cbuffer      NA          NA    1        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// POSITION                 0   xyz         0     NONE   float   xyz 
// NORMAL                   0   xyz         1     NONE   float   xyz 
// TANGENT                  0   xyz         2     NONE   float   xyz 
// BINORMAL                 0   xyz         3     NONE   float   xyz 
// TEXCOORD                 0   xy          4     NONE   float   xy  
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
// POSITION                 0   xyz         1     NONE   float   xyz 
// NORMAL                   0   xyz         2     NONE   float   xyz 
// TANGENT                  0   xyz         3     NONE   float   xyz 
// BINORMAL                 0   xyz         4     NONE   float   xyz 
// TEXCOORD                 0   xy          5     NONE   float   xy  
//
vs_4_1
dcl_globalFlags refactoringAllowed
dcl_constantbuffer cb0[8], immediateIndexed
dcl_constantbuffer cb1[7], immediateIndexed
dcl_input v0.xyz
dcl_input v1.xyz
dcl_input v2.xyz
dcl_input v3.xyz
dcl_input v4.xy
dcl_output_siv o0.xyzw, position
dcl_output o1.xyz
dcl_output o2.xyz
dcl_output o3.xyz
dcl_output o4.xyz
dcl_output o5.xy
dcl_temps 6
mul r0.xyzw, cb0[1].yyyy, cb1[1].xyzw
mad r0.xyzw, cb1[0].xyzw, cb0[1].xxxx, r0.xyzw
mad r0.xyzw, cb1[2].xyzw, cb0[1].zzzz, r0.xyzw
mad r0.xyzw, cb1[3].xyzw, cb0[1].wwww, r0.xyzw
mul r1.xyzw, r0.xyzw, cb0[4].yyyy
mul r2.xyzw, cb0[0].yyyy, cb1[1].xyzw
mad r2.xyzw, cb1[0].xyzw, cb0[0].xxxx, r2.xyzw
mad r2.xyzw, cb1[2].xyzw, cb0[0].zzzz, r2.xyzw
mad r2.xyzw, cb1[3].xyzw, cb0[0].wwww, r2.xyzw
mad r1.xyzw, r2.xyzw, cb0[4].xxxx, r1.xyzw
mul r3.xyzw, cb0[2].yyyy, cb1[1].xyzw
mad r3.xyzw, cb1[0].xyzw, cb0[2].xxxx, r3.xyzw
mad r3.xyzw, cb1[2].xyzw, cb0[2].zzzz, r3.xyzw
mad r3.xyzw, cb1[3].xyzw, cb0[2].wwww, r3.xyzw
mad r1.xyzw, r3.xyzw, cb0[4].zzzz, r1.xyzw
mul r4.xyzw, cb0[3].yyyy, cb1[1].xyzw
mad r4.xyzw, cb1[0].xyzw, cb0[3].xxxx, r4.xyzw
mad r4.xyzw, cb1[2].xyzw, cb0[3].zzzz, r4.xyzw
mad r4.xyzw, cb1[3].xyzw, cb0[3].wwww, r4.xyzw
mad r1.xyzw, r4.xyzw, cb0[4].wwww, r1.xyzw
mov r5.xyz, v0.xyzx
mov r5.w, l(1.000000)
dp4 o0.x, r5.xyzw, r1.xyzw
mul r1.xyzw, r0.xyzw, cb0[5].yyyy
mad r1.xyzw, r2.xyzw, cb0[5].xxxx, r1.xyzw
mad r1.xyzw, r3.xyzw, cb0[5].zzzz, r1.xyzw
mad r1.xyzw, r4.xyzw, cb0[5].wwww, r1.xyzw
dp4 o0.y, r5.xyzw, r1.xyzw
mul r1.xyzw, r0.xyzw, cb0[6].yyyy
mul r0.xyzw, r0.xyzw, cb0[7].yyyy
mad r0.xyzw, r2.xyzw, cb0[7].xxxx, r0.xyzw
mad r1.xyzw, r2.xyzw, cb0[6].xxxx, r1.xyzw
mad r1.xyzw, r3.xyzw, cb0[6].zzzz, r1.xyzw
mad r0.xyzw, r3.xyzw, cb0[7].zzzz, r0.xyzw
mad r0.xyzw, r4.xyzw, cb0[7].wwww, r0.xyzw
mad r1.xyzw, r4.xyzw, cb0[6].wwww, r1.xyzw
dp4 o0.z, r5.xyzw, r1.xyzw
dp4 o0.w, r5.xyzw, r0.xyzw
dp4 o1.x, r5.xyzw, cb1[0].xyzw
dp4 o1.y, r5.xyzw, cb1[1].xyzw
dp4 o1.z, r5.xyzw, cb1[2].xyzw
dp3 r0.x, v1.xyzx, cb1[4].xyzx
dp3 r0.y, v1.xyzx, cb1[5].xyzx
dp3 r0.z, v1.xyzx, cb1[6].xyzx
dp3 r0.w, r0.xyzx, r0.xyzx
rsq r0.w, r0.w
mul o2.xyz, r0.wwww, r0.xyzx
dp3 r0.x, v2.xyzx, cb1[4].xyzx
dp3 r0.y, v2.xyzx, cb1[5].xyzx
dp3 r0.z, v2.xyzx, cb1[6].xyzx
dp3 r0.w, r0.xyzx, r0.xyzx
rsq r0.w, r0.w
mul o3.xyz, r0.wwww, r0.xyzx
dp3 r0.x, v3.xyzx, cb1[4].xyzx
dp3 r0.y, v3.xyzx, cb1[5].xyzx
dp3 r0.z, v3.xyzx, cb1[6].xyzx
dp3 r0.w, r0.xyzx, r0.xyzx
rsq r0.w, r0.w
mul o4.xyz, r0.wwww, r0.xyzx
mov o5.xy, v4.xyxx
ret 
// Approximately 61 instruction slots used
#endif

const BYTE g_meshVertexShader[] =
{
     68,  88,  66,  67, 136,  47, 
      1, 115, 190,  64, 253, 184, 
      1,  40, 136, 157, 176, 144, 
    204, 222,   1,   0,   0,   0, 
     28,  14,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
     60,   3,   0,   0, 240,   3, 
      0,   0, 200,   4,   0,   0, 
    160,  13,   0,   0,  82,  68, 
     69,  70,   0,   3,   0,   0, 
      2,   0,   0,   0, 116,   0, 
      0,   0,   2,   0,   0,   0, 
     28,   0,   0,   0,   1,   4, 
    254, 255,   0,   1,   0,   0, 
    204,   2,   0,   0,  92,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
    101,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  99,  98,  67,  97, 
    109, 101, 114,  97,   0,  99, 
     98,  80, 101, 114,  79,  98, 
    106, 101,  99, 116,   0, 171, 
    171, 171,  92,   0,   0,   0, 
      4,   0,   0,   0, 164,   0, 
      0,   0, 144,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 101,   0,   0,   0, 
      6,   0,   0,   0,  92,   1, 
      0,   0, 208,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   4,   1,   0,   0, 
      0,   0,   0,   0,  64,   0, 
      0,   0,   2,   0,   0,   0, 
     16,   1,   0,   0,   0,   0, 
      0,   0,  32,   1,   0,   0, 
     64,   0,   0,   0,  64,   0, 
      0,   0,   2,   0,   0,   0, 
     16,   1,   0,   0,   0,   0, 
      0,   0,  43,   1,   0,   0, 
    128,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
     56,   1,   0,   0,   0,   0, 
      0,   0,  72,   1,   0,   0, 
    140,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
     76,   1,   0,   0,   0,   0, 
      0,   0, 118, 105, 101, 119, 
     77,  97, 116, 114, 105, 120, 
      0, 171,   3,   0,   3,   0, 
      4,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    112, 114, 111, 106,  77,  97, 
    116, 114, 105, 120,   0, 101, 
    121, 101,  80, 111, 115, 105, 
    116, 105, 111, 110,   0, 171, 
      1,   0,   3,   0,   1,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 112,  97, 
    100,   0,   0,   0,   3,   0, 
      1,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    236,   1,   0,   0,   0,   0, 
      0,   0,  64,   0,   0,   0, 
      2,   0,   0,   0,  16,   1, 
      0,   0,   0,   0,   0,   0, 
    251,   1,   0,   0,  64,   0, 
      0,   0,  64,   0,   0,   0, 
      2,   0,   0,   0,  16,   1, 
      0,   0,   0,   0,   0,   0, 
     17,   2,   0,   0, 128,   0, 
      0,   0,  64,   0,   0,   0, 
      0,   0,   0,   0, 128,   2, 
      0,   0,   0,   0,   0,   0, 
    144,   2,   0,   0, 192,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0, 160,   2, 
      0,   0,   0,   0,   0,   0, 
    176,   2,   0,   0, 196,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0, 160,   2, 
      0,   0,   0,   0,   0,   0, 
    189,   2,   0,   0, 200,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0, 160,   2, 
      0,   0,   0,   0,   0,   0, 
    119, 111, 114, 108, 100,  84, 
    114,  97, 110, 115, 102, 111, 
    114, 109,   0, 119, 111, 114, 
    108, 100,  73, 110, 118, 101, 
    114, 115, 101,  84, 114,  97, 
    110, 115, 112, 111, 115, 101, 
      0, 109,  97, 116, 101, 114, 
    105,  97, 108,   0,  97, 109, 
     98, 105, 101, 110, 116,   0, 
    171, 171,   1,   0,   3,   0, 
      1,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    100, 105, 102, 102, 117, 115, 
    101,   0, 115, 112, 101,  99, 
    117, 108,  97, 114,   0, 101, 
    109, 105, 115, 115, 105, 118, 
    101,   0, 171, 171,  26,   2, 
      0,   0,  36,   2,   0,   0, 
      0,   0,   0,   0,  52,   2, 
      0,   0,  36,   2,   0,   0, 
     16,   0,   0,   0,  60,   2, 
      0,   0,  36,   2,   0,   0, 
     32,   0,   0,   0,  69,   2, 
      0,   0,  36,   2,   0,   0, 
     48,   0,   0,   0,   5,   0, 
      0,   0,   1,   0,  16,   0, 
      0,   0,   4,   0,  80,   2, 
      0,   0, 104,  97, 115,  68, 
    105, 102, 102, 117, 115, 101, 
     77,  97, 112,   0, 171, 171, 
      0,   0,   1,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 104,  97, 
    115,  78, 111, 114, 109,  97, 
    108,  77,  97, 112,   0, 104, 
     97, 115,  83, 112, 101,  99, 
    117, 108,  97, 114,  77,  97, 
    112,   0,  77, 105,  99, 114, 
    111, 115, 111, 102, 116,  32, 
     40,  82,  41,  32,  72,  76, 
     83,  76,  32,  83, 104,  97, 
    100, 101, 114,  32,  67, 111, 
    109, 112, 105, 108, 101, 114, 
     32,  54,  46,  51,  46,  57, 
     54,  48,  48,  46,  49,  54, 
     51,  56,  52,   0, 171, 171, 
     73,  83,  71,  78, 172,   0, 
      0,   0,   5,   0,   0,   0, 
      8,   0,   0,   0, 128,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      7,   7,   0,   0, 137,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      7,   7,   0,   0, 144,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      7,   7,   0,   0, 152,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
      7,   7,   0,   0, 161,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   4,   0,   0,   0, 
      3,   3,   0,   0,  80,  79, 
     83,  73,  84,  73,  79,  78, 
      0,  78,  79,  82,  77,  65, 
     76,   0,  84,  65,  78,  71, 
     69,  78,  84,   0,  66,  73, 
     78,  79,  82,  77,  65,  76, 
      0,  84,  69,  88,  67,  79, 
     79,  82,  68,   0, 171, 171, 
     79,  83,  71,  78, 208,   0, 
      0,   0,   6,   0,   0,   0, 
      8,   0,   0,   0, 152,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0, 164,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      7,   8,   0,   0, 173,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      7,   8,   0,   0, 180,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
      7,   8,   0,   0, 188,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   4,   0,   0,   0, 
      7,   8,   0,   0, 197,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   5,   0,   0,   0, 
      3,  12,   0,   0,  83,  86, 
     95,  80,  79,  83,  73,  84, 
     73,  79,  78,   0,  80,  79, 
     83,  73,  84,  73,  79,  78, 
      0,  78,  79,  82,  77,  65, 
     76,   0,  84,  65,  78,  71, 
     69,  78,  84,   0,  66,  73, 
     78,  79,  82,  77,  65,  76, 
      0,  84,  69,  88,  67,  79, 
     79,  82,  68,   0, 171, 171, 
     83,  72,  68,  82, 208,   8, 
      0,   0,  65,   0,   1,   0, 
     52,   2,   0,   0, 106,   8, 
      0,   1,  89,   0,   0,   4, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      7,   0,   0,   0,  95,   0, 
      0,   3, 114,  16,  16,   0, 
      0,   0,   0,   0,  95,   0, 
      0,   3, 114,  16,  16,   0, 
      1,   0,   0,   0,  95,   0, 
      0,   3, 114,  16,  16,   0, 
      2,   0,   0,   0,  95,   0, 
      0,   3, 114,  16,  16,   0, 
      3,   0,   0,   0,  95,   0, 
      0,   3,  50,  16,  16,   0, 
      4,   0,   0,   0, 103,   0, 
      0,   4, 242,  32,  16,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0, 101,   0,   0,   3, 
    114,  32,  16,   0,   1,   0, 
      0,   0, 101,   0,   0,   3, 
    114,  32,  16,   0,   2,   0, 
      0,   0, 101,   0,   0,   3, 
    114,  32,  16,   0,   3,   0, 
      0,   0, 101,   0,   0,   3, 
    114,  32,  16,   0,   4,   0, 
      0,   0, 101,   0,   0,   3, 
     50,  32,  16,   0,   5,   0, 
      0,   0, 104,   0,   0,   2, 
      6,   0,   0,   0,  56,   0, 
      0,   9, 242,   0,  16,   0, 
      0,   0,   0,   0,  86, 133, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  11, 242,   0,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   6, 128, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   2,   0,   0,   0, 
    166, 138,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  11, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0, 246, 143,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   8, 242,   0,  16,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     86, 133,  32,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
     56,   0,   0,   9, 242,   0, 
     16,   0,   2,   0,   0,   0, 
     86, 133,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      6, 128,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,  11, 
    242,   0,  16,   0,   2,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   2,   0, 
      0,   0, 166, 138,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  50,   0, 
      0,  11, 242,   0,  16,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      3,   0,   0,   0, 246, 143, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,   6, 128,  32,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,   9, 242,   0,  16,   0, 
      3,   0,   0,   0,  86, 133, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  11, 242,   0,  16,   0, 
      3,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   6, 128, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,  70,  14, 
     16,   0,   3,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   3,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   2,   0,   0,   0, 
    166, 138,  32,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
     70,  14,  16,   0,   3,   0, 
      0,   0,  50,   0,   0,  11, 
    242,   0,  16,   0,   3,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0, 246, 143,  32,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  70,  14,  16,   0, 
      3,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   3,   0,   0,   0, 
    166, 138,  32,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  56,   0,   0,   9, 
    242,   0,  16,   0,   4,   0, 
      0,   0,  86, 133,  32,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  50,   0,   0,  11, 
    242,   0,  16,   0,   4,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   6, 128,  32,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,  70,  14,  16,   0, 
      4,   0,   0,   0,  50,   0, 
      0,  11, 242,   0,  16,   0, 
      4,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      2,   0,   0,   0, 166, 138, 
     32,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,  70,  14, 
     16,   0,   4,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   4,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   3,   0,   0,   0, 
    246, 143,  32,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     70,  14,  16,   0,   4,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      4,   0,   0,   0, 246, 143, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     54,   0,   0,   5, 114,   0, 
     16,   0,   5,   0,   0,   0, 
     70,  18,  16,   0,   0,   0, 
      0,   0,  54,   0,   0,   5, 
    130,   0,  16,   0,   5,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  63,  17,   0, 
      0,   7,  18,  32,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   5,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  86, 133, 
     32,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
      6, 128,  32,   0,   0,   0, 
      0,   0,   5,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      3,   0,   0,   0, 166, 138, 
     32,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   4,   0, 
      0,   0, 246, 143,  32,   0, 
      0,   0,   0,   0,   5,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  17,   0, 
      0,   7,  34,  32,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   5,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  86, 133, 
     32,   0,   0,   0,   0,   0, 
      6,   0,   0,   0,  56,   0, 
      0,   8, 242,   0,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     86, 133,  32,   0,   0,   0, 
      0,   0,   7,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,   6, 128,  32,   0, 
      0,   0,   0,   0,   7,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
      6, 128,  32,   0,   0,   0, 
      0,   0,   6,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      3,   0,   0,   0, 166, 138, 
     32,   0,   0,   0,   0,   0, 
      6,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   3,   0, 
      0,   0, 166, 138,  32,   0, 
      0,   0,   0,   0,   7,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   4,   0,   0,   0, 
    246, 143,  32,   0,   0,   0, 
      0,   0,   7,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      4,   0,   0,   0, 246, 143, 
     32,   0,   0,   0,   0,   0, 
      6,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     17,   0,   0,   7,  66,  32, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   5,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  17,   0, 
      0,   7, 130,  32,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   5,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  17,   0,   0,   8, 
     18,  32,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      5,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,  17,   0, 
      0,   8,  34,  32,  16,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   5,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     17,   0,   0,   8,  66,  32, 
     16,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   5,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   2,   0, 
      0,   0,  16,   0,   0,   8, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  70,  18,  16,   0, 
      1,   0,   0,   0,  70, 130, 
     32,   0,   1,   0,   0,   0, 
      4,   0,   0,   0,  16,   0, 
      0,   8,  34,   0,  16,   0, 
      0,   0,   0,   0,  70,  18, 
     16,   0,   1,   0,   0,   0, 
     70, 130,  32,   0,   1,   0, 
      0,   0,   5,   0,   0,   0, 
     16,   0,   0,   8,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  18,  16,   0,   1,   0, 
      0,   0,  70, 130,  32,   0, 
      1,   0,   0,   0,   6,   0, 
      0,   0,  16,   0,   0,   7, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     68,   0,   0,   5, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   7, 
    114,  32,  16,   0,   2,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     16,   0,   0,   8,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  18,  16,   0,   2,   0, 
      0,   0,  70, 130,  32,   0, 
      1,   0,   0,   0,   4,   0, 
      0,   0,  16,   0,   0,   8, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  70,  18,  16,   0, 
      2,   0,   0,   0,  70, 130, 
     32,   0,   1,   0,   0,   0, 
      5,   0,   0,   0,  16,   0, 
      0,   8,  66,   0,  16,   0, 
      0,   0,   0,   0,  70,  18, 
     16,   0,   2,   0,   0,   0, 
     70, 130,  32,   0,   1,   0, 
      0,   0,   6,   0,   0,   0, 
     16,   0,   0,   7, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  68,   0, 
      0,   5, 130,   0,  16,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   7, 114,  32, 
     16,   0,   3,   0,   0,   0, 
    246,  15,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  16,   0, 
      0,   8,  18,   0,  16,   0, 
      0,   0,   0,   0,  70,  18, 
     16,   0,   3,   0,   0,   0, 
     70, 130,  32,   0,   1,   0, 
      0,   0,   4,   0,   0,   0, 
     16,   0,   0,   8,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  18,  16,   0,   3,   0, 
      0,   0,  70, 130,  32,   0, 
      1,   0,   0,   0,   5,   0, 
      0,   0,  16,   0,   0,   8, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  70,  18,  16,   0, 
      3,   0,   0,   0,  70, 130, 
     32,   0,   1,   0,   0,   0, 
      6,   0,   0,   0,  16,   0, 
      0,   7, 130,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  68,   0,   0,   5, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7, 114,  32,  16,   0, 
      4,   0,   0,   0, 246,  15, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  54,   0,   0,   5, 
     50,  32,  16,   0,   5,   0, 
      0,   0,  70,  16,  16,   0, 
      4,   0,   0,   0,  62,   0, 
      0,   1,  83,  84,  65,  84, 
    116,   0,   0,   0,  61,   0, 
      0,   0,   6,   0,   0,   0, 
      0,   0,   0,   0,  11,   0, 
      0,   0,  57,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0
};
