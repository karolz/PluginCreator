#pragma once

#include "SlateBasics.h"

#include "KismetEditorUtilities.h"
#include "DlgPickAssetPath.h"

#include "DesktopPlatformModule.h"
#include "ModuleDescriptor.h"
#include "PluginDescriptor.h"
#include "ISourceControlModule.h"
#include "IPluginManager.h"

#include "PluginHelpers.h"

#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"

#include "Editor/PropertyEditor/Public/PropertyEditing.h"




enum ETemplateType
{
	ETT_Blank,
	ETT_Basic,
	ETT_Advanced
};

class SPluginCreatorTabContent : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SPluginCreatorTabContent)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
	/** Get a widget that will be displayed under Blank template tab*/
	TSharedRef<SWidget> GetBlankTemplateDescriptionWidget();

	/** Get a widget that will be displayed under Basic template tab*/
	TSharedRef<SWidget> GetBasicTemplateDescriptionWidget();

	/** Get a widget that will be displayed under Advanced template tab*/
	TSharedRef<SWidget> GetAdvancedTemplateDescriptionWidget();

	bool IsPluginNameValid() const { return bPluginNameIsValid; }

	/** Getter for Editor icon brush*/
	const FSlateBrush* GetPluginEditorImage() const;

	/** Getter for toolbar button icon brush*/
	const FSlateBrush* GetPluginButtonImage() const;

	/** Called upon Editor icon click*/
	FReply OnChangePluginEditorIcon();

	/** Called upon Toolbar button icon click*/
	FReply OnChangePluginButtonIcon();

	/** Bring up a dialog that will let us choose a png file
	*	@param OutFiles - files that were selected
	*	@return wether files were opened or dialog was closed*/
	bool OpenImageDialog(TArray<FString>& OutFiles);

	/** Called when user switches templates tab
	*	@param InNewTab - selected tab name*/
	void OnTemplatesTabSwitched(const FText& InNewTab);

	/** Init property panel that will display plugin descriptor details*/
	void InitDetailsView();

	void OnUsePrivatePublicSplitChanged(ESlateCheckBoxState::Type InState);

	/** Called when Plugin Name textbox changes value*/
	void OnPluginNameTextChanged(const FText& InText);

	/** Helper function to create directory under given path
	*	@param InPath - path to a directory that you want to create*/
	bool MakeDirectory(FString InPath);

	/** Helper function to delete given directory
	* @param InPath - full path to directory that you want to remove*/
	void DeletePluginDirectory(FString InPath);

	/** This is where all the magic happens.
	*	Create actual plugin using parameters collected from other widgets*/
	FReply OnCreatePluginClicked();

	/** Name of the plugin you want to create*/
	FText PluginNameText;

	/** Plugin descriptor that will be used to create .uplugin file*/
	FPluginDescriptor Descriptor;

	/** Property panel used to display plugin descriptor parameters*/
	TSharedPtr<class IDetailsView> PropertyView;

	/** We need UObject based class to display it in property panel*/
	class UPluginDescriptorObject* DescriptorObject;

	/** Do you want to use Private/Public source folder split? 
	*	If so, headers will go to Public folder and source files will be put to Private.
	*	True by default*/
	bool bUsePublicPrivateSplit;

	/** Textbox widget that user will put plugin name in*/
	TSharedPtr<SEditableTextBox> PluginNameTextBox;

	/** Widget switcher used to switch between different templates tabs*/
	TSharedPtr<class SWidgetSwitcher> TemplateWidgetSwitcher;

	bool bPluginNameIsValid;

	/** Type of selected template. ETT_Blank by default.*/
	ETemplateType TemplateType;

	/** Full path to plugin editor icon*/
	FString PluginEditorIconPath;

	/** Full path to toolbar button icon*/
	FString PluginButtonIconPath;

	TSharedPtr<FSlateDynamicImageBrush> PluginEditorIconBrush;
	TSharedPtr<FSlateDynamicImageBrush> PluginButtonIconBrush;
};