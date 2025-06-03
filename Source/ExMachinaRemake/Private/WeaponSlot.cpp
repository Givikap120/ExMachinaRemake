// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSlot.h"
#include "CustomFunctionLibrary.h"

// Sets default values
UWeaponSlot::UWeaponSlot()
{
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

void UWeaponSlot::GetSize(int32& SizeX, int32& SizeY) const
{
	UWeapon::GetSizeForWeaponType(SlotType, SizeX, SizeY);
}

