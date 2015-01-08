#include "PluginCreatorPrivatePCH.h"
#include "PluginHelpers.h"

#define LOCTEXT_NAMESPACE "PluginHelpers"

bool FPluginHelpers::WriteOutputFile(const FString& OutputFilename, const FString& OutputFileContents, FText& OutFailReason)
{
	if (FFileHelper::SaveStringToFile(OutputFileContents, *OutputFilename))
	{
		return true;
	}

	FFormatNamedArguments Args;
	Args.Add(TEXT("OutputFilename"), FText::FromString(OutputFilename));
	OutFailReason = FText::Format(LOCTEXT("FailedToWriteOutputFile", "Failed to write output file \"{OutputFilename}\". Perhaps the file is Read-Only?"), Args);
	return false;
}

bool FPluginHelpers::ReadTemplateFile(const FString& TemplateFileName, FString& OutFileContents, FText& OutFailReason)
{
	const FString FullFileName = FPaths::EnginePluginsDir() / TEXT("PluginCreatorPlugin") / TEXT("Templates") / TemplateFileName;
	if (FFileHelper::LoadFileToString(OutFileContents, *FullFileName))
	{
		return true;
	}

	FFormatNamedArguments Args;
	Args.Add(TEXT("FullFileName"), FText::FromString(FullFileName));
	OutFailReason = FText::Format(LOCTEXT("FailedToReadTemplateFile", "Failed to read template file \"{FullFileName}\""), Args);
	return false;
}

FString FPluginHelpers::MakeCommaDelimitedList(const TArray<FString>& InList, bool bPlaceQuotesAroundEveryElement)
{
	FString ReturnString;

	for (auto ListIt = InList.CreateConstIterator(); ListIt; ++ListIt)
	{
		FString ElementStr;
		if (bPlaceQuotesAroundEveryElement)
		{
			ElementStr = FString::Printf(TEXT("\"%s\""), **ListIt);
		}
		else
		{
			ElementStr = *ListIt;
		}

		if (ReturnString.Len() > 0)
		{
			// If this is not the first item in the list, prepend with a comma
			ElementStr = FString::Printf(TEXT(", %s"), *ElementStr);
		}

		ReturnString += ElementStr;
	}

	return ReturnString;
}

bool FPluginHelpers::CreatePluginBuildFile(const FString& NewBuildFileName, const FString& PluginName, FText& OutFailReason, FString TemplateType)
{
	FString Template;

	FString TemplateFileName = TEXT("PluginModule.Build.cs.template");

	if (!ReadTemplateFile(TemplateType.IsEmpty() ? TemplateFileName : TemplateType / TemplateFileName, Template, OutFailReason))
	{
		return false;
	}

	TArray<FString> PublicIncludePathsNames;
	TArray<FString> PrivateIncludePathsNames;
	TArray<FString> PublicDependencyModuleNames;
	TArray<FString> PrivateDependencyModuleNames;
	TArray<FString> DynamicallyLoadedModuleNames;

	PrivateDependencyModuleNames.Add("Slate");
	PrivateDependencyModuleNames.Add("SlateCore");

	FString FinalOutput = Template.Replace(TEXT("%PUBLIC_INCLUDE_PATHS_NAMES%"), *MakeCommaDelimitedList(PublicIncludePathsNames), ESearchCase::CaseSensitive);
	FinalOutput = FinalOutput.Replace(TEXT("%PRIVATE_INCLUDE_PATHS_NAMES%"), *MakeCommaDelimitedList(PrivateIncludePathsNames), ESearchCase::CaseSensitive);
	FinalOutput = FinalOutput.Replace(TEXT("%PUBLIC_DEPENDENCY_MODULE_NAMES%"), *MakeCommaDelimitedList(PublicDependencyModuleNames), ESearchCase::CaseSensitive);
	FinalOutput = FinalOutput.Replace(TEXT("%PRIVATE_DEPENDENCY_MODULE_NAMES%"), *MakeCommaDelimitedList(PrivateDependencyModuleNames), ESearchCase::CaseSensitive);
	FinalOutput = FinalOutput.Replace(TEXT("%DYNAMICALLY_LOADED_MODULES_NAMES%"), *MakeCommaDelimitedList(DynamicallyLoadedModuleNames), ESearchCase::CaseSensitive);
	FinalOutput = FinalOutput.Replace(TEXT("%MODULE_NAME%"), *PluginName, ESearchCase::CaseSensitive);

	return WriteOutputFile(NewBuildFileName, FinalOutput, OutFailReason);
}

bool FPluginHelpers::CreatePluginHeaderFile(const FString& FolderPath, const FString& PluginName, FText& OutFailReason, FString TemplateType)
{
	FString Template;

	FString TemplateFileName = TEXT("PluginModule.h.template");

	if (!ReadTemplateFile(TemplateType.IsEmpty() ? TemplateFileName : TemplateType / TemplateFileName, Template, OutFailReason))
	{
		return false;
	}

	TArray<FString> PublicHeaderIncludes;

	FString	FinalOutput = Template.Replace(TEXT("%PUBLIC_HEADER_INCLUDES%"), *MakeIncludeList(PublicHeaderIncludes), ESearchCase::CaseSensitive);
	FinalOutput = FinalOutput.Replace(TEXT("%PLUGIN_NAME%"), *PluginName, ESearchCase::CaseSensitive);

	return WriteOutputFile(FolderPath / PluginName + TEXT(".h"), FinalOutput, OutFailReason);

	if (!WriteOutputFile(FolderPath / PluginName + TEXT(".h"), FinalOutput, OutFailReason))
	{
		return false;
	}
}

bool FPluginHelpers::CreatePluginCPPFile(const FString& FolderPath, const FString& PluginName, FText& OutFailReason, FString TemplateType)
{
	FString Template;

	FString TemplateFileName = TEXT("PluginModule.cpp.template");

	if (!ReadTemplateFile(TemplateType.IsEmpty() ? TemplateFileName : TemplateType / TemplateFileName, Template, OutFailReason))
	{
		return false;
	}

	TArray<FString> PublicHeaderIncludes;

	FString	FinalOutput = Template.Replace(TEXT("%PUBLIC_HEADER_INCLUDES%"), *MakeIncludeList(PublicHeaderIncludes), ESearchCase::CaseSensitive);
	FinalOutput = FinalOutput.Replace(TEXT("%PLUGIN_NAME%"), *PluginName, ESearchCase::CaseSensitive);

	return WriteOutputFile(FolderPath / PluginName + TEXT(".cpp"), FinalOutput, OutFailReason);

	if (!WriteOutputFile(FolderPath / PluginName + TEXT(".cpp"), FinalOutput, OutFailReason))
	{
		return false;
	}
}

bool FPluginHelpers::CreatePrivatePCHFile(const FString& FolderPath, const FString& PluginName, FText& OutFailReason, FString TemplateType)
{
	FString Template;
	FString TemplateFileName = TEXT("PluginModulePCH.h.template");

	if (!ReadTemplateFile(TemplateType.IsEmpty() ? TemplateFileName : TemplateType / TemplateFileName, Template, OutFailReason))
	{
		return false;
	}

	TArray<FString> PublicHeaderIncludes;

	FString	FinalOutput = Template.Replace(TEXT("%PLUGIN_NAME%"), *PluginName, ESearchCase::CaseSensitive);

	return WriteOutputFile(FolderPath / PluginName + TEXT("PrivatePCH.h"), FinalOutput, OutFailReason);
}

FString FPluginHelpers::MakeIncludeList(const TArray<FString>& InList)
{
	FString ReturnString;

	for (auto ListIt = InList.CreateConstIterator(); ListIt; ++ListIt)
	{
		ReturnString += FString::Printf(TEXT("#include \"%s\"") LINE_TERMINATOR, **ListIt);
	}

	return ReturnString;
}

bool FPluginHelpers::SavePluginDescriptor(const FString& NewProjectFilename, const FPluginDescriptor& PluginDescriptor)
{
	FString Text;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&Text);

	Writer->WriteObjectStart();

	// Write all the simple fields

	Writer->WriteValue(TEXT("FileVersion"), PluginDescriptor.FileVersion);

	Writer->WriteValue(TEXT("FriendlyName"), PluginDescriptor.FriendlyName);
	Writer->WriteValue(TEXT("Version"), PluginDescriptor.Version);
	Writer->WriteValue(TEXT("VersionName"), PluginDescriptor.VersionName);
	Writer->WriteValue(TEXT("CreatedBy"), PluginDescriptor.CreatedBy);
	Writer->WriteValue(TEXT("CreatedByURL"), PluginDescriptor.CreatedByURL);
	Writer->WriteValue(TEXT("Category"), PluginDescriptor.Category);
	Writer->WriteValue(TEXT("Description"), PluginDescriptor.Description);
	Writer->WriteValue(TEXT("EnabledByDefault"), PluginDescriptor.bEnabledByDefault);


	// Write the module list
	FModuleDescriptor::WriteArray(Writer.Get(), TEXT("Modules"), PluginDescriptor.Modules);

	Writer->WriteObjectEnd();

	Writer->Close();

	return FFileHelper::SaveStringToFile(Text, *NewProjectFilename);
}

bool FPluginHelpers::CreatePluginStyleFiles(const FString& PrivateFolderPath, const FString& PublicFolderPath, const FString& PluginName, FText& OutFailReason, FString TemplateType)
{
	{
		// Make Style.h file first

		FString Template;
		FString TemplateFileName = TEXT("PluginStyle.h.template");

		if (!ReadTemplateFile(TemplateType.IsEmpty() ? TemplateFileName : TemplateType / TemplateFileName, Template, OutFailReason))
		{
			return false;
		}

		TArray<FString> PublicHeaderIncludes;

		FString	FinalOutput = Template.Replace(TEXT("%PLUGIN_NAME%"), *PluginName, ESearchCase::CaseSensitive);

		if (!WriteOutputFile(PublicFolderPath / PluginName + TEXT("Style.h"), FinalOutput, OutFailReason))
		{
			// fail here
		}
	}

	{
		// Now .cpp file

		FString Template;

		FString TemplateFileName = TEXT("PluginStyle.cpp.template");

		if (!ReadTemplateFile(TemplateType.IsEmpty() ? TemplateFileName : TemplateType / TemplateFileName, Template, OutFailReason))
		{
			return false;
		}

		TArray<FString> PublicHeaderIncludes;

		FString	FinalOutput = Template.Replace(TEXT("%PLUGIN_NAME%"), *PluginName, ESearchCase::CaseSensitive);

		if (!WriteOutputFile(PrivateFolderPath / PluginName + TEXT("Style.cpp"), FinalOutput, OutFailReason))
		{
			// fail here
		}
	}

	return true;
	
}

bool FPluginHelpers::CreateCommandsFile(const FString& FolderPath, const FString& PluginName, FText& OutFailReason, FString TemplateType)
{
	FString Template;

	FString TemplateFileName = TEXT("PluginCommands.h.template");

	if (!ReadTemplateFile(TemplateType.IsEmpty() ? TemplateFileName : TemplateType / TemplateFileName, Template, OutFailReason))
	{
		return false;
	}

	TArray<FString> PublicHeaderIncludes;
	FString ButtonLabel = TEXT("BUTTON LABEL");

	FString	FinalOutput = Template.Replace(TEXT("%PLUGIN_NAME%"), *PluginName, ESearchCase::CaseSensitive);
	FinalOutput = FinalOutput.Replace(TEXT("%PLUGIN_BUTTON_LABEL%"), *PluginName, ESearchCase::CaseSensitive);

	return WriteOutputFile(FolderPath / PluginName + TEXT("Commands.h"), FinalOutput, OutFailReason);
}

bool FPluginHelpers::NameContainsOnlyLegalCharacters(const FString& TestName, FString& OutIllegalCharacters)
{
	bool bContainsIllegalCharacters = false;

	// Only allow alphanumeric characters in the file name
	bool bFoundAlphaNumericChar = false;
	for (int32 CharIdx = 0; CharIdx < TestName.Len(); ++CharIdx)
	{
		const FString& Char = TestName.Mid(CharIdx, 1);
		if (!FChar::IsAlnum(Char[0]) && Char != TEXT("_"))
		{
			if (!OutIllegalCharacters.Contains(Char))
			{
				OutIllegalCharacters += Char;
			}

			bContainsIllegalCharacters = true;
		}
	}

	return !bContainsIllegalCharacters;
}

#undef LOCTEXT_NAMESPACE