#include "PostgreSQLBlueprintLibrary.h"
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

// CONNECT --------------------------------------------------------------
bool UPostgreSQLBlueprintLibrary::PG_Connect(
	const FString& Host,
	int32 Port,
	const FString& Database,
	const FString& User,
	const FString& Password,
	bool UseSSL,
	const FString& SSLMode,
	bool UseSSLRootCert,
	const FString& SSLRootCertPath,
	UPostgreSQLConnection*& OutConnection,
	FString& OutError
)
{
	OutError.Empty();
	OutConnection = nullptr;

#if !PLATFORM_WINDOWS
	OutError = TEXT("PostgreSQL plugin currently supports Windows only.");
	return false;
#else

	// Base connection string
	FString ConnStr = FString::Printf(
		TEXT("host=%s port=%d dbname=%s user=%s password=%s"),
		*Host,
		Port,
		*Database,
		*User,
		*Password
	);

	// Optional SSL Mode
	if (UseSSL)
	{
		if (!SSLMode.IsEmpty())
		{
			ConnStr += FString::Printf(TEXT(" sslmode=%s"), *SSLMode);
		}
		else
		{
			// Default fallback for safety
			ConnStr += TEXT(" sslmode=require");
		}
	}
	else
	{
		ConnStr += TEXT(" sslmode=disable");
	}

	// Optional root certificate
	if (UseSSLRootCert && !SSLRootCertPath.IsEmpty())
	{
		ConnStr += FString::Printf(TEXT(" sslrootcert=%s"), *SSLRootCertPath);
	}

	// Convert for libpq
	FTCHARToUTF8 Utf8Conn(*ConnStr);
	PGconn* Raw = PQconnectdb(Utf8Conn.Get());

	if (!Raw)
	{
		OutError = TEXT("PQconnectdb returned NULL connection handle.");
		return false;
	}

	if (PQstatus(Raw) != CONNECTION_OK)
	{
		const char* Err = PQerrorMessage(Raw);
		OutError = UTF8_TO_TCHAR(Err ? Err : "Unknown PostgreSQL connection error");
		PQfinish(Raw);
		return false;
	}

	// Success ? store in connection object
	OutConnection = NewObject<UPostgreSQLConnection>();
	OutConnection->SetHandle(Raw);

	return true;
#endif
}

// DISCONNECT -----------------------------------------------------------

void UPostgreSQLBlueprintLibrary::PG_Disconnect(UPostgreSQLConnection* Connection)
{
	if (Connection)
	{
		Connection->Close();
	}
}

bool UPostgreSQLBlueprintLibrary::PG_IsConnected(UPostgreSQLConnection* Connection)
{
	return Connection && Connection->IsConnected();
}

// NON-QUERY ------------------------------------------------------------

bool UPostgreSQLBlueprintLibrary::PG_ExecuteNonQuery(
	UPostgreSQLConnection* Connection,
	const FString& Sql,
	FString& OutError
)
{
	OutError.Reset();

#if !PLATFORM_WINDOWS
	OutError = TEXT("PostgreSQL plugin is currently implemented for Windows only.");
	return false;
#else
	if (!Connection || !Connection->GetHandle())
	{
		OutError = TEXT("Invalid PostgreSQL connection.");
		return false;
	}

	PGconn* RawConn = Connection->GetHandle();

	FTCHARToUTF8 SqlUtf8(*Sql);
	PGresult* Res = PQexec(RawConn, SqlUtf8.Get());

	if (!Res)
	{
		OutError = TEXT("PQexec returned null result.");
		return false;
	}

	ExecStatusType Status = PQresultStatus(Res);
	if (Status != PGRES_COMMAND_OK)
	{
		const char* Err = PQresultErrorMessage(Res);
		OutError = UTF8_TO_TCHAR(Err ? Err : "Unknown PostgreSQL error");
		PQclear(Res);
		return false;
	}

	PQclear(Res);
	return true;
#endif
}

// QUERY ----------------------------------------------------------------
bool UPostgreSQLBlueprintLibrary::PG_ExecuteQuery(
	UPostgreSQLConnection* Connection,
	const FString& Sql,
	const FString& PrimaryKeyColumn,
	TMap<FString, FPostgreSQLRow>& OutRowsByPK,
	TArray<FString>& OutRowsCSV,
	FString& OutError
)
{
	OutRowsByPK.Reset();
	OutRowsCSV.Reset();
	OutError.Reset();

#if !PLATFORM_WINDOWS
	OutError = TEXT("PostgreSQL plugin is currently implemented for Windows only.");
	return false;
#else
	if (!Connection || !Connection->GetHandle())
	{
		OutError = TEXT("Invalid PostgreSQL connection.");
		return false;
	}

	if (PrimaryKeyColumn.IsEmpty())
	{
		OutError = TEXT("PrimaryKeyColumn must not be empty.");
		return false;
	}

	PGconn* RawConn = Connection->GetHandle();

	FTCHARToUTF8 SqlUtf8(*Sql);
	PGresult* Res = PQexec(RawConn, SqlUtf8.Get());
	if (!Res)
	{
		OutError = TEXT("PQexec returned null result.");
		return false;
	}

	ExecStatusType Status = PQresultStatus(Res);
	if (Status != PGRES_TUPLES_OK)
	{
		const char* Err = PQresultErrorMessage(Res);
		OutError = UTF8_TO_TCHAR(Err ? Err : "Unknown PostgreSQL query error");
		PQclear(Res);
		return false;
	}

	const int NumRows = PQntuples(Res);
	const int NumFields = PQnfields(Res);

	// Find index of primary key column once
	int32 PkFieldIndex = -1;
	for (int ColIdx = 0; ColIdx < NumFields; ++ColIdx)
	{
		const char* Name = PQfname(Res, ColIdx);
		const FString ColName = UTF8_TO_TCHAR(Name ? Name : "");

		if (ColName.Equals(PrimaryKeyColumn, ESearchCase::IgnoreCase))
		{
			PkFieldIndex = ColIdx;
			break;
		}
	}

	if (PkFieldIndex == -1)
	{
		OutError = FString::Printf(
			TEXT("Primary key column '%s' not found in result set."),
			*PrimaryKeyColumn
		);
		PQclear(Res);
		return false;
	}

	for (int RowIdx = 0; RowIdx < NumRows; ++RowIdx)
	{
		FPostgreSQLRow Row;
		FString PkValue;

		// PK value
		if (PQgetisnull(Res, RowIdx, PkFieldIndex))
		{
			OutError = FString::Printf(
				TEXT("Primary key column '%s' is NULL at row %d."),
				*PrimaryKeyColumn,
				RowIdx
			);
			PQclear(Res);
			return false;
		}
		else
		{
			const char* PkVal = PQgetvalue(Res, RowIdx, PkFieldIndex);
			PkValue = UTF8_TO_TCHAR(PkVal ? PkVal : "");
		}

		// Build row struct and CSV line
		FString CsvLine;

		for (int ColIdx = 0; ColIdx < NumFields; ++ColIdx)
		{
			const char* Name = PQfname(Res, ColIdx);
			const char* Val = PQgetisnull(Res, RowIdx, ColIdx)
				? ""
				: PQgetvalue(Res, RowIdx, ColIdx);

			const FString ColName = UTF8_TO_TCHAR(Name ? Name : "");
			const FString Value = UTF8_TO_TCHAR(Val ? Val : "");

			// Store in row map
			Row.Columns.Add(ColName, Value);

			// Append to CSV line (simple comma-separated, no escaping)
			if (ColIdx > 0)
			{
				CsvLine.Append(TEXT(","));
			}
			CsvLine.Append(Value);
		}

		// Map PK -> row
		OutRowsByPK.Add(PkValue, Row);

		// Preserve row order as CSV
		OutRowsCSV.Add(CsvLine);
	}

	PQclear(Res);
	return true;
#endif
}