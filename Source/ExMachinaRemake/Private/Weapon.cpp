#include "Weapon.h"
#include <CustomFunctionLibrary.h>
#include "JsonObjectConverter.h"

void UWeapon::ApplyStats(const FWeaponStats& Stats)
{
	// UItem
	
	DisplayName = Stats.DisplayName;
	Price = Stats.Price;
	Icon = Stats.Icon;

	// Stats
	Damage = Stats.Damage;
	WeaponType = Stats.WeaponType;
	FiringRate = Stats.FiringRate;
	FiringRange = Stats.FiringRange * 1000; // convert units
	ChargeSize = Stats.ChargeSize;
	RecharingTime = Stats.RecharingTime;
	IsAuto = Stats.IsAuto;
	Barrels = Stats.Barrels;
	BarrelType = Stats.BarrelType;

	// Assets
	FireSound = Stats.FireSound;
	TailSound = Stats.TailSound;
	Model = Stats.Model;
}

void UWeapon::SetFromTable(UDataTable* DataTable, FName RowName)
{
	if (!DataTable) return;

	FWeaponStats* Row = DataTable->FindRow<FWeaponStats>(RowName, "");
	if (!Row) return;

	BaseStats = *Row;
	Name = RowName;

	ApplyStats(BaseStats);
	CurrentCharge = ChargeSize;
}

UWeapon* UWeapon::CreateWeaponFromTable(TSubclassOf<UWeapon> Class, UDataTable* DataTable, FName RowName)
{
	auto Result = Cast<UWeapon>(UCustomFunctionLibrary::CreateObject(Class));
	Result->SetFromTable(DataTable, RowName);
	return Result;
}

FName UWeapon::GeTraceBarrel() const
{
	switch (BarrelType)
	{
	case EBarrelType::Normal:
		return Barrels[0];

	case EBarrelType::Alternate:
		return Barrels[CurrentBarrel];

	default:
		return "";
	}
}

TArray<FName> UWeapon::GetParticleBarrels() const
{
	switch (BarrelType)
	{
	case EBarrelType::Normal:
		return Barrels;

	case EBarrelType::Alternate:
		return { Barrels[CurrentBarrel] };

	default:
		return TArray<FName>();
	}
}

FBarrelSockets UWeapon::GetUseBarrels()
{
	switch (BarrelType)
	{
	case EBarrelType::Normal:
		return { Barrels[0], Barrels };

	case EBarrelType::Alternate:
		if (++CurrentBarrel >= Barrels.Num()) CurrentBarrel = 0; // switch to the next barrel
		return { Barrels[CurrentBarrel], TArray<FName>{Barrels[CurrentBarrel]} };

	default:
		return FBarrelSockets();
	};
}

int32 UWeapon::GetSizeX() const
{
	return GetSizeXForWeaponType(WeaponType);
}

int32 UWeapon::GetSizeY() const
{
	return GetSizeYForWeaponType(WeaponType);
}

int32 UWeapon::GetSizeXForWeaponType(EWeaponType Type)
{
	switch (Type)
	{
	case EWeaponType::SmallGun:
		return 2;
	case EWeaponType::BigGun:
		return 2;
	case EWeaponType::GiantGun:;
		return 3;
	case EWeaponType::SideGun:
		return 3;
	case EWeaponType::Special:
		return 2;
	default:
		return 0;
	}
}

int32 UWeapon::GetSizeYForWeaponType(EWeaponType Type)
{
	switch (Type)
	{
	case EWeaponType::None:
		return 0;
	case EWeaponType::SmallGun:
		return 2;
	case EWeaponType::BigGun:
		return 3;
	case EWeaponType::GiantGun:
		return 4;
	case EWeaponType::SideGun:
		return 2;
	case EWeaponType::Special:
		return 1;
	default:
		return 0;
	}
}

void UWeapon::GetSizeForWeaponType(EWeaponType Type, int32& SizeX, int32& SizeY)
{
	SizeX = GetSizeXForWeaponType(Type);
	SizeY = GetSizeYForWeaponType(Type);
}

FF_SaveGame_Weapon UWeapon::SaveWeapon() const
{
	FF_SaveGame_Item BaseSave = SaveItem();

	FF_SaveGame_Weapon SaveData;
	FF_SaveGame_Item* SavePtr = &SaveData;
	FF_SaveGame_Item::StaticStruct()->CopyScriptStruct(SavePtr, &BaseSave);

	SaveData.WeaponStats = BaseStats;
	SaveData.CurrentCharge = CurrentCharge;

	return SaveData;
}

void UWeapon::LoadWeapon(const FF_SaveGame_Weapon& SaveWeapon)
{
	LoadItem(SaveWeapon);

	BaseStats = SaveWeapon.WeaponStats;
	CurrentCharge = SaveWeapon.CurrentCharge;
}

FString UWeapon::Serialize() const
{
	FF_SaveGame_Weapon Saved = SaveWeapon();

	FString OutputString;
	if (FJsonObjectConverter::UStructToJsonObjectString(Saved, OutputString))
		return OutputString;

	return TEXT("");
}

void UWeapon::Deserialize(FString Json)
{
	FF_SaveGame_Weapon Loaded;
	if (FJsonObjectConverter::JsonObjectStringToUStruct<FF_SaveGame_Weapon>(Json, &Loaded, 0, 0))
		LoadItem(Loaded);
}