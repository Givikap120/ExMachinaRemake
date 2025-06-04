#include "GadgetWeapon.h"
#include <CustomFunctionLibrary.h>
#include "JsonObjectConverter.h"

void UGadgetWeapon::ApplyToStats(UPARAM(ref) FWeaponStats& WeaponStats)
{
	WeaponStats.Damage *= 1 + 0.01f * Stats.DamageIncrease;
	WeaponStats.FiringRate *= 1 + 0.01f * Stats.FiringRateIncrease;
	WeaponStats.FiringRange *= 1 + 0.01f * Stats.FiringRangeIncrease;
	WeaponStats.RecharingTime /= 1 + 0.01f * Stats.RechargingSpeedIncrease;
}

UGadgetWeapon* UGadgetWeapon::CreateGadgetWeaponFromTable(TSubclassOf<UGadgetWeapon> Class, UDataTable* DataTable, FName RowName)
{
	auto Result = Cast<UGadgetWeapon>(UCustomFunctionLibrary::CreateObject(Class));
	Result->SetFromTable(DataTable, RowName);
	return Result;
}

void UGadgetWeapon::SetFromTable(UDataTable* DataTable, FName RowName)
{
	if (!DataTable) return;

	FGadgetWeaponStats* Row = DataTable->FindRow<FGadgetWeaponStats>(RowName, "");
	if (!Row) return;

	// UItem
	Name = RowName;
	DisplayName = Row->DisplayName;
	Price = Row->Price;
	Icon = Row->Icon;

	Stats = *Row;
}

FF_SaveGame_GadgetWeapon UGadgetWeapon::SaveGadgetWeapon() const
{
	FF_SaveGame_Item BaseSave = SaveItem();
	FF_SaveGame_GadgetWeapon SaveData;

	FF_SaveGame_Item* SavePtr = &SaveData;
	FF_SaveGame_Item::StaticStruct()->CopyScriptStruct(SavePtr, &BaseSave);

	SaveData.GadgetStats = Stats;

	return SaveData;
}

void UGadgetWeapon::LoadGadgetWeapon(const FF_SaveGame_GadgetWeapon& SaveGadgetWeapon)
{
	LoadItem(SaveGadgetWeapon);

	Stats = SaveGadgetWeapon.GadgetStats;
}

FString UGadgetWeapon::Serialize() const
{
	FF_SaveGame_GadgetWeapon Saved = SaveGadgetWeapon();

	FString OutputString;
	if (FJsonObjectConverter::UStructToJsonObjectString(Saved, OutputString))
		return OutputString;

	return TEXT("");
}

void UGadgetWeapon::Deserialize(FString Json)
{
	FF_SaveGame_GadgetWeapon Loaded;
	if (FJsonObjectConverter::JsonObjectStringToUStruct<FF_SaveGame_GadgetWeapon>(Json, &Loaded, 0, 0))
		LoadGadgetWeapon(Loaded);
}
