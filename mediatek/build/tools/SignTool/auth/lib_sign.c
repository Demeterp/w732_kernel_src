
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <type_defs.h>
#include <sign_img.h>
#include <sign_header.h>
#include <sec_wrapper.h>
#include <sign_extension.h>

#define MOD                         "SignLib"

#define DBG                         printf


SEC_CRYPTO_HASH_TYPE g_hash_type = SEC_CRYPTO_HASH_SHA1;
SEC_CRYPTO_SIGNATURE_TYPE g_sig_type = SEC_CRYPTO_SIG_RSA1024;

void * mcpy(void *dst, const void *src, int  cnt)
{
    char *tmp = dst;
    const char *s = src;

    while (cnt--)
    {
        *tmp++ = *s++;
    }
    
    return dst;
}

int mcmp (const void *cs, const void *ct, int cnt)
{
    const uchar *su1, *su2;
    int res = 0;

    for (su1 = cs, su2 = ct; 0 < cnt; ++su1, ++su2, cnt--)
    {
        if ((res = *su1 - *su2) != 0)
        {
            break;
        }
    }
    
    return res;
}


int chk_img (char *img_name)
{    
    uint32 br = 0;
    FILE *img_fd = fopen(img_name,"r");
    SEC_IMG_HEADER sec;    

    if(img_fd == 0)
    {
        MSG("[%s] %s not found\n",MOD,img_name);
        return -1;
    }    
    
    br = fread((uchar *)&sec,1,sizeof(SEC_IMG_HEADER),img_fd);
    
    if(br == 0)
    {
        MSG("\n[%s] %s is empty\n",MOD,img_name);
        
        return -2;
    }
    
    if(SEC_IMG_MAGIC == sec.magic_num)
    {           
        MSG("signed already (0x%x)\n",sec.magic_num);
        
        return -3;
    }

    return 0;
}


int imp_key (char *kf, char *kp, char gen_hdr, FILE *gen_fd)
{
    CUSTOMER_SEC_INTER cust;
    uchar *r0,*r1,*r2;    
    uchar line[300];    

    /* ------------------------------------- */
    /* import key                            */
    /* ------------------------------------- */    
    FILE *key_fd = fopen(kf,"r");        
    if(key_fd == 0)
    {    
        MSG("[%s] %s not found\n",MOD,kf);
        return -1;
    }
    else
    {
        while(fgets(line,300,key_fd) != NULL)
        {
            r0 = strtok(line," ");
            r1 = strtok(NULL, " ");
            r2 = strtok(NULL, " \n");

            /* ------------------------------------- */
            /* parse key                             */
            /* ------------------------------------- */            
            if ( 0 == mcmp(r0,CUSTOM_RSA_N,strlen(CUSTOM_RSA_N)))
            {
                MSG("[%s] import CUSTOM_RSA_N\n",MOD);
                
                mcpy(cust.key_rsa_n,r2+PREFIX_SIZE,sizeof(cust.key_rsa_n));    

                /* ------------------------------------- */
                /* write key to generated header file    */
                /* ------------------------------------- */                
                if (TRUE == gen_hdr)                
                {
                    fwrite ("#define ",1,strlen("#define "),gen_fd);      
                    fwrite (kp,1,strlen(kp),gen_fd); 
                    fwrite ("_",1,strlen("_"),gen_fd); 
                    fwrite (CUSTOM_RSA_N,1,strlen(CUSTOM_RSA_N),gen_fd);                     
                    fwrite (" \"",1,strlen(" \""),gen_fd);            
                    fwrite (cust.key_rsa_n,1,sizeof(cust.key_rsa_n),gen_fd);      
                    fwrite ("\"\n",1,strlen("\"\n"),gen_fd);                   
                }
            }

            else if ( 0 == mcmp(r0,CUSTOM_RSA_D,strlen(CUSTOM_RSA_D)))
            {
                MSG("[%s] import CUSTOM_RSA_D\n",MOD);
                
                mcpy(cust.key_rsa_d,r2+PREFIX_SIZE,sizeof(cust.key_rsa_d));    
            }

            else if ( 0 == mcmp(r0,CUSTOM_RSA_E,strlen(CUSTOM_RSA_E)))
            {
                MSG("[%s] import CUSTOM_RSA_E\n",MOD);
                
                mcpy(cust.key_rsa_e,r2+PREFIX_SIZE,sizeof(cust.key_rsa_e));    

                /* ------------------------------------- */
                /* write rsa key to generate header file */
                /* ------------------------------------- */                
                if (TRUE == gen_hdr)                
                {                
                    fwrite ("#define ",1,strlen("#define "),gen_fd);      
                    fwrite (kp,1,strlen(kp),gen_fd); 
                    fwrite ("_",1,strlen("_"),gen_fd); 
                    fwrite (CUSTOM_RSA_E,1,strlen(CUSTOM_RSA_E),gen_fd);      
                    fwrite (" \"",1,strlen(" \""),gen_fd);
                    fwrite (cust.key_rsa_e,1,sizeof(cust.key_rsa_e),gen_fd);      
                    fwrite ("\"\n",1,strlen("\"\n"),gen_fd);   
                }
            }
            else
            {
                MSG("[%s] %s format error\n",MOD,kf);
                return -1;        
            }            
        }
    }

    /* ------------------------------------- */
    /* initialize internal crypto engine     */
    /* ------------------------------------- */    
    cust_init_key(cust.key_rsa_n, sizeof(cust.key_rsa_n),
                    cust.key_rsa_d, sizeof(cust.key_rsa_d),
                    cust.key_rsa_e, sizeof(cust.key_rsa_e));

    /* close key file */
    fclose (key_fd);    

    return 0;
}

int gen_hdr (char *cfg_name, char *hdr_name, char* img_name, char *hs_name)
{
    SEC_IMG_HEADER sec = {0};
    uchar *r0,*r1,*r2;    
    uchar line[300];    
    SEC_EXTENTION_CFG *p_ext_cfg = (SEC_EXTENTION_CFG *)get_ext_cfg();    
    bool b_ext_offset[MAX_VERITY_COUNT] = {0};
    bool b_ext_length[MAX_VERITY_COUNT] = {0};
    unsigned int tmp, i;

    p_ext_cfg->chunk_size = SEC_CHUNK_SIZE_ZERO;
    /* ------------------------------------- */
    /* open image config file                */
    /* ------------------------------------- */    
    FILE *config_fd = fopen(cfg_name,"r");        
    
    if(config_fd == 0)
    {    
        MSG("[%s] %s not found\n",MOD,cfg_name);
        return -1;
    }
    else
    {
        while(fgets(line,300,config_fd) != NULL)
        {
            r0 = strtok(line," ");
            r1 = strtok(NULL, " ");
            r2 = strtok(NULL, " \n");

            /* ------------------------------------- */
            /* parse image config                    */
            /* ------------------------------------- */
            if ( 0 == mcmp(r0,CUSTOM_NAME,strlen(CUSTOM_NAME)))
            {
                if(0 == mcmp(r2,"NULL",strlen("NULL")))
                {
                    MSG("[%s] empty customer name '%s'\n",MOD,r2);
                    memset(sec.cust_name,0,sizeof(sec.cust_name));  
                }
                else
                {
                    mcpy(sec.cust_name,r2,sizeof(sec.cust_name));    
                }
            }
            else if ( 0 == mcmp(r0,IMAGE_VERSION,strlen(IMAGE_VERSION)))
            {
                sec.img_ver = atoi(r2);
            }
            else if ( 0 == mcmp(r0,VERIFY_OFFSET,strlen(VERIFY_OFFSET)))
            {
                if (strlen(VERIFY_OFFSET) != strlen(r0))
                {
                    sscanf(r0, "VERIFY_OFFSET[%d]", &tmp);
                    p_ext_cfg->verify_offset[tmp] = atoi(r2);
                    b_ext_offset[tmp] = TRUE;
                    MSG("[%s] VERIFY_OFFSET[%d]=%d\n",MOD, tmp, p_ext_cfg->verify_offset[tmp]);
                }
                else
                {
                    sec.s_off = atoi(r2);
                }
            }
            else if ( 0 == mcmp(r0,VERIFY_LENGTH,strlen(VERIFY_LENGTH)))
            {
                if (strlen(VERIFY_LENGTH) != strlen(r0))
                {
                    sscanf(r0, "VERIFY_LENGTH[%d]", &tmp);
                    p_ext_cfg->verify_length[tmp] = atoi(r2);
                    b_ext_length[tmp] = TRUE;
                    MSG("[%s] VERIFY_LENGTH[%d]=%d\n",MOD, tmp, p_ext_cfg->verify_length[tmp]);
                }
                else
                {
                    sec.s_len = atoi(r2);   
                }       
            }
            else if ( 0 == mcmp(r0,VERIFY_COUNT,strlen(VERIFY_COUNT)))
            {
                p_ext_cfg->verify_count = atoi(r2);
                set_hdr_version(SEC_HDR_V3);
                MSG("[%s] VERIFY_COUNT=%d\n",MOD, p_ext_cfg->verify_count);
            }
            else if ( 0 == mcmp(r0,CHUNK_SIZE,strlen(CHUNK_SIZE)))
            {
                p_ext_cfg->chunk_size = atoi(r2);
                MSG("[%s] CHUNK_SIZE=%d\n",MOD, p_ext_cfg->chunk_size);
            }
            else if (';' == r0[0])
            {
    
            }
            else if (NULL == r1)
            {
    
            }            
            else
            {
                MSG("[%s] %s format error\n",MOD,cfg_name);
                return -1;        
            }            
        }

        /* check config format for v3 */
        if( is_hdr_version3() )
        {
            for(i=0 ; i<p_ext_cfg->verify_count ; i++)
            {
                if( !b_ext_offset[i] || !b_ext_length[i])
                {
                    MSG("[%s] %s config setting error\n",MOD,cfg_name);
                    return -1;    
                }
            }
        }
    }

    /* ------------------------------------- */
    /* fill and write header                 */
    /* ------------------------------------- */  
    COMPILE_ASSERT(SEC_IMG_HDR_SZ == sizeof(sec));
    
    FILE *hdr_fd = fopen(hdr_name,"wb");

    /* config common part */
    FILE *img_fd = fopen(img_name,"r"); 
    if(img_fd == 0)
    {
        MSG("[%s] %s not found\n",MOD,img_name);
        goto img_err;
    }    
    fseek(img_fd, 0, SEEK_END);
    sec.img_len = ftell(img_fd);
    fclose(img_fd);
    sec.magic_num = SEC_IMG_MAGIC;
    sec.img_off = SEC_IMG_HDR_SZ;
    sec.sig_off = sec.img_off + sec.img_len;

    if( is_hdr_version3() )
    {
        /* config header for v3 */
        if(config_header_v3_chk(&sec))
            goto check_err;
    }
    else
    {
        /* config header for v1 and v2 */
        if(config_header_v1_v2_chk(&sec))
            goto check_err;
    }
    

    /* ------------------------------------- */
    /* write to image                        */
    /* ------------------------------------- */    
    fwrite (&sec, 1 , sizeof(sec) , hdr_fd);      

    /* ------------------------------------- */
    /* dump information                      */
    /* ------------------------------------- */    
    MSG("[%s] hdr.magic_num     = 0x%x\n",MOD,sec.magic_num,sec.magic_num);
    MSG("[%s] hdr.cust_name     = %s\n",MOD,sec.cust_name); 
    MSG("[%s] hdr.img_ver       = %d (0x%x)\n",MOD,sec.img_ver,sec.img_ver);     
    MSG("[%s] hdr.img_off       = %d (0x%x)\n",MOD,sec.img_off,sec.img_off);     
    MSG("[%s] hdr.img_len       = %d (0x%x)\n",MOD,sec.img_len,sec.img_len);     
    MSG("[%s] hdr.sign_off      = %d (0x%x)\n",MOD,sec.s_off,sec.s_off);     
    MSG("[%s] hdr.sign_len      = %d (0x%x)\n",MOD,sec.s_len,sec.s_len);     
    MSG("[%s] hdr.hs_off        = %d (0x%x)\n",MOD,sec.sig_off,sec.sig_off);         
    MSG("[%s] hdr.hs_len        = %d (0x%x)\n",MOD,sec.sig_len,sec.sig_len);                

img_err:
check_err:
    /* close header file */
    fclose (hdr_fd);        
    
    return 0;
}




int pro_img_v1_v2(char *hs_name, char *img_name,char *hdr_name)
{
    uint32 br = 0;
    uchar *d_buf = NULL;
    uchar c_buf[SEC_IMG_HDR_SZ];    
    uchar *sig;
    uchar *hash;    
    uint32 i = 0;    
    SEC_IMG_HEADER *sec = NULL;    

    /* ------------------------------------- */
    /* open hash and signature file          */
    /* ------------------------------------- */    
    FILE *hs_fd = fopen(hs_name,"wb");      
    
    if(hs_fd == 0)
    {
        MSG("[%s] %s not found\n",MOD,hs_name);
        goto _err;
    }

    /* ------------------------------------- */
    /* read image header                     */
    /* ------------------------------------- */    
    
    FILE *hdr_fd = fopen(hdr_name,"r");            

    br = fread(c_buf,1,SEC_IMG_HDR_SZ,hdr_fd); /* read header */    

    sec = (SEC_IMG_HEADER *)c_buf;

    if(br == 0)
    {
        MSG("\n[%s] read '%s' image hdr fail, read bytes = '%d'\n",MOD,hdr_name,br);
        goto _err;
    }


    /* ------------------------------------- */
    /* initialize buffer                     */
    /* ------------------------------------- */    
    d_buf = (uchar*) malloc(SEC_IMG_HDR_SZ + sec->s_len*sizeof(char));
    sig = (uchar*) malloc(get_sigature_size(g_sig_type));
    hash = (uchar*) malloc(get_hash_size(g_hash_type));

    mcpy(d_buf,c_buf,SEC_IMG_HDR_SZ);

    /* ------------------------------------- */
    /* read image content                    */
    /* ------------------------------------- */    
    FILE *img_fd = fopen(img_name,"r");        
    
    if(img_fd == 0)
    {
        MSG("[%s] %s not found\n",MOD,img_name);
        goto _err;
    }
       
    fseek(img_fd,sec->s_off*sizeof(char),SEEK_SET);   
    
    br = fread(d_buf+SEC_IMG_HDR_SZ,1,sec->s_len,img_fd);

    if(br == 0)
    {
        MSG("\n[%s] read image content fail, read bytes = '%d'\n",MOD,br);
        goto _err;  
    }

    /* ------------------------------------- */
    /* Sign  
     * @1 : file
     * @2 : file length to be signed
     * @3 : signature
     * @3 : signature length */
    /* ------------------------------------- */     
    if( cust_sign(d_buf,SEC_IMG_HDR_SZ + sec->s_len,sig,get_sigature_size(g_sig_type)) == -1)
    {
        MSG("[%s] Sign %s fail\n",MOD,img_name);
        goto _err;
    }

    /* ------------------------------------- */
    /* Hash  
     * @1 : file
     * @2 : file length to be hashed
     * @3 : hash
     * @3 : hash length */
    /* ------------------------------------- */     
    if( cust_hash(d_buf,SEC_IMG_HDR_SZ + sec->s_len,hash,get_hash_size(g_hash_type)) == -1)
    {
        MSG("[%s] Sign %s fail\n",MOD,img_name);
        goto _err;
    }

    /* ------------------------------------- */
    /* dump hash value for debug             */
    /* ------------------------------------- */    
    MSG("[%s] Hash value : \n",MOD);    
    for(i=0;i<get_hash_size(g_hash_type);i++)
    {
        MSG("0x%x,",hash[i]);
    }
    MSG("\n",MOD);    

    /* ------------------------------------- */
    /* write hash and signature              */
    /* ------------------------------------- */    
    fwrite (sig , 1 , get_sigature_size(g_sig_type), hs_fd);   
    fwrite (hash , 1 , get_hash_size(g_hash_type) , hs_fd);
    fclose (hs_fd);    


    free(d_buf);    
    return 0;

_err:

    free(d_buf);
    return -1;

}

#define DUMP_MORE_FOR_DEBUG 0

int gen_hash_by_chunk(FILE *img_fd, uint32 img_hash_off, uint32 img_hash_len,
    uchar *final_hash_buf, SEC_CRYPTO_HASH_TYPE hash_type, uint32 chunk_size)
{
    uint32 br = 0;
    uint32 i = 0, ret = 0; 
    uchar *chunk_buf = NULL;
    uchar *hash_tmp;
    uchar *hash_comb;
    uint32 seek_pos = 0;
    uint32 hash_size = get_hash_size(hash_type);
    uint32 chunk_count = ((img_hash_len-1)/chunk_size)+1;
    uint32 read_size = 0;
    uint32 left_size = 0;

    if(!img_hash_len)
    {
        
        MSG("[%s] hash length is zero, no need to do hash\n",MOD);
        ret = -1;
        memset(final_hash_buf, 0x00, hash_size);
        goto end_error;        
    }

#if DUMP_MORE_FOR_DEBUG    
    DBG("[%s] Hash size is %d (0x%x)\n",MOD, hash_size, hash_size);
    DBG("[%s] Offset is %d (0x%x)\n",MOD, img_hash_off, img_hash_off);
    DBG("[%s] Size is %d (0x%x)\n",MOD, img_hash_len, img_hash_len);
    DBG("[%s] Chunk size is %d (0x%x)\n",MOD, chunk_size, chunk_size);
    DBG("[%s] Chunk count is %d (0x%x)\n",MOD, chunk_count, chunk_count);
#endif

    /* allocate hash buffer */    
    hash_tmp = (uchar*) malloc(hash_size);
    hash_comb = (uchar*) malloc(hash_size*2);
    memset(hash_tmp, 0x00, hash_size);
    memset(hash_comb, 0x00, hash_size*2);

    /* allocate buffer with known chunk size */
    chunk_buf = (uchar*) malloc(chunk_size);

    /* caculate first hash */
    seek_pos = img_hash_off;
    left_size = img_hash_len;
    read_size = (left_size>=chunk_size)?chunk_size:left_size;
    fseek(img_fd,seek_pos*sizeof(char),SEEK_SET);  
    br = fread(chunk_buf,1,read_size,img_fd);    
    if(br != read_size)
    {
        MSG("[%s] read image content fail, read offset = '0x%x'\n",MOD,seek_pos);
        ret = -2;
        goto end_error;  
    }
    if( cust_hash(chunk_buf,read_size,hash_tmp,hash_size) == -1)
    {
        MSG("[%s] hash fail, offset is '0x%x'(A)\n",MOD,seek_pos);
        ret = -3;
        goto end_error;
    }

#if DUMP_MORE_FOR_DEBUG
    /* ------------------------------------- */
    /* dump hash value for debug             */
    /* ------------------------------------- */    
    DBG("[%s] Data value(4 bytes) ==> (0x%x, 0x%x, 0x%x, 0x%x) \n",MOD, 
            chunk_buf[0], chunk_buf[1], chunk_buf[2], chunk_buf[3]);    
    DBG("[%s] Hash value(single) (0x%x): \n",MOD, seek_pos);    
    for(i=0;i<hash_size;i++)
    {
        DBG("0x%x,",hash_tmp[i]);
    }
    DBG("\n",MOD);    
#endif

    /* copy to compose buffer (first block) */
    mcpy(hash_comb,hash_tmp,hash_size);

    /* move next */
    seek_pos += read_size;
    left_size -= read_size;

    /* loop hash */
    while(left_size)
    {
        /* load data */
        read_size = (left_size>=chunk_size)?chunk_size:left_size;
        fseek(img_fd,seek_pos*sizeof(char),SEEK_SET);  
        br = fread(chunk_buf,1,read_size,img_fd);   
            
        if(br != read_size)
        {
            MSG("[%s] read image content fail, read offset = '0x%x'\n",MOD,seek_pos);
            ret = -4;
            goto end_error;  
        }
    
        /* caculate this hash */        
        if( cust_hash(chunk_buf,read_size,hash_tmp,hash_size) == -1)
        {
            MSG("[%s] hash fail, offset is '0x%x'(B)\n",MOD,seek_pos);
            ret = -5;
            goto end_error;
        }

#if DUMP_MORE_FOR_DEBUG
        /* ------------------------------------- */
        /* dump hash value for debug             */
        /* ------------------------------------- */  
        DBG("[%s] Data value(4 bytes) ==> (0x%x, 0x%x, 0x%x, 0x%x) \n",MOD, 
            chunk_buf[0], chunk_buf[1], chunk_buf[2], chunk_buf[3]);   
        DBG("[%s] Hash value(single) (0x%x): \n",MOD, seek_pos);    
        for(i=0;i<hash_size;i++)
        {
            DBG("0x%x,",hash_tmp[i]);
        }
        DBG("\n",MOD);  
#endif

        /* compose two hash to buffer (second block) */
        mcpy(hash_comb+hash_size,hash_tmp,hash_size);

        /* caculate compose hash */
        if( cust_hash(hash_comb,hash_size*2,hash_tmp,hash_size) == -1)
        {
            MSG("[%s] hash fail, offset is '0x%x'(C)\n",MOD,seek_pos);
            ret = -6;
            goto end_error;
        }

#if DUMP_MORE_FOR_DEBUG
        /* ------------------------------------- */
        /* dump hash value for debug             */
        /* ------------------------------------- */ 
        DBG("[%s] Data value(4 bytes) ==> (0x%x, 0x%x, 0x%x, 0x%x) \n",MOD, 
            hash_comb[0], hash_comb[1], hash_comb[2], hash_comb[3]);    
        DBG("[%s] Hash value(comp): \n",MOD);    
        for(i=0;i<hash_size;i++)
        {
            DBG("0x%x,",hash_tmp[i]);
        }
        DBG("\n",MOD);  
#endif

        /* save this hash to compose buffer (first block) */
        mcpy(hash_comb,hash_tmp,hash_size);

        /* move next */        
        seek_pos += read_size;
        left_size -= read_size;
    }
    
    /* ------------------------------------- */
    /* dump hash value for debug             */
    /* ------------------------------------- */    
#if DUMP_MORE_FOR_DEBUG    
    DBG("[%s] Hash value(final) : \n",MOD);    
    for(i=0;i<hash_size;i++)
    {
        DBG("0x%x,",hash_tmp[i]);
    }
    DBG("\n",MOD);    
#endif

    /* copy hash */
    mcpy(final_hash_buf,hash_tmp,hash_size);
    
end_error:
    free(hash_comb);
    free(chunk_buf);
    free(hash_tmp);

    return ret;
}

int pro_img_v3(char *hs_name, char *img_name,char *hdr_name)
{
    uint32 br = 0;
    uchar *d_buf = NULL;
    uchar *d_buf_prt = NULL;
    uchar c_buf[SEC_IMG_HDR_SZ];    
    uchar *sig;
    uchar *hash;    
    uint32 i = 0, ret = 0;    
    SEC_IMG_HEADER *sec = NULL;
    SEC_EXTENTION_CFG *p_ext_cfg = (SEC_EXTENTION_CFG *)get_ext_cfg();  
    SEC_EXTENSTION_CRYPTO *crypto_ext = NULL;
    SEC_FRAGMENT_CFG *frag_ext = NULL;
    SEC_EXTENSTION_HASH_ONLY **hash_only_ext;
    SEC_EXTENSTION_END_MARK *end_ext = NULL;
    uint32 total_size = 0;
    uint32 real_chunk_size = 0;

    /* ------------------------------------- */
    /* open hash and signature file          */
    /* ------------------------------------- */    
    FILE *hs_fd = fopen(hs_name,"wb");      
    
    if(hs_fd == 0)
    {
        MSG("[%s] %s not found\n",MOD,hs_name);
        goto _init_fail;
    }

    /* ------------------------------------- */
    /* read image header                     */
    /* ------------------------------------- */    
    
    FILE *hdr_fd = fopen(hdr_name,"r");            

    br = fread(c_buf,1,SEC_IMG_HDR_SZ,hdr_fd); /* read header */    

    sec = (SEC_IMG_HEADER *)c_buf;

    if(br == 0)
    {
        MSG("\n[%s] read '%s' image hdr fail, read bytes = '%d'\n",MOD,hdr_name,br);
        ret = -1;
        goto _hdr_fail;
    }

    /* ------------------------------------- */
    /* initialize buffer                     */
    /* ------------------------------------- */   
    sig = (uchar*) malloc(get_sigature_size(g_sig_type));
    hash = (uchar*) malloc(get_hash_size(g_hash_type));

    /* ------------------------------------- */
    /* initialize extnesion header buffer       */
    /* ------------------------------------- */ 
    crypto_ext = (SEC_EXTENSTION_CRYPTO *) allocate_ext_crypto();
    frag_ext = (SEC_FRAGMENT_CFG *) allocate_ext_frag();
    hash_only_ext = (SEC_EXTENSTION_HASH_ONLY **)malloc(p_ext_cfg->verify_count * sizeof(SEC_EXTENSTION_HASH_ONLY *));
    for(i=0;i<p_ext_cfg->verify_count;i++)
    {
        hash_only_ext[i] = (SEC_EXTENSTION_HASH_ONLY *) allocate_ext_hash_only(g_hash_type);
    }
    end_ext = (SEC_EXTENSTION_END_MARK *) allocate_ext_end();


    /* ------------------------------------- */
    /* initial extenstion header                    */
    /* ------------------------------------- */     
    crypto_ext->hash_type = g_hash_type;
    crypto_ext->sig_type = g_sig_type;
    crypto_ext->enc_type = SEC_CRYPTO_ENC_UNKNOWN;
    frag_ext->frag_count = p_ext_cfg->verify_count;
    frag_ext->chunk_size = p_ext_cfg->chunk_size;
    for(i=0;i<p_ext_cfg->verify_count;i++)
    {
        hash_only_ext[i]->hash_offset = p_ext_cfg->verify_offset[i];
        hash_only_ext[i]->hash_len = p_ext_cfg->verify_length[i];
    }

    /* ----------------------------------------- */
    /* generate hash for each region by chunk size      */
    /* ----------------------------------------- */    
    FILE *img_fd = fopen(img_name,"r");        
    
    if(img_fd == 0)
    {
        MSG("[%s] %s not found\n",MOD,img_name);
        ret = -1;
        goto _img_open_fail;
    }  

    for(i=0;i<p_ext_cfg->verify_count;i++)
    {
        if(frag_ext->chunk_size == 0)
        {
            real_chunk_size = hash_only_ext[i]->hash_len;
        }
        else
        {
            real_chunk_size = frag_ext->chunk_size;
        }
        if(gen_hash_by_chunk(img_fd, hash_only_ext[i]->hash_offset, hash_only_ext[i]->hash_len,
            hash, hash_only_ext[i]->sub_type, real_chunk_size)!=0)
        {
            ret = -1;
            goto _ext_hash_fail;
        }
        mcpy(hash_only_ext[i]->hash_data,hash,get_hash_size(g_hash_type));
    }

    /* ------------------------------------- */
    /* prepare buffer                     */
    /* ------------------------------------- */   
    total_size =    SEC_IMG_HDR_SZ + 
                    p_ext_cfg->verify_count*get_hash_size(g_hash_type)+                            
                    sizeof(*crypto_ext)+
                    sizeof(*frag_ext)+
                    p_ext_cfg->verify_count*get_ext_hash_only_struct_size(g_hash_type)+
                    sizeof(*end_ext);
    d_buf = (uchar*) malloc(total_size);
    d_buf_prt = d_buf;

    /* copy header */
    mcpy(d_buf_prt,c_buf,SEC_IMG_HDR_SZ);
    d_buf_prt += SEC_IMG_HDR_SZ;
    
    /* copy hash */
    for(i=0;i<p_ext_cfg->verify_count;i++)
    {
        mcpy(d_buf_prt,hash_only_ext[i]->hash_data,get_hash_size(g_hash_type));
        d_buf_prt += get_hash_size(g_hash_type);
    }
    
    /* copy crypto extension */
    mcpy(d_buf_prt,crypto_ext,sizeof(*crypto_ext));
    d_buf_prt += sizeof(*crypto_ext);

    /* copy frag extension */
    mcpy(d_buf_prt,frag_ext,sizeof(*frag_ext));
    d_buf_prt += sizeof(*frag_ext);

    /* copy hash extension */
    for(i=0;i<p_ext_cfg->verify_count;i++)
    {
        mcpy(d_buf_prt,hash_only_ext[i],get_ext_hash_only_struct_size(g_hash_type));
        d_buf_prt += get_ext_hash_only_struct_size(g_hash_type);
    }

    /* copy end mark extension */
    mcpy(d_buf_prt,end_ext,sizeof(*end_ext));
    d_buf_prt += sizeof(*end_ext);

    /* ------------------------------------- */
    /* generate hash                    */
    /* ------------------------------------- */  
    if( cust_hash(d_buf,total_size,hash,get_hash_size(g_hash_type)) == -1)
    {
        MSG("[%s] Sign %s fail\n",MOD,img_name);
        ret = -1;
        goto _final_hash_fail;
    }

    /* ------------------------------------- */
    /* generate signature                    */
    /* ------------------------------------- */  
    if( cust_sign(d_buf,total_size,sig,get_sigature_size(g_sig_type)) == -1)
    {
        MSG("[%s] Sign %s fail\n",MOD,img_name);
        ret = -1;
        goto _final_sign_fail;
    }

    /* ------------------------------------- */
    /* dump hash value for debug             */
    /* ------------------------------------- */ 
#if DUMP_MORE_FOR_DEBUG        
    {
        unsigned loop_count = total_size/8;
        unsigned remain_count = total_size%8;
        MSG("[%s] Total verify size is : %d\n",MOD, total_size);    
        for(i=0; i<loop_count ; i++)
        {    
            DBG("[%s] Data value [%d-%d]==> (0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x) \n",MOD, 
                i*8, (i+1)*8-1, 
                d_buf[0+i*8], d_buf[1+i*8], d_buf[2+i*8], d_buf[3+i*8], 
                d_buf[4+i*8], d_buf[5+i*8], d_buf[6+i*8], d_buf[7+i*8]);  
        }
        if(remain_count)
        {
            DBG("[%s] Data value [%d-%d]==> (",MOD,loop_count*8, loop_count*8+remain_count);
            for(i=0; i<remain_count ; i++)
            {    
                DBG("0x%x,", d_buf[loop_count*8+i]);  
            }
            DBG(") \n");
        }
    }
#endif    
    MSG("[%s] Hash value : \n",MOD);    
    for(i=0;i<get_hash_size(g_hash_type);i++)
    {
        MSG("0x%x,",hash[i]);
    }
    MSG("\n",MOD);    

    /* ------------------------------------- */
    /* write hash and signature              */
    /* ------------------------------------- */    
    fwrite (sig , 1 , get_sigature_size(g_sig_type), hs_fd);   
    fwrite (hash , 1 , get_hash_size(g_hash_type) , hs_fd);

    
    /* ------------------------------------- */
    /* write extension              */
    /* ------------------------------------- */ 
    d_buf_prt = d_buf;
    d_buf_prt += SEC_IMG_HDR_SZ + p_ext_cfg->verify_count*get_hash_size(g_hash_type);
    fwrite (d_buf_prt , 1 , 
        total_size-(SEC_IMG_HDR_SZ+p_ext_cfg->verify_count*get_hash_size(g_hash_type)), 
        hs_fd);   
    
    fclose (hs_fd);    

_final_sign_fail:
_final_hash_fail:
    free(d_buf);
_ext_hash_fail:    
_img_open_fail:
_img_read_fail:
    free(end_ext);
    free(frag_ext);
    free(crypto_ext);
    free(hash_only_ext);
    free(hash);
    free(sig);
_hdr_fail:
    fclose(hdr_fd);
_init_fail:

    return ret;
}

int pro_img(char *hs_name, char *img_name,char *hdr_name)
{
    if( is_hdr_version3() )
    {
        return pro_img_v3(hs_name, img_name, hdr_name);
    }
    else
    {
        return pro_img_v1_v2(hs_name, img_name, hdr_name);
    }
}


