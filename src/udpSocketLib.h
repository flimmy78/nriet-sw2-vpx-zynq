#ifndef _udpSocketLib_h_20100810
#define _udpSocketLib_h_20100810
/**********************************************************************************************************
*updsocketLib ����UDPЭ�����ͨѶ�ĺ�����װ,���ò�������vxWorks6.8,
*
*����˵��:
*int udpCreateSocket(char * strAddr, char * strGroup, int nPort);
* ����:��ʼ��һ��UDPͨѶ���
* ����:
*  1: char * strAddr  ��Ҫ������ڵ�ip��ַ,����������ڼ����鲥ʱ��Ч,��ʾѡ����һ�����������鲥,������ü����鲥,��ΪNULL����
*  2: char * strGroup ��Ҫ������鲥��ַ,����������鲥,��ΪNULL����
*  3: int nPort       ��Ҫ������ڵĶ˿ڵ�ַ
*  ����:
*  ID �� ���ڻ����0 ��ʾ�ɹ�,-1��ʾʧ��,���ID�����Ժ�������ʹ�õ�.
*  ˵��: ��ഴ��64�����
*
*int udpBindPeer(int nID,char * strPeerAddr, int nPeerPort);
* ����: �󶨷��͵�ַ,�����Ժ�ʹ��udpSendPeer�������з���,������Ҫÿ��ָ��
* ����: 
*  1: int nID  ���������
*  2: char * strPeerAddr  �Է�ip��ַ
*  3: int nPeerPort �Է��˿�
* ����:
*  0 ��ʾ�ɹ�,-1��ʾʧ��
*
*
*����:
*

#include "stdio.h"
#include "udpSocketLib.h"

static int nSocketID0 = -1;

static void OnSocket(void * pParent,int nID,char * buf,int len)
{
	char * peeraddress;
	int peerport;
	peeraddress = udpGetRecvAddress(nID);
	peerport = udpGetRecvPort(nID);
	if(len < 0) return;
	printf("Recv from addr :");
	printf(peeraddress);
	printf("\n");
	printf("Recv from port : %d\n",peerport);
	udpSendPeerEx(nID,peeraddress,peerport,(void *)buf,len);
}
void NetInit()
{
	nSocketID0 = udpCreateSocket("168.168.168.9","230.168.168.168",5190);
	if(nSocketID0 < 0)
	{
		printf("udp socket create fail\n");
		return;
	}
	udpAttachRecv(nSocketID0,(FUNCPTR)OnSocket);
	udpBindPeer(nSocketID0,"230.168.168.168",6003);
}
void SendEx(char * peeraddr, int peerport)
{
	int i;
	char buf[256];
	for(i = 0;i<256;i++)
	{
		buf[i] = (i+0x30)&0xff;
	}
	udpSendPeerEx(nSocketID0,peeraddr,peerport,buf,256);
}
void Send()
{
	int i;
	char buf[256];
	for(i = 0;i<256;i++)
	{
		buf[i] = (i+0x30)&0xff;
	}
	udpSendPeer(nSocketID0,buf,256);
}

*
*
************************************************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif


typedef int 		(*FUNCPTR) ();
#define OK		0
#define ERROR		(-1)

int udpCreateSocket(char * strAddr, char * strGroup, int nPort);/*return socket id,Fail -1*/
int udpBindPeer(int nID,char * strPeerAddr, int nPeerPort);/*OK return 0, Fail -1*/
int udpSendPeer(int nID,void * pSendBuf,int nLen);/*OK return 0, Fail -1*/
int udpSendPeerEx(int nID,char * strPeerAddr, int nPeerPort,void * pSendBuf,int nLen);/*OK return 0, Fail -1*/
int udpAttachRecv(int nID,FUNCPTR callbackRecv);	/*OK return 0, Fail -1*/
int udpEnableBroadCast(int nID);
int udpDisableBroadCast(int nID);
int udpDestroySocket(int nID);
int udpSetParentData(int nID,void * pParentData);
void * udpGetParentData(int nID);
char * udpGetRecvAddress(int nID);
int udpGetRecvPort(int nID);
int udpGetSocketFD(int nID);
int udpJoinGroup(int nID,char * strAddr,char * strGroup);

#ifdef __cplusplus
}
#endif

#endif/*_udpSocketLib_h_20100810*/

