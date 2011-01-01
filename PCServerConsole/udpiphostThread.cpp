
#include "stdafx.h"



void udp_sendiphostname(int udp_port){


     WORD wVersionRequested;
	 WSADATA wsaData;
	 int err;

	 wVersionRequested = MAKEWORD(2,2);

	 err = WSAStartup(wVersionRequested,&wsaData);

	 if(err!=0){
         printf("��Ϣ:<��ȡIP> �޷����׽��ִ���\n");
        return ;
	 }

	 if(LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wVersion)!=2) {

         printf("��Ϣ:<��ȡIP> DLL �汾����ȷ��Ӧ�� 2.2 �汾\n");
	     WSACleanup();
		 return ;
	 }

	 SOCKET sockServer = socket(AF_INET,SOCK_DGRAM,0);

	 SOCKADDR_IN addrServer;

	 addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	 addrServer.sin_family=AF_INET;
	 addrServer.sin_port=htons(udp_port);


	 int _ret = bind(sockServer,(SOCKADDR *)&addrServer,sizeof(SOCKADDR));

	 if(_ret!=0){
		 printf("��Ϣ:<��ȡIP> �󶨶˿�%dʧ��\n",udp_port);
		 WSACleanup();
		 return ;
	 }

	 printf("��Ϣ:<��ȡIP> �����˿�%d\n",udp_port);


	 SOCKADDR_IN addrClient;

	 int len = sizeof(SOCKADDR);

	 char recvBuf[1024]; // ����ָ��

     char hostName[512];  // ������
     char hostAddress[512];  // ����IP
     char msg[1024];          // ��Ϣ=����IP-����IP-����IP-������



	 ::printf("��Ϣ:<��ȡIP> ��ʼ���ɹ�\n");

	 while(true){

         ::memset(recvBuf,0,sizeof(recvBuf));
	     ::memset(hostName,0,sizeof(hostName));
	     ::memset(hostAddress,0,sizeof(hostAddress));
	     ::memset(msg,0,sizeof(msg));

		 ::printf("��Ϣ:<��ȡIP> ����ȴ��㲥��Ϣ\n");
          int result = recvfrom(sockServer,recvBuf,sizeof(recvBuf),0,(SOCKADDR *)&addrClient,&len);
		  ::printf("��Ϣ:<��ȡIP> ���յ��㲥��Ϣ %d\n",result);
		  if(result==SOCKET_ERROR){
              int errCode=WSAGetLastError();
			  printf("��Ϣ:<��ȡIP> ʧ�� �����:%d\n",errCode);
			//  return 1;
			//  break;
		  }else {
			  // cmd:android->pc:getiphostname
			  ::printf("��Ϣ:<��ȡIP> ���յ��㲥��Ϣ %s\n",recvBuf);
			  if(strcmp(recvBuf,"cmd:android->pc:getiphostname")==0){
			     
			     HOSTENT *hostEntry;

	             gethostname(hostName , sizeof(hostName));
                 printf("��Ϣ:<��ȡIP> get hostname: %s\n",hostName);
                 hostEntry = gethostbyname( hostName );
                 //��ȡIP��ַ
                 if( hostEntry != 0 ){
                     // ������ڶ�IP �µ� BUG
					 for(int _i=0;hostEntry->h_addr_list[_i]!=0;_i++){
						 char ip[32];
						 // ���� BUG ���ָ�����"-" ��Ϊ "#" ,ԭ�� ���������԰���"-" �ַ��������������"#"�ַ�
						 sprintf_s(ip,"%d.%d.%d.%d#",hostEntry->h_addr_list[_i][0]&0x00ff,
							 hostEntry->h_addr_list[_i][1]&0x00ff,hostEntry->h_addr_list[_i][2]&0x00ff,hostEntry->h_addr_list[_i][3]&0x00ff);
						 strcat_s(hostAddress,ip);
					 }
                 }
	             printf("��Ϣ:<��ȡIP> get ip: %s\n",hostAddress);
                 sprintf_s(msg,"%s%s",hostAddress,hostName);
	             printf("��Ϣ:<��ȡIP> %s\n",msg);
				 ::printf("��Ϣ:<��ȡIP> ����IP HOSTNAME %s\n",msg);
                 int iret = sendto(sockServer,msg,sizeof(msg),0,(sockaddr*)&addrClient,sizeof(addrClient));
				 ::printf("��Ϣ:<��ȡIP> ����IP HOSTNAME %d\n",iret);
                 if (iret == SOCKET_ERROR) {
       
	                   printf("��Ϣ:<��ȡIP> ����IPʧ�� �����:%d\t\n ",WSAGetLastError()); 

                         // ::MessageBox(NULL,_T("Error"),_T("Error"),0);
				 }else {
				       printf("��Ϣ:<��ȡIP> ����IP�ɹ�\n");
					  // break;
				 }
				 // return 0;
				 // break;
			   }
		  }
	 }

	 closesocket(sockServer);
	 WSACleanup();
	 ::printf("��Ϣ:<��ȡIP> �˳��ɹ�\n");
	 return ;

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