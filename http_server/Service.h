#ifndef __SERVICE_H
#define __SERVICE_H

#include <windows.h>
#include <string>

/**
* Win32 service class (abstract).
* @usage create service subclass, instanize subclass and call run() method.
* @remark service is singleton (you can have only one service instance).
* @author enikey.
*/
class Service
{
public:
	/**
	* Creates new Service class instance.
	* @remark pointer to global service (singleton) saves here.
	* @param name name of service.
	*/
	static Service * Instance();

	/**
	* Deletes service instance.
	* @remark releases singleton pointer.
	*/
	virtual ~Service();

	/**
	* Starts service execution.
	*/
	void run() ;

protected:
	/**
	* Creates new Service class instance.
	* @remark pointer to global service (singleton) saves here.
	* @param name name of service.
	*/
	Service(const TCHAR *name);

	/**
	* Called from service control manager when service needs to start.
	*/
	virtual void onStart() = 0;
	/**
	* Service main.
	*/
	virtual void main() = 0;
	/**
	* Called from service control manager when service needs to stop/
	*/
	virtual void onStop() = 0;

	/**
	* Win32 API service main function.
	* @see MSDN for details.
	*/
	static void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);
	void WINAPI Main(DWORD dwArgc, LPTSTR *lpszArgv);
	/**
	* Win32 API service control handler.
	* @see MSDN for details.
	*/
	static void WINAPI ServiceControlHandler(DWORD dwCtrlCode);
	void WINAPI ControlHandler(DWORD dwCtrlCode);


protected:
	/**
	* @fixme add comment to it.
	*/
	bool reportStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode,
		DWORD dwWaitHint);

protected:
	/**
	* Service name for SCM.
	*/
	//StringStorage m_name;
	TCHAR m_name[256];

	/**
	* Current service status.
	*/
	SERVICE_STATUS m_status;
	/**
	* Service handle.
	*/
	SERVICE_STATUS_HANDLE m_statusHandle;

	/**
	* Flag determinates if service is terminating.
	*/
	volatile bool m_isTerminating;

	/**
	* Service (global instance).
	*/
	static Service *g_service;
};

#endif //__SERVICE_H
