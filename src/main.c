/*
 * Copyright (c) 2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/kernel.h>
#include <linux/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include "iic_1848.h"
#include "gpio.h"

extern void hlSrioInit();
extern int Init_1848();
extern unsigned int SRIO_LOCAL_REG_READ(unsigned int offset);
extern void SRIO_LOCAL_REG_WRITE(unsigned int  offset, unsigned int writedata);
extern void SlectUartMenu(void);
extern void NetInit();
extern int Xil_In32(uint64_t phyaddr);
extern void Xil_Out32(uint64_t phyaddr, uint32_t val);
extern void WriteLogFlash();
extern void WriteLogFlashFinish();
extern int IicSelfTest();
extern void LinkDetect();

unsigned int g_slotNum = 6;
unsigned int g_chassisNum = 0;

void SetGpioReg(unsigned int addrBase,unsigned int addrOffset,unsigned int value)
{
	Xil_Out32(addrBase+addrOffset, value);
}

int GetGpioReg(unsigned int addrBase,unsigned int addrOffset)
{
	int ans=0;

	ans=Xil_In32(addrBase+addrOffset);
	return ans;
}

int main()
{
	unsigned int _speed = 0;
	unsigned int _SrioDelayTime = 25;
	unsigned short _bSrio=1;
	unsigned int _ret=0,_data=0;
	unsigned short _bSysReset = 0;
	FILE *pFile = NULL;
	pid_t fpid;

	printf("\r\n-----------------Linux Switch_App start(Version 1.0)-------------------\r\n");

	/*read slot id*/
	SetGpioReg(GPIO_BASE_ADDR,0x4,0xFFFFFFFF);
	_data = GetGpioReg(GPIO_BASE_ADDR,0x0);

	g_chassisNum = (_data & 0x1e0)>>5;
	g_slotNum = ~_data & 0x1f;
	printf("chassisNum = %d, slotNum = %d\r\n",g_chassisNum,g_slotNum);

	Init_1848();

	pFile = fopen("/mnt/delayTime.txt","rb");
	if( pFile == NULL )
	{
		printf("��ö��ʱ���ļ�delayTime.txtʧ�ܣ�����Ĭ��ö��ʱ��Ϊ25S.\n");
		_bSrio = 1;
		_SrioDelayTime = 25;
	}
	else
	{
		fread((void*)&_SrioDelayTime,1,4,pFile);
		fread((void*)&_bSrio,1,2,pFile);
		fclose( pFile );
	}

	pFile = fopen("/mnt/speed.txt","rb");
	if( pFile == NULL )
	{
		printf("���ٶ������ļ�speed.txtʧ�ܣ�����Ĭ���ٶ�Ϊ3.125G.\n");
		_speed = 0;
	}
	else
	{
		fread((void*)&_speed,1,4,pFile);
		fclose( pFile );
	}

	if( _speed == 0 )	//3.125G
	{
		printf("������Ĭ����������Ϊ 3.125G, ����Ҫ�޸�.\r\n");
	}
	else if( _speed == 1 )	//5.0G
	{
		iic_SwSpeedBaudSet(1);
	}
	else if( _speed == 2 )	//6.25G
	{
		iic_SwSpeedBaudSet(2);
	}
	else
	{
		printf("���󣡶�ȡ�ٶ�ģʽʧ�ܣ���ǰ��ȡ�ٶ�ģʽΪ speed = %d\r\n",_speed);
	}

	//����ttl 3.2ms
   	iic_TTL_csr(0x7d00000);
   	//ʹ�ܶ˿�ͳ��
   	iic_SwPortCounterEnable();

   	printf("\r\n----�Ƿ��Զ�ö�� = %d,ö���ӳ�ʱ��Ϊ = %d(s)----\r\n",_bSrio,_SrioDelayTime);
   	if( _SrioDelayTime > 300 )
	{
		_SrioDelayTime = 25;
		printf("\r\n----�Զ�ö��ʱ������̫��,�Զ��޸�Ϊ 25(s)----\r\n");
	}

   	sleep(_SrioDelayTime);

   	//������
	iic_SrioErrorDetect();
	//����·״̬
	iic_SwPortRepair();
	//�������״̬
	iic_SwPortClear();

	_ret = SRIO_LOCAL_REG_READ(0x158);
	printf("----zynq 158----0x%x\r\n",_ret);
	do
	{
		if ((_ret & 0x030300) == 0)
			break;

		printf("----recover zynq srio----\r\n");
		SRIO_LOCAL_REG_WRITE(0x158,_ret);
		usleep(10000);
		_ret = SRIO_LOCAL_REG_READ(0x158);
	} while (1);

	if( (_bSrio == 1) && (g_slotNum == 6) )
	{
		WriteLogFlash();
		hlSrioInit();
		WriteLogFlashFinish();
	}

	//�ж��Ƿ���Ҫϵͳ��λ
	pFile = fopen("/mnt/sysResetFile.txt","rb");
	if( pFile == NULL )
	{
		printf("��ϵͳ��λ�����ļ�sysResetFile.txtʧ�ܣ�����Ĭ�ϲ�����ϵͳ��λ _bSysReset = 0.\n");
		_bSysReset = 0;
	}
	else
	{
		fread((void*)&_bSysReset,1,2,pFile);
		fclose( pFile );
	}

	if( _bSysReset != 0 )
		iic_SystemReset();

	if( g_slotNum == 6 )
		NetInit();

	fpid = fork();

	if( fpid <0 )
	{
		printf("fork error!\n");
	}
	else if( fpid == 0 )			//������
	{
		while(1)
		{
			IicSelfTest();			//i2c�Լ��ϱ�
			LinkDetect();			//����������·���
		}
	}
	else
	{
		SlectUartMenu();
	}


    return 0;
}



