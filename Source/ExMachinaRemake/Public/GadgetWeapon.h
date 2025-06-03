// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gadget.h"
#include <Weapon.h>
#include "GadgetWeapon.generated.h"

USTRUCT(BlueprintType)
struct FGadgetWeaponStats : public FItemStats
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DamageIncrease;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FiringRateIncrease;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FiringRangeIncrease;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RechargingSpeedIncrease;
};

USTRUCT(BlueprintType)
struct FF_SaveGame_GadgetWeapon : public FF_SaveGame_Item
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGadgetWeaponStats GadgetStats;
};


UCLASS()
class EXMACHINAREMAKE_API UGadgetWeapon : public UGadget
{
	GENERATED_BODY()
public:
	virtual EItemType GetItemType() const override { return EItemType::GadgetWeapon; }

	virtual int32 GetSizeX() const override { return 1; }
	virtual int32 GetSizeY() const override { return 2; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGadgetWeaponStats Stats;

	UFUNCTION(BlueprintCallable)
	void ApplyToStats(UPARAM(ref) FWeaponStats& WeaponStats);

	UFUNCTION(BlueprintCallable, Category = "Creation")
	static UGadgetWeapon* CreateGadgetWeaponFromTable(TSubclassOf<UGadgetWeapon> Class, UDataTable* DataTable, FName RowName);

	void SetFromTable(UDataTable* DataTable, FName RowName) override;

	// Save Game
	UFUNCTION(BlueprintPure, Category = "SaveGame")
	FF_SaveGame_GadgetWeapon SaveGadgetWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void LoadGadgetWeapon(const FF_SaveGame_GadgetWeapon& SaveGadgetWeapon);

	virtual FString Serialize() const override;
	virtual void Deserialize(FString Json) override;

};
