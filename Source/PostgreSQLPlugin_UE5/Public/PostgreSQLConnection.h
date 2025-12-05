#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PostgreSQLConnection.generated.h"

struct pg_conn;
typedef struct pg_conn PGconn;

UCLASS(BlueprintType)
class POSTGRESQLPLUGIN_UE5_API UPostgreSQLConnection : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "PostgreSQL")
	bool IsConnected() const;

	UFUNCTION(BlueprintCallable, Category = "PostgreSQL")
	void Close();

	PGconn* GetHandle() const { return Conn; }
	void SetHandle(PGconn* InConn);

protected:
	virtual void BeginDestroy() override;

private:
	PGconn* Conn = nullptr;
};