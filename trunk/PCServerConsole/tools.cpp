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