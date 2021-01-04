/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		flash
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
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
//  @brief      У��FLASH�Ƿ�������
//  @param      sector_num 		��Ҫ�����������      ������Χ0 -��FLASH_SECTOR_NUM-1��
//  @param      page_num	 	��Ҫ�������ҳ�ı��    ������Χ0 - (FLASH_SECTOR_PAGE_NUM-1)
//  @return     uint8			����1�����ݣ�����0û�����ݣ������Ҫ�������ݵ�����д���µ�������Ӧ�ö������������в�������
//  @since      v1.0
//  Sample usage:               flash_check(500,0);//У��500����������=��0ҳ�Ƿ�������
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_check(uint32 sector_num, uint32 page_num)
{
    uint8 i;
    
    SCB_CleanInvalidateDCache(); //��ȡflashǰ���������ջ���
    for(i=0; i<FLASH_PAGE_SIZE/4; i++)
    {
        if(0xFFFFFFFF != flash_read(sector_num,page_num*FLASH_PAGE_SIZE/4+i,uint32))
        {
            return 1;//flash��Ҫ����
        }
    }
    
    return 0;//flash����Ҫ����
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASHҳ��ȡ
//  @param      sector_num 		��Ҫ��ȡ���������       ������Χ0 -��FLASH_SECTOR_NUM-1��
//  @param      page_num	 	��Ҫ��ȡ�������ڼ�ҳ      ������Χ0 - (FLASH_SECTOR_PAGE_NUM-1)
//  @param      *buf	 		���ݽ��ջ������׵�ַ
//  @param      len		 		д����ֽ��� ˵������len=1���ʾд��len*4���ֽ�   ������Χ1 - 64
//  @return     void
//  @since      v1.0
//  Sample usage:               flash_read_page(500,0,(void *)buf,64);//��flash500��������0ҳ��������� ǰ64���֣�������ֱ�ʾ�ĸ��ֽڣ� д��buf����
//  һ��flash���� ��Ϊ��n��������Ȼ��һ���ж��ҳ��ÿһ��ҳ�й̶���С
//  ���İ��Դ���flash ÿ������Ϊ4KB��ÿ��ҳ��256�ֽڣ����һ��������16ҳ
//-------------------------------------------------------------------------------------------------------------------
void flash_read_page(uint32 sector_num, uint32 page_num, void *buf, uint8 len)
{
    uint8 i;
    
    assert(FLASH_SECTOR_NUM>sector_num);                //У������Ϸ���
    assert(FLASH_SECTOR_PAGE_NUM>page_num);             //У������Ϸ���
    assert(FLASH_PAGE_SIZE>=(len*4));                   //У������Ϸ���
    DCACHE_CleanInvalidateByRange(FLASH_BASE_ADDR + sector_num * FLASH_SECTOR_SIZE, len*4);//��ȡflashǰ���������ջ���
    
    for(i=0; i<len; i++)
    {
        *((uint32 *)buf + i) = flash_read(sector_num,page_num*FLASH_PAGE_SIZE+i,uint32);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH��������.
//  @param      sector_num 		��Ҫ�������������      ������Χ0 -��FLASH_SECTOR_NUM-1��
//  @return     uint8			������������ʧ�ܣ�����0�����ɹ�
//  @since      v1.0
//  Sample usage:               uint32 dat = flash_erase_sector(1000);
//  @note       �˺������������жϺ����ڵ���
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_erase_sector(uint32 sector_num)
{
    status_t status;
    
    assert(FLASH_SECTOR_NUM>sector_num);//У������Ϸ���
    
    status = flexspi_nor_flash_erase_sector(EXAMPLE_FLEXSPI, sector_num * FLASH_SECTOR_SIZE);
    return status;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASHҳ���
//  @param      sector_num 		��Ҫд����������       ������Χ0 -��FLASH_SECTOR_NUM-1��
//  @param      page_num	 	������д��������ڼ�ҳ   ������Χ0 - (FLASH_SECTOR_PAGE_NUM-1)
//  @param      *buf	 		���ݻ������׵�ַ
//  @param      len		 		д����ֽ��� ˵������len=1���ʾд��len*4���ֽ�   ������Χ1 - 64
//  @return     uint8			��������д��ʧ�ܣ�����0д��ɹ�
//  @since      v1.0
//  Sample usage:               flash_write_page(500,0,(const void *)buf,64);//��buf������������� ǰ64���֣�������ֱ�ʾ�ĸ��ֽڣ� д��flash500��������0ҳ
//  һ��flash���� ��Ϊ��n��������Ȼ��һ���ж��ҳ��ÿһ��ҳ�й̶���С
//  ���İ��Դ���flash ÿ������Ϊ4KB��ÿ��ҳ��256�ֽڣ����һ��������16ҳ
//  @note       �˺������������жϺ����ڵ���
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_page_program(uint32 sector_num, uint32 page_num, const uint32 *buf, uint8 len)
{
    status_t status;
    
    assert(FLASH_SECTOR_NUM>sector_num);                //У������Ϸ���
    assert(FLASH_SECTOR_PAGE_NUM>page_num);             //У������Ϸ���
    assert(FLASH_PAGE_SIZE>=(len*4));                   //У������Ϸ���
    
    status = flexspi_nor_flash_page_program(EXAMPLE_FLEXSPI, sector_num * FLASH_SECTOR_SIZE + FLASH_PAGE_SIZE*page_num, (const uint32_t *)buf, len);
    return status;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH��ʼ������
//  @param      void 		    
//  @return     uint8           ����������ʼ��ʧ�ܣ�����0��ʼ���ɹ�
//  @since      v1.0
//  Sample usage:               
//  @note       �˺������������жϺ����ڵ���
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_init(void)
{
    status_t status;
    
    status = (uint8)flexspi_nor_init();
    return status;
    
}

