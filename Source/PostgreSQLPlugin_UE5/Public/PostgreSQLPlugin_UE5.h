#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPostgreSQL, Log, All);

class POSTGRESQLPLUGIN_UE5_API FPostgreSQLPlugin_UE5Module : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static inline FPostgreSQLPlugin_UE5Module& Get()
	{
		return FModuleManager::LoadModuleChecked<FPostgreSQLPlugin_UE5Module>("PostgreSQLPlugin_UE5");
	}

private:
	void* LibPQHandle = nullptr;
};