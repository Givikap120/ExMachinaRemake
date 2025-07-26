// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/VehiclePart.h"
#include "Inventory.h"
#include "Cargo.generated.h"

USTRUCT(BlueprintType)
struct FCargoStats : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	int32 CapacityX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	int32 CapacityY;
};

USTRUCT(BlueprintType)
struct FCargoFullStats : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	FVehiclePartStats PartStats;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	FCargoStats CargoStats;
};

USTRUCT(BlueprintType)
struct FSaveGame_Cargo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSaveGame_VehiclePart VehiclePartSave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FF_SaveGame_Inventory InventorySave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCargoStats CargoStats;
};

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EXMACHINAREMAKE_API UCargo : public UVehiclePart
{
	GENERATED_BODY()
	
public:
	inline UInventory* GetInventory() const { return Inventory; }

	UFUNCTION(BlueprintCallable, Category = "Cargo")
	TArray<FInventoryItem> GetInventoryItems() const;

	virtual TArray<UItem*> GetAllItems() const override;
	virtual bool HasItem(UItem* Item) const override;
	virtual void RemoveAllItems() override;

	UFUNCTION(BlueprintCallable, Category = "Cargo")
	UItem* RemoveItemFromInventoryAt(int32 PosX, int32 PosY);

	UFUNCTION(BlueprintCallable, Category = "Cargo")
	bool TryAddItemToInventory(UItem* Item, int32 PosX, int32 PosY);

	UFUNCTION(BlueprintCallable, Category = "Cargo")
	bool TryDropItemToInventory(UItem* Item);

	virtual void InitComponents() override;

	UFUNCTION(BlueprintCallable, Category = "Cargo")
	void InitInventory();

	UFUNCTION(BlueprintCallable, Category = "Cargo")
	void ValidateCargoInventory();

	virtual void SetFromTable(UDataTable* DataTable, FName RowName) override;

	virtual void CopyTo(UVehiclePart* Target) const override;

	UFUNCTION(BlueprintCallable, Category = "Cargo")
	FSaveGame_Cargo SaveCargo() const;

	UFUNCTION(BlueprintCallable, Category = "Cargo")
	void LoadCargo(const FSaveGame_Cargo& Save);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Cargo")
	UInventory* Inventory;

	UPROPERTY(BlueprintReadOnly, Category = "Cargo")
	FCargoStats CargoStats;
};
