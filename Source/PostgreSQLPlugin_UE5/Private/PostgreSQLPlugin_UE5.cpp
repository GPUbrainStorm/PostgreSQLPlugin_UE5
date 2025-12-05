#include "PostgreSQLPlugin_UE5.h"
#include "Interfaces/IPluginManager.h"
#include "Modules/ModuleManager.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"

DEFINE_LOG_CATEGORY(LogPostgreSQL);

#define LOCTEXT_NAMESPACE "FPostgreSQLPlugin_UE5Module"

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

void FPostgreSQLPlugin_UE5Module::StartupModule()
{
#if PLATFORM_WINDOWS
	TArray<FString> Dirs;

	if (TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("PostgreSQLPlugin_UE5")))
	{
		Dirs.Add(FPaths::Combine(Plugin->GetBaseDir(), TEXT("Binaries/Win64")));
	}

	Dirs.Add(FPaths::Combine(FPaths::ProjectDir(), TEXT("Binaries/Win64")));
	Dirs.Add(FPlatformProcess::BaseDir());

	for (const FString& Dir : Dirs)
	{
		FPlatformProcess::PushDllDirectory(*Dir);
		if (!LibPQHandle)
		{
			LibPQHandle = FPlatformProcess::GetDllHandle(TEXT("libpq.dll"));
		}
		FPlatformProcess::PopDllDirectory(*Dir);

		if (LibPQHandle)
		{
			break;
		}
	}

	if (!LibPQHandle)
	{
		UE_LOG(LogPostgreSQL, Error, TEXT("Failed to load libpq.dll"));
	}
	else
	{
		UE_LOG(LogPostgreSQL, Log, TEXT("Successfully loaded libpq.dll"));
	}
#endif
}

void FPostgreSQLPlugin_UE5Module::ShutdownModule()
{
#if PLATFORM_WINDOWS
	if (LibPQHandle)
	{
		FPlatformProcess::FreeDllHandle(LibPQHandle);
		LibPQHandle = nullptr;
	}
#endif
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPostgreSQLPlugin_UE5Module, PostgreSQLPlugin_UE5)