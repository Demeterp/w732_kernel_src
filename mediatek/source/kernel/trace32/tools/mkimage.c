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


#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PART_MAGIC        0x58881688
#define BLK_SIZE          512

typedef union {
    struct {    
        unsigned int magic;        /* partition magic */
        unsigned int dsize;        /* partition data size */
        char         name[32];     /* partition name */
    } info;
    unsigned char data[BLK_SIZE];
} part_hdr_t;

unsigned int filesize(char *name)
{
    struct stat statbuf;

    if(stat(name, &statbuf) != 0) {
        fprintf(stderr, "Cannot open file %s\n", name);
        exit(0);
    }
    return statbuf.st_size;
}

char *readfile(char *name, unsigned int size)
{
    FILE *f;
    char *buf;

    f = fopen(name, "rb");
    if(f == NULL) {
        fprintf(stderr, "Cannot open file %s\n", name);
        exit(0);
    }
    buf = (char *)malloc(size);
    if (!buf) {
        fprintf(stderr, "error while malloc(%ld)\n", size);
        fclose(f);
        exit(1);
    }
    if(fread(buf, 1, size, f) != size) {
        fprintf(stderr, "Error while reading file %s\n", name);
        fclose(f);
        exit(0);
    }
    fclose(f);
    return buf;
}

int main(int argc, char *argv[])
{
    part_hdr_t part_hdr;
    char *img;
    uint32_t imgsize;

    if(argc != 3) {
        fprintf(stderr, "\nUsage: ./mkimage <image_file> <image_name>\n\n");
        fprintf(stderr, "  e.g. <image_file>: Image, rootfs.gz\n");
        fprintf(stderr, "  e.g. <image_name>: KERNEL, ROOTFS\n\n");
        return 0;
    }
    
    memset(&part_hdr, 0xff, sizeof(part_hdr_t));

    part_hdr.info.magic = PART_MAGIC;
    part_hdr.info.dsize = filesize(argv[1]);
    strncpy(part_hdr.info.name, argv[2], sizeof(part_hdr.info.name));

    /*
    fprintf(stderr, "[MKIMAGE] '%s'(%d bytes)\n", part_hdr.info.name,
        part_hdr.info.dsize);
    */

    img = readfile(argv[1], part_hdr.info.dsize);

    write(STDOUT_FILENO, &part_hdr, sizeof(part_hdr_t));
    write(STDOUT_FILENO, img, part_hdr.info.dsize);

    return 0;
}
