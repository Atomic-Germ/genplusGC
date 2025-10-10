/****************************************************************************
 * Genesis Plus GX - GUI Drawing Helper Functions
 *
 * Adapted from snes9xGC video.cpp (Tantric 2009)
 * Modified for Genesis Plus GX (2024)
 *
 * gui_drawing.c - GUI rendering implementation
 ***************************************************************************/

#include <gccore.h>
#include "gui_drawing.h"
#include "gui/font.h"
#include <string.h>
#include <malloc.h>

/* External vmode from gx_video.c */
extern GXRModeObj *vmode;

/* 2D projection matrix for GUI */
static Mtx GUImodelView2D;

/****************************************************************************
 * GUI_InitVideo
 *
 * Initialize GUI rendering system
 ***************************************************************************/
void GUI_InitVideo(void)
{
    /* Setup 2D projection matrix for GUI rendering */
    Mtx44 p;
    
    guOrtho(p, vmode->efbHeight/2, -(vmode->efbHeight/2), 
            -(vmode->fbWidth/2), vmode->fbWidth/2, 100, 1000);
    GX_LoadProjectionMtx(p, GX_ORTHOGRAPHIC);
    
    guMtxIdentity(GUImodelView2D);
    guMtxTransApply(GUImodelView2D, GUImodelView2D, 
                    vmode->fbWidth/2, vmode->efbHeight/2, -100);
}

/****************************************************************************
 * GUI_DrawImg
 *
 * Draw texture at position with rotation, scale, and alpha
 * Adapted from snes9xGC Menu_DrawImg()
 ***************************************************************************/
void GUI_DrawImg(f32 xpos, f32 ypos, u16 width, u16 height, u8 data[],
                 f32 degrees, f32 scaleX, f32 scaleY, u8 alpha)
{
    if (data == NULL)
        return;

    GXTexObj texObj;

    /* Initialize texture object */
    GX_InitTexObj(&texObj, data, width, height, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GX_LoadTexObj(&texObj, GX_TEXMAP0);
    GX_InvalidateTexAll();

    /* Setup texture environment */
    GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    /* Calculate transformation matrix */
    Mtx m, m1, m2, mv;
    width >>= 1;
    height >>= 1;

    /* Identity and scale */
    guMtxIdentity(m1);
    guMtxScaleApply(m1, m1, scaleX, scaleY, 1.0);
    
    /* Rotation */
    guVector axis = (guVector){0, 0, 1};
    guMtxRotAxisDeg(m2, &axis, degrees);
    guMtxConcat(m2, m1, m);

    /* Translation */
    guMtxTransApply(m, m, xpos + width, ypos + height, 0);
    guMtxConcat(GUImodelView2D, m, mv);
    GX_LoadPosMtxImm(mv, GX_PNMTX0);

    /* Draw textured quad */
    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
    
    GX_Position3f32(-width, -height, 0);
    GX_Color4u8(0xFF, 0xFF, 0xFF, alpha);
    GX_TexCoord2f32(0, 0);

    GX_Position3f32(width, -height, 0);
    GX_Color4u8(0xFF, 0xFF, 0xFF, alpha);
    GX_TexCoord2f32(1, 0);

    GX_Position3f32(width, height, 0);
    GX_Color4u8(0xFF, 0xFF, 0xFF, alpha);
    GX_TexCoord2f32(1, 1);

    GX_Position3f32(-width, height, 0);
    GX_Color4u8(0xFF, 0xFF, 0xFF, alpha);
    GX_TexCoord2f32(0, 1);
    
    GX_End();

    /* Restore matrix */
    GX_LoadPosMtxImm(GUImodelView2D, GX_PNMTX0);

    /* Restore rendering state */
    GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);
}

/****************************************************************************
 * GUI_DrawRectangle
 *
 * Draw rectangle (filled or outline) at specified position
 * Adapted from snes9xGC Menu_DrawRectangle()
 ***************************************************************************/
void GUI_DrawRectangle(f32 x, f32 y, f32 width, f32 height, GXColor color, u8 filled)
{
    long n = 4;
    f32 x2 = x + width;
    f32 y2 = y + height;
    guVector v[] = {
        {x, y, 0.0f}, 
        {x2, y, 0.0f}, 
        {x2, y2, 0.0f}, 
        {x, y2, 0.0f}, 
        {x, y, 0.0f}
    };
    
    u8 fmt = GX_TRIANGLEFAN;

    if (!filled)
    {
        fmt = GX_LINESTRIP;
        n = 5;
    }

    GX_Begin(fmt, GX_VTXFMT0, n);
    for (long i = 0; i < n; ++i)
    {
        GX_Position3f32(v[i].x, v[i].y, v[i].z);
        GX_Color4u8(color.r, color.g, color.b, color.a);
    }
    GX_End();
}

/****************************************************************************
 * GUI_DrawText
 *
 * Basic text drawing using bitmap font system
 ***************************************************************************/
void GUI_DrawText(int x, int y, const char *text, int size, GXColor color)
{
    if (!text)
        return;
    
    /* Use existing font system */
    FONT_write((char*)text, size, x, y, 640, color);
}
