/************************************************************
*
*   ПРОЕКТ: RenderClass
*   ФАЙЛ: cRender.cpp
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

#include "cRender.h"

int cRender::CreateFont( int size, UINT style, LPCSTR font_name )
{
    ID3DXFont *nFont;

    D3DXCreateFontA( gDevice, size, 0, style, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, font_name, &nFont );

    pFonts.push_back( nFont );

    return pFonts.size() - 1;
}

int cRender::CreateLine()
{
    ID3DXLine *nLine;

    D3DXCreateLine( gDevice, &nLine );

    pLines.push_back( nLine );

    return pLines.size() - 1;
}

int cRender::CreateSprite( LPCVOID data, size_t szData, UINT width, UINT height )
{
    LPDIRECT3DTEXTURE9 nTexture;

    if ( width == 0 || height == 0 )
        D3DXCreateTextureFromFileInMemory( gDevice, data, szData, &nTexture );
    else
        D3DXCreateTextureFromFileInMemoryEx( gDevice, data, szData, width, height, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &nTexture );

    pSprites.push_back( nTexture );

    return pSprites.size() - 1;
}

int cRender::CreateTexture( UINT width, UINT height )
{
    RENDERTEXTURE nRender;

    gDevice->CreateTexture( width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &nRender.pTexture, NULL );

    nRender.pTexture->GetSurfaceLevel( 0, &nRender.pSurface );

    nRender.width = width;
    nRender.height = height;

    pTextures.push_back( nRender );

    return pTextures.size() - 1;
}

int cRender::CreatePixelShaderAssemble( char *assemble )
{
    LPD3DXBUFFER pshader = NULL;
    LPDIRECT3DPIXELSHADER9 nShader = 0;

    if ( D3DXAssembleShader( assemble, strlen( assemble ), 0, 0, 0, &pshader, 0 ) == D3D_OK ) 
        gDevice->CreatePixelShader( (DWORD*)pshader->GetBufferPointer(), &nShader );

    pShaders.push_back( nShader );

    return pShaders.size() - 1;
}

int cRender::CreatePixelShader( char *hlsl, char *function_name )
{
    LPD3DXBUFFER pshader = NULL;
    LPDIRECT3DPIXELSHADER9 nShader = 0;

    if ( D3DXCompileShader( hlsl, strlen( hlsl ), 0, 0, function_name, SHADER_VERSION, 0, &pshader, 0, 0 ) == D3D_OK )
        gDevice->CreatePixelShader( (DWORD*)pshader->GetBufferPointer(), &nShader );

    pShaders.push_back( nShader );

    return pShaders.size() - 1;
}

void cRender::LostDevice()
{
    size_t i = 0;

    // fonts
    for ( i = 0; i < pFonts.size(); i++ )
    {
        if ( pFonts.at( i ) )
            pFonts.at( i )->OnLostDevice();
    }

    // lines
    for ( i = 0; i < pLines.size(); i++ )
    {
        if ( pLines.at( i ) )
        {
            pLines.at( i )->OnLostDevice();
        }
    }

    // textures
    for ( i = 0; i < pTextures.size(); i++ )
    {
        if ( pTextures.at( i ).pSurface )
        {
            pTextures.at( i ).pSurface->Release();
            pTextures.at( i ).pSurface = NULL;
        }

        if ( pTextures.at( i ).pTexture )
        {
            pTextures.at( i ).pTexture->Release();
            pTextures.at( i ).pTexture = NULL;
        }
    }
    pTextures.clear();

    // surfaces
    if ( old_surface_dummy )
    {
        old_surface_dummy->Release();
        old_surface_dummy = NULL;
    }
    if ( old_surface )
    {
        old_surface->Release();
        old_surface = NULL;
    }

    // sprite
    if ( sprite_for_draw )
        sprite_for_draw->OnLostDevice();

    gInit = false;
}

void cRender::ResetDevice()
{
    size_t i = 0;

    // fonts
    for ( i = 0; i < pFonts.size(); i++ )
    {
        if ( pFonts.at( i ) )
            pFonts.at( i )->OnResetDevice();
    }

    // lines
    for ( i = 0; i < pLines.size(); i++ )
    {
        if ( pLines.at( i ) )
            pLines.at( i )->OnResetDevice();
    }

    // sprite
    if ( sprite_for_draw )
        sprite_for_draw->OnResetDevice();
}

void cRender::CheckObjects( LPDIRECT3DDEVICE9 Device )
{
    if ( !gInit || !gDevice || Device != gDevice )
    {
        gDevice = Device;

        gInit = false;
    }

    if ( !gInit )
    {
        size_t i = 0;

        // fonts
        for ( i = 0; i < pFonts.size(); i++ )
        {
            if ( pFonts.at( i ) )
            {
                pFonts.at( i )->Release();
                pFonts.at( i ) = NULL;
            }
        }
        pFonts.clear();

        // lines
        for ( i = 0; i < pLines.size(); i++ )
        {
            if ( pLines.at( i ) )
            {
                pLines.at( i )->Release();
                pLines.at( i ) = NULL;
            }
        }
        pLines.clear();

        // sprites
        for ( i = 0; i < pSprites.size(); i++ )
        {
            if ( pSprites.at( i ) )
            {
                pSprites.at( i )->Release();
                pSprites.at( i ) = NULL;
            }
        }
        pSprites.clear();

        // shaders
        for ( i = 0; i < pShaders.size(); i++ )
        {
            if ( pShaders.at( i ) )
            {
                pShaders.at( i )->Release();
                pShaders.at( i ) = NULL;
            }
        }
        pShaders.clear();

        // sprite
        if ( sprite_for_draw )
        {
            sprite_for_draw->Release();
            sprite_for_draw = NULL;
        }

        // vertex buffer
        if ( vertex )
        {
            vertex->Release();
            vertex = NULL;
        }

        if ( InitFunction )
        {
            D3DXCreateSprite( gDevice, &sprite_for_draw );
            gDevice->CreateVertexBuffer( sizeof( D3DTLVERTEX ) * 4, 0, ( D3DFVF_XYZRHW | D3DFVF_TEX1 ), D3DPOOL_MANAGED, &vertex, NULL );

            gDevice->GetRenderTarget( 0, &old_surface );
            gDevice->GetDepthStencilSurface( &old_surface_dummy );

            InitFunction();

            gInit = true;
        }
    }
}

UINT cRender::SaveState( UINT state )
{
    if ( state & ST_TEXTURE)
        gDevice->GetTexture( 0, &st_pTexture );

    if ( state & ST_FVF )
        gDevice->GetFVF( &st_dwFVF );

    if ( state & ST_SHADER )
        gDevice->GetPixelShader( &st_pShader );

    if ( state & ST_CONSTANT )
        gDevice->GetPixelShaderConstantF( 0, st_constant, 1 );

    return state;
}

void cRender::RestartState( UINT state )
{
    if ( state & ST_TEXTURE )
        gDevice->SetTexture( 0, st_pTexture );

    if ( state & ST_FVF )
        gDevice->SetFVF( st_dwFVF );

    if ( state & ST_SHADER )
        gDevice->SetPixelShader( st_pShader );

    if ( state & ST_CONSTANT )
        gDevice->SetPixelShaderConstantF( 0, st_constant, 1 );
}

void cRender::DrawText( LPSTR text, float x, float y, UINT align, DWORD Color, UINT font_num )
{
    if ( !pFonts.at( font_num ) )
        return;

    RECT rect = { (long)x, (long)y, (long)x, (long)y };

    pFonts.at( font_num )->DrawTextA( NULL, text, -1, &rect, align | DT_NOCLIP, Color );
}

void cRender::DrawLine( UINT num_line, float x1, float y1, float x2, float y2, DWORD Color, float size )
{
    if ( !pLines.at( num_line ) )
        return;

    D3DXVECTOR2 points[] = { { x1, y1 },{ x2, y2 } };

    pLines.at( num_line )->SetWidth( size );
    //pLines.at( num_line )->SetAntialias( true );

    pLines.at( num_line )->Begin();
    pLines.at( num_line )->Draw( points, 2, Color );
    pLines.at( num_line )->End();
}

void cRender::DrawSprite( float x, float y, UINT texture_num, D3DCOLOR color )
{
    if ( !pSprites.at( texture_num ) )
        return;

    sprite_for_draw->Begin( D3DXSPRITE_ALPHABLEND );
    sprite_for_draw->Draw( pSprites.at( texture_num ), NULL, NULL, &D3DXVECTOR3( x, y, 0 ), color );
    sprite_for_draw->End();
}

void cRender::DrawBox( float x, float y, float width, float height, DWORD Color )
{
    D3DVERTEX vertices[ 4 ] = { 
        { x, y, 0, 1.0f, Color },
        { x + width, y, 0, 1.0f, Color },
        { x, y + height, 0, 1.0f, Color },
        { x + width, y + height, 0, 1.0f, Color } 
    };

    UINT state = SaveState( ST_FVF | ST_TEXTURE );

    gDevice->SetTexture( 0, NULL );
    gDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );

    gDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertices, sizeof( D3DVERTEX ) );

    RestartState( state );
}

void cRender::DrawBorder( float x, float y, float width, float height, float size, DWORD Color )
{
    DrawBox( x - size, y - size, width + 2 * size, size, Color );
    DrawBox( x + width, y - size, size, height + size * 2, Color );
    DrawBox( x - size, y + height, width + size * 2, size, Color );
    DrawBox( x - size, y - size, size, height + size * 2, Color );
}

bool cRender::DrawVertexBegin( float x, float y, float w, float h )
{
    if ( !vertex )
        return false;

    D3DTLVERTEX Vertex[ 4 ] = { 
        { x, y, 0, 1, 0, 0 },
        { x + w, y, 0, 1, 1, 0 },
        { x, y + h, 0, 1, 0, 1 },
        { x + w, y + h, 0, 1, 1, 1 } 
    };

    void *pVertexBuffer = NULL;
    vertex->Lock( 0, sizeof( D3DTLVERTEX ) * 4, &pVertexBuffer, 0 );
    memcpy( pVertexBuffer, Vertex, sizeof( D3DTLVERTEX ) * 4 );
    vertex->Unlock();

    return true;
}

void cRender::DrawVertexEnd()
{
    gDevice->SetStreamSource( 0, vertex, 0, sizeof( D3DTLVERTEX ) );
    gDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
    gDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}

void cRender::SetTexture( UINT stage, UINT num_texture )
{
    if ( !pTextures.at( num_texture ).pTexture )
        return;

    gDevice->SetTexture( stage, pTextures.at( num_texture ).pTexture );
}

bool cRender::RenderTextureBegin( UINT num_texture )
{
    if ( !pTextures.at( num_texture ).pTexture )
        return false;

    return ( gDevice->SetRenderTarget( 0, pTextures.at( num_texture ).pSurface ) == D3D_OK );
}

void cRender::RenderTextureEnd()
{
    gDevice->SetDepthStencilSurface( old_surface_dummy );
    gDevice->SetRenderTarget( 0, old_surface );
}

void cRender::ClearTextures()
{
    for ( UINT i = 0; i < pTextures.size(); i++ )
    {
        if ( RenderTextureBegin( i ) )
        {
            D3DRECT rect = { 0, 0, (long)pTextures.at( i ).width, (long)pTextures.at( i ).height };
            gDevice->Clear( 1, &rect, ( D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ), 0, 0, 0 );

            RenderTextureEnd();
        }
    }
}

void cRender::SetConstantPixelShaderF( UINT reg, float par1, float par2, float par3, float par4 )
{
    float params[ 4 ] = { par1, par2, par3, par4 };

    gDevice->SetPixelShaderConstantF( reg, params, 1 );
}

void cRender::SetPixelShader( UINT num_ps )
{
    if ( !pShaders.at( num_ps ) )
        return;

    gDevice->SetPixelShader( pShaders.at( num_ps ) );
}