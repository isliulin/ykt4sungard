/* --------------------------------------------
 * ��������: F847130.c
 * ��������: 2005-12-7
 * ��������: �Ž�
 * �汾��Ϣ: 1.0.0.0
 * ������: EXECL ������Ϣ����
 * --------------------------------------------
 * �޸�����:
 * �޸���Ա:
 * �޸�����:
 * �汾��Ϣ:
 * ��ע��Ϣ:
 * --------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpack.h"
#include "errdef.h"
#include "pubdef.h"
#include "pubdb.h"
#include "pubfunc.h"
#include "dbfunc.h"
#include "fdsqc.h"

int F847130(TRUSERID *handle,int iRequest,ST_PACK *rPack,int *pRetCode,char *szMsg)
{
	int ret=0;
	int i=0;
	int iCnt=0;
	int flag=0;
	double dTotalAmt=0.0;
	T_t_tif_subsidytmp tSubsidyTmp;
	T_t_tif_subsidytmp tSubsidyTmpOut;
	T_t_tif_subsidy  tSubsidy;
	ST_CPACK aPack;
	ST_PACK *out_pack = &(aPack.pack);

	memset(&tSubsidyTmp,0,sizeof(T_t_tif_subsidytmp));
	ResetNormalCPack(&aPack,0,1);
	SetCol(handle,0);
	SetCol(handle,F_SCUST_LIMIT,F_VSMESS,0);

	tSubsidyTmp.seqno=rPack->lvol1;					//˳���
	if(strlen(rPack->scust_no)<1)
	{
		*pRetCode=E_INPUT_OPER_NULL;
		goto L_RETU;
	}
	
	if(1==tSubsidyTmp.seqno)
	{
		flag=1;
		//ȡ���κ�
		getsysdatetime(tSubsidyTmp.batch_no);
		ret=DB_t_tif_subsidytmp_del_by_oper_code(rPack->scust_no);
		if(ret)
		{
			if(DB_NOTFOUND!=ret)
			{
				*pRetCode=E_DB_SUBSIDY_D;
				goto L_RETU;
			}
		}
	}
	else
	{
		des2src(tSubsidyTmp.batch_no,rPack->scust_limit);
		if(strlen(tSubsidyTmp.batch_no)!=14)
		{
			*pRetCode=E_INPUT_BATCH_NO;
			goto L_RETU;
		}
	}
	getsysdate(tSubsidyTmp.tx_date);
	getsystime(tSubsidyTmp.tx_time);
	des2src(tSubsidyTmp.stuemp_no,rPack->scust_auth);	//ѧ��
	des2src(tSubsidyTmp.subsidy_no,rPack->sdate0);		//�������κ�
	des2src(tSubsidyTmp.oper_code,rPack->scust_no);	//����Ա����
	des2src(tSubsidyTmp.expire_date,rPack->sdate2);		//��������
	ret=IsInvalidDateTime(tSubsidyTmp.expire_date,"YYYYMMDD");
	if(ret)
	{
		sprintf(szMsg,"ѧ��%s�����ڸ�ʽ����ȷ,��ȷ��ʽΪYYYYMMDD",tSubsidyTmp.stuemp_no);
		*pRetCode=ret;
		goto L_RETU;
	}
	tSubsidyTmp.amount=rPack->damt0;				//�������
	strcpy(tSubsidyTmp.status,"1");						//״̬(1-δ��ȡ��2-����ȡ)
	//���һ��ͨϵͳ�Ƿ񲻴��ڸ�ѧ/����
	ret=IsInexistenceStuEmp(tSubsidyTmp.stuemp_no);
	if(ret)
	{
		if(E_STUNO_NOT_EXIST==ret)
		{
			sprintf(szMsg,"ѧ��%s��һ��ͨϵͳ������",tSubsidyTmp.stuemp_no);
		}
		*pRetCode=ret;
		goto L_RETU;
	}
	ret=DB_t_tif_subsidytmp_add(&tSubsidyTmp);
	if(ret)
	{
		if(DB_REPEAT==ret)
		{
			if(1==tSubsidyTmp.seqno)
			{
				//��������г�ͻ���������������µ����κ�
				for(i=0;i<3;i++)
				{
					sleep(1);
					getsysdatetime(tSubsidyTmp.batch_no);
					ret=DB_t_tif_subsidytmp_add(&tSubsidyTmp);
					if(ret)
					{
						if(DB_REPEAT==SQLCODE)
							continue;
						else
						{
							*pRetCode=E_DB_SUBSIDYTMP_I;
							goto L_RETU;
						}
					}
					break;
				}
				if(i>=3)
				{
					writelog(LOG_ERR,"batch_no[%s]seqno[%d]",tSubsidyTmp.batch_no,tSubsidyTmp.seqno);
					*pRetCode=E_DB_SUBSIDYTMP_E;
					goto L_RETU;
				}
			}
			else
			{
				writelog(LOG_ERR,"batch_no[%s]seqno[%d]",tSubsidyTmp.batch_no,tSubsidyTmp.seqno);
				*pRetCode=E_DB_SUBSIDYTMP_E;
				goto L_RETU;
			}
		}
		else
		{
			writelog(LOG_ERR,"batch_no[%s]seqno[%d]",tSubsidyTmp.batch_no,tSubsidyTmp.seqno);
			*pRetCode=E_DB_SUBSIDYTMP_I;
			goto L_RETU;
		}
	}
	if(rPack->scust_type[0]=='1')
	{
		//���һ��,��ʼ����ʱ�����ݵ��뵽��ʽ��
		writelog(LOG_DEBUG,"�����%d����¼,���κ�:%s,ѧ��:%s",tSubsidyTmp.seqno,tSubsidyTmp.batch_no,tSubsidyTmp.stuemp_no);
		iCnt=0;
		ret=DB_t_tif_subsidytmp_open_select_by_c0_and_batch_no(tSubsidyTmp.batch_no);
		while(1)
		{
			memset(&tSubsidyTmpOut,0,sizeof(tSubsidyTmpOut));
			ret=DB_t_tif_subsidytmp_fetch_select_by_c0(&tSubsidyTmpOut);
			if(ret)
			{
				if(DB_NOTFOUND==ret)
				{
					if(iCnt>0)
						break;
					else
					{
						*pRetCode=E_DB_SUBSIDYTMP_N;
						goto L_RETU;
					}
				}
				else
				{
					*pRetCode=E_DB_SUBSIDYTMP_R;
					goto L_RETU;
				}
			}
			memcpy(&tSubsidy,&tSubsidyTmpOut,sizeof(tSubsidy));
			//���벹����Ϣ��
			ret=DB_t_tif_subsidy_add(&tSubsidy);
			if(ret)
			{
				DB_t_tif_subsidytmp_close_select_by_c0();
				if(DB_REPEAT==ret)
					*pRetCode=E_DB_STUEMPNO_RE;
				else
					*pRetCode=E_DB_SUBSIDY_I;
				sprintf(szMsg,"���κ�%s,��%d��ѧ��Ϊ%s�ļ�¼����,���ܸ�ѧ�ŵĲ�����¼�Ѿ�����",tSubsidy.batch_no,tSubsidy.seqno,tSubsidy.stuemp_no);
				goto L_RETU;
			}
			dTotalAmt+=tSubsidy.amount;
			++iCnt;
		}
		sprintf(out_pack->vsmess,"���κ�:%s,���ɹ�����%d��������Ϣ,��������ܹ�%.2lfԪ",tSubsidyTmp.batch_no,iCnt,dTotalAmt);
		flag=2;
	}
	if(flag!=2)
	{
		sprintf(out_pack->vsmess,"�����%d����¼,���κ�:%s,ѧ��:%s",tSubsidyTmp.seqno,tSubsidyTmp.batch_no,tSubsidyTmp.stuemp_no);
	}
	des2src(out_pack->scust_limit,tSubsidyTmp.batch_no);
	PutRow(handle,out_pack,pRetCode,szMsg);
	writelog(LOG_DEBUG,out_pack->vsmess);
	return 0;
L_RETU:
	return -1;
}