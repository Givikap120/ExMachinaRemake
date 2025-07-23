#include "Slots/WeaponSlot.h"
#include "CustomFunctionLibrary.h"

void UWeaponSlot::GetSize(int32& SizeX, int32& SizeY) const
{
	UWeapon::GetSizeForWeaponType(SlotType, SizeX, SizeY);
}

bool UWeaponSlot::TrySetWeapon(UWeapon* NewWeapon)
{
	if (!IsValid(NewWeapon))
	{
		SetWeapon(nullptr);
		return true;
	}

	if (NewWeapon->WeaponType == SlotType)
	{
		SetWeapon(NewWeapon);
		return true;
	}

	return false;
}

bool UWeaponSlot::TryDropWeapon(UWeapon* NewWeapon)
{
	if (HasWeapon()) return false;
	return TrySetWeapon(NewWeapon);
}

UWeapon* UWeaponSlot::RemoveWeapon()
{
	if (!HasWeapon()) return nullptr;

	UWeapon* RemovedWeapon = Weapon;
	SetWeapon(nullptr);

	return RemovedWeapon;
}

void UWeaponSlot::SetFromStats(FWeaponSlotStats Stats)
{
	SlotType = Stats.SlotType;
	PosX = Stats.PosX;
	PosY = Stats.PosY;
	SocketName = Stats.SocketName;
}

UWeapon* UWeaponSlot::ValidateWeapon()
{
	// It's valid if no weapon or it has correct type
	if (!HasWeapon() || Weapon->WeaponType == SlotType) return nullptr;

	return RemoveWeapon();
}

bool UWeaponSlot::IsValidSlot() const
{
	return SlotType != EWeaponType::None && !SocketName.IsNone();
}

void UWeaponSlot::SetWeapon(UWeapon* NewWeapon)
{
	Weapon = NewWeapon;
	OnWeaponChanged.Broadcast();
}

void UWeaponSlot::SetFromTable(UDataTable* DataTable, FName RowName)
{
	if (!DataTable) return;

	FWeaponSlotStats* Row = DataTable->FindRow<FWeaponSlotStats>(RowName, "");
	if (!Row) return;

	SlotType = Row->SlotType;
	PosX = Row->PosX;
	PosY = Row->PosY;
	SocketName = Row->SocketName;
}

UWeaponSlot* UWeaponSlot::CreateWeaponSlotFromTable(TSubclassOf<UWeaponSlot> Class, UDataTable* DataTable, FName RowName)
{
	auto Result = Cast<UWeaponSlot>(UCustomFunctionLibrary::CreateObject(Class));
	Result->SetFromTable(DataTable, RowName);
	return Result;
}
