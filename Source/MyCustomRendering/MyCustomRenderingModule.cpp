#include "MyCustomRenderingModule.h"
#include "MyCustomRenderingPass.h"
#include "Modules/ModuleManager.h"
#include "RenderGraphUtils.h"
#include "./UnrealShaders/MyCustomPS.h"
#include "./UnrealShaders/MyCustomVS.h"

void FMyCustomRenderingModule::StartupModule()
{

	//GameSourceDir  This function returns the directory path to the source code of the game project.
	// FPaths::Combine(...) This function combines multiple path segments into one complete path.
	FString BaseDir = FPaths::Combine(FPaths::GameSourceDir(), TEXT("MyCustomRendering"));
	//it further combines this path with "Shaders", pointing to the Shaders directory within your MyCustomRenderingModule.
	FString ModuleShaderDir = FPaths::Combine(BaseDir, TEXT("Shaders"));

	//AddShaderSourceDirectoryMapping  This function maps a virtual shader directory to a real file system directory It allows Unreal Engine to know where to find your shader files when using the /MyCustomRenderingModule virtual directory in your shader code.
	AddShaderSourceDirectoryMapping(TEXT("/MyCustomRenderingModule"), ModuleShaderDir);
}


void FMyCustomRenderingModule::ShutdownModule()
{
    // Cleanup if needed
}

IMPLEMENT_MODULE(FMyCustomRenderingModule, MyCustomRendering)