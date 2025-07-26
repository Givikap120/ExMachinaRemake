// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/VehiclePart.h"
#include "Items/GadgetCommon.h"
#include "Items/GadgetWeapon.h"
#include "Cabin.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGadgetCommonChangedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGadgetWeaponChangedSignature);

USTRUCT(BlueprintType)
struct FCabinStats : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	float MaxSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	float MaxTorque;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	int GadgetCommonSlots;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	int GadgetWeaponSlots;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	TSoftObjectPtr<USoundWave> EngineSound;
};

USTRUCT(BlueprintType)
struct FCabinFullStats : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	FVehiclePartStats PartStats;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	FCabinStats CabinStats;
};

USTRUCT(BlueprintType)
struct FSaveGame_Cabin
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSaveGame_VehiclePart VehiclePartSave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCabinStats CabinStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FF_SaveGame_GadgetCommon> GadgetCommonSaves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FF_SaveGame_GadgetWeapon> GadgetWeaponSaves;
};

class UAudioComponent;
class UGadgetSlot;

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EXMACHINAREMAKE_API UCabin : public UVehiclePart
{
	GENERATED_BODY()
	
public:
	virtual TArray<UItem*> GetAllItems() const override;
	virtual bool HasItem(UItem* Item) const override;
	virtual void RemoveAllItems() override;

	virtual void InitComponents() override;

	UFUNCTION(BlueprintCallable, Category = "Cabin")
	void InitGadgetSlots();

	UFUNCTION(BlueprintCallable, Category = "Cabin")
	float GetMaxSpeedMps() const;

	UFUNCTION(BlueprintCallable, Category = "Cabin")
	FWeaponStats AdjustWeaponStats(FWeaponStats Stats) const;

	virtual void SetFromTable(UDataTable* DataTable, FName RowName) override;

	virtual void CopyTo(UVehiclePart* Target) const override;

	UFUNCTION(BlueprintCallable, Category = "Cabin")
	FSaveGame_Cabin SaveCabin() const;

	UFUNCTION(BlueprintCallable, Category = "Cabin")
	void LoadCabin(const FSaveGame_Cabin& Save);

public:
	UPROPERTY(BlueprintAssignable, Category = "Cabin")
	FOnGadgetCommonChangedSignature OnGadgetCommonChanged;

	UPROPERTY(BlueprintAssignable, Category = "Cabin")
	FOnGadgetWeaponChangedSignature OnGadgetWeaponChanged;

	UPROPERTY(BlueprintReadWrite, Category = "Cabin")
	UAudioComponent* GasSoundComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Cabin")
	TArray<UGadgetSlot*> GadgetCommonSlots;

	UPROPERTY(BlueprintReadOnly, Category = "Cabin")
	TArray<UGadgetSlot*> GadgetWeaponSlots;

	UPROPERTY(BlueprintReadWrite, Category = "Cabin")
	FCabinStats CabinStats;

protected:
	float AdjustSpeedWithGadgets(float Speed) const;

private:
	void ResizeGadgetSlots(TArray<UGadgetSlot*>& Slots, int32 NewSize, EItemType GadgetType);

	UFUNCTION()
	void BroadcastGadgetCommonChanged() { OnGadgetCommonChanged.Broadcast(); }

	UFUNCTION()
	void BroadcastGadgetWeaponChanged() { OnGadgetWeaponChanged.Broadcast(); }
};
