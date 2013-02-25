/****************************************************************************
*
*�ļ�(File):         usbprog.c
*
*�޸�(Modify):       2011/8/9 11:37:56
*
*����(Author):       USER
*
*����(Compile):      ����ƽ̨(Smart Platform)
*
*����(Description):
*
*
*
----------------------------------------------------------------------------
|
| Version | Datetime             |   Author    | Description
| --------+----------------------+-------------+----------------------------
|
| V1.00  2011/8/9 11:37:56           USER 
----------------------------------------------------------------------------
****************************************************************************/

#include <API.h>
#include <usb.h>
//#include "D12ci.h"
//#include "db.h"
#include "usbprog.h"
#include "extra.h"
#include "d12.h"
#include "in_call.h"
//#include "Common.h"

//���ṹ�Ĵ�С
extern unsigned short g_rec_size[NUMOFDBTABLE];
//������Ŀ�����
extern char g_blocknum[NUMOFDBTABLE];	
//��������
extern unsigned char *g_strTableName[NUMOFDBTABLE];

struct TableInfo
{
	//���ڶ�������¼
	unsigned int TotalExist[NUMOFDBTABLE];
	//��Ч�ļ�¼��
	unsigned int TotalValid[NUMOFDBTABLE];
	//����¼�ĳ���
	unsigned int RecordSize[NUMOFDBTABLE];	
	//������Ŀ�����
	unsigned int RecordBlackCount[NUMOFDBTABLE];
};

//�������ܣ�
//	USB�ӿ����ݴ���
//�������룺
//	void
//�������أ�
//	0 �ɹ�
//	1 F1�˳�
//	2 û��USB�ӿ�
//	3 û��������PC
//	4 δ֪����
//	5 ��¼�ĳ��ȴ�С128
// 	6 ���ݿⲻ�ܴ��ڵ�ǰ�ļ�¼
//	7 ���Ӽ�¼ʧ��
int Usb_Data_Transmit(void)
{
	int fsid = 0;	
	char flag = 0;	
	int IntRet = 0;
	int RetValue = 0;
	int pos_type = 0;
	char CharTempA[20];	
	unsigned int DataLen = 0;	
	unsigned int RecordCount = 0;
	unsigned int RecordCountA = 0;
	struct TableInfo TableInfoObj;	
	unsigned char cmd_op = 0;
	unsigned char cmd_type = 0;
	USER_INFO *pTAB1_STRUCTObj = NULL;	
	unsigned char CharTemp[MAX_NUMBER_DT_DATA + 1];
	int g_MachineNo = 10;
	//char end_flag = 0;		//chen
	
	memset(&TableInfoObj, '\0', sizeof(struct TableInfo));

	pos_type = Sys_Get_POS_Type();
	if(pos_type == POS_ARM7_0528)
	{
		DispStr_CE(0,3,"û��USB�ӿ�",DISP_CENTER|DISP_CLRSCR);
		return(2);
	}

	if(USB_Init())
	{
		RetValue = 8;
		goto EndFlag;
		//return(8);
	}
	
	DispStr_CE(0, 2, "������������...", DISP_CENTER | DISP_CLRSCR);
	for(fsid = 0; fsid < NUMOFDBTABLE; fsid++)
	{
		count_db_toal(fsid,&TableInfoObj.TotalValid[fsid], &TableInfoObj.TotalExist[fsid]);	  
		TableInfoObj.RecordSize[fsid] = g_rec_size[fsid];
		TableInfoObj.RecordBlackCount[fsid] = g_blocknum[fsid];
	}

	DispStr_CE(0, 2, "USB���ݴ���...",DISP_CENTER|DISP_CLRSCR);
	while(1)
	{
		if(KEY_Read() == KEY_F1)	
		{
			RetValue = 1;
			goto EndFlag;
			//return(1);
		}		
		memset(CharTemp, '\0', sizeof(CharTemp));
		IntRet = USBDownLongString(CharTemp, &DataLen);
		if(IntRet == 0)
		{
			cmd_op = CharTemp[0];
			cmd_type = CharTemp[1];

			//ѯ���ֳ��豸��PC������״̬
			if((cmd_op == USB_CMD_REQ) && (cmd_type == USB_CMD_DISCONNECT))
			{				
				memset(CharTemp, '\0', sizeof(CharTemp));
				CharTemp[0] = USB_CMD_RTC;
				CharTemp[1] = USB_CMD_DISCONNECT;
				IntRet = USBUpLongString(CharTemp, 2);
				if(IntRet == -1)
				{
					RetValue = 1;
					goto EndFlag;
					//return(1);
				}
				else if(IntRet == -2)
				{
					RetValue = 3;
					goto EndFlag;				
					//return(3);
				}
//				DispStr_CE(0, 2, "USB���ݴ���...",DISP_CENTER|DISP_CLRSCR);
			}					
			//���ص�ǰ������Ϣ
			else if((cmd_op == USB_CMD_REQ) && (cmd_type == USB_CMD_TABLE_INFO))
			{
				DispStr_CE(0, 2, "���ͱ���ϢC...", DISP_CENTER|DISP_CLRSCR);
				
				//�������
				fsid = CharTemp[2];
				memset(CharTemp, '\0', sizeof(CharTemp));
				CharTemp[0] = USB_CMD_RTC;
				CharTemp[1] = USB_CMD_TABLE_INFO;
				
				//���д��ڵļ�¼����
				CharTemp[2] = (unsigned char)(TableInfoObj.TotalExist[fsid] >> 24);
				CharTemp[3] = (unsigned char)(TableInfoObj.TotalExist[fsid] >> 16);
				CharTemp[4] = (unsigned char)(TableInfoObj.TotalExist[fsid] >> 8);
				CharTemp[5] = (unsigned char)TableInfoObj.TotalExist[fsid];			

				//������Ч�ļ�¼��
				CharTemp[6] = (unsigned char)(TableInfoObj.TotalValid[fsid] >> 24);
				CharTemp[7] = (unsigned char)(TableInfoObj.TotalValid[fsid] >> 16);
				CharTemp[8] = (unsigned char)(TableInfoObj.TotalValid[fsid] >> 8);
				CharTemp[9] = (unsigned char)TableInfoObj.TotalValid[fsid];			

				//����¼�ĳ���
				CharTemp[10] = (unsigned char)(TableInfoObj.RecordSize[fsid] >> 24);
				CharTemp[11] = (unsigned char)(TableInfoObj.RecordSize[fsid] >> 16);
				CharTemp[12] = (unsigned char)(TableInfoObj.RecordSize[fsid] >> 8);
				CharTemp[13] = (unsigned char)TableInfoObj.RecordSize[fsid];			

				//������Ŀ�����
				CharTemp[14] = (unsigned char)(TableInfoObj.RecordBlackCount[fsid] >> 24);
				CharTemp[15] = (unsigned char)(TableInfoObj.RecordBlackCount[fsid] >> 16);
				CharTemp[16] = (unsigned char)(TableInfoObj.RecordBlackCount[fsid] >> 8);
				CharTemp[17] = (unsigned char)TableInfoObj.RecordBlackCount[fsid];			
				
				IntRet = USBUpLongString(CharTemp, 18);
				if(IntRet == -1)
				{
					RetValue = 1;
					goto EndFlag;				
					//return(1);
				}
				else if(IntRet == -2)
				{
					RetValue = 3;
					goto EndFlag;				
					//return(3);
				}					
				
//				DispStr_CE(0, 2, "USB���ݴ���...",DISP_CENTER|DISP_CLRSCR);
				
			}
			//�ܹ�USB���ֳ��豸�ϵ���Ϣ����PC��
			else  if((cmd_op == USB_CMD_REQ) && (cmd_type == USB_CMD_SEND_DATA))
			{
				
				DispStr_CE(0, 2, "���ͱ���ϢA...",DISP_CENTER|DISP_CLRSCR);
				
				//�������
				fsid = CharTemp[3];
				if(TableInfoObj.RecordSize[fsid] > 128)
				{
					//����׼������
					memset(CharTemp, '\0', sizeof(CharTemp));
					CharTemp[0] = USB_CMD_RTC;
					CharTemp[1] = USB_CMD_SEND_DATA;				
					CharTemp[2] = USB_CMD_OPER_ERR;
					IntRet = USBUpLongString(CharTemp, 3);
					if(IntRet == -1)
					{
						RetValue = 1;
						goto EndFlag;				
						//return(1);
					}
					else if(IntRet == -2)
					{
						RetValue = 3;
						goto EndFlag;				
						//return(3);
					}					
					else
					{
						RetValue = 5;
						goto EndFlag;									
						//return(5);
					}
				}

				//���ݷ���
				//�Լ�¼Ϊ��λ�������ݣ���󳤶Ȳ��ܳ���128���ֽ�
				for(RecordCount = 0; RecordCount < TableInfoObj.TotalExist[fsid]; RecordCount++)
				{					
					DispStr_CE(0, 0, "���ڷ��ͱ���Ϣ",DISP_CENTER|DISP_CLRSCR);			
					//����׼������
					memset(CharTemp, '\0', sizeof(CharTemp));
					CharTemp[0] = USB_CMD_RTC;
					CharTemp[1] = USB_CMD_SEND_DATA;		
					CharTemp[2] = 0xff;		
					
					memset(CharTempA, '\0', sizeof(CharTempA));					
					sprintf(CharTempA, "�ܹ�%d����¼", TableInfoObj.TotalExist[fsid]);
					DispStr_CE(0, 2, CharTempA,DISP_CENTER);

					memset(CharTempA, '\0', sizeof(CharTempA));					
					sprintf(CharTempA, "��������%d��", RecordCount + 1);
					DispStr_CE(0, 4, CharTempA,DISP_CENTER);
					
					pTAB1_STRUCTObj = NULL;	
			    	 	pTAB1_STRUCTObj = (USER_INFO *)DB_jump_to_record(fsid, RecordCount, &flag);
			  	   	if(flag) 
					{
						continue;					
			     		}
										
					//����׼������
					memcpy(&CharTemp[3], pTAB1_STRUCTObj, TableInfoObj.RecordSize[fsid]);
					//DispStr_CE(0, 0, CharTemp ,DISP_CENTER|DISP_CLRSCR);
					IntRet = USBUpLongString(CharTemp, TableInfoObj.RecordSize[fsid] + 3);
					if(IntRet == -1)
					{
						RetValue = 1;
						goto EndFlag;				
						//return(1);
					}
					else if(IntRet == -2)
					{
						RetValue = 3;
						goto EndFlag;				
						//return(3);
					}						

					if(RecordCount == (TableInfoObj.TotalExist[fsid] - 1))
					{
						RetValue = 0;		//debug
						//USBUpLongString("over", 4);	//debug
						goto EndFlag;		//debug
						break;
					}
					USBDownLongString(CharTemp, &DataLen);
					//Sys_Delay_MS(200);
				}												
//				if (RecordCount == TableInfoObj.TotalExist[fsid]) {
//					RetValue = 0;
//					goto EndFlag;
//				}
//				DispStr_CE(0, 2, "USB���ݴ���...",DISP_CENTER|DISP_CLRSCR);
			}
			else  if((cmd_op == USB_CMD_REQ) && (cmd_type == USB_CMD_RECV_DATA))
			{
				//�������
				fsid = CharTemp[2];										
				RecordCountA = (((int)CharTemp[3] << 24) & 0xFF000000) + (((int)CharTemp[4] << 16) & 0x00FF0000) + (((int)CharTemp[5] << 8) & 0x0000FF00) + (int)CharTemp[6]; 
				/*
				{
					char CharTempB[20];
					memset(CharTempB, '\0', sizeof(CharTempB));
					sprintf(CharTempB, "%d,%d,%d,%d", RecordCountA, TableInfoObj.TotalExist[fsid], RecordCountA, DB_capacity(fsid));
					DispStr_CE(0,1,CharTempB,DISP_CENTER|DISP_CLRSCR);

					memset(CharTempB, '\0', sizeof(CharTempB));
					sprintf(CharTempB, "%d,%d,%d,%d,%d", CharTemp[3], CharTemp[4], CharTemp[5], CharTemp[6], DataLen);
					DispStr_CE(0,4,CharTempB,DISP_CENTER);
					
					//delay_and_wait_key(20, EXIT_KEY_ALL, 30);								
				}
				*/
				
				//�ж��Ƿ���ȫ�����浽���ݿ�
				if((TableInfoObj.TotalExist[fsid] + RecordCountA) >= DB_capacity(fsid))
				{
					//����׼������
					memset(CharTemp, '\0', sizeof(CharTemp));
					CharTemp[0] = USB_CMD_RTC;
					CharTemp[1] = USB_CMD_RECV_DATA;		
					CharTemp[2] = USB_CMD_OPER_ERR;		
					IntRet = USBUpLongString(CharTemp, 3);
					if(IntRet == -1)
					{
						RetValue = 1;
						goto EndFlag;				
						//return(1);
					}
					else if(IntRet == -2)
					{
						RetValue = 3;
						goto EndFlag;				
						//return(3);
					}	
					else
					{
						RetValue = 6;
						goto EndFlag;							
						//return(6);
					}
				}

				//for(RecordCount = 0; RecordCount < RecordCountA; RecordCount++)
				{
					DispStr_CE(0, 4, "���ռ�¼...",DISP_CENTER|DISP_CLRSCR);

					/*
					memset(CharTempA, '\0', sizeof(CharTempA));					
					sprintf(CharTempA, "��%d����¼", RecordCountA);
					DispStr_CE(0, 2, CharTempA,DISP_CENTER);

					memset(CharTempA, '\0', sizeof(CharTempA));					
					sprintf(CharTempA, "��������%d��", RecordCount + 1);
					DispStr_CE(0, 4, CharTempA,DISP_CENTER);
					*/
					
					//����׼������
					memset(CharTemp, '\0', sizeof(CharTemp));
					CharTemp[0] = USB_CMD_RTC;
					CharTemp[1] = USB_CMD_RECV_DATA;				
					IntRet = USBUpLongString(CharTemp, 2);
					if(IntRet == -1)
					{
						RetValue = 1;
						goto EndFlag;				
						//return(1);
					}
					else if(IntRet == -2)
					{
						RetValue = 3;
						goto EndFlag;				
						//return(3);
					}					

					memset(CharTemp, '\0', sizeof(CharTemp));
			      	IntRet = USBDownLongString(CharTemp, &DataLen);
					if(IntRet == -1)
					{
						RetValue = 1;
						goto EndFlag;				
						//return(1);
					}
					else if(IntRet == -2)
					{
						RetValue = 3;
						goto EndFlag;				
						//return(3);
					}											
					
					if(!DB_add_record(fsid, (void*)CharTemp))
					{                     
						RetValue = 7;
						goto EndFlag;						
						//return(7);                                                           		
					}					
					TableInfoObj.TotalExist[fsid]++;
					TableInfoObj.TotalValid[fsid]++;                                                                             		
				}		
				DispStr_CE(0, 2, "USB���ݴ���...",DISP_CENTER|DISP_CLRSCR);
			}	
			else if((cmd_op == USB_CMD_REQ) && (cmd_type == USB_CMD_DEL_FSID))
			{									
				DispStr_CE(0, 3, "ɾ����¼", DISP_CENTER | DISP_CLRSCR);
				RetValue = DB_erase_filesys(CharTemp[2]);
				memset(CharTemp, '\0', sizeof(CharTemp));
				CharTemp[0] = USB_CMD_RTC;
				CharTemp[1] = USB_CMD_DEL_FSID;				
				if(RetValue == 0)
				{					
					CharTemp[2] = 0;
				}
				else if(RetValue == -1)
				{
					CharTemp[2] = 1;
				}
				else 
				{
					CharTemp[2] = 1;					
				}
						
				IntRet = USBUpLongString(CharTemp, 2);
				if(IntRet == -1)
				{
					RetValue = 1;
					goto EndFlag;
				}
				else if(IntRet == -2)
				{
					RetValue = 3;
					goto EndFlag;				
				}
				DispStr_CE(0, 2, "USB���ݴ���...",DISP_CENTER|DISP_CLRSCR);
			}				
			else if((cmd_op == USB_CMD_REQ) && (cmd_type == USB_CMD_SENDMACHINEPARA))
			{	
				warning_beep(1);
				DispStr_CE(0, 3, "�ϴ��ֳֻ�����", DISP_CENTER | DISP_CLRSCR);
				
				memset(CharTemp, '\0', sizeof(CharTemp));
				CharTemp[0] = USB_CMD_RTC;
				CharTemp[1] = USB_CMD_SENDMACHINEPARA;				
				CharTemp[2] = g_MachineNo & 0xFF;				
						
				IntRet = USBUpLongString(CharTemp, 3);
				if(IntRet == -1)
				{
					RetValue = 1;
					goto EndFlag;
				}
				else if(IntRet == -2)
				{
					RetValue = 3;
					goto EndFlag;				
				}
				DispStr_CE(0, 2, "USB���ݴ���...",DISP_CENTER|DISP_CLRSCR);
			}				
		}
									
		//F1 ����
		else if(IntRet == -1)
		{
			RetValue = 1;
			goto EndFlag;			
			//return(1);				
		}
		//û��������PC
		else if(IntRet == -2)
		{
			RetValue = 3;
			goto EndFlag;					
			//return(3);				
		}
		else
		{				
			RetValue = 4;
			goto EndFlag;					
			//return(4);		
		}				
	}
EndFlag:
	USB_Disconnect();
	return(RetValue);	
}


//�������ܣ�
//	���մ�PC�˴������������
//�������룺
//	unsigned char *DataBuffer
//		�������ݵĻ�����
//	unsigned long *BufferLen
//		���ݵĳ���
//�������أ�
//	0 �����ɹ�
//	-1 F1������
//	-2 û��������PC
int USBDownLongString(unsigned char DataBuffer[], unsigned int *DataLen)
{
	short ret = -1;					// ��������ֵ
	short cnt_now;
	unsigned short	i_st;						// �ж�����
	unsigned int	 expect_len;
	unsigned int real_len = 0;	
	unsigned char recvbuf[100];				// ���ջ�����
	/*
	if(USB_Init())
	{
		return(-2);
	}
	*/
	for(;;)
	{
		if(KEY_Read() == KEY_F1)	
		{
			ret = -1;
			goto end;
		}
		i_st = D12_Read_Interrupt_Register();				// ��ȡPDIUSBD12�жϼĴ���ֵ
		if(i_st != 0) 
		{
			if(i_st & D12_INT_SUSPENDCHANGE)
				USB_Suspend();								// ���߹���ı�
			if(i_st & D12_INT_ENDP0IN)
				USB_EP0_Txdone();							// ���ƶ˵㷢�����ݴ���
			if(i_st & D12_INT_ENDP0OUT)
				USB_EP0_Rxdone();							// ���ƶ˵�������ݴ���
			if(i_st & D12_INT_ENDP1OUT)
			{
				D12_Read_Last_Status(2);
				D12_Read_Endpoint(2, 16, recvbuf);
				if(recvbuf[0] == 'E') 
				{
					break;
				}
				else if(recvbuf[0] != 'S')					// ����ϴ�ģʽ
				{
					memcpy(&expect_len,recvbuf,4);
					D12_Write_Endpoint(3, 4, (unsigned char*)&expect_len); 			
				}			
			}
			if(i_st & D12_INT_ENDP2OUT)
			{
				D12_Read_Last_Status(4);
				cnt_now = D12_Read_Endpoint(4, 64, recvbuf);
				memcpy((DataBuffer + real_len), recvbuf, cnt_now);
				real_len += cnt_now;
			}
		}
	}
	*DataLen = real_len;
	//if(real_len == expect_len)
		ret = 0;
end:
//	USB_Disconnect();
	return ret;
}


//�������ܣ�
//	���մ�PC�˴������������
//�������룺
//	unsigned char *DataBuffer
//		�������ݵĻ�����
//	unsigned long *BufferLen
//		���ݵĳ���
//�������أ�
//	0 �����ɹ�
//	-1 F1������
//	-2 û��������PC
//ע��
//	�˺�����ܴ���128���ֽڵ����ݡ�
int USBUpLongString(unsigned char DataBuffer[], unsigned int DataLen)
{
	int blk_cnt;						// ������
	int blk_res;						// ����1024�ֽڵ�ʣ���ֽ���
	int blk_idx=0;					// ѭ������
	int cnt_now;						// ���η����ֽ���
	int chr_idx;						// ����ѭ������
	short ret = -1;					// ��������ֵ		
	unsigned short i_st;					// �ж�����
	unsigned char recvbuf[4];		// ���ջ�����
	/*
	if(USB_Init())
	{
		ret = -2;
		return ret;
	}
	*/
	blk_cnt = (DataLen / 64);						// �������
	blk_res = (DataLen % 64);
	blk_cnt += ( (blk_res>0) ? 1 : 0 );
	blk_res = ((blk_res==0) ? 64 : blk_res);
	
	for(;;)
	{
		if(KEY_Read() == KEY_F1)	
		{
			ret = -1;
			goto end;
		}
		i_st = D12_Read_Interrupt_Register();				// ��ȡPDIUSBD12�жϼĴ���ֵ
		if(i_st != 0) 
		{
			if(i_st & D12_INT_SUSPENDCHANGE)
				USB_Suspend();								// ���߹���ı�
			if(i_st & D12_INT_ENDP0IN)
				USB_EP0_Txdone();							// ���ƶ˵㷢�����ݴ���
			if(i_st & D12_INT_ENDP0OUT)
				USB_EP0_Rxdone();							// ���ƶ˵�������ݴ���
			if(i_st & D12_INT_ENDP1OUT)
			{
				D12_Read_Last_Status(2);
				D12_Read_Endpoint(2,1,recvbuf);
				if(recvbuf[0] == 'S')
				{					
					D12_Read_Last_Status(3);
					D12_Write_Endpoint(3, 4, (unsigned char*)&DataLen);			
				}
				if(recvbuf[0] == 'E') 
				{
					break;
				}				
			}
			if(i_st & D12_INT_ENDP2OUT)
			{	
				D12_Read_Last_Status(4);
				D12_Read_Endpoint(4, 1, recvbuf);
				if(recvbuf[0] == 'S')
				{
					for(chr_idx = 0; chr_idx < 2; chr_idx++)	// һ�η��� 128 byte
					{
						if(blk_idx >= blk_cnt)
							break;
							 		
						cnt_now = ((blk_idx != (blk_cnt - 1)) ? 64 : blk_res );
						D12_Read_Last_Status(5);
						D12_Write_Endpoint(5, cnt_now, (DataBuffer + 64 * blk_idx));
						blk_idx++;	
					}
				}
			}
		}
	}
	ret = 0;
end:
//	USB_Disconnect();
	return ret;
}



