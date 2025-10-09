/****************************************************************************
 * Genesis Plus GX - GUI System
 *
 * Based on libwiigui from snes9xGC (Tantric 2009)
 * Adapted for Genesis Plus GX (2024)
 *
 * gui_imagedata.cpp - Image data handler
 ***************************************************************************/

#include "gui.h"

/**
 * Constructor for the GuiImageData class.
 */
GuiImageData::GuiImageData(const u8 * img)
{
    data = NULL;
    width = 0;
    height = 0;

    // TODO: Implement PNG decoding via PNGU
    // For now, just store the pointer for structure purposes
    if (img)
    {
        // Placeholder - actual PNG decoding will be added later
        // data = DecodePNG(img, &width, &height);
        (void)img; // Suppress unused parameter warning
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
