#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PostgreSQLTypes.h"
#include "PostgreSQLBlueprintLibrary.generated.h"

class UPostgreSQLConnection;

UCLASS()
class POSTGRESQLPLUGIN_UE5_API UPostgreSQLBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// CONNECT ----------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "PostgreSQL", meta = (DisplayName = "PG Connect"))
	static bool PG_Connect(
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
	);

	// DISCONNECT -------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "PostgreSQL", meta = (DisplayName = "PG Disconnect"))
	static void PG_Disconnect(UPostgreSQLConnection* Connection);

	UFUNCTION(BlueprintPure, Category = "PostgreSQL", meta = (DisplayName = "PG Is Connected"))
	static bool PG_IsConnected(UPostgreSQLConnection* Connection);

	// NON-QUERY (CREATE / INSERT / UPDATE / DELETE / ALTER ...) --------
	UFUNCTION(BlueprintCallable, Category = "PostgreSQL", meta = (DisplayName = "PG Execute Non Query"))
	static bool PG_ExecuteNonQuery(
		UPostgreSQLConnection* Connection,
		const FString& Sql,
		FString& OutError
	);

	// QUERY (SELECT) ---------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "PostgreSQL", meta = (DisplayName = "PG Execute Query"))
	static bool PG_ExecuteQuery(
		UPostgreSQLConnection* Connection,
		const FString& Sql,
		const FString& PrimaryKeyColumn,
		TMap<FString, FPostgreSQLRow>& OutRowsByPK,
		TArray<FString>& OutRowsCSV,
		FString& OutError
	);
};