/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		flash
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.26
 * @Target core		NXP RT1021DAG5A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-02-18
 ********************************************************************************************************************/

#include "fsl_common.h"
#include "fsl_flexspi.h"
#include "fsl_cache.h"
#include "zf_flash.h"





//-------------------------------------------------------------------------------------------------------------------
//  @brief      校验FLASH是否有数据
//  @param      sector_num 		需要检查的扇区编号      参数范围0 -（FLASH_SECTOR_NUM-1）
//  @param      page_num	 	需要检查扇区页的编号    参数范围0 - (FLASH_SECTOR_PAGE_NUM-1)
//  @return     uint8			返回1有数据，返回0没有数据，如果需要对有数据的区域写入新的数据则应该对所在扇区进行擦除操作
//  @since      v1.0
//  Sample usage:               flash_check(500,0);//校验500号扇区，低=第0页是否有数据
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_check(uint32 sector_num, uint32 page_num)
{
    uint8 i;
    
    SCB_CleanInvalidateDCache(); //读取flash前，最好先清空缓存
    for(i=0; i<FLASH_PAGE_SIZE/4; i++)
    {
        if(0xFFFFFFFF != flash_read(sector_num,page_num*FLASH_PAGE_SIZE/4+i,uint32))
        {
            return 1;//flash需要擦除
        }
    }
    
    return 0;//flash不需要擦除
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH页读取
//  @param      sector_num 		需要读取的扇区编号       参数范围0 -（FLASH_SECTOR_NUM-1）
//  @param      page_num	 	需要读取该扇区第几页      参数范围0 - (FLASH_SECTOR_PAGE_NUM-1)
//  @param      *buf	 		数据接收缓冲区首地址
//  @param      len		 		写入的字节数 说明这里len=1则表示写入len*4个字节   参数范围1 - 64
//  @return     void
//  @since      v1.0
//  Sample usage:               flash_read_page(500,0,(void *)buf,64);//将flash500号扇区第0页里面的内容 前64个字（这里的字表示四个字节） 写入buf数组
//  一般flash都是 分为有n个扇区，然后一个有多个页，每一个页有固定大小
//  核心板自带的flash 每个扇区为4KB，每个页有256字节，因此一个扇区有16页
//-------------------------------------------------------------------------------------------------------------------
void flash_read_page(uint32 sector_num, uint32 page_num, void *buf, uint8 len)
{
    uint8 i;
    
    assert(FLASH_SECTOR_NUM>sector_num);                //校验参数合法性
    assert(FLASH_SECTOR_PAGE_NUM>page_num);             //校验参数合法性
    assert(FLASH_PAGE_SIZE>=(len*4));                   //校验参数合法性
    DCACHE_CleanInvalidateByRange(FLASH_BASE_ADDR + sector_num * FLASH_SECTOR_SIZE, len*4);//读取flash前，最好先清空缓存
    
    for(i=0; i<len; i++)
    {
        *((uint32 *)buf + i) = flash_read(sector_num,page_num*FLASH_PAGE_SIZE+i,uint32);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH擦除扇区.
//  @param      sector_num 		需要擦除的扇区编号      参数范围0 -（FLASH_SECTOR_NUM-1）
//  @return     uint8			返回其他擦除失败，返回0擦除成功
//  @since      v1.0
//  Sample usage:               uint32 dat = flash_erase_sector(1000);
//  @note       此函数不允许在中断函数内调用
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_erase_sector(uint32 sector_num)
{
    status_t status;
    
    assert(FLASH_SECTOR_NUM>sector_num);//校验参数合法性
    
    status = flexspi_nor_flash_erase_sector(EXAMPLE_FLEXSPI, sector_num * FLASH_SECTOR_SIZE);
    return status;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH页编程
//  @param      sector_num 		需要写入的扇区编号       参数范围0 -（FLASH_SECTOR_NUM-1）
//  @param      page_num	 	将数据写入该扇区第几页   参数范围0 - (FLASH_SECTOR_PAGE_NUM-1)
//  @param      *buf	 		数据缓冲区首地址
//  @param      len		 		写入的字节数 说明这里len=1则表示写入len*4个字节   参数范围1 - 64
//  @return     uint8			返回其他写入失败，返回0写入成功
//  @since      v1.0
//  Sample usage:               flash_write_page(500,0,(const void *)buf,64);//将buf数组里面的内容 前64个字（这里的字表示四个字节） 写入flash500号扇区第0页
//  一般flash都是 分为有n个扇区，然后一个有多个页，每一个页有固定大小
//  核心板自带的flash 每个扇区为4KB，每个页有256字节，因此一个扇区有16页
//  @note       此函数不允许在中断函数内调用
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_page_program(uint32 sector_num, uint32 page_num, const uint32 *buf, uint8 len)
{
    status_t status;
    
    assert(FLASH_SECTOR_NUM>sector_num);                //校验参数合法性
    assert(FLASH_SECTOR_PAGE_NUM>page_num);             //校验参数合法性
    assert(FLASH_PAGE_SIZE>=(len*4));                   //校验参数合法性
    
    status = flexspi_nor_flash_page_program(EXAMPLE_FLEXSPI, sector_num * FLASH_SECTOR_SIZE + FLASH_PAGE_SIZE*page_num, (const uint32_t *)buf, len);
    return status;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH初始化函数
//  @param      void 		    
//  @return     uint8           返回其他初始化失败，返回0初始化成功
//  @since      v1.0
//  Sample usage:               
//  @note       此函数不允许在中断函数内调用
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_init(void)
{
    status_t status;
    
    status = (uint8)flexspi_nor_init();
    return status;
    
}

