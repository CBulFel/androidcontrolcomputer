
#include "stdafx.h"



void tcp_listenoper(int tcp_port) {

 WORD wVersionRequested;
	WSADATA wsaData;

	int err;

    wVersionRequested = MAKEWORD(2,2);

    err = WSAStartup(wVersionRequested,&wsaData);

	if(err!=0){
        printf("��Ϣ:<��������> �޷����׽��ִ���\n");

		//::MessageBox(NULL,_T("�޷����׽���"),_T("����"),0);
		return ;

	}

	if(LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wVersion)!=2) {
       printf("��Ϣ:<��������> DLL �汾����ȷ��Ӧ�� 2.2 �汾\n");

	   // ::MessageBox(NULL,_T("DLL �汾����ȷ��Ӧ�� 2.2 �汾"),_T("����"),0);
       WSACleanup();
	   return ;
	}

	SOCKET sockSrv = socket(AF_INET,SOCK_STREAM,0);

	SOCKADDR_IN addrSrv;


	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);

	addrSrv.sin_family=AF_INET;
    
	// �˿ڼ��� 
	addrSrv.sin_port=htons(tcp_port);

	int _ret = bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));



    if(_ret!=0){
		 printf("��Ϣ:<��������> �󶨶˿�%dʧ��\n",tcp_port);
		 WSACleanup();
		 return ;
	 }


	printf("��Ϣ:<��������> �����˿�%d\n",tcp_port);

	listen(sockSrv,5);

	SOCKADDR_IN addrClient;

	int len = sizeof(SOCKADDR);

   ::printf("��Ϣ:<��������> ��ʼ���ɹ�\n");

	while(true) {

		 ::printf("��Ϣ:<��������> ����ȴ�������Ϣ\n");

        SOCKET sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&len);


		char recvBuf[1024];

		::memset(recvBuf,0,sizeof(recvBuf));
		
		int len = recv(sockConn,recvBuf,sizeof(recvBuf),0);

        
		printf("��Ϣ:<��������> :%s\n",recvBuf);

		if(len>0){
			if(strcmp(recvBuf,"cmd:android->pc:shutdown")==0){
				//::MessageBox(NULL,_T("cmd:android->pc:close"),_T("��ʾ"),0);
			    ShutDown();
			}else if(strcmp(recvBuf,"cmd:android->pc:reboot")==0){
			    //
				//::MessageBox(NULL,_T("cmd:android->pc:reboot"),_T("��ʾ"),0);
				::Reboot();
			}else if(strcmp(recvBuf,"cmd:android->pc:logoff")==0){
			    //
				//::MessageBox(NULL,_T("cmd:android->pc:logoff"),_T("��ʾ"),0);
				::LogOff();

			}
		}else {
			printf("��Ϣ:<��������> ʧ��");
          // ::MessageBox(NULL,_T("����ָ��ʧ��"),_T("����"),0);
		}

		//::MessageBox(NULL,*recvBuf,_T("��������"),0);
        closesocket(sockConn);
       
	}

	WSACleanup();

     return ;
}