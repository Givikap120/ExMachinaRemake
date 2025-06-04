#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.h"
#include "Inventory.generated.h"

USTRUCT(BlueprintType)
struct EXMACHINAREMAKE_API FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItem* Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y;

	int32 GetEndX() const;
	int32 GetEndY() const;
};

USTRUCT(BlueprintType)
struct EXMACHINAREMAKE_API FF_SaveGame_InventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EncodedClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EncodedItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y;
};

USTRUCT(BlueprintType)
struct EXMACHINAREMAKE_API FF_SaveGame_Inventory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CapacityX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CapacityY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FF_SaveGame_InventoryItem> EncodedItems;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAddedSignature, const FInventoryItem&, InventoryItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChangedSignature);

UCLASS(BlueprintType)
class EXMACHINAREMAKE_API UInventory : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventoryItem> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CapacityX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CapacityY;

	UFUNCTION(BlueprintCallable)
	bool TryDropItemInside(UItem* Item, FInventoryItem& OutPlacedItem);

	UFUNCTION(BlueprintCallable)
	bool TryPlaceItem(UItem* Item, int32 X, int32 Y, FInventoryItem& OutPlacedItem);

	UFUNCTION(BlueprintCallable)
	UItem* RemoveItemAt(int32 X, int32 Y);

	UFUNCTION(BlueprintCallable)
	UItem* RemoveItem(int32 Index);

	UFUNCTION(BlueprintCallable)
	void Validate();

	UFUNCTION(BlueprintCallable)
	void Clear();

	UFUNCTION(BlueprintPure)
	bool HasItem(const UItem* Item) const;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryChangedSignature OnInventoryChanged;

	UPROPERTY(BlueprintAssignable)
	FOnItemAddedSignature OnItemAdded;

	// Save Game

	UFUNCTION(BlueprintPure, Category = "SaveGame")
	FF_SaveGame_Inventory SaveInventory() const;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void LoadInventory(const FF_SaveGame_Inventory& SaveData);

private:
	bool IsValidPosition(const UItem* Item, int32 X, int32 Y, const UItem* Except = nullptr) const;
};
