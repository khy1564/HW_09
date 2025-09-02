using UnrealBuildTool;

public class ChatBaseballGame : ModuleRules
{
	public ChatBaseballGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",

            "UMG", "Slate", "SlateCore",
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PublicIncludePaths.AddRange(new string[] { "ChatBaseballGame" });
    }
}
