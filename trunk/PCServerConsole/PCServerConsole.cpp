// PCServerConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


// �ػ�
void ShutDown()
{
HANDLE hToken;
TOKEN_PRIVILEGES tkp;
//������ǰ��������
if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))
   return;
//��ȡ�������ձ�ʾ�������ض���ϵͳ������Ȩ��
LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);
tkp.PrivilegeCount=1;
tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
//������������Ȩ��
AdjustTokenPrivileges(hToken,FALSE,&tkp,0,(PTOKEN_PRIVILEGES)NULL,0);
//�ػ���Ҫʵ������ֻ�轫��һ����������EWX_REBOOT|EWX_FORCE
//ע����һ����������EWX_LOGOFF|EWX_FORCE
ExitWindowsEx(EWX_SHUTDOWN|EWX_FORCE,0);

}

//����
void Reboot()
{
HANDLE hToken;
TOKEN_PRIVILEGES tkp;
if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))
   return;
LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);
tkp.PrivilegeCount=1;
tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
AdjustTokenPrivileges(hToken,FALSE,&tkp,0,(PTOKEN_PRIVILEGES)NULL,0);
    ExitWindowsEx(EWX_REBOOT|EWX_FORCE,0);

}

//ע��
void LogOff()
{
HANDLE hToken;
TOKEN_PRIVILEGES tkp;
if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))
   return;
LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);
tkp.PrivilegeCount=1;
tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
AdjustTokenPrivileges(hToken,FALSE,&tkp,0,(PTOKEN_PRIVILEGES)NULL,0);
    ExitWindowsEx(EWX_LOGOFF|EWX_FORCE,0);

}



// UDP ���㲥 IP �� ������

//int udp_sendiphostname(int udp_port);
  int udp_sendiphostname(int udp_port);



// TCP �������ܲ���ָ��
int tcp_listenoper(int tcp_port);



int udp_sendiphostname(int udp_port){


     WORD wVersionRequested;
	 WSADATA wsaData;
	 int err;

	 wVersionRequested = MAKEWORD(2,2);

	 err = WSAStartup(wVersionRequested,&wsaData);

	 if(err!=0){
         printf("��Ϣ:��ȡIP �޷����׽��ִ���\n");
        return 1;
	 }

	 if(LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wVersion)!=2) {

         printf("��Ϣ:��ȡIP DLL �汾����ȷ��Ӧ�� 2.2 �汾\n");
	       WSACleanup();
		   return 1;
	 }

	 SOCKET sockServer = socket(AF_INET,SOCK_DGRAM,0);

	 SOCKADDR_IN addrServer;

	 addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	 addrServer.sin_family=AF_INET;
	 addrServer.sin_port=htons(udp_port);


	 int _ret = bind(sockServer,(SOCKADDR *)&addrServer,sizeof(SOCKADDR));

	 if(_ret!=0){
		 printf("��Ϣ:��ȡIP �󶨶˿�%dʧ��\n",udp_port);
		 WSACleanup();
		 return 1;
	 }

	 printf("��Ϣ:��ȡIP �����˿�%d\n",udp_port);


	 SOCKADDR_IN addrClient;

	 int len = sizeof(SOCKADDR);

	 char recvBuf[1024]; // ����ָ��

     char hostName[512];  // ������
     char hostAddress[512];  // ����IP
     char msg[1024];          // ��Ϣ=����IP-������

	 ::memset(recvBuf,0,sizeof(recvBuf));
	 ::memset(hostName,0,sizeof(hostName));
	 ::memset(hostAddress,0,sizeof(hostAddress));
	 ::memset(msg,0,sizeof(msg));

	 ::printf("��Ϣ:��ȡIP ��ʼ���ɹ�\n");

	 while(true){
		 ::printf("��Ϣ:��ȡIP ����ȴ��㲥��Ϣ\n");
          int result = recvfrom(sockServer,recvBuf,sizeof(recvBuf),0,(SOCKADDR *)&addrClient,&len);
		  ::printf("��Ϣ:��ȡIP ���յ��㲥��Ϣ %d\n",result);
		  if(result==SOCKET_ERROR){
              int errCode=WSAGetLastError();
			  printf("��Ϣ:��ȡIP ʧ�� �����:%d\n",errCode);
			//  return 1;
			//  break;
		  }else {
			  // cmd:android->pc:getiphostname
			  ::printf("��Ϣ:��ȡIP ���յ��㲥��Ϣ %s\n",recvBuf);
			  if(strcmp(recvBuf,"cmd:android->pc:getiphostname")==0){
			     
			     HOSTENT *hostEntry;

	             gethostname(hostName , sizeof(hostName));
                 printf("��Ϣ:get hostname: %s\n",hostName);
                 hostEntry = gethostbyname( hostName );
                 //��ȡIP��ַ
                 if( hostEntry != 0 ){
                      sprintf_s(hostAddress , "%d.%d.%d.%d" ,(hostEntry->h_addr_list[0][0]&0x00ff),(hostEntry->h_addr_list[0][1]&0x00ff),
                         (hostEntry->h_addr_list[0][2]&0x00ff) ,(hostEntry->h_addr_list[0][3]&0x00ff));
                 }
	             printf("��Ϣ:get ip: %s\n",hostAddress);
                 sprintf_s(msg,"%s-%s",hostAddress,hostName);
	             printf("��Ϣ:%s\n",msg);
				 ::printf("��Ϣ:��ȡIP ����IP HOSTNAME %s\n",msg);
                 int iret = sendto(sockServer,msg,sizeof(msg),0,(sockaddr*)&addrClient,sizeof(addrClient));
				 ::printf("��Ϣ:��ȡIP ����IP HOSTNAME %d\n",iret);
                 if (iret == SOCKET_ERROR) {
       
	                   printf("��Ϣ:����IPʧ�� �����:%d\t\n ",WSAGetLastError()); 

                         // ::MessageBox(NULL,_T("Error"),_T("Error"),0);
				 }else {
				       printf("��Ϣ:����IP�ɹ�\n");
					   break;
				 }
				 // return 0;
				 // break;
			   }
		  }
	 }

	 closesocket(sockServer);
	 WSACleanup();

	 ::printf("��Ϣ:��ȡIP �˳��ɹ�\n");
	 return 0;

}






int _tmain(int argc, _TCHAR* argv[])
{


	int udp_port = 20000;

	int tcp_port = 30000;

    int iret;

    // ��ӡ��Ϣ

	printf("***************************************************************************\n");
	printf("****                �ֻ����Ƶ��� V1.12  ����: liaowufeng               ****\n");
	printf("***************************************************************************\n");
	printf("*  ע��:����ʱ�벻Ҫ�رմ˴��ڡ�                                          *\n");






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
}





int tcp_listenoper(int tcp_port) {

 WORD wVersionRequested;
	WSADATA wsaData;

	int err;

    wVersionRequested = MAKEWORD(2,2);

    err = WSAStartup(wVersionRequested,&wsaData);

	if(err!=0){
        printf("��Ϣ:�������� �޷����׽��ִ���\n");

		//::MessageBox(NULL,_T("�޷����׽���"),_T("����"),0);
		return 1;

	}

	if(LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wVersion)!=2) {
       printf("��Ϣ:�������� DLL �汾����ȷ��Ӧ�� 2.2 �汾\n");

	   // ::MessageBox(NULL,_T("DLL �汾����ȷ��Ӧ�� 2.2 �汾"),_T("����"),0);
       WSACleanup();
	   return 1;
	}

	SOCKET sockSrv = socket(AF_INET,SOCK_STREAM,0);

	SOCKADDR_IN addrSrv;


	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);

	addrSrv.sin_family=AF_INET;
    
	// �˿ڼ��� 
	addrSrv.sin_port=htons(tcp_port);

	int _ret = bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

    if(_ret!=0){
		 printf("��Ϣ:�������� �󶨶˿�%dʧ��\n",tcp_port);
		 WSACleanup();
		 return 1;
	 }

	 printf("��Ϣ:�������� �����˿�%d\n",tcp_port);

	listen(sockSrv,5);

	SOCKADDR_IN addrClient;

	int len = sizeof(SOCKADDR);

	

	while(true) {
        SOCKET sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&len);


		char recvBuf[1024];

		::memset(recvBuf,0,sizeof(recvBuf));
		
		int len = recv(sockConn,recvBuf,sizeof(recvBuf),0);

        
		printf("��Ϣ:��������:%s\n",recvBuf);

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
			printf("��Ϣ:��������ʧ��");
          // ::MessageBox(NULL,_T("����ָ��ʧ��"),_T("����"),0);
		}

		//::MessageBox(NULL,*recvBuf,_T("��������"),0);
        closesocket(sockConn);
       
	}

	WSACleanup();

     return 0;
}



/*
int udp_sendiphostname(int udp_port){


  WSADATA wsaData;
   WORD wVersion = 0x0202;

   sockaddr_in server_addr,client_addr;

   if(WSAStartup(wVersion,&wsaData)!=0){
	   ::MessageBox(NULL,_T("Error"),_T("Error"),0);
       return 1;
   
   }

   SOCKET server = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

   
   server_addr.sin_addr.S_un.S_addr=INADDR_ANY;

   server_addr.sin_family=AF_INET;


   client_addr.sin_addr.S_un.S_addr=INADDR_BROADCAST;
   client_addr.sin_family=AF_INET;

   client_addr.sin_port=htons(udp_port);

   BOOL bBroadcast;

   setsockopt(server,SOL_SOCKET,SO_BROADCAST,(char *)&bBroadcast,sizeof(bBroadcast));

   char hostName[256];
   char hostAddress[256];
   char msg[512];

   	::memset(hostName,0,sizeof(hostName));

	::memset(hostAddress,0,sizeof(hostAddress));

	::memset(msg,0,sizeof(msg));


   HOSTENT *hostEntry;

   gethostname(hostName , sizeof(hostName));
   printf("get hostname: %s\n",hostName);

    hostEntry = gethostbyname( hostName );
    //��ȡIP��ַ
    if( hostEntry != 0 )
    {
	  
       sprintf_s(hostAddress , "%d.%d.%d.%d" ,(hostEntry->h_addr_list[0][0]&0x00ff),(hostEntry->h_addr_list[0][1]&0x00ff),
                   (hostEntry->h_addr_list[0][2]&0x00ff) ,(hostEntry->h_addr_list[0][3]&0x00ff));

      
    }

	printf("get ip: %s\n",hostAddress);

    sprintf_s(msg,"%s-%s",hostAddress,hostName);

	printf("%s\n",msg);



  
   int iret = sendto(server,msg,sizeof(msg),0,(sockaddr*)&client_addr,sizeof(client_addr));

   
   if (iret == SOCKET_ERROR) {
       
	   printf( "Call   sendto   failed.   Error   number:   %d\t\n ",   WSAGetLastError()); 


      // ::MessageBox(NULL,_T("Error"),_T("Error"),0);
   }


   closesocket(server);

   WSACleanup();


	return 0;



}
*/