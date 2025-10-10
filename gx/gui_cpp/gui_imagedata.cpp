/****************************************************************************
 * Genesis Plus GX - GUI System
 *
 * Based on libwiigui from snes9xGC (Tantric 2009)
 * Adapted for Genesis Plus GX (2024)
 *
 * gui_imagedata.cpp - Image data handler
 ***************************************************************************/

#include "gui.h"
#include <string.h>
#include <malloc.h>

/* C interface to Genesis Plus GX texture loading */
extern "C" {
    typedef struct
    {
        u32 width;
        u32 height;
        u8 *data;
    } gx_texture;
    
    gx_texture *gxTextureOpenPNG(const u8 *png_data, FILE *png_file);
    void gxTextureClose(gx_texture **texture);
}

/**
 * Constructor for the GuiImageData class.
 */
GuiImageData::GuiImageData(const u8 * img)
{
    data = NULL;
    width = 0;
    height = 0;

    if (!img)
        return;
    
    /* Use Genesis Plus GX PNG loader */
    gx_texture *texture = gxTextureOpenPNG(img, NULL);
    
    if (texture)
    {
        width = texture->width;
        height = texture->height;
        
        /* Copy texture data (GuiImageData owns the data) */
        if (texture->data)
        {
            int len = width * height * 4;  /* RGBA8 format */
            if (len % 32) 
                len += (32 - len % 32);
            
            data = (u8 *)memalign(32, len);
            if (data)
            {
                memcpy(data, texture->data, len);
                DCFlushRange(data, len);
            }
        }
        
        /* Free the temporary texture */
        gxTextureClose(&texture);
    }
}

/**
 * Destructor for the GuiImageData class.
 */
GuiImageData::~GuiImageData()
{
    if (data)
    {
        free(data);
        data = NULL;
    }
}

u8 * GuiImageData::GetImage()
{
    return data;
}

int GuiImageData::GetWidth()
{
    return width;
}

int GuiImageData::GetHeight()
{
    return height;
}
