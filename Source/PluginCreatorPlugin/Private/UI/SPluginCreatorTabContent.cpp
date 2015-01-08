#include "PluginCreatorPrivatePCH.h"
#include "SPluginCreatorTabContent.h"

#include "STabButtonList.h"
#include "PluginDescriptorObject.h"

#include "SWidgetSwitcher.h"

#define LOCTEXT_NAMESPACE "PluginCreatorPluginModule"

static FText BlankTemplateName = LOCTEXT("BlankTemplateTabLabel", "Blank");
static FText BasicTemplateName = LOCTEXT("BasicTemplateTabLabel", "Basic");
static FText AdvancedTemplateName = LOCTEXT("AdvancedTemplateTabLabel", "Advanced");

void SPluginCreatorTabContent::Construct(const FArguments& InArgs)
{
	TemplateType = ETT_Blank;

	bUsePublicPrivateSplit = true;
	bPluginNameIsValid = false;

	InitDetailsView();

	TArray<FText> ButtonLabels;
	ButtonLabels.Add(BlankTemplateName);
	ButtonLabels.Add(BasicTemplateName);
	ButtonLabels.Add(AdvancedTemplateName);

	PluginEditorIconPath = FPaths::EnginePluginsDir() / TEXT("PluginCreatorPlugin/Resources") / TEXT("DefaultIcon128.png");
	PluginButtonIconPath = PluginEditorIconPath;

	TSharedPtr<STabButtonList> TabList = SNew(STabButtonList)
		.ButtonLabels(ButtonLabels)
		.OnTabButtonSelectionChanged(FOnTextChanged::CreateRaw(this, &SPluginCreatorTabContent::OnTemplatesTabSwitched));

	FSlateFontInfo PluginNameFont = FPluginCreatorStyle::Get().GetFontStyle(TEXT("PluginNameFont"));

	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.Padding(10)
			[
				SNew(SSplitter)
				.Orientation(Orient_Horizontal)
				.PhysicalSplitterHandleSize(5)
				+SSplitter::Slot()
				.Value(0.45f)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBorder)
						.BorderImage(FPluginCreatorStyle::Get().GetBrush(TEXT("DarkGrayBackground")))
						.Padding(15)
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							[
								SAssignNew(PluginNameTextBox, SEditableTextBox)
								.Font(PluginNameFont)
								.HintText(LOCTEXT("PluginNameTextHint", "Plugin name"))
								.OnTextChanged(FOnTextChanged::CreateRaw(this, &SPluginCreatorTabContent::OnPluginNameTextChanged))
							]
							+ SVerticalBox::Slot()
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							.Padding(3)
							[
								SNew(SCheckBox)
								.OnCheckStateChanged(this, &SPluginCreatorTabContent::OnUsePrivatePublicSplitChanged)
								.IsChecked(ESlateCheckBoxState::Checked)
								.Content()
								[
									SNew(STextBlock)
									.Text(LOCTEXT("PrivatePublicCheckboxLabel", "Use Private/Public source folder split"))
								]
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(SHorizontalBox)
								+ SHorizontalBox::Slot()
								.HAlign(HAlign_Center)
								[
									SNew(SBox)
									.WidthOverride(64)
									.HeightOverride(64)
									[
										SNew(SButton)
										.ToolTipText(LOCTEXT("PluginEditorIconTooltip", "This icon will be displayed in plugin editor list. Click to choose a file."))
										.OnClicked(this, &SPluginCreatorTabContent::OnChangePluginEditorIcon)
										.ButtonStyle(FPluginCreatorStyle::Get(), TEXT("NoBorderButton"))
										.ContentPadding(0)
										.Content()
										[
											SNew(SImage)
											.Image(this, &SPluginCreatorTabContent::GetPluginEditorImage)
										]

									]
								]
								+ SHorizontalBox::Slot()
								.HAlign(HAlign_Center)
								[
									SNew(SBox)
									.WidthOverride(64)
									.HeightOverride(64)
									[
										SNew(SButton)
										.ToolTipText(LOCTEXT("PluginButtonIconTooltip", "This icon will be displayed on editor toolbar and under window menu. Click to choose a file."))
										.OnClicked(this, &SPluginCreatorTabContent::OnChangePluginButtonIcon)
										.ButtonStyle(FPluginCreatorStyle::Get(), TEXT("NoBorderButton"))
										.ContentPadding(0)
										.Content()
										[
											SNew(SImage)
											.Image(this, &SPluginCreatorTabContent::GetPluginButtonImage)
										]

									]
								]
							]
						]
					]
					+ SVerticalBox::Slot()
					.Padding(5)
					[
						PropertyView.ToSharedRef()
					]
				]
				+ SSplitter::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.Padding(0)
					.AutoHeight()
					[
						TabList.ToSharedRef()
					]
					+ SVerticalBox::Slot()
					.Padding(0)
					[
						SNew(SBorder)
						.BorderImage(FPluginCreatorStyle::Get().GetBrush(TEXT("DarkGrayBackground")))
						.Padding(15)
						[
							SAssignNew(TemplateWidgetSwitcher, SWidgetSwitcher)
							.WidgetIndex(0)
							+ SWidgetSwitcher::Slot()
							[
								GetBlankTemplateDescriptionWidget()
							]
							+ SWidgetSwitcher::Slot()
							[
								GetBasicTemplateDescriptionWidget()
							]
							+ SWidgetSwitcher::Slot()
							[
								GetAdvancedTemplateDescriptionWidget()
							]
						]
					]
				]
			]

			+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5)
				[
					SNew(SButton)
					.ContentPadding(5)
					.IsEnabled(this, &SPluginCreatorTabContent::IsPluginNameValid)
					.HAlign(HAlign_Center)
					.Text(LOCTEXT("GoButtonLabel", "Create New Plugin!"))
					.OnClicked(this, &SPluginCreatorTabContent::OnCreatePluginClicked)
				]
		];
}

TSharedRef<SWidget> SPluginCreatorTabContent::GetBlankTemplateDescriptionWidget()
{
	return
	SNew(SVerticalBox)
	+ SVerticalBox::Slot()
	.AutoHeight()
	[
		SNew(SRichTextBlock)
		.Text(LOCTEXT("BlankTemplateDesc", "Create a blank plugin with a minimal code amount.\nUse it if you want to setup everything by yourself from scratch and/or you're not going to need UI for your plugin.\nPlugin created with this template will appear only in Editor's plugin list.\n\nThis template will generate the following structure:"))
		.AutoWrapText(true)
	]
	+ SVerticalBox::Slot()
	.Padding(20)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
		SNew(SImage)
		.Image(FPluginCreatorStyle::Get().GetBrush("BlankPluginSource"))
	];
}

TSharedRef<SWidget> SPluginCreatorTabContent::GetBasicTemplateDescriptionWidget()
{
	return
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SRichTextBlock)
			.Text(LOCTEXT("BasicTemplateDesc", "Create a plugin that will add a button to Level Editor's toolbar.\nYou will be able to easily implement \"OnButtonClick\" Event to test all needed functionality right away!\n\nThis template will generate the following structure:"))
			.AutoWrapText(true)
		]
	+ SVerticalBox::Slot()
		.Padding(20)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SImage)
			.Image(FPluginCreatorStyle::Get().GetBrush("BasicPluginSource"))
		];
}

TSharedRef<SWidget> SPluginCreatorTabContent::GetAdvancedTemplateDescriptionWidget()
{
	return
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SRichTextBlock)
			.Text(LOCTEXT("AdvancedTemplateDesc", "Create a plugin that will add a button to Level Editor's toolbar and spawn a default, empty tab window upon button click.\nThis template will be the best choice if you're going to create a plugin that extends Editor UI quite heavily and/or you would like to have your very own editor window;).\n\nThis template will generate the following structure:"))
			.AutoWrapText(true)
		]
	+ SVerticalBox::Slot()
		.Padding(20)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SImage)
			.Image(FPluginCreatorStyle::Get().GetBrush("BasicPluginSource"))
		];
}

const FSlateBrush* SPluginCreatorTabContent::GetPluginEditorImage() const
{
	return PluginEditorIconBrush.IsValid() ? PluginEditorIconBrush.Get() : FPluginCreatorStyle::Get().GetBrush("DefaultPluginIcon");
}

const FSlateBrush* SPluginCreatorTabContent::GetPluginButtonImage() const
{
	return PluginButtonIconBrush.IsValid() ? PluginButtonIconBrush.Get() : FPluginCreatorStyle::Get().GetBrush("DefaultPluginIcon");
}

FReply SPluginCreatorTabContent::OnChangePluginEditorIcon()
{
	TArray<FString> OutFiles;

	bool bOpened = OpenImageDialog(OutFiles);


	if (bOpened && OutFiles.Num() > 0)
	{
		FString Filename = OutFiles[0];

		FName BrushName(*Filename);

		PluginEditorIconPath = Filename;

		PluginEditorIconBrush = MakeShareable(new FSlateDynamicImageBrush(BrushName, FVector2D(64, 64)));
	}

	return FReply::Handled();
}

FReply SPluginCreatorTabContent::OnChangePluginButtonIcon()
{
	TArray<FString> OutFiles;

	bool bOpened = OpenImageDialog(OutFiles);


	if (bOpened && OutFiles.Num() > 0)
	{
		FString Filename = OutFiles[0];

		FName BrushName(*Filename);

		PluginButtonIconPath = Filename;

		PluginButtonIconBrush = MakeShareable(new FSlateDynamicImageBrush(BrushName, FVector2D(64, 64)));
	}

	return FReply::Handled();
}

bool SPluginCreatorTabContent::OpenImageDialog(TArray<FString>& OutFiles)
{
	// TODO: store recent path so the next time user opens a dialog it opens in last loaded path
	void* ParentWindowPtr = FSlateApplication::Get().GetActiveTopLevelWindow()->GetNativeWindow()->GetOSWindowHandle();

	if (ParentWindowPtr)
	{
		IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

		if (DesktopPlatform)
		{
			FString FileTypes = TEXT("Images (*.png)|*.png|All Files (*.*)|*.*");

			return DesktopPlatform->OpenFileDialog(
				ParentWindowPtr,
				"ChoosePluginEditorIcon",
				TEXT(""),
				TEXT(""),
				FileTypes,
				EFileDialogFlags::None,
				OutFiles
				);
		}
	}

	return false;
}

void SPluginCreatorTabContent::OnTemplatesTabSwitched(const FText& InNewTab)
{
	if (TemplateWidgetSwitcher.IsValid())
	{
		if (InNewTab.EqualToCaseIgnored(BlankTemplateName))
		{
			TemplateWidgetSwitcher->SetActiveWidgetIndex(0);
			TemplateType = ETT_Blank;
		}
		else if (InNewTab.EqualToCaseIgnored(BasicTemplateName))
		{
			TemplateWidgetSwitcher->SetActiveWidgetIndex(1);
			TemplateType = ETT_Basic;
		}
		else
		{
			TemplateWidgetSwitcher->SetActiveWidgetIndex(2);
			TemplateType = ETT_Advanced;
		}
	}
}

void SPluginCreatorTabContent::InitDetailsView()
{
	// Create a property view
	FPropertyEditorModule& EditModule = FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs DetailsViewArgs( /*bUpdateFromSelection=*/ false, /*bLockable=*/ false, /*bAllowSearch=*/ false, /*bObjectsUseNameArea=*/ true, /*bHideSelectionTip=*/ true, /*InNotifyHook=*/ NULL, /*InSearchInitialKeyFocus=*/ false, /*InViewIdentifier=*/ NAME_None);

	PropertyView = EditModule.CreateDetailView(DetailsViewArgs);

	DescriptorObject = NewObject<UPluginDescriptorObject>();

	PropertyView->SetObject(DescriptorObject, true);
}

void SPluginCreatorTabContent::OnUsePrivatePublicSplitChanged(ESlateCheckBoxState::Type InState)
{
	bUsePublicPrivateSplit = InState == ESlateCheckBoxState::Checked;
}

void SPluginCreatorTabContent::OnPluginNameTextChanged(const FText& InText)
{
	// Early exit if text is empty
	if (InText.IsEmpty())
	{
		bPluginNameIsValid = false;
		PluginNameText = FText();
		return;
	}

	FString IllegalCharacters;
	// Dont allow commas, dots etc
	if (!FPluginHelpers::NameContainsOnlyLegalCharacters(InText.ToString(), IllegalCharacters))
	{
		PluginNameTextBox->SetError(FString::Printf(TEXT("Plugin name cannot contain illegal characters like: %s"), *IllegalCharacters));
		bPluginNameIsValid = false;
		return;
	}

	//
	const TArray< FPluginStatus > Plugins = IPluginManager::Get().QueryStatusForAllPlugins();
	for (auto PluginIt(Plugins.CreateConstIterator()); PluginIt; ++PluginIt)
	{
		const auto& PluginStatus = *PluginIt;

		if (PluginStatus.Name == InText.ToString())
		{
			PluginNameTextBox->SetError(TEXT("Plugin with this name already exists!"));
			bPluginNameIsValid = false;

			return;
		}
	}


	PluginNameTextBox->SetError(FText::GetEmpty());
	bPluginNameIsValid = true;

	PluginNameText = InText;

	if (DescriptorObject)
	{
		DescriptorObject->FriendlyName = InText.ToString();
	}
}

bool SPluginCreatorTabContent::MakeDirectory(FString InPath)
{
	if (!IFileManager::Get().MakeDirectory(*InPath))
	{
		UE_LOG(PluginCreatorPluginLog, Log, TEXT("Failed to create directory under %s"), *InPath);
		return false;
	}

	return true;
}

void SPluginCreatorTabContent::DeletePluginDirectory(FString InPath)
{
	IFileManager::Get().DeleteDirectory(*InPath, false, true);
}

FReply SPluginCreatorTabContent::OnCreatePluginClicked()
{
	FString AutoPluginName = PluginNameText.ToString();

	TArray<FString> CreatedFiles;
	TArray<FString> StartupModuleNames;

	FText LocalFailReason;

	FReply ReturnReply = FReply::Unhandled();

	// Create main plugin dir
	const FString PluginFolder = FPaths::EnginePluginsDir() / AutoPluginName;
	if (!MakeDirectory(PluginFolder))
	{
		return FReply::Unhandled();
	}

	// Create resource folder
	const FString ResourcesFolder = PluginFolder / TEXT("Resources");
	if (!MakeDirectory(ResourcesFolder))
	{
		DeletePluginDirectory(*PluginFolder);
		return FReply::Unhandled();
	}

		{
			// Copy editor icon
			const FString DestinationFile = ResourcesFolder / TEXT("Icon128.png");

			if (IFileManager::Get().Copy(*DestinationFile, *PluginEditorIconPath, false) != COPY_OK)
			{
				UE_LOG(PluginCreatorPluginLog, Log, TEXT("Couldnt copy icon file %s"), *PluginEditorIconPath);
				DeletePluginDirectory(*PluginFolder);

				return FReply::Unhandled();
			}
			else
			{
				CreatedFiles.Add(DestinationFile);
			}
		}

		{
			if (TemplateType != ETT_Blank)
			{
				// Copy button icon
				const FString DestinationFile = ResourcesFolder / TEXT("ButtonIcon.png");

				if (IFileManager::Get().Copy(*DestinationFile, *PluginButtonIconPath, false) != COPY_OK)
				{
					UE_LOG(PluginCreatorPluginLog, Log, TEXT("Couldnt copy icon file %s"), *PluginButtonIconPath);
					DeletePluginDirectory(*PluginFolder);

					return FReply::Unhandled();
				}
				else
				{
					CreatedFiles.Add(DestinationFile);
				}
			}
		}

		// Create source folder
		const FString SourceFolder = PluginFolder / TEXT("Source");
		if (!MakeDirectory(SourceFolder))
		{
			return FReply::Unhandled();
		}

		// Save descriptor file as .uplugin file
		FString UPluginFilePath = PluginFolder / AutoPluginName + TEXT(".uplugin");

		{
			FPluginDescriptor Descriptor;

			if (DescriptorObject)
			{
				DescriptorObject->FillDescriptor(Descriptor);
			}

			// TODO: let user choose what kind of modules he/she wants to include in new plugin
			StartupModuleNames.Add(AutoPluginName);

			for (int32 Idx = 0; Idx < StartupModuleNames.Num(); Idx++)
			{
				Descriptor.Modules.Add(FModuleDescriptor(*StartupModuleNames[Idx], EHostType::Developer));
			}

			// Save the descriptor using JSon
			if (!FPluginHelpers::SavePluginDescriptor(UPluginFilePath, Descriptor))
			{
				UE_LOG(PluginCreatorPluginLog, Log, TEXT("Couldnt save plugin descriptor under %s"), *UPluginFilePath);
				DeletePluginDirectory(*PluginFolder);
				return FReply::Unhandled();
			}
		}

		const FString PluginSourceFolder = SourceFolder / AutoPluginName;
		if (!MakeDirectory(PluginSourceFolder))
		{
			return FReply::Unhandled();
		}

		FString PrivateSourceFolder = PluginSourceFolder;
		FString PublicSourceFolder = PluginSourceFolder;

		// Create Private/Public folder split if user wants to
		if (bUsePublicPrivateSplit)
		{
			PrivateSourceFolder = PluginSourceFolder / TEXT("Private");
			if (!MakeDirectory(PrivateSourceFolder))
			{
				return FReply::Unhandled();
			}

			PublicSourceFolder = PluginSourceFolder / TEXT("Public");
			if (!MakeDirectory(PublicSourceFolder))
			{
				return FReply::Unhandled();
			}
		}

		FString TemplateTypeName;

		if (TemplateType == ETT_Blank) TemplateTypeName = BlankTemplateName.ToString();
		if (TemplateType == ETT_Basic) TemplateTypeName = BasicTemplateName.ToString();
		if (TemplateType == ETT_Advanced) TemplateTypeName = AdvancedTemplateName.ToString();


		// Based on chosen template create build, and other source files
		if (!FPluginHelpers::CreatePluginBuildFile(PluginSourceFolder / AutoPluginName + TEXT(".Build.cs"), AutoPluginName, LocalFailReason, TemplateTypeName))
		{
			UE_LOG(PluginCreatorPluginLog, Log, TEXT("Failed to create plugin build file. %s"), *(LocalFailReason.ToString()));
			DeletePluginDirectory(*PluginFolder);
			return FReply::Unhandled();
		}
		else
		{

		}

		if (!FPluginHelpers::CreatePluginHeaderFile(PublicSourceFolder, AutoPluginName, LocalFailReason, TemplateTypeName))
		{
			UE_LOG(PluginCreatorPluginLog, Log, TEXT("Failed to create plugin header file. %s"), *(LocalFailReason.ToString()));
			DeletePluginDirectory(*PluginFolder);
			return FReply::Unhandled();
		}

		if (!FPluginHelpers::CreatePrivatePCHFile(PrivateSourceFolder, AutoPluginName, LocalFailReason, TemplateTypeName))
		{
			UE_LOG(PluginCreatorPluginLog, Log, TEXT("Failed to create plugin PCH file. %s"), *(LocalFailReason.ToString()));
			DeletePluginDirectory(*PluginFolder);
			return FReply::Unhandled();
		}

		if (!FPluginHelpers::CreatePluginCPPFile(PrivateSourceFolder, AutoPluginName, LocalFailReason, TemplateTypeName))
		{
			UE_LOG(PluginCreatorPluginLog, Log, TEXT("Failed to create plugin cpp file. %s"), *(LocalFailReason.ToString()));
			DeletePluginDirectory(*PluginFolder);
			return FReply::Unhandled();
		}

		if (TemplateType != ETT_Blank)
		{
			if (!FPluginHelpers::CreatePluginStyleFiles(PrivateSourceFolder, PublicSourceFolder, AutoPluginName, LocalFailReason, TemplateTypeName))
			{
				UE_LOG(PluginCreatorPluginLog, Log, TEXT("Failed to create plugin styles files. %s"), *(LocalFailReason.ToString()));
				DeletePluginDirectory(*PluginFolder);
				return FReply::Unhandled();
			}

			if (!FPluginHelpers::CreateCommandsFile(PublicSourceFolder, AutoPluginName, LocalFailReason, TemplateTypeName))
			{
				UE_LOG(PluginCreatorPluginLog, Log, TEXT("Failed to create plugin commands file. %s"), *(LocalFailReason.ToString()));
				DeletePluginDirectory(*PluginFolder);
				return FReply::Unhandled();
			}
		}

		const FText Message = FText::Format(LOCTEXT("CreatePluginSuccess", "Successfully created {0}.\nWould you like to open its folder?"), FText::FromString(AutoPluginName));

		if (FMessageDialog::Open(EAppMsgType::YesNo, Message) == EAppReturnType::Yes)
		{
			FPlatformProcess::ExploreFolder(*UPluginFilePath);
		}

		return FReply::Handled();
}
#undef LOCTEXT_NAMESPACE