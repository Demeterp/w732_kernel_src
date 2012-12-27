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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_CMD 28

#define PRESS_OK_KEY		_IO('k', 1)
#define RELEASE_OK_KEY		_IO('k', 2)
#define PRESS_MENU_KEY		_IO('k', 3)
#define RELEASE_MENU_KEY	_IO('k', 4)
#define PRESS_UP_KEY		_IO('k', 5)
#define RELEASE_UP_KEY		_IO('k', 6)
#define PRESS_DOWN_KEY		_IO('k', 7)
#define RELEASE_DOWN_KEY	_IO('k', 8)
#define PRESS_LEFT_KEY		_IO('k', 9)
#define RELEASE_LEFT_KEY	_IO('k', 10)
#define PRESS_RIGHT_KEY		_IO('k', 11)
#define RELEASE_RIGHT_KEY	_IO('k', 12)
#define PRESS_HOME_KEY		_IO('k', 13)
#define RELEASE_HOME_KEY	_IO('k', 14)
#define PRESS_BACK_KEY		_IO('k', 15)
#define RELEASE_BACK_KEY	_IO('k', 16)
#define PRESS_CALL_KEY		_IO('k', 17)
#define RELEASE_CALL_KEY	_IO('k', 18)
#define PRESS_ENDCALL_KEY	_IO('k', 19)
#define RELEASE_ENDCALL_KEY	_IO('k', 20)
#define PRESS_VLUP_KEY		_IO('k', 21)
#define RELEASE_VLUP_KEY	_IO('k', 22)
#define PRESS_VLDOWN_KEY	_IO('k', 23)
#define RELEASE_VLDOWN_KEY	_IO('k', 24)
#define PRESS_FOCUS_KEY		_IO('k', 25)
#define RELEASE_FOCUS_KEY	_IO('k', 26)
#define PRESS_CAMERA_KEY	_IO('k', 27)
#define RELEASE_CAMERA_KEY	_IO('k', 28)

char *cmd_str[] = 
{
    "OK_P\n", "OK_R\n", "MENU_P\n", "MENU_R\n", "UP_P\n", "UP_R\n", "DOWN_P\n", "DOWN_R\n",
    "LEFT_P\n", "LEFT_R\n", "RIGHT_P\n", "RIGHT_R\n", "HOME_P\n", "HOME_R\n", "BACK_P\n", "BACK_R\n",
    "CALL_P\n", "CALL_R\n", "HANG_P\n", "HANG_R\n", "V_UP_P\n", "V_UP_R\n", "V_DOWN_P\n", "V_DOWN_R\n",
    "FOCUS_P\n", "FOCUS_R\n", "CAM_P\n", "CAM_R\n"
};

int main(int argc, char *argv[])
{
    int kpd_fd = 0;
    FILE *pfd = NULL;
    char str[128];
    unsigned int round = 0;
    int match = 0;
    int i = 0;    
    int count = 1;
    int ioctl_num = 0;
    int forever = 0;
    clock_t time;
    unsigned int duration;
    
    if(argc < 2)
    {
        printf("Usage : kpd_auto <CONFIG_FILE_PATH> forever\n");
        return 0;
    }
        
    if(argc==3)
        forever = ((atoi(argv[2]) == 1)? 1 : 0);
    else if (argc==2)
        forever = 0;

    printf("forever = %d\n", forever);
    if((pfd=fopen(argv[1],"r"))==NULL) 
    {
        printf("Can't open file : %s\n", argv[1]);
        exit(1);
    }
    
    kpd_fd = open("/dev/mt6516-kpd", O_RDWR, 0);    
    if(kpd_fd == -1)
    {
        printf("----error: can't open mt6516-kpd----\n");
    }    
    
    do
    {
        rewind(pfd);  
        round++;
        time = clock();
        duration = time / CLOCKS_PER_SEC;
        printf("[KPD AUTO TEST] Already run %8u seconds\n", duration);
        printf("[KPD AUTO TEST] Round %8u ready GO !\n", round);    
        
        while(fgets(str, 128, pfd) != NULL)
        {
            count = 0;
            for(i=0; i < MAX_CMD; i++)
            {        
                if(! strcmp(str, cmd_str[i]))
                {            
                    ioctl_num = _IO('k', i + 1);
                    match = 1;
                    break;
                }
                if(str[0] == '#')
                {
                    //printf("# %c\n", str[1]);
                    count = atoi((str+1));
                    printf("delay = %d sec\n", count);
                    //count = (int)str[1] - 48;
                    break;
                }            
            }
            if(match)
            {
                //printf("count down time is %d\n", count);  
                //printf("ioctl_num = %d\n", ioctl_num);
                match = 0;
                ioctl(kpd_fd, ioctl_num);
            }
            sleep(count);
        }       
    }while(forever);
    fclose(pfd);
    close(kpd_fd);
    return 0;
}

