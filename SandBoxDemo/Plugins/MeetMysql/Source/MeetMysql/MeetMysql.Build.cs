// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class MeetMysql : ModuleRules
{
	public MeetMysql(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		this.bEnableExceptions = true;
		
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
				Path.Combine(ModuleDirectory,"ThirdParty", "include")
			}
			);
		
		PublicSystemLibraryPaths.Add(Path.Combine(ModuleDirectory,"ThirdParty", "lib"));

		//添加静态库
		PublicSystemLibraries.Add(Path.Combine(ModuleDirectory,"ThirdParty", "lib", "mysqlcppconn-static.lib"));
		// //加入动态库
		RuntimeDependencies.Add(Path.Combine(ModuleDirectory,"ThirdParty", "lib", "libcrypto-1_1-x64.dll"));
		RuntimeDependencies.Add(Path.Combine(ModuleDirectory,"ThirdParty", "lib", "libssl-1_1-x64.dll"));
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{

				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
