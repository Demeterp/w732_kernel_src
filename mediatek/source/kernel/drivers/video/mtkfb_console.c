/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#include <linux/font.h>
#include <linux/string.h>
#include <linux/semaphore.h>
#include <linux/slab.h>

#include <disp_drv_platform.h>
#include "mtkfb_console.h"

// ---------------------------------------------------------------------------

typedef struct
{
    struct semaphore sem;

    unsigned char *fb_addr;
    unsigned int fb_width;
    unsigned int fb_height;
    unsigned int fb_bpp;
    unsigned int fg_color;
    unsigned int bg_color;
    unsigned int rows;
    unsigned int cols;
    unsigned int cursor_row;
    unsigned int cursor_col;
	unsigned int font_width;
	unsigned int font_height;
} MFC_CONTEXT;

// ---------------------------------------------------------------------------

#define MFC_WIDTH           (ctxt->fb_width)
#define MFC_HEIGHT          (ctxt->fb_height)
#define MFC_BPP             (ctxt->fb_bpp)
#define MFC_PITCH           (MFC_WIDTH * MFC_BPP)

#define MFC_FG_COLOR        (ctxt->fg_color)
#define MFC_BG_COLOR        (ctxt->bg_color)

#define MFC_FONT            font_vga_8x16
#define MFC_FONT_WIDTH      (MFC_FONT.width)
#define MFC_FONT_HEIGHT     (MFC_FONT.height)
#define MFC_FONT_DATA       (MFC_FONT.data)

#define MFC_ROW_SIZE        (MFC_FONT_HEIGHT * MFC_PITCH)
#define MFC_ROW_FIRST       ((BYTE*)(ctxt->fb_addr))
#define MFC_ROW_SECOND      (MFC_ROW_FIRST + MFC_ROW_SIZE)
#define MFC_ROW_LAST        (MFC_ROW_FIRST + MFC_SIZE - MFC_ROW_SIZE)
#define MFC_SIZE            (MFC_ROW_SIZE * ctxt->rows)
#define MFC_SCROLL_SIZE     (MFC_SIZE - MFC_ROW_SIZE)

#define MAKE_TWO_RGB565_COLOR(high, low)  (((low) << 16) | (high))

#define MFC_LOCK()                                                          \
    do {                                                                    \
        if (down_interruptible(&ctxt->sem)) {                               \
            printk("[MFC] ERROR: Can't get semaphore in %s()\n",            \
                   __FUNCTION__);                                           \
            ASSERT(0);                                                      \
            return MFC_STATUS_LOCK_FAIL;                                    \
        }                                                                   \
    } while (0)
    
#define MFC_UNLOCK()                                                        \
    do {                                                                    \
        up(&ctxt->sem);                                                     \
    } while (0)


// ---------------------------------------------------------------------------


static void _MFC_DrawChar(MFC_CONTEXT *ctxt, UINT32 x, UINT32 y, char c)
{
    BYTE ch = *((BYTE*)&c);
	const BYTE *cdat;
    BYTE *dest;
	INT32 rows, offset;

    int font_draw_table16[4];

    ASSERT(x <= (MFC_WIDTH - MFC_FONT_WIDTH));
    ASSERT(y <= (MFC_HEIGHT - MFC_FONT_HEIGHT));

    offset = y * MFC_PITCH + x * MFC_BPP;
    dest = (MFC_ROW_FIRST + offset);

    switch (MFC_BPP) {
    case 2:
        font_draw_table16[0] = MAKE_TWO_RGB565_COLOR(MFC_BG_COLOR, MFC_BG_COLOR);
        font_draw_table16[1] = MAKE_TWO_RGB565_COLOR(MFC_BG_COLOR, MFC_FG_COLOR);
        font_draw_table16[2] = MAKE_TWO_RGB565_COLOR(MFC_FG_COLOR, MFC_BG_COLOR);
        font_draw_table16[3] = MAKE_TWO_RGB565_COLOR(MFC_FG_COLOR, MFC_FG_COLOR);

        cdat = (const BYTE*)MFC_FONT_DATA + ch * MFC_FONT_HEIGHT;
        
        for (rows = MFC_FONT_HEIGHT; rows--; dest += MFC_PITCH)
        {
            BYTE bits = *cdat++;
        
            ((UINT32*)dest)[0] = font_draw_table16[bits >> 6];
            ((UINT32*)dest)[1] = font_draw_table16[bits >> 4 & 3];
            ((UINT32*)dest)[2] = font_draw_table16[bits >> 2 & 3];
            ((UINT32*)dest)[3] = font_draw_table16[bits & 3];
        }
        break;

    default:
        ASSERT(0);
    }
}


static void _MFC_ScrollUp(MFC_CONTEXT *ctxt)
{
    const UINT32 BG_COLOR = MAKE_TWO_RGB565_COLOR(MFC_BG_COLOR, MFC_BG_COLOR);
    
    UINT32 *ptr = (UINT32 *)MFC_ROW_LAST;
    int i = MFC_ROW_SIZE / sizeof(UINT32);

    memcpy(MFC_ROW_FIRST, MFC_ROW_SECOND, MFC_SCROLL_SIZE);
    
    while(--i >= 0) {
        *ptr ++ = BG_COLOR;
    }
}


static void _MFC_Newline(MFC_CONTEXT *ctxt)
{
    ///Bin:add for filling the color for the blank of this column
    while(ctxt->cursor_col < ctxt->cols)
    {
        _MFC_DrawChar(ctxt,
                      ctxt->cursor_col * MFC_FONT_WIDTH,
                      ctxt->cursor_row * MFC_FONT_HEIGHT,
                      ' ');

        ++ ctxt->cursor_col;    
    }
    ++ ctxt->cursor_row;
    ctxt->cursor_col = 0;

    /* Check if we need to scroll the terminal */
    if (ctxt->cursor_row >= ctxt->rows) {
        /* Scroll everything up */
        _MFC_ScrollUp(ctxt);

        /* Decrement row number */
        -- ctxt->cursor_row;
    }
}


#define CHECK_NEWLINE()                     \
    do {                                    \
        if (ctxt->cursor_col >= ctxt->cols) \
            _MFC_Newline(ctxt);             \
    } while(0)

static void _MFC_Putc(MFC_CONTEXT *ctxt, const char c)
{
    CHECK_NEWLINE();

    switch (c) {
    case '\n':		/* next line */
        _MFC_Newline(ctxt);
        break;

    case '\r':		/* carriage return */
        ctxt->cursor_col = 0;
        break;

    case '\t':		/* tab 8 */
        ctxt->cursor_col += 8;
        ctxt->cursor_col &= ~0x0007;
        CHECK_NEWLINE();
        break;

    default:		/* draw the char */
        _MFC_DrawChar(ctxt,
                      ctxt->cursor_col * MFC_FONT_WIDTH,
                      ctxt->cursor_row * MFC_FONT_HEIGHT,
                      c);
        ++ ctxt->cursor_col;
        CHECK_NEWLINE();
    }
}

// ---------------------------------------------------------------------------

MFC_STATUS MFC_Open(MFC_HANDLE *handle,
                    void *fb_addr,
                    unsigned int fb_width,
                    unsigned int fb_height,
                    unsigned int fb_bpp,
                    unsigned int fg_color,
                    unsigned int bg_color)
{
    MFC_CONTEXT *ctxt = NULL;

    if (NULL == handle || NULL == fb_addr) 
        return MFC_STATUS_INVALID_ARGUMENT;

    if (fb_bpp != 2)
        return MFC_STATUS_NOT_IMPLEMENTED;  // only support RGB565

    ctxt = kzalloc(sizeof(MFC_CONTEXT), GFP_KERNEL);
    if (!ctxt) return MFC_STATUS_OUT_OF_MEMORY;

//    init_MUTEX(&ctxt->sem);
	sema_init(&ctxt->sem, 1);	
    ctxt->fb_addr   = fb_addr;
    ctxt->fb_width  = fb_width;
    ctxt->fb_height = fb_height;
    ctxt->fb_bpp    = fb_bpp;
    ctxt->fg_color  = fg_color;
    ctxt->bg_color  = bg_color;
    ctxt->rows      = fb_height / MFC_FONT_HEIGHT;
    ctxt->cols      = fb_width / MFC_FONT_WIDTH;
	ctxt->font_width = MFC_FONT_WIDTH;
	ctxt->font_height = MFC_FONT_HEIGHT;

    *handle = ctxt;

    return MFC_STATUS_OK;
}


MFC_STATUS MFC_Close(MFC_HANDLE handle)
{
    if (!handle)
        return MFC_STATUS_INVALID_ARGUMENT;

    kfree(handle);

    return MFC_STATUS_OK;
}


MFC_STATUS MFC_SetColor(MFC_HANDLE handle,
                        unsigned int fg_color, 
                        unsigned int bg_color)
{
    MFC_CONTEXT *ctxt = (MFC_CONTEXT *)handle;

    if (!ctxt) 
        return MFC_STATUS_INVALID_ARGUMENT;

    MFC_LOCK();
    ctxt->fg_color = fg_color;
    ctxt->bg_color = bg_color;
    MFC_UNLOCK();

    return MFC_STATUS_OK;
}


MFC_STATUS MFC_ResetCursor(MFC_HANDLE handle)
{
    MFC_CONTEXT *ctxt = (MFC_CONTEXT *)handle;

    if (!ctxt) 
        return MFC_STATUS_INVALID_ARGUMENT;

    MFC_LOCK();
    ctxt->cursor_row = ctxt->cursor_col = 0;
    MFC_UNLOCK();

    return MFC_STATUS_OK;
}


MFC_STATUS MFC_Print(MFC_HANDLE handle, const char *str)
{
    MFC_CONTEXT *ctxt = (MFC_CONTEXT *)handle;
	int count = 0;

    if (!ctxt || !str) 
        return MFC_STATUS_INVALID_ARGUMENT;

    MFC_LOCK();

    count = strlen(str);

	while (count--)
		_MFC_Putc(ctxt, *str++);

    MFC_UNLOCK();

    return MFC_STATUS_OK;
}

MFC_STATUS MFC_SetMem(MFC_HANDLE handle, const char *str, UINT32 color)
{
    MFC_CONTEXT *ctxt = (MFC_CONTEXT *)handle;
	int count = 0;
	int i,j;
	UINT32 *ptr;
    
	if (!ctxt || !str) 
        return MFC_STATUS_INVALID_ARGUMENT;

    MFC_LOCK();

    count = strlen(str);
	count = count * MFC_FONT_WIDTH;

	for(j=0;j<MFC_FONT_HEIGHT;j++){
		ptr = (UINT32 *)(ctxt->fb_addr + (j + 1) * MFC_PITCH - count * ctxt->fb_bpp);
		for(i=0;i<count * ctxt->fb_bpp/sizeof(UINT32);i++){
			*ptr ++ = color;
		}
	}

    MFC_UNLOCK();

    return MFC_STATUS_OK;
}

MFC_STATUS MFC_LowMemory_Printf(MFC_HANDLE handle, const char *str, UINT32 fg_color, UINT32 bg_color)
{
    MFC_CONTEXT *ctxt = (MFC_CONTEXT *)handle;
	int count = 0;
	unsigned int col, row, fg_color_mfc, bg_color_mfc;
    
	if (!ctxt || !str) 
        return MFC_STATUS_INVALID_ARGUMENT;

    MFC_LOCK();

    count = strlen(str);
////store cursor_col and row for printf low memory char temply
	row = ctxt->cursor_row;
	col = ctxt->cursor_col;
	ctxt->cursor_row = 0;
	ctxt->cursor_col = ctxt->cols - count;
    fg_color_mfc = ctxt->fg_color;
    bg_color_mfc = ctxt->bg_color;
	
///////////
    ctxt->fg_color = fg_color;
    ctxt->bg_color = bg_color;
	while (count--)
		_MFC_Putc(ctxt, *str++);

////restore cursor_col and row for printf low memory char temply
	ctxt->cursor_row = row;
	ctxt->cursor_col = col;
    ctxt->fg_color = fg_color_mfc;
    ctxt->bg_color = bg_color_mfc;
///////////

	
    MFC_UNLOCK();

    return MFC_STATUS_OK;
}
