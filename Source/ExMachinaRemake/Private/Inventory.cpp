// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

int32 FInventoryItem::GetEndX() const
{
    return X + Item->GetSizeX() - 1;
}

int32 FInventoryItem::GetEndY() const
{
    return Y + Item->GetSizeY() - 1;
}

bool UInventory::IsValidPosition(const UItem* Item, int32 StartX, int32 StartY, const UItem* Except) const
{
    if (!IsValid(Item)) return false;

    int32 EndX = StartX + Item->GetSizeX() - 1;
    int32 EndY = StartY + Item->GetSizeY() - 1;

    if (EndX >= CapacityX || EndY >= CapacityY) return false;

    for (const FInventoryItem& InventoryItem : Items)
    {
        if (InventoryItem.Item == Except) continue;
        
        int32 InventoryItemEndX = InventoryItem.GetEndX();
        int32 InventoryItemEndY = InventoryItem.GetEndY();

        bool bRight = StartX > InventoryItemEndX;
        bool bDown = StartY > InventoryItemEndY;
        bool bLeft = InventoryItem.X > EndX;
        bool bUp = InventoryItem.Y > EndY;

        if ((bRight || bDown || bLeft || bUp) == false) return false;
    }

    return true;
}

bool UInventory::TryDropItemInside(UItem* Item, FInventoryItem& OutPlacedItem)
{
    if (!IsValid(Item)) return false;

    for (int32 i = 0; i < CapacityY; i++)
    {
        for (int32 j = 0; j < CapacityX; j++)
        {
            bool bResult = TryPlaceItem(Item, j, i, OutPlacedItem);
            if (bResult) return true;
        }
    }

    return false;
}

bool UInventory::TryPlaceItem(UItem* Item, int32 X, int32 Y, FInventoryItem& OutPlacedItem)
{
    if (!IsValidPosition(Item, X, Y)) return false;

    OutPlacedItem = FInventoryItem{ Item, X, Y };
    Items.Add(OutPlacedItem);

    OnItemAdded.Broadcast(OutPlacedItem);
    OnInventoryChanged.Broadcast();

    return true;
}

UItem* UInventory::RemoveItemAt(int32 X, int32 Y)
{
    for (int32 i = 0; i < Items.Num(); ++i)
    {
        FInventoryItem& Item = Items[i];
        if (Item.X != X || Item.Y != Y) continue;

        UItem* DeletedItem = Item.Item;
        Items.RemoveAt(i);
        OnInventoryChanged.Broadcast();

        return DeletedItem;
    }

    return nullptr;
}

UItem* UInventory::RemoveItem(int32 Index)
{
    UItem* Item = Items[Index].Item;
    Items.RemoveAt(Index);
    OnInventoryChanged.Broadcast();

    return Item;
}

void UInventory::Validate()
{
    for (int32 i = Items.Num() - 1; i >= 0; --i)
    {
        FInventoryItem& Item = Items[i];
        bool bValid = IsValidPosition(Item.Item, Item.X, Item.Y, Item.Item);
        if (bValid) continue;

        UItem* RemovedItem = RemoveItem(i);

        FInventoryItem Dummy;

        // Try readd
        TryDropItemInside(RemovedItem, Dummy);
    }
}

void UInventory::Clear()
{
    Items.Empty();
    OnInventoryChanged.Broadcast();
}

bool UInventory::HasItem(const UItem* Item) const
{
    if (!IsValid(Item)) return false;

    for (const FInventoryItem& InventoryItem : Items)
    {
        if (InventoryItem.Item == Item) return true;
    }

    return false;
}

FF_SaveGame_Inventory UInventory::SaveInventory() const
{
    FF_SaveGame_Inventory SaveData;
    SaveData.CapacityX = CapacityX;
    SaveData.CapacityY = CapacityY;

    for (const FInventoryItem& InvItem : Items)
    {
        if (!IsValid(InvItem.Item)) continue;

        FF_SaveGame_InventoryItem SaveItem;

        SaveItem.EncodedItem = InvItem.Item->Serialize();
        SaveItem.EncodedClass = InvItem.Item->GetClass()->GetPathName();

        SaveItem.X = InvItem.X;
        SaveItem.Y = InvItem.Y;

        SaveData.EncodedItems.Add(SaveItem);
    }

    return SaveData;
}


void UInventory::LoadInventory(const FF_SaveGame_Inventory& SaveData)
{
    CapacityX = SaveData.CapacityX;
    CapacityY = SaveData.CapacityY;

    Items.Empty();

    for (const FF_SaveGame_InventoryItem& SaveItem : SaveData.EncodedItems)
    {
        FInventoryItem InvItem;
        InvItem.X = SaveItem.X;
        InvItem.Y = SaveItem.Y;

        if (SaveItem.EncodedItem.IsEmpty() || SaveItem.EncodedClass.IsEmpty())
            continue;
        
        UClass* ItemClass = LoadObject<UClass>(nullptr, *SaveItem.EncodedClass);
        if (!ItemClass)
            continue;

        InvItem.Item = NewObject<UItem>(this, ItemClass);

        if (!InvItem.Item)
            continue;

        InvItem.Item->Deserialize(SaveItem.EncodedItem);
        Items.Add(InvItem);
    }

    OnInventoryChanged.Broadcast();
}
