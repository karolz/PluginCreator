using UnrealBuildTool;
using System.IO;
 
public class PluginCreatorPlugin : ModuleRules
{
    public PluginCreatorPlugin(TargetInfo Target)
    {
        PrivateIncludePaths.AddRange(new string[] { "PluginCreatorPlugin/Private", "PluginCreatorPlugin/Private/UI", "PluginCreatorPlugin/Private/Helpers"  });

        PrivateIncludePathModuleNames.AddRange(new string[] { "DesktopPlatform" });

        PrivateDependencyModuleNames.AddRange(
            new string[] {
				"Core",
				"CoreUObject",
				"Slate",
				"SlateCore",
				"Engine",
                "InputCore",
                "ClassViewer",
				"AssetTools",
				"UnrealEd", // for FAssetEditorManager
				"KismetWidgets",
				"GraphEditor",
				"Kismet",  // for FWorkflowCentricApplication
				"PropertyEditor",
				"RenderCore",
				"LevelEditor",
				"EditorStyle",
                "DesktopPlatform",
                "Projects",
                //"SourceControl",
			}
            );

        DynamicallyLoadedModuleNames.AddRange(
            new string[] {
                "DesktopPlatform",
            });
    }
}