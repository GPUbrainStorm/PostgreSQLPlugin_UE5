using System.IO;
using UnrealBuildTool;

public class PostgreSQLPlugin_UE5 : ModuleRules
{
    public PostgreSQLPlugin_UE5(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new[]
        {
            Path.Combine(PluginDirectory, "Source", "ThirdParty", "include"),
            Path.Combine(ModuleDirectory, "Public"),
        });

        PrivateIncludePaths.AddRange(new[]
        {
            Path.Combine(ModuleDirectory, "Private"),
        });

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "Projects",
            "Slate",
            "SlateCore",
        });

        PrivateDependencyModuleNames.AddRange(new[]
        {
            "CoreUObject",
            "Engine",
        });

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            string TP = Path.Combine(PluginDirectory, "Source", "ThirdParty");
            string IncDir = Path.Combine(TP, "include");
            string LibDir = Path.Combine(TP, "lib", "Win64");
            string BinDir = Path.Combine(TP, "bin", "Win64");

            PublicIncludePaths.Add(IncDir);

            PublicAdditionalLibraries.Add(Path.Combine(LibDir, "libpq.lib"));

            // Delay-load only libpq (entry DLL)
            PublicDelayLoadDLLs.AddRange(new[]
            {
                "libpq.dll",
            });

            // Copy ALL DLLs from ThirdParty/bin/Win64 to the packaged binary directory
            if (Directory.Exists(BinDir))
            {
                string[] dllFiles = Directory.GetFiles(BinDir, "*.dll");
                foreach (string dllPath in dllFiles)
                {
                    string dllName = Path.GetFileName(dllPath);

                    // Stage into the output folder next to the game exe
                    RuntimeDependencies.Add(
                        Path.Combine("$(BinaryOutputDir)", dllName),
                        dllPath
                    );

                    // Also stage as NonUFS so it’s included in the packaged build
                    RuntimeDependencies.Add(
                        dllPath,
                        StagedFileType.NonUFS
                    );
                }
            }
        }
    }
}