// PCServerConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"












int _tmain(int argc, _TCHAR* argv[])
{


	int udp_port = 20000;

	int tcp_port = 30000;

   // int iret;

    // ��ӡ��Ϣ

	printf("***************************************************************************\n");
	printf("****                �ֻ����Ƶ��� V1.141  ����: liaowufeng              ****\n");
	printf("***************************************************************************\n");
	printf("*  ע��:����ʱ�벻Ҫ�رմ˴��ڡ�                                          *\n");
	printf("*  --------------------------------------------------------------------   *\n");
    printf("*  Version 1.141                                                          *\n");
	printf("*  1 ���� BUG ���ָ�����\"-\" ��Ϊ \"#\" ,                                    *\n");
	printf("	 ԭ�� ���������԰���\"-\" �ַ��������������\"#\"�ַ�                     *\n");
	printf("*  2 �޸�android �ֻ���ʾʱ���һЩ                                       *\n");
	printf("*  3 ����android �ֻ�����ʱ��BUG                                          *\n");
	printf("*  ---------------------------------------------------------------------  *\n");	
    printf("*  Version 1.14                                                           *\n");
	printf("*  1 ���� ���ڶ���������IP �µ�BUG                                        *\n");
	printf("*  2 ֧�ֿ��ƶ�̨����                                                     *\n");
	printf("*  ---------------------------------------------------------------------  *\n");



HANDLE udpThreadHandle,commandThreadHandle;
DWORD udpThreadID=0,commandThreadID=0;

   // �����㲥IP �߳�

udpThreadHandle = ::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)udp_sendiphostname,(LPVOID)udp_port,0,&udpThreadID);

if(udpThreadHandle!=NULL){
	printf("<������> �ɹ����� ��ȡIP �߳� %d\n",udpThreadID);
	// ��������������� �߳�
    commandThreadHandle = ::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)tcp_listenoper,(LPVOID)tcp_port,0,&commandThreadID);
	if(commandThreadHandle!=NULL){
	   
	   printf("<������> �ɹ����� �������� �߳� %d\n",commandThreadID);
	}else {
       ::MessageBox(NULL,_T("����ִ�� �������������߳� ʧ�ܣ�������ִ��"),_T("����"),0);
	   exit(1);
	   return 1;
	}
}else {

    ::MessageBox(NULL,_T("����ִ�� ������ȡIP�߳� ʧ�ܣ�������ִ��"),_T("����"),0);
	exit(1);
	return 1;

}

while(true);

return 0;



/*
	if(udp_sendiphostname(udp_port)==0){

	      iret = tcp_listenoper(tcp_port);

		  if(iret!=0){
              ::MessageBox(NULL,_T("����ִ��ʧ�ܣ�������ִ��"),_T("����"),0);
			  exit(1);
		      return 1;
		  }
	
	}else {
        ::MessageBox(NULL,_T("����ִ��ʧ�ܣ�������ִ��"),_T("����"),0);
		exit(1);
		return 1;

	}


	return 0;

	*/
}









