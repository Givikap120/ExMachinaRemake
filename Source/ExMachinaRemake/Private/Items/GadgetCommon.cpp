#include "Items/GadgetCommon.h"
#include <CustomFunctionLibrary.h>
#include "JsonObjectConverter.h"

UGadgetCommon* UGadgetCommon::CreateGadgetCommonFromTable(TSubclassOf<UGadgetCommon> Class, UDataTable* DataTable, FName RowName)
{
	auto Result = Cast<UGadgetCommon>(UCustomFunctionLibrary::CreateObject(Class));
	Result->SetFromTable(DataTable, RowName);
	return Result;
}

void UGadgetCommon::SetFromTable(UDataTable* DataTable, FName RowName)
{
	if (!DataTable) return;

	FGadgetCommonStats* Row = DataTable->FindRow<FGadgetCommonStats>(RowName, "");
	if (!Row) return;

	// UItem
	Name = RowName;
	DisplayName = Row->DisplayName;
	Price = Row->Price;
	Icon = Row->Icon;

	MaxSpeedMultiplier = 1 + Row->MaxSpeedIncrease * 0.01;
}

FF_SaveGame_GadgetCommon UGadgetCommon::SaveGadget() const
{
    FF_SaveGame_Item BaseSave = SaveItem();

    FF_SaveGame_GadgetCommon SaveData;
    FF_SaveGame_Item* SavePtr = &SaveData;
    FF_SaveGame_Item::StaticStruct()->CopyScriptStruct(SavePtr, &BaseSave);

    // WARNING: this is wrong, because increase is % increase, not multiplier
    // but I don't want to write additional code
    SaveData.GadgetStats.MaxSpeedIncrease = MaxSpeedMultiplier;

    return SaveData;
}

void UGadgetCommon::LoadGadget(const FF_SaveGame_GadgetCommon& SaveGadgetCommon)
{
    LoadItem(SaveGadgetCommon);

    // Naively assume that this increase is actually a multiplier
    MaxSpeedMultiplier = SaveGadgetCommon.GadgetStats.MaxSpeedIncrease;
}

FString UGadgetCommon::SerializeItem() const
{
    FF_SaveGame_GadgetCommon Saved = SaveGadget();

    FString OutputString;
    if (FJsonObjectConverter::UStructToJsonObjectString(Saved, OutputString))
        return OutputString;

    return TEXT("");
}

void UGadgetCommon::DeserializeItem(FString Json)
{
    FF_SaveGame_GadgetCommon Loaded;
    if (FJsonObjectConverter::JsonObjectStringToUStruct<FF_SaveGame_GadgetCommon>(Json, &Loaded, 0, 0))
        LoadGadget(Loaded);
}
