#include "PostgreSQLConnection.h"
#include "PostgreSQLPlugin_UE5.h"

#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"
THIRD_PARTY_INCLUDES_START
#include "Windows/AllowWindowsPlatformTypes.h"

extern "C"
{
#include "libpq-fe.h"
}

#include "Windows/HideWindowsPlatformTypes.h"
THIRD_PARTY_INCLUDES_END
#endif

bool UPostgreSQLConnection::IsConnected() const
{
#if PLATFORM_WINDOWS
	if (!Conn)
	{
		return false;
	}
	return PQstatus(Conn) == CONNECTION_OK;
#else
	return false;
#endif
}

void UPostgreSQLConnection::Close()
{
#if PLATFORM_WINDOWS
	if (Conn)
	{
		PQfinish(Conn);
		Conn = nullptr;
	}
#endif
}

void UPostgreSQLConnection::SetHandle(PGconn* InConn)
{
	Conn = InConn;
}

void UPostgreSQLConnection::BeginDestroy()
{
	Close();
	Super::BeginDestroy();
}