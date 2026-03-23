// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SandBoxDemo : ModuleRules
{
	public SandBoxDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara","UMG", "CoreOnline","GameplayAbilities","GameplayTasks","GameplayTags" ,"MeetMysql", "HTTP" });


		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "Json", "JsonUtilities" });
		PublicIncludePaths.AddRange(new string[] { "SandBoxDemo", "Niagara" });

		//PublicIncludePaths.Add("D:\\Studytools\\Epic Games\\UE_5.4\\Engine\\Plugins\\FX\\Niagara\\Source\\Niagara\\Classes");
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
