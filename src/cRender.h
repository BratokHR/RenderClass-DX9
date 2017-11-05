/************************************************************
*
*   ПРОЕКТ: RenderClass
*   ФАЙЛ: cRender.h
*   РАЗРАБОТЧИК: BratokHR
*
*   Этот класс был написан для облегчения работы с Directx
*   и предоставляется "как есть". Вы можете копировать его,
*   изменять и т.д. и т.п. Делайте что хотите, просто пом-
*   ните кто написал это добро:)
*
*   Больше информации тут: https://guthub
*
************************************************************/

#ifndef _RENDER_H_
#define _RENDER_H_

#include <vector>
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define ST_FVF      1
#define ST_TEXTURE  2
#define ST_SHADER   4
#define ST_CONSTANT 8

#define SHADER_VERSION "ps_2_0"

struct D3DVERTEX
{
    float x, y, z, rhw;
    DWORD color;
};

struct D3DTLVERTEX 
{ 
    float x, y, z, w, tu, tv; 
};

typedef struct
{
    LPDIRECT3DTEXTURE9 pTexture;
    LPDIRECT3DSURFACE9 pSurface;
    UINT width;
    UINT height;
} RENDERTEXTURE;

class cRender
{
private:
    bool gInit;
    LPDIRECT3DDEVICE9 gDevice;
    LPD3DXSPRITE sprite_for_draw;
    LPDIRECT3DVERTEXBUFFER9 vertex;
    LPDIRECT3DSURFACE9 old_surface_dummy;
    LPDIRECT3DSURFACE9 old_surface;
    std::vector<LPD3DXFONT> pFonts;
    std::vector<LPDIRECT3DTEXTURE9> pSprites;
    std::vector<RENDERTEXTURE> pTextures;
    std::vector<LPDIRECT3DPIXELSHADER9> pShaders;
    std::vector<LPD3DXLINE> pLines;

    LPDIRECT3DBASETEXTURE9 st_pTexture;
    LPDIRECT3DPIXELSHADER9 st_pShader;
    DWORD st_dwFVF;
    float st_constant[ 4 ];

public:
    void( *InitFunction )( );
    int CreateFont( int size, UINT style, LPCSTR font_name ); // return the font number
    int CreateLine();
    int CreateSprite( LPCVOID data, size_t szData, UINT width = 0, UINT height = 0 );
    int CreateTexture( UINT width, UINT height );
    int CreatePixelShaderAssemble( char *assemble );
    int CreatePixelShader( char *hlsl, char *function_name = "main" );

    void DrawText( LPSTR text, float x, float y, UINT align, DWORD Color, UINT font_num );
    void DrawLine( UINT num_line, float x1, float y1, float x2, float y2, DWORD Color, float size = 1.0f );
    void DrawSprite( float x, float y, UINT texture_num, D3DCOLOR Color = 0xFFFFFFFF );
    void DrawBox( float x, float y, float width, float height, DWORD Color );
    void DrawBorder( float x, float y, float width, float height, float size, DWORD Color );

    bool DrawVertexBegin( float x, float y, float w, float h );
    void DrawVertexEnd();
    void SetTexture( UINT stage, UINT num_texture );

    bool RenderTextureBegin( UINT num_texture );
    void RenderTextureEnd();

    void ClearTextures();

    void SetConstantPixelShaderF( UINT reg, float par1, float par2 = 0, float par3 = 0, float par4 = 0 );
    void SetPixelShader( UINT num_ps );

    void ResetDevice();
    void LostDevice();
    void CheckObjects( LPDIRECT3DDEVICE9 Device );
    UINT SaveState( UINT state );
    void RestartState( UINT state );
};

#endif