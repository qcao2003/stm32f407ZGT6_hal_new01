/**
 ****************************************************************************************************
 * @file        mjpeg.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-16
 * @brief       MJPEG视频处理 代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211116
 * 第一次发布
 *
 ****************************************************************************************************
 */
#include "./MJPEG/mjpeg.h"
#include "./MALLOC/malloc.h"
#include "./FATFS/source/ff.h"
#include "./BSP/LCD/lcd.h"


/* 简单快速的内存分配,以提高速度 */
#define MJPEG_MAX_MALLOC_SIZE       38 * 1024     /* 最大可以分配38K字节 */

struct jpeg_decompress_struct *cinfo;
struct my_error_mgr *jerr;
uint8_t *jpegbuf;           /* jpeg数据缓存指针 */
uint32_t jbufsize;          /* jpeg buf大小 */
uint16_t imgoffx, imgoffy;  /* 图像在x,y方向的偏移量 */
uint8_t *jmembuf;           /* mjpeg解码的 内存池 */
uint32_t jmempos;           /* 内存池指针 */

/**
 * @brief       mjpeg申请内存
 * @param       num   : 下一次分配的地址起始地址
 * @retval      无
 */
void *mjpeg_malloc(uint32_t num)
{
    uint32_t curpos = jmempos;  /* 此次分配的起始地址 */
    jmempos += num;             /* 下一次分配的起始地址 */

    if (jmempos > 38 * 1024)
    {
        printf("mem error:%d,%d", curpos, num);
    }

    return (void *)&jmembuf[curpos];    /* 返回申请到的内存首地址 */
}

/**
 * @brief       错误退出
 * @param       cinfo   : JPEG编码解码控制结构体
 * @retval      无
 */
static void my_error_exit(j_common_ptr cinfo)
{
    my_error_ptr myerr = (my_error_ptr) cinfo->err;
    (*cinfo->err->output_message) (cinfo);
    longjmp(myerr->setjmp_buffer, 1);
}

/**
 * @brief       发出消息
 * @param       cinfo       : JPEG编码解码控制结构体
 * @param       msg_level   : 消息等级
 * @retval      无
 */
METHODDEF(void) my_emit_message(j_common_ptr cinfo, int msg_level)
{
    my_error_ptr myerr = (my_error_ptr) cinfo->err;

    if (msg_level < 0)
    {
        printf("emit msg:%d\r\n", msg_level);
        longjmp(myerr->setjmp_buffer, 1);
    }
}

/**
 * @brief       初始化资源,不执行任何操作
 * @param       cinfo   : JPEG编码解码控制结构体
 * @retval      无
 */
static void init_source(j_decompress_ptr cinfo)
{
    /* 不需要做任何事情. */
    return;
}

/**
 * @brief       填充输入缓冲区,一次性读取整帧数据
 * @param       cinfo   : JPEG编码解码控制结构体
 * @retval      无
 */
static boolean fill_input_buffer(j_decompress_ptr cinfo)
{
    if (jbufsize == 0) /* 结束了 */
    {
        printf("jd read off\r\n");
        jpegbuf[0] = (uint8_t) 0xFF;        /* 填充结束符 */
        jpegbuf[1] = (uint8_t) JPEG_EOI;
        cinfo->src->next_input_byte = jpegbuf;
        cinfo->src->bytes_in_buffer = 2;
    }
    else
    {
        cinfo->src->next_input_byte = jpegbuf;
        cinfo->src->bytes_in_buffer = jbufsize;
        jbufsize -= jbufsize;
    }

    return TRUE;
}

/**
 * @brief       在文件里面,跳过num_bytes个数据
 * @param       cinfo       : JPEG编码解码控制结构体
 * @param       num_bytes   : 跳过字节长度
 * @retval      无
 */
static void skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
    /* Just a dumb implementation for now.  Could use fseek() except
    * it doesn't work on pipes.  Not clear that being smart is worth
    * any trouble anyway --- large skips are infrequent.
    */
    if (num_bytes > 0)
    {
        while (num_bytes > (long) cinfo->src->bytes_in_buffer)
        {
            num_bytes -= (long)cinfo->src->bytes_in_buffer;
            (void)cinfo->src->fill_input_buffer(cinfo);
            /* note we assume that fill_input_buffer will never
            * return FALSE, so suspension need not be handled.
            */
        }

        cinfo->src->next_input_byte += (size_t) num_bytes;
        cinfo->src->bytes_in_buffer -= (size_t) num_bytes;
    }
}

/**
 * @brief       在解码结束后,被jpeg_finish_decompress函数调用
 * @param       cinfo       : JPEG编码解码控制结构体
 * @retval      无
 */
static void term_source(j_decompress_ptr cinfo)
{
    /* 不做任何处理 */
    return;
}

/**
 * @brief       初始化jpeg解码数据源
 * @param       cinfo   : JPEG编码解码控制结构体
 * @retval      无
 */
static void jpeg_filerw_src_init(j_decompress_ptr cinfo)
{
    if (cinfo->src == NULL)     /* first time for this JPEG object? */
    {
        cinfo->src = (struct jpeg_source_mgr *)
                     (*cinfo->mem->alloc_small)((j_common_ptr) cinfo, JPOOL_PERMANENT,
                                                sizeof(struct jpeg_source_mgr));
    }

    cinfo->src->init_source = init_source;
    cinfo->src->fill_input_buffer = fill_input_buffer;
    cinfo->src->skip_input_data = skip_input_data;
    cinfo->src->resync_to_restart = jpeg_resync_to_restart; /* use default method */
    cinfo->src->term_source = term_source;
    cinfo->src->bytes_in_buffer = 0;    /* forces fill_input_buffer on first read */
    cinfo->src->next_input_byte = NULL; /* until buffer loaded */
}

/**
 * @brief       初始化jpeg解码数据源
 * @param       offx   : x方向的偏移
 * @param       offy   : y方向的偏移
 * @retval      0,成功;
 *              1,失败
 */
uint8_t mjpegdec_init(uint16_t offx, uint16_t offy)
{
    cinfo = mymalloc(SRAMCCM, sizeof(struct jpeg_decompress_struct));
    jerr = mymalloc(SRAMCCM, sizeof(struct my_error_mgr));
    jmembuf = mymalloc(SRAMCCM, MJPEG_MAX_MALLOC_SIZE);     /* MJPEG解码内存池申请 */

    if (cinfo == 0 || jerr == 0 || jmembuf == 0)
    {
        mjpegdec_free();
        return 1;
    }

    /* 保存图像在x,y方向的偏移量 */
    imgoffx = offx;
    imgoffy = offy;
    
    return 0;
}

/**
 * @brief       mjpeg结束,释放内存
 * @param       无
 * @retval      无
 */
void mjpegdec_free(void)
{
    myfree(SRAMCCM, cinfo);
    myfree(SRAMCCM, jerr);
    myfree(SRAMCCM, jmembuf);
}

/**
 * @brief       解码一副JPEG图片
 * @param       buf     : jpeg数据流数组
 * @param       bsize   : 数组大小
 * @retval      0,成功;
 *              其他,错误
 */
uint8_t mjpegdec_decode(uint8_t *buf, uint32_t bsize)
{
    JSAMPARRAY buffer;

    if (bsize == 0)return 1;

    jpegbuf = buf;
    jbufsize = bsize;
    jmempos = 0; /* MJEPG解码,重新从0开始分配内存 */

    cinfo->err = jpeg_std_error(&jerr->pub);
    jerr->pub.error_exit = my_error_exit;
    jerr->pub.emit_message = my_emit_message;

    //if(bsize>20*1024)printf("s:%d\r\n",bsize);
    if (setjmp(jerr->setjmp_buffer))    /* 错误处理 */
    {
        jpeg_abort_decompress(cinfo);
        jpeg_destroy_decompress(cinfo);
        return 2;
    }

    jpeg_create_decompress(cinfo);
    jpeg_filerw_src_init(cinfo);
    jpeg_read_header(cinfo, TRUE);
    cinfo->dct_method = JDCT_IFAST;
    cinfo->do_fancy_upsampling = 0;
    jpeg_start_decompress(cinfo);
    lcd_set_window(imgoffx, imgoffy, cinfo->output_width, cinfo->output_height);
    lcd_write_ram_prepare();        /* 开始写入GRAM */

    while (cinfo->output_scanline < cinfo->output_height)
    {
        jpeg_read_scanlines(cinfo, buffer, 1);
    }

    lcd_set_window(0, 0, lcddev.width, lcddev.height);  /* 恢复窗口 */
    jpeg_finish_decompress(cinfo);
    jpeg_destroy_decompress(cinfo);
    
    return 0;
}
















