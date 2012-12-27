#ifndef __G2D_DRV_API_H__
#define __G2D_DRV_API_H__

#define G2D_MAX_LAYER 3

#define G2D_COLOR_FORMAT_RGB565                              0x01
#define G2D_COLOR_FORMAT_UYVY                                0x02
#define G2D_COLOR_FORMAT_RGB888                              0x03
#define G2D_COLOR_FORMAT_ARGB8888                            0x04
#define G2D_COLOR_FORMAT_PARGB8888                           0x05
#define G2D_COLOR_FORMAT_XRGB8888                            0x06
#define G2D_COLOR_FORMAT_BGRA8888                            0x0C
#define G2D_COLOR_FORMAT_BGR888                              0x13
#define G2D_COLOR_FORMAT_ABGR8888                            0x14
#define G2D_COLOR_FORMAT_PABGR8888                           0x15
#define G2D_COLOR_FORMAT_XBGR8888                            0x16

#define G2D_LX_CON_ROTATE_0                                  0x00000
#define G2D_LX_CON_ROTATE_H_FLIP_90                          0x10000
#define G2D_LX_CON_ROTATE_H_FLIP_0                           0x20000
#define G2D_LX_CON_ROTATE_90                                 0x30000
#define G2D_LX_CON_ROTATE_H_FLIP_180                         0x40000
#define G2D_LX_CON_ROTATE_270                                0x50000
#define G2D_LX_CON_ROTATE_180                                0x60000
#define G2D_LX_CON_ROTATE_H_FLIP_270                         0x70000

typedef struct {
    int x;
    int y;
} g2d_coordinate_t;

typedef struct {
    unsigned int addr;

    g2d_coordinate_t offset;

    unsigned int width;
    unsigned int height;
    unsigned int pitch;
    unsigned int format;
    unsigned int transform;

    unsigned int sdxdx;
    unsigned int sdxdy;
    unsigned int sdydx;
    unsigned int sdydy;
    unsigned int sx_init;
    unsigned int sy_init;

    unsigned int enable;
} g2d_layer_t;

typedef struct {
    // affine transform config
    int enable_affine_transform;
    int affine_smpl;

    // Enable alpha blending bitblt:
    //    (1) Per-pixel alpha blending if source with alpha channel
    //    (2) Constant alpha blending if source w/o alpha channel
    int enable_alpha_blending;
    unsigned char const_alpha;  // only used in case (2)

    int enable_bg_color;
    unsigned int bg_color;

    int end_of_frame;
    int fb_id;

    int num_layer;
    g2d_layer_t src[G2D_MAX_LAYER];
    g2d_layer_t dst;
} g2d_context_t;


typedef struct {
    int idx;
    unsigned int addr;
    unsigned int width;
    unsigned int height;
    unsigned int pitch;
    unsigned int format;
} g2d_buffer_t;


/*******************************************************************************/

int g2d_drv_get_status(void);
void g2d_drv_reset(void);
void g2d_drv_run(g2d_context_t *g2d_ctx);

/*******************************************************************************/

#define G2D_IOCTL_MAGIC      'G'

#define G2D_IOCTL_ASYNCCMD     _IOW(G2D_IOCTL_MAGIC, 1, g2d_context_t)
#define G2D_IOCTL_SYNCCMD      _IOW(G2D_IOCTL_MAGIC, 2, g2d_context_t)

#define G2D_IOCTL_CONNECTFB    _IO (G2D_IOCTL_MAGIC, 5)
#define G2D_IOCTL_DISCONNECTFB _IO (G2D_IOCTL_MAGIC, 7)
#define G2D_IOCTL_GETFBBUFF    _IOR(G2D_IOCTL_MAGIC, 9, g2d_buffer_t)

#endif // __G2D_DRV_API_H__
