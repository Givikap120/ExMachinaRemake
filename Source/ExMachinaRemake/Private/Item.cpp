// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "JsonObjectConverter.h"

void UItem::GetSize(int32& SizeX, int32& SizeY) const
{
	SizeX = GetSizeX();
	SizeY = GetSizeY();
}

void UItem::SetFromTable(UDataTable* DataTable, FName RowName)
{
	if (!DataTable) return;

	FItemStats* Row = DataTable->FindRow<FItemStats>(RowName, "");
	if (!Row) return;

	// UItem
	Name = RowName;
	DisplayName = Row->DisplayName;
	Price = Row->Price;
	Icon = Row->Icon;
}

UItem::UItem()
{
}

FF_SaveGame_Item UItem::SaveItem() const
{
	FF_SaveGame_Item Out;
	Out.Name = Name;
	Out.DisplayName = DisplayName;
	Out.Price = Price;
	Out.Icon = Icon;
	return Out;
}

void UItem::LoadItem(const FF_SaveGame_Item& SaveItem)
{
	Name = SaveItem.Name;
	DisplayName = SaveItem.DisplayName;
	Price = SaveItem.Price;
	Icon = SaveItem.Icon;
}

FString UItem::Serialize() const
{
	FF_SaveGame_Item Saved = SaveItem();

	FString OutputString;
	if (FJsonObjectConverter::UStructToJsonObjectString(Saved, OutputString))
		return OutputString;

	return TEXT("");
}

void UItem::Deserialize(FString Json)
{
	FF_SaveGame_Item Loaded;
	if (FJsonObjectConverter::JsonObjectStringToUStruct<FF_SaveGame_Item>(Json, &Loaded, 0, 0))
		LoadItem(Loaded);
}