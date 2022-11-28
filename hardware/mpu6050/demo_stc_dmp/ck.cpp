/* Windows API ���ں�����ṹ��

	CreateFile()	//�򿪴���(�򿪳ɹ����ش�����Դ�����ʧ�ܷ���-1)
	SetupComm()		//���ô��ڶ�д�Ļ�������С�ģ���
	PurgeComm()		//��մ��ڻ�����
	GetCommState()	//��ȡ���ڵĳ�ʼ����
	SetCommState()		//���ƴ��ڸ�����
	GetCommTimeouts()	//��ȡ���ڳ�ʱ��ǰ������
	SetCommTimeouts()	//���ƴ��ڳ�ʱ����(д��COMMTIMEOUTS�ṹ�������)
	ReadFile()		//����������
	COMMTIMEOUTS	//����ṹ�������ù��ڴ��ڳ�ʱ�����(д������Ч)
	DCB						//����ṹ�������ù��ڴ��ڸ����������(д������Ч)
	
	
	
*/
/*���������ʾ
	Quaternion = ��Ԫ��
	Yaw=ƫ��
	Pitch=����
	Roll=����
	
*/

#include <stdio.h>
#include <windows.h>
#include <math.h>


int dmpGetQuaternion1(int *data, unsigned char *packet);	//ȡ��DMP��Ԫ��ԭʼ����
int dmpGetQuaternion2(float *q,unsigned char* packet); //�������Ԫ��
void dmpGetyawpitchroll(float *ypr,float *q);	//����ŷ���� Roll,Pitch,Yaw

int main(int argc, char *agrv[])	//�ַ����һλΪ\0��
{
	int btl;
	char comk[6]="com1";

	if(argc > 1 )
	{
		btl = 0;
		while((agrv[1][btl]!='\0') && (btl<5))
		{
			comk[btl] = agrv[1][btl];
			btl++;
		}
		if(argc > 2)
		{
			sscanf(agrv[2],"%d",&btl);
		}
		else
		{
			btl = 9600;
		}
	}
	else
	{
		btl = 9600;
	}
	
	HANDLE hCom;	//���洮�ھ��
	hCom = CreateFile(comk,	//�򿪵Ĵ�����
										GENERIC_READ|GENERIC_WRITE, //�������д(������дҲ����)
										0, //��ռ��ʽ(�����)
										NULL,	//���ð�ȫ�����Խṹ��ȱʡֵΪNULL
										OPEN_EXISTING, //�򿪶����Ǵ���
										0, //ͬ����ʽ
										NULL);	//�Դ��ڶ��Ըò���������ΪNULL

	if (hCom==(HANDLE)-1)
	{
		printf("�򿪴���ʧ��");
		return -1;
	}
	SetupComm(hCom,1024,1024); //���뻺����������������Ĵ�С����1024
	COMMTIMEOUTS TimeOuts;  //�����ֻ���趨�������д���趨��ô����趨�Ͳ�����Ч
	//�趨����ʱ  
	TimeOuts.ReadIntervalTimeout=1000;  
	TimeOuts.ReadTotalTimeoutMultiplier=500;  
	TimeOuts.ReadTotalTimeoutConstant=5000;  
	//�趨д��ʱ  
	TimeOuts.WriteTotalTimeoutMultiplier=500;  
	TimeOuts.WriteTotalTimeoutConstant=2000;  
	
	SetCommTimeouts(hCom,&TimeOuts); //���ó�ʱ  
	
	DCB dcb;
	GetCommState(hCom,&dcb);	//��ȡ���ڵĳ�ʼ����
	dcb.BaudRate=btl; //������Ϊ9600  
	dcb.ByteSize=8; //ÿ���ֽ���8λ  
	dcb.Parity=NOPARITY; //����żУ��λ  
	dcb.StopBits=1; //����ֹͣλ  
	SetCommState(hCom,&dcb);	//���ƴ��ڸ�����
	
	PurgeComm(hCom,PURGE_TXCLEAR|PURGE_RXCLEAR); //��մ��ڻ�����(����д��һ�����)
	
	
	//������
	unsigned char data[8];//
	DWORD wCount;//��ȡ���ֽ��� (�ɹ����ַ���)
	BOOL bReadStat;  //
	bReadStat=ReadFile(hCom,data,8,&wCount,NULL);	//��ȡ�ַ�
	float q[4],ypr[3];


	while(1)	//��ѭ���������ŷ����
	{
	bReadStat=ReadFile(hCom,data,8,&wCount,NULL);	//��ȡ�ַ�
	dmpGetQuaternion2(q,data);	//ȡ����Ԫ�� W X Y Z
	dmpGetyawpitchroll(ypr,q); //����Ƕ�

	//��� �Ƕ�
	printf("roll=%f ",ypr[0]);
	printf("pitch=%f ",ypr[1]);
	printf("yaw=%f \n",ypr[2]);
	}
	return 0;
}
void dmpGetyawpitchroll(float *ypr,float *q)	//����Ƕ�
{
  // Roll = Atan2(2 *(Y * Z + W * X) , W * W -X * X -Y * Y + Z * Z) 
 ypr[0] = atan2(2 *(q[2] * q[3] + q[0] * q[1]) , q[0] * q[0] -q[1] * q[1] -q[2] * q[2] + q[3] * q[3])*57.3; 
  // Pitch = asin(-2 * (X * Z - W * Y))  
 ypr[1]=asin(-2*(q[1]*q[3]-q[0]*q[2]))*57.3;
  // Yaw   = atan2(2 * (X * Y + W * Z) ,W * W + X * X - Y * Y - Z * Z) 
 ypr[2]=atan2(2*(q[1] * q[2] + q[0] * q[3]) , q[0] * q[0] +q[1] * q[1] -q[2] * q[2] - q[3] * q[3])*57.3; 
}
int dmpGetQuaternion1(int *data, unsigned char *packet)	//���DMP��Ԫ��ԭʼ����
{
	int i,k;
    data[0] = ((packet[0] << 8) + packet[1]);
    data[1] = ((packet[2] << 8) + packet[3]);
    data[2] = ((packet[4] << 8) + packet[5]);
    data[3] = ((packet[6] << 8) + packet[7]);
    for(i=0;i<4;i++)// ��16λ�����Ĳ��룬ת����32λ�ĸ�������
    {
		if((data[i]&0x8000) !=0)
		{
			if(data[i]!=0x8000)
			{
				k=data[i];
				data[i]=0xffff;
				data[i]=~data[i];
				data[i]=data[i]|k;
			}
			else
				data[i]=-32768;
			}
			
		}
    return 1;
}
int dmpGetQuaternion2(float *q,unsigned char* packet)	//ȡ����Ԫ��
{	
    int qI[4];
    unsigned char status = dmpGetQuaternion1(qI, packet);
    if (status) {
        q[0] = (float)qI[0] / 16384.0f;
        q[1] = (float)qI[1] / 16384.0f;
        q[2] = (float)qI[2] / 16384.0f;
        q[3] = (float)qI[3] / 16384.0f;
        return 1;
    }
    return status; 
}

