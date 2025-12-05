#pragma once

#include "CoreMinimal.h"
#include "PostgreSQLTypes.generated.h"

USTRUCT(BlueprintType)
struct FPostgreSQLRow
{
	GENERATED_BODY()

	// Column name -> text value
	UPROPERTY(BlueprintReadOnly, Category = "PostgreSQL")
	TMap<FString, FString> Columns;
};