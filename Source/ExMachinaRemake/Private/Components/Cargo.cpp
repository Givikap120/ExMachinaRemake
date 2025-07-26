#include "Components/Cargo.h"
#include "Items/Item.h"

TArray<FInventoryItem> UCargo::GetInventoryItems() const
{
    if (!IsValid(Inventory)) return TArray<FInventoryItem>();
    return Inventory->Items;
}

TArray<UItem*> UCargo::GetAllItems() const
{
    TArray<UItem*> Items = UVehiclePart::GetAllItems();

    if (!IsValid(Inventory)) return Items;

    for (const FInventoryItem& InventoryItem : Inventory->Items)
    {
        if (IsValid(InventoryItem.Item))
        {
            Items.Add(InventoryItem.Item);
        }
    }

    return Items;
}

bool UCargo::HasItem(UItem* Item) const
{
    if (UVehiclePart::HasItem(Item)) return true;
    if (IsValid(Inventory)) return Inventory->HasItem(Item);
    return false;
}

void UCargo::RemoveAllItems()
{
    UVehiclePart::RemoveAllItems();
    if (IsValid(Inventory)) return Inventory->Clear();
}

UItem* UCargo::RemoveItemFromInventoryAt(int32 PosX, int32 PosY)
{
    if(!IsValid(Inventory)) return nullptr;
    return Inventory->RemoveItemAt(PosX, PosY);
}

bool UCargo::TryAddItemToInventory(UItem* Item, int32 PosX, int32 PosY)
{
    if (!IsValid(Inventory) || !IsValid(Item)) return false;
    return Inventory->TryPlaceItem(Item, PosX, PosY);
}

bool UCargo::TryDropItemToInventory(UItem* Item)
{
    if (!IsValid(Inventory) || !IsValid(Item)) return false;
    return Inventory->TryDropItemInside(Item);
}

void UCargo::InitComponents()
{
    UVehiclePart::InitComponents();
    InitInventory();
}

void UCargo::InitInventory()
{
    if (!IsValid(Inventory))
    {
        Inventory = NewObject<UInventory>(this);
    }
    
    Inventory->CapacityX = CargoStats.CapacityX;
    Inventory->CapacityY = CargoStats.CapacityY;
}

void UCargo::ValidateCargoInventory()
{
    if (!IsValid(Inventory)) return;
    return Inventory->Validate();
}

void UCargo::SetFromTable(UDataTable* DataTable, FName RowName)
{
    if (!IsValid(DataTable)) return;

    FCargoFullStats* Row = DataTable->FindRow<FCargoFullStats>(RowName, "");
    if (!Row) return;

    VehiclePartStats = Row->PartStats;
    CargoStats = Row->CargoStats;

    InitComponents();
}

void UCargo::CopyTo(UVehiclePart* Target) const
{
    UCargo* TargetCargo = Cast<UCargo>(Target);
    if (!IsValid(TargetCargo)) return;

    TargetCargo->CargoStats = CargoStats;
    UVehiclePart::CopyTo(TargetCargo);

    UInventory* TargetInventory = TargetCargo->GetInventory();
    if (IsValid(TargetInventory)) TargetInventory->Items = GetInventoryItems();
}

FSaveGame_Cargo UCargo::SaveCargo() const
{
    FSaveGame_Cargo SaveData;

    SaveData.VehiclePartSave = SaveVehiclePart();
    SaveData.CargoStats = CargoStats;
    if (IsValid(Inventory)) SaveData.InventorySave =  Inventory->SaveInventory();

    return SaveData;
}

void UCargo::LoadCargo(const FSaveGame_Cargo& Save)
{
    CargoStats = Save.CargoStats;
    LoadVehiclePart(Save.VehiclePartSave);
    if (IsValid(Inventory)) Inventory->LoadInventory(Save.InventorySave);
}