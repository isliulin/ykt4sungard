/* --------------------------------------------
 * ��������: F930029.cpp
 * ��������: 2007-10-15
 * ��������: ����
 * �汾��Ϣ: 1.0.0.0
 * ������:  �ռ�ϴ�»�������ˮ
 * --------------------------------------------*/
#define _IN_SQC_
#include <stdio.h>
#include <string.h>
#include "pubdef.h"
#include "errdef.h"
#include "pubfunc.h"
#include "pubdb.h"
#include "dbfunc.h"
#include "account.h"
#include "busqc.h"

static int check(T_t_tif_rcvdtl *p)
{
	int ret=0;
	if(strlen(p->crc)==0)
	{
		return E_999_CRC;							//�ϴ���ˮCRCУ�����
	}
	if(strcmp(p->tx_date,"20000000") != 0)
	{
		ret=IsInvalidDateTime(p->tx_date,"YYYYMMDD");
		if(ret)
		{
			return E_999_DATE;						//�ϴ���ˮ������������
		}
	}
	else
		getsysdate(p->tx_date);
	if(strcmp(p->tx_time,"000000") != 0)
	{
		ret=IsInvalidDateTime(p->tx_time,"HHMMSS");
		if(ret)
		{
			return E_999_TIME;							//�ϴ���ˮ����ʱ������
		}
	}
	else
		getsystime(p->tx_time);
	// &&(p->tx_mark!=0xF0)
	
	if((p->tx_mark!=0XFA))
	{
		return 	p->tx_mark+E_999_0X00;			//�ѷ�������Ϊ���ױ��
	}
	if(p->in_bala - p->amount != p->out_bala)
	{
		return E_999_CARD_BALANCE;					//�ϴ���ˮ���뿨�������
	}
	return 0;
}

int F950029(TRUSERID *handle,int iRequest,ST_PACK *rPack,int *pRetCode,char *szMsg)
{

	int ret=0;
	T_t_tif_rcvdtl tRcvdtl;

	/*
	ret=chk_dyn_key(rPack->lcert_code,rPack->scust_limit2);
	if(ret)
	{
		writelog(LOG_ERR,"subsys_id[%d]dyn_key[%s]",rPack->lcert_code,rPack->scust_limit2);
		*pRetCode =  ret;
		goto L_RETU;
	}
	*/
	memset(&tRcvdtl,0,sizeof(tRcvdtl));

	tRcvdtl.serial_no= rPack->lvol4;							//�ϴ�����ˮ��
	tRcvdtl.tx_mark=rPack->lvol12;							//999���ױ��
	tRcvdtl.sys_id  = rPack->lcert_code;						//�ϴ�����վ��ʶ(ǰ�û�ע���)
	des2src(tRcvdtl.deviceid, rPack->sdate1);				//�����豸ID
	des2src(tRcvdtl.inpower_no,rPack->stx_pwd);				//�ն��豸��Ȩ��
	tRcvdtl.comu_ver=rPack->lbank_acc_type;					//ͨ�Ű汾��
	int ver = tRcvdtl.comu_ver / 10;
	if( 0x81 == tRcvdtl.comu_ver || 0x81 == ver )
	{
//	tRcvdtl.run_reason = rPack->lbank_acc_type2;				//����ԭ��
		tRcvdtl.fee_code = rPack->lbank_acc_type2;				// ������ԭ�򱣴浽�շѿ�Ŀ��
	}
	else
	{
		tRcvdtl.fee_code = 0;
	}
	des2src(tRcvdtl.crc,rPack->sbank_code2);				//CRCУ��

	//tRcvdtl.man_fee = rPack->lvol11;						//
	tRcvdtl.cardno= rPack->lvol5;							//���׿���
	//tRcvdtl.purse_no = rPack->lvol6;						//����Ǯ����
	tRcvdtl.purse_no = 0;
	tRcvdtl.in_bala= rPack->lvol9;							//�뿨���
	tRcvdtl.out_bala = rPack->lvol10;						//�������
	tRcvdtl.amount = rPack->lvol8;							//�������ѽ��
	if(tRcvdtl.amount>0)
		tRcvdtl.amount = -tRcvdtl.amount;
	tRcvdtl.total_cnt= rPack->lvol7;							//��ǰ�����ʻ����Ѵ���(�ۼ�ʹ�ô���)
	tRcvdtl.tx_code = 930031;								//���״���
 	snprintf(tRcvdtl.tx_date,9,"20%s",rPack->spost_code);		//��������(��ʽ�����������)
	des2src(tRcvdtl.tx_time,rPack->spost_code2);			//����ʱ��(��ʽ�������ʱ��)
	getsystimestamp(tRcvdtl.col_timestamp);
	ret=check(&tRcvdtl);
	if(ret)
	{
		tRcvdtl.err_code=ret;
		if(tRcvdtl.tx_mark==240 || tRcvdtl.tx_mark==2)
		{
			tRcvdtl.status[0]='6';							//��;�ο�����
			// ���Ϊ��;�ο�
			tRcvdtl.tx_mark = 2;
		}
		else
		{
			tRcvdtl.status[0]='5';								//��Ч����
			strncpy(tRcvdtl.deal_date,tRcvdtl.col_timestamp,8);
			strncpy(tRcvdtl.deal_time,tRcvdtl.col_timestamp+8,6);
		}
	}
	else
		tRcvdtl.status[0]='1';
	g_dbchkerr=0;
	ret = DB_t_tif_rcvdtl_add(&tRcvdtl);
	if (ret)
	{
		g_dbchkerr=1;
		writelog(LOG_DEBUG,"serial_no[%d]cardno[%d]deviceid[%s]tx_date[%s]tx_time[%s]",tRcvdtl.serial_no,tRcvdtl.cardno,tRcvdtl.deviceid,tRcvdtl.tx_date,tRcvdtl.tx_time);
		if(DB_REPEAT==ret)
			return 0;
		else
			*pRetCode = E_DB_TRADESERIAL_I;
		goto L_RETU;
	}
	g_dbchkerr=1;
	return 0;
L_RETU:
	return -1;
}
