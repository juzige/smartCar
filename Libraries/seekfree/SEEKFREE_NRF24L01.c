/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		SEEKFREE_NRF24L01.c
 * @brief      		NRF24L01������
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 * @note	
					NRF24L01���߶��壺
					------------------------------------ 
					    NRF24L01    ��Ƭ��            
					    CE          C5
					    CSN         A5
					    MOSI        B23
					    SCK         B20
					                
					    MISO        B22
					    IRQ         B21
					------------------------------------ 
 ********************************************************************************************************************/



#include "SEEKFREE_NRF24L01.h"





#define NRF_CE(x)   gpio_set(NRF_CE_PIN,x)
#define NRF_CSN(x)  gpio_set(NRF_CSN_PIN,x)
#define NRF_MOSI(x) gpio_set(NRF_MOSI_PIN,x)
#define NRF_SCK(x)  gpio_set(NRF_SCK_PIN,x)

#define NRF_MISO    gpio_get(NRF_MISO_PIN)
#define NRF_IRQ     gpio_get(NRF_IRQ_PIN)


uint8 NRF_RF_Addr[NRF_ADDR_WIDTH]={0xff,0xff,0xff,0xff,0xff};   // TX,RX��ַ,�����и���

uint8 NRF_RX_Buff[NRF_DATA_WIDTH];                      	// �������ݻ�����
uint8 NRF_TX_Buff[NRF_DATA_WIDTH];                      	// �������ݻ�����



// ����: NRF_SPI_WR_Byte()
// ����: ͨ��SPIдһ��byte��nRF24L01+,ͬʱ��nRF24L01+��ȡһ��byte
// ����: byte   ��д���ֽ�����
//       return ��ȡ�����ֽ�����
//		�ڲ�ʹ���û��������
uint8 NRF_SPI_WR_Byte(uint8 byte)
{
    uint8 i;
        for(i=0; i<8; i++){                            		// ѭ��8��
            NRF_MOSI(byte & 0x80);               			// ��byte���λ�����NRF_MOSI_PIN
            byte <<= 1;                                		// ��һλ��λ�����λ
            NRF_SCK (1);                            		// ����NRF_SCK_PIN,nRF24L01��NRF_MOSI_PIN����1λ����,ͬʱ��NRF_MISO_PIN���1λ����
            byte |= NRF_MISO;                       		// ��NRF_MISO_PIN��byte���λ
            NRF_SCK (0);                            		// NRF_SCK_PIN�õ�
        }	
    return(byte);                                      		// ���ض�����һ�ֽ�
}

// ����: NRF_SPI_W_Cmd_Byte()
// ����: ��������д��nRF24L01+,ͬʱ����status�ֽ�
// ����: cmd    ������
//       return ����status״̬
//		�ڲ�ʹ���û��������
uint8 NRF_SPI_W_Cmd_Byte(uint8 cmd)
{
    uint8 status;
    NRF_CSN (0);                                    		// NRF_CSN_PIN�õ�,��ʼ��������
    status = NRF_SPI_WR_Byte(cmd);                      	// д��Ĵ�����ַ,ͬʱ����״̬��
    NRF_CSN (1);                                    		// NRF_CSN_PIN����,�������ݴ���
    return(status);                                     	// ����״̬�Ĵ���    
}

// ����: NRF_SPI_W_Reg_Byte()
// ����: ��valд��addr��Ӧ�ļĴ�����ַ,ͬʱ����status�ֽ�
// ����: cmd    ������
//       val    ��д��Ĵ�������ֵ
//       return ����status״̬
//		�ڲ�ʹ���û��������
uint8 NRF_SPI_W_Reg_Byte(uint8 cmd, uint8 val)
{
    uint8 status;
    NRF_CSN (0);                                    		// NRF_CSN_PIN�õ�,��ʼ��������
    status = NRF_SPI_WR_Byte(cmd);                      	// д��Ĵ�����ַ,ͬʱ����״̬��
    NRF_SPI_WR_Byte(val);                               	// Ȼ��д���ݵ��üĴ���
    NRF_CSN (1);                                    		// NRF_CSN_PIN����,�������ݴ���
    return(status);                                     	// ����״̬�Ĵ���    
}

// ����: NRF_SPI_W_Reg_Byte()
// ����: ��ȡaddr����Ӧ�ļĴ�����ַ
// ����: cmd    ������
//       return ���ؼĴ����ڵ���ֵ
//		�ڲ�ʹ���û��������
uint8 NRF_SPI_R_Reg_Byte(uint8 cmd)
{
    uint8 val;
    NRF_CSN (0);                                    		// NRF_CSN_PIN�õ�,��ʼ��������
    NRF_SPI_WR_Byte(cmd);                               	// д��Ĵ�����ַ
    val = NRF_SPI_WR_Byte(0);                           	// Ȼ��ӸüĴ���������
    NRF_CSN (1);                                    		// NRF_CSN_PIN����,�������ݴ���
    return(val);                                        	// ���ؼĴ�������
}

// ����: NRF_SPI_W_Reg_Burst()
// ����: ����д��len���ֽڵ�addrָ���ļĴ�����,ͬʱ����status״̬��
// ����: cmd    ������
//       *p     ��д������ָ��
//       len    ��д�����ݳ���
//       return ����status״̬
//		�ڲ�ʹ���û��������
uint8 NRF_SPI_W_Reg_Burst(uint8 cmd, uint8 *p, uint8 len)
{
    uint8 status, i;
    NRF_CSN (0);                                    		// NRF_CSN_PIN�õ�,��ʼ��������
    status = NRF_SPI_WR_Byte(cmd);                      	// ѡ��Ĵ���,ͬʱ����״̬��
    for(i=0; i<len; i++)	
        NRF_SPI_WR_Byte(p[i]);                          	// ����ֽ�д��nRF24L01+
    NRF_CSN (1);                                    		// NRF_CSN_PIN����,�������ݴ���
    return(status);                                     	// ����״̬�Ĵ���    
}

// ����: NRF_SPI_R_Reg_Burst()
// ����: ������ȡlen���ֽڵ�addrָ���ļĴ�����,ͬʱ����status״̬��
// ����: cmd     ������
//       *p     ����ȡ����ָ��
//       len    ����ȡ���ݳ���
//       return ����status״̬
//		�ڲ�ʹ���û��������
uint8 NRF_SPI_R_Reg_Burst(uint8 cmd, uint8 *p, uint8 len)
{
    uint8 status, i;
    NRF_CSN (0);                                    		// NRF_CSN_PIN�õ�,��ʼ��������
    status = NRF_SPI_WR_Byte(cmd);                      	// ѡ��Ĵ���,ͬʱ����״̬��
    for(i=0; i<len; i++)	
        p[i] = NRF_SPI_WR_Byte(0);                      	// ����ֽڴ�nRF24L01+����
    NRF_CSN (1);                                    		// NRF_CSN_PIN����,�������ݴ���
    return(status);                                     	// ����״̬�Ĵ���    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      NRF��麯��
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void NRF_Check(void)
{
    uint8 temp;
    NRF_SPI_W_Reg_Byte(NRF_CMD_W_REGISTER | NRF_REG_RX_PW_P0,   NRF_DATA_WIDTH);    // ����RXͨ��0���ݿ��
    temp = NRF_SPI_R_Reg_Byte(NRF_CMD_R_REGISTER | NRF_REG_RX_PW_P0);               // ��ȡRXͨ��0���ݿ��
    
    while(NRF_DATA_WIDTH != temp);
    //�����������while����������¼������
    //1.nrfδ����
    //2.nrf�뵥Ƭ�����Ӳ��ɿ���������޷�ͨ�Ŷ�����Ϊ���
    //3.nrf�ǻ���,���NRF���µģ����Ŀ����Լ���
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      NRF����ģ���ʼ��
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void NRF_Dev_Init(void)
{
    gpio_init(NRF_CE_PIN  ,GPO,0);
    gpio_init(NRF_CSN_PIN ,GPO,0);
    gpio_init(NRF_MOSI_PIN,GPO,0);
    gpio_init(NRF_SCK_PIN ,GPO,0);
              
    gpio_init(NRF_MISO_PIN,GPI,0);
    gpio_init(NRF_IRQ_PIN ,GPI,0);
    
    port_init_NoAlt (NRF_CE_PIN  , PULLUP ); 
    port_init_NoAlt (NRF_CSN_PIN , PULLUP ); 
    port_init_NoAlt (NRF_MOSI_PIN, PULLUP ); 
    port_init_NoAlt (NRF_SCK_PIN , PULLUP ); 
                     
    port_init_NoAlt (NRF_MISO_PIN, PULLUP ); 
    port_init_NoAlt (NRF_IRQ_PIN , PULLUP ); 
    
    NRF_CE  (0);                                    		// �������ģʽ
    NRF_CSN (1);                                    		// SPI��ֹ
    NRF_SCK (0);                                    		// SPIʱ���õ�
    
    NRF_CE (0);                                             // NRF_CE_PIN�õ�,�������ģʽ
    NRF_Check();                                            
    NRF_SPI_W_Reg_Byte(NRF_CMD_W_REGISTER | NRF_REG_CONFIG,     NRF_RCD_TX_DS_DI        // ���÷�������ж�
                                                              | NRF_RCD_MAX_RT_DI       // ���÷��ͳ�ʱ�ж�
                                                              | NRF_RCD_CRC_EN          // ����CRC
                                                              | NRF_RCD_CRC16           // CRC16
                                                              | NRF_RCD_PWR_UP          // �ϵ�ģʽ
                                                              | NRF_RCD_PRIM_PRX);      // RXģʽ
    NRF_SPI_W_Reg_Byte(NRF_CMD_W_REGISTER | NRF_REG_EN_AA,      NRF_RCD_PIPE0_AA_EN);   // ʹ��ͨ��0�Զ�Ӧ��
    NRF_SPI_W_Reg_Byte(NRF_CMD_W_REGISTER | NRF_REG_EN_RXADDR,  NRF_RCD_PIPE0_RX_EN);   // ʹ�ܽ���ͨ��0
    NRF_SPI_W_Reg_Byte(NRF_CMD_W_REGISTER | NRF_REG_SETUP_AW,   NRF_RCD_AW_5byte);      // ����RX��ַ���
    NRF_SPI_W_Reg_Byte(NRF_CMD_W_REGISTER | NRF_REG_SETUP_RETR, NRF_RCD_ARC_10|NRF_RCD_ARD_250us);        // ʹ���Զ��ط�  ���ʱ��250US
    NRF_SPI_W_Reg_Byte(NRF_CMD_W_REGISTER | NRF_REG_RF_CH,      NRF_RF_CH);             // ������Ƶͨ��
    NRF_SPI_W_Reg_Byte(NRF_CMD_W_REGISTER | NRF_REG_RF_SETUP,   NRF_RCD_RF_DR_2Mbps     // ���ô�������Ϊ2Mbps
                                                              | NRF_RCD_RF_PWR_0dBm);   // ���书��Ϊ0dBm
    NRF_SPI_W_Reg_Byte(NRF_CMD_W_REGISTER | NRF_REG_RX_PW_P0,   NRF_DATA_WIDTH);        // ����RXͨ��0���ݿ��

    NRF_SPI_W_Reg_Burst(NRF_CMD_W_REGISTER | NRF_REG_RX_ADDR_P0, NRF_RF_Addr, NRF_ADDR_WIDTH);   // ����RXͨ��0��ַ
    NRF_SPI_W_Cmd_Byte(NRF_CMD_FLUSH_RX);                                               // ���RXFIFO
    NRF_CE (1);                                                                     	// NRF_CE_PIN�ø�,��������ģʽ
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      NRF����ģ�����ݷ��ͺ���
//  @param      *p			Ҫ���͵����ݵ�ַ
//  @return     void
//  @since      v1.0
//  Sample usage:			��ʼ��nRF24L01+,������ת��Ϊ����״̬���������ݻ������е�����,������ɺ���ת��Ϊ����״̬
//-------------------------------------------------------------------------------------------------------------------
void NRF_Send_Packet(uint8 *p)
{
    uint8 sta=0;
    
    NRF_CE (0);                                                                     	// NRF_CE_PIN�õ�,�������ģʽ
    NRF_SPI_W_Reg_Byte(NRF_CMD_W_REGISTER | NRF_REG_CONFIG, NRF_RCD_RX_DR_DI            // ���÷�������ж�
                                                          | NRF_RCD_MAX_RT_DI           // ���÷��ͳ�ʱ�ж�
                                                          | NRF_RCD_CRC_EN              // ����CRC
                                                          | NRF_RCD_CRC16               // CRC16
                                                          | NRF_RCD_PWR_UP              // �ϵ�ģʽ
                                                          | NRF_RCD_PRIM_PTX);          // TXģʽ

    NRF_SPI_W_Reg_Burst(NRF_CMD_W_REGISTER | NRF_REG_TX_ADDR, NRF_RF_Addr, NRF_ADDR_WIDTH);  // ����TX��ַ
    NRF_SPI_W_Reg_Burst(NRF_CMD_W_TX_PAYLOAD, p, NRF_DATA_WIDTH);                       // д�뷢������
    NRF_CE (1);                                                                     	// NRF_CE_PIN�ø�,��������ģʽ

    while(1)
    {
        sta = NRF_SPI_R_Reg_Byte(NRF_CMD_R_REGISTER | NRF_REG_STATUS);
        if(sta&0x10)//����ʧ��
        {
            NRF_SPI_W_Reg_Byte(NRF_CMD_W_REGISTER | NRF_REG_STATUS, 0x10); 				// д��TX_DR,����NRF_IRQ_PIN�ж�
            NRF_SPI_W_Reg_Byte(NRF_CMD_FLUSH_TX,0xff);
            break;
        }
        if(sta&0x20)//���ͳɹ�
        {
            NRF_SPI_W_Reg_Byte(NRF_CMD_W_REGISTER | NRF_REG_STATUS, NRF_RCD_TX_DR); 	// д��TX_DR,����NRF_IRQ_PIN�ж�
            break;   
        }
    }
    NRF_CE (0);                                                                     	// NRF_CE_PIN�õ�,�������ģʽ
    NRF_SPI_W_Reg_Byte(NRF_CMD_W_REGISTER | NRF_REG_CONFIG,     NRF_RCD_TX_DS_DI        // ���÷�������ж�
                                                              | NRF_RCD_MAX_RT_DI       // ���÷��ͳ�ʱ�ж�
                                                              | NRF_RCD_CRC_EN          // ����CRC
                                                              | NRF_RCD_CRC16           // CRC16
                                                              | NRF_RCD_PWR_UP          // �ϵ�ģʽ
                                                              | NRF_RCD_PRIM_PRX);      // RXģʽ
    NRF_CE (1);                                                                     	// NRF_CE_PIN�ø�,��������ģʽ
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      NRF����ģ�����ݽ��պ���
//  @param      *p			׼�����յ����ݵ������ַ
//  @return     uint8		���ճɹ���־,Ϊ1ʱ�������ݽ��ճɹ�
//  @since      v1.0		
//  Sample usage:			���nRF24L01+�Ƿ���յ�����,���յ�����ʱ�����ݶ�ȡ���������ݻ�������
//-------------------------------------------------------------------------------------------------------------------
uint8 NRF_Rece_Packet(uint8 *p)
{
    uint8 sta=0;
    if(NRF_IRQ==0){                                                                 	// �ȴ�NRF_IRQ_PIN�õ�
        sta = NRF_SPI_R_Reg_Byte(NRF_CMD_R_REGISTER | NRF_REG_STATUS);                  // ��ȡSTATUS�Ĵ���
        if(sta & NRF_RCD_RX_DR){                                                        // RX_DSΪ1ʱ�������յ�����
            NRF_SPI_W_Reg_Byte(NRF_CMD_W_REGISTER | NRF_REG_STATUS, NRF_RCD_RX_DR);     // д��RX_DS,����NRF_IRQ_PIN�ж�
            NRF_SPI_R_Reg_Burst(NRF_CMD_R_RX_PAYLOAD, p, NRF_DATA_WIDTH);               // ��ȡnRF24L01+�еĽ�������
            return(1);                                                                  // ���ؽ�����ɱ�־
        }
    }
    return(0);                                                                          // ���ؽ��ռ��ʧ�ܱ�־
}





