#include <stdio.h>
#include <unistd.h>

extern int IicSelfTest();
extern void hlSrioInit();
extern void hlSrioTopoShow();
extern void hlSrioLUTShow();
extern void hlSrioReInit();
extern void IIC_SrioPortStaticShow();
extern void UartSetSrioDelay(unsigned short bEnum,int delay);
extern void iic_SrioErrorDetect();
extern void iic_SwPortRepair();
extern void iic_SwPortClear();
extern void iic_TTL_csr(unsigned int ttl_count);
extern void iic_SwSpeedBaudSet(unsigned int speedIndex);
extern void iic_SwPLLLaneShow();
extern void OpenLogFile();
extern void ReadLogInfo();
extern void CloseLogFile();
extern void OpenSysReset();
extern void CloseSysReset();

extern unsigned int g_slotNum;
static unsigned char _precRecvData[5]={'*','*','*','*','*'};		/*For Mis-operation Protection, add by GZXu,2017.04.10*/
static int curIndex = 0;											/*For Mis-operation Protection, add by GZXu,2017.04.10*/

void ShowTab()
{
	int _usec =8000;

	printf ("\r\n------------����������鿴�����ò˵��������������ڴ���������'enter'--------------\r\n");
	usleep(_usec);
	printf("Keyboard-(1)	�鿴ö�����˽ṹ.\r\n");
	usleep(_usec);
	printf("Keyboard-(2)	�鿴ö��·�ɱ�.\r\n");
	usleep(_usec);
	printf("Keyboard-(3)	�鿴�������ڲ��˿ڼ���ͳ��.\r\n");
	usleep(_usec);
	printf("Keyboard-(4)	���ý����忪�����Զ�ö��.\r\n");
	usleep(_usec);
	printf("Keyboard-(5)	���ý���������ֶ�ö��.\r\n");
	usleep(_usec);
	printf("Keyboard-(6)	���ý����忪��5�������Զ�ö��.\r\n");
	usleep(_usec);
	printf("Keyboard-(7)	���ý����忪��25�������Զ�ö��.\r\n");
	usleep(_usec);
	printf("Keyboard-(8)	���ý����忪��50�������Զ�ö��.\r\n");
	usleep(_usec);
	printf("Keyboard-(9)	���ý����忪��70�������Զ�ö��.\r\n");
	usleep(_usec);
	printf("Keyboard-(a)	���ý����忪��90�������Զ�ö��.\r\n");
	usleep(_usec);
	printf("Keyboard-(0)	���ý���������ֶ���ö��.\r\n");
	usleep(_usec);
	printf("Keyboard-(d)	�鿴�������ڲ���·����������Ϣ.\r\n");
	usleep(_usec);
	printf("Keyboard-(r)	���ý�����˿ڼ�Lane��·����״̬�޸�.\r\n");
	usleep(_usec);
	printf("Keyboard-(t)	���ý�����TTLʹ�ܣ�TTLֵ��Ϊ32����.\r\n");
	usleep(_usec);
	printf("Keyboard-(f)	���ý�����TTL��ʹ��.\r\n");
	usleep(_usec);
	printf("Keyboard-(s)	���ý�������˿�����Ϊ3.125G��Ĭ��ֵ��.\r\n");
	usleep(_usec);
	printf("Keyboard-(w)	���ý�������˿�����Ϊ5.0G.\r\n");
	usleep(_usec);
	printf("Keyboard-(l)	���ý�������˿�����Ϊ6.25G.\r\n");
	usleep(_usec);
	printf("Keyboard-(p)	�鿴��������˿ڵ�������Ϣ.\r\n");
	usleep(_usec);
	printf("Keyboard-(c)	�鿴������ö����־.\r\n");
	usleep(_usec);
	printf("Keyboard-(C)	�������´�ö����־����.\r\n");
	usleep(_usec);
	printf("Keyboard-(D)	���ùر�ö����־����.\r\n");
	usleep(_usec);
	printf("Keyboard-(B)	���ô�ϵͳ��λ����.\r\n");
	usleep(_usec);
	printf("Keyboard-(b)	���ùر�ϵͳ��λ����.\r\n");
	usleep(_usec);
	printf("Keyboard-(Q)	�˳���������ģʽ.\r\n");
	usleep(_usec);
	printf("-----------------------------------------------------------------------------------------------\r\n");
	usleep(_usec);
}


//*To Prevent Mis-Operations for Switch Configuration,  add by GZXu,2017.04.10*//
int isValidLabel(unsigned char recvch)
{
	_precRecvData[curIndex] = recvch;
	if (_precRecvData[0] == 'e') {
				curIndex = 1;
				if (_precRecvData[1] == 'n') {
								curIndex = 2;
								if (_precRecvData[2] == 't') {
												curIndex = 3;
												if (_precRecvData[3] == 'e') {
															curIndex = 4;
																if (_precRecvData[4] == 'r') {
																		curIndex = 0;
																		_precRecvData[0] = '*';
																		_precRecvData[1] = '*';
																		_precRecvData[2] = '*';
																		_precRecvData[3] = '*';
																		_precRecvData[4] = '*';
																		return 1;
																} else if (_precRecvData[4] == '*') {
																		return 0;
																} else {
																		curIndex = 0;
																		_precRecvData[0] = '*';
																		_precRecvData[1] = '*';
																		_precRecvData[2] = '*';
																		_precRecvData[3] = '*';
																		_precRecvData[4] = '*';
																		return 0;
																}
												} else if (_precRecvData[3] == '*') {
													return 0;
												} else {
													curIndex = 0;
													_precRecvData[0] = '*';
													_precRecvData[1] = '*';
													_precRecvData[2] = '*';
													_precRecvData[3] = '*';
													return 0;
												}
								} else if (_precRecvData[2] == '*') {
									return 0;
								} else {
									curIndex = 0;
									_precRecvData[0] = '*';
									_precRecvData[1] = '*';
									_precRecvData[2] = '*';
									return 0;
								}
				} else if (_precRecvData[1] == '*') {
					return 0;
				} else {
					curIndex = 0;
					_precRecvData[0] = '*';
					_precRecvData[1] = '*';
					return 0;
				}
	} else if (_precRecvData[0] == '*') {
		return 0;
	} else {
		curIndex = 0;
		_precRecvData[0] = '*';
		return 0;
	}
}

void printArr()
{
	int i;
	for(i=0;i<5;i++)
	{
		printf("%c ",_precRecvData[i]);
	}
	printf("\n");
}

void SlectUartMenu(void)
{
	unsigned char _recvData;
	unsigned int _bRunning,_flag;
	unsigned int _ttl;

	_recvData = '0';
	_flag = 0;
	_bRunning = 1;
	sleep(1);
	ShowTab();

	while(_bRunning)
	{
		_recvData = '0';
		_recvData = getchar();

		if (_recvData == '1') {
			hlSrioTopoShow();
			sleep(1);
			printf("->");
		}else if (_recvData == '2') {
			hlSrioLUTShow();
			sleep(1);
			printf("->");
		}else if (_recvData == '3') {
			IIC_SrioPortStaticShow();
			sleep(1);
			printf("->");
		} else if (_recvData == 'd') {
			iic_SrioErrorDetect();
			sleep(1);
			printf("->");
		}else if(_recvData == '\n'||_recvData == '\r')
		{
			ShowTab();
			sleep(1);
			printf("->");
		}

		if (!_flag) {
			if (isValidLabel(_recvData)) {
				_flag = 1;
				printf("*******��������״̬*******\r\n");
				sleep(1);
				//ShowTab();
				continue;
			} else if (_recvData == 'Q') {
				printf("*****��ǰ״̬���ǲ鿴״̬*****\r\n");
				sleep(1);
			}
		}

		if(_flag)
		{
			if (_recvData != '\r'&& _recvData != '\n')
			{
				sleep(1);
			}
			switch (_recvData)
			{
				case 'Q':
				{
					_flag = 0;
					printf("*****�뿪����״̬������鿴״̬*****\r\n");
					sleep(1);
					break;
				}
				case '4':
				{
					UartSetSrioDelay(0,10);
					sleep(1);
					printf("->");
					break;
				}
				case '5':
				{
					hlSrioInit();
					sleep(1);
					printf("->");
					break;
				}
				case '6':
				{
					UartSetSrioDelay(1,5);
					sleep(1);
					printf("->");
					break;
				}
				case '7':
				{
					UartSetSrioDelay(1,25);
					sleep(1);
					printf("->");
					break;
				}
				case '8':
				{
					UartSetSrioDelay(1,50);
					sleep(1);
					printf("->");
					break;
				}
				case '9':
				{
					UartSetSrioDelay(1,70);
					sleep(1);
					printf("->");
					break;
				}
				case 'a':
				{
					UartSetSrioDelay(1,90);
					sleep(1);
					printf("->");
					break;
				}
				case '0':
				{
					hlSrioReInit();
					sleep(1);
					printf("->");
					break;
				}
				case 'c':
				{
					ReadLogInfo();
					printf("->");
					break;
				}
				case 'C':
				{
					OpenLogFile();
					usleep(1000);
					printf("�����´���־��¼���� .\r\n");
					printf("->");
					break;
				}
				case 'D':
				{
					CloseLogFile();
					usleep(1000);
					printf("�ѹر���־��¼���� .\r\n");
					printf("->");
					break;
				}
				case 'B':
				{
					OpenSysReset();
					usleep(1000);
					printf("�Ѵ�ϵͳ��λ���� .\r\n");
					printf("->");
					break;
				}
				case 'b':
				{
					CloseSysReset();
					usleep(1000);
					printf("�ѹر�ϵͳ��λ���� .\r\n");
					printf("->");
					break;
				}
				case 'r':
				{
					iic_SwPortRepair();
					iic_SwPortClear();
					sleep(1);
					printf("->");
					break;
				}
				case 't':
				{
					_ttl=0x4E200000;
					iic_TTL_csr(_ttl);
					sleep(1);
					printf("->");
					break;
				}
				case 'f':
				{
					_ttl=0;
					iic_TTL_csr(_ttl);
					sleep(1);
					printf("->");
					break;
				}
				case 's':
				{
					iic_SwSpeedBaudSet(0);
					sleep(1);
					printf("->");
					break;
				}
				case 'w':
				{
					iic_SwSpeedBaudSet(1);
					sleep(1);
					printf("->");
					break;
				}
				case 'l':
				{
					iic_SwSpeedBaudSet(2);
					sleep(1);
					printf("->");
					break;
				}
				case 'p':
				{
					iic_SwPLLLaneShow();
					sleep(1);
					printf("->");
					break;
				}
				default:
				{
					sleep(1);
				}
			}
		}

	}

}




