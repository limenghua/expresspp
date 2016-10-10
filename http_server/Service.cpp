#include "stdafx.h"
#include "Service.h"
#include "string.h"

Service::Service(const TCHAR *name)
{
	_tcscpy(m_name, name);
}

Service::~Service()
{
}

void Service::run()
{
	SERVICE_TABLE_ENTRY ServiceTable[2];
	ServiceTable[0].lpServiceName = (LPWSTR)m_name;
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTIONW)&Service::ServiceMain;

	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;

	StartServiceCtrlDispatcher(ServiceTable);
}

void Service::ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
	Instance()->Main(dwArgc, lpszArgv);
}
void Service::Main(DWORD dwArgc, LPTSTR *lpszArgv)
{
	onStart();

	m_status.dwServiceType = SERVICE_WIN32;
	m_status.dwCurrentState = SERVICE_START_PENDING;
	m_status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	m_status.dwWin32ExitCode = 0;
	m_status.dwServiceSpecificExitCode = 0;
	m_status.dwCheckPoint = 0;
	m_status.dwWaitHint = 0;

	m_statusHandle = RegisterServiceCtrlHandler(
		(LPWSTR)m_name,
		(LPHANDLER_FUNCTION)&Service::ServiceControlHandler);

	if (m_statusHandle == (SERVICE_STATUS_HANDLE)0)
	{
		// registering control handler failed
		return;
	}

	// We report the running status to SCM. 
	m_status.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(m_statusHandle, &m_status);

	main();

	onStop();

}

void Service::ServiceControlHandler(DWORD dwCtrlCode)
{
	Instance()->ControlHandler(dwCtrlCode);
}

void Service::ControlHandler(DWORD dwCtrlCode)
{
	switch (dwCtrlCode)
	{
	case SERVICE_CONTROL_STOP:
		onStop();
		m_status.dwWin32ExitCode = 0;
		m_status.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(m_statusHandle, &m_status);
		return;

	case SERVICE_CONTROL_SHUTDOWN:
		onStop();
		m_status.dwWin32ExitCode = 0;
		m_status.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(m_statusHandle, &m_status);
		return;

	default:
		break;
	}
}