// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Slots/WeaponSlot.h"
#include "Items/Weapon.h"
#include "GarageShopItem.h"
#include "Engine/Texture2D.h"
#include "VehiclePart.generated.h"

class UWeaponInstance;

USTRUCT(BlueprintType)
struct FVehiclePartStats : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	FString DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	FName CarName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	float Price;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	float Mass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	float Durability;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	float RepairCoef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	FWeaponSlotStats WeaponSlots;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	TSoftObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	TSoftObjectPtr<UTexture2D> Icon;
};

USTRUCT(BlueprintType)
struct FSaveGame_VehiclePart
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVehiclePartStats VehiclePartStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FF_SaveGame_Weapon WeaponStats;
};

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EXMACHINAREMAKE_API UVehiclePart : public UActorComponent, public IGarageShopItem
{
	GENERATED_BODY()

public:
	UVehiclePart();

	UPROPERTY(BlueprintReadWrite, Category = "Vehicle Part")
	FVehiclePartStats VehiclePartStats;

	UPROPERTY(BlueprintReadWrite, Category = "Vehicle Part")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(BlueprintReadWrite, Category = "Vehicle Part")
	UWeaponInstance* WeaponInstance;

	UPROPERTY(BlueprintReadOnly, Category = "Vehicle Part")
	UWeaponSlot* WeaponSlot;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Vehicle Part")
	TSubclassOf<UWeapon> DefaultWeaponClass = UWeapon::StaticClass();

public:
	UFUNCTION(BlueprintCallable, Category = "Vehicle Part")
	virtual void InitComponents();

	UFUNCTION(BlueprintCallable, Category = "Vehicle Part")
	void InitWeaponSlot();

	UFUNCTION(BlueprintCallable, Category = "Vehicle Part")
	void InitMeshComponent();

	UFUNCTION(BlueprintCallable, Category = "Vehicle Part")
	float GetTotalCost() const;

	UFUNCTION(BlueprintCallable, Category = "Vehicle Part")
	UWeapon* GetWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Vehicle Part")
	virtual TArray<UItem*> GetAllItems() const;

	UFUNCTION(BlueprintCallable, Category = "Vehicle Part")
	virtual bool HasItem(UItem* Item) const;

	UFUNCTION(BlueprintCallable, Category = "Vehicle Part")
	void WeaponsAimTo(const FVector& Location);

	UFUNCTION(BlueprintCallable, Category = "Vehicle Part")
	void UpdateWeaponInstanceFromSlot();

	UFUNCTION(BlueprintCallable, Category = "Vehicle Part")
	UWeapon* ValidateWeaponSlot();

	UFUNCTION(BlueprintCallable, Category = "Vehicle Part")
	virtual void RemoveAllItems();

	UFUNCTION(BlueprintCallable, Category = "Vehicle Part")
	bool TryDropWeaponToSlot(UWeapon* Weapon);

	UFUNCTION(BlueprintCallable, Category = "Vehicle Part")
	FSaveGame_VehiclePart SaveVehiclePart() const;

	UFUNCTION(BlueprintCallable, Category = "Vehicle Part")
	void LoadVehiclePart(const FSaveGame_VehiclePart& Save);

	// Temp function
	UFUNCTION(BlueprintCallable, Category = "Vehicle Part")
	float GetFiringRange() const;

	UFUNCTION(BlueprintCallable, Category = "Vehicle Part")
	virtual void SetFromTable(UDataTable* DataTable, FName RowName) {}

	/// <summary>
	/// Copies this vehicle part stats to other.
	/// WARNING: this DOES NOT binds Target to the owner of this vehicle part.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Vehicle Part")
	virtual void CopyTo(UVehiclePart* Target) const;

	/// <summary>
	/// Creates and copies this vehicle part stats to the new one.
	/// WARNING: this DOES NOT binds new object to the owner of this vehicle part.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Vehicle Part")
	virtual UVehiclePart* Clone() const;

	virtual FString GetDisplayName() const override { return VehiclePartStats.DisplayName; }
	virtual float GetPrice() const override { return VehiclePartStats.Price; }
	virtual UTexture2D* GetIcon() const override { return VehiclePartStats.Icon.LoadSynchronous();	}
};
