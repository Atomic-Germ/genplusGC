/****************************************************************************
 * Genesis Plus GX - GUI Drawing Helper Functions
 *
 * Adapted from snes9xGC video.cpp (Tantric 2009)
 * Modified for Genesis Plus GX (2024)
 *
 * gui_drawing.h - GUI rendering helper functions
 ***************************************************************************/

#ifndef _GUI_DRAWING_H_
#define _GUI_DRAWING_H_

#include <gccore.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize GUI rendering system */
void GUI_InitVideo(void);

/* Draw image/texture at position with transformations */
void GUI_DrawImg(f32 xpos, f32 ypos, u16 width, u16 height, u8 data[],
                 f32 degrees, f32 scaleX, f32 scaleY, u8 alpha);

/* Draw rectangle (filled or outline) */
void GUI_DrawRectangle(f32 x, f32 y, f32 width, f32 height, GXColor color, u8 filled);

/* Draw text at position (basic implementation) */
void GUI_DrawText(int x, int y, const char *text, int size, GXColor color);

#ifdef __cplusplus
}
#endif

#endif /* _GUI_DRAWING_H_ */
