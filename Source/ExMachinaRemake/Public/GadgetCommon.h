// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gadget.h"
#include "GadgetCommon.generated.h"

USTRUCT(BlueprintType)
struct FGadgetCommonStats : public FItemStats
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxSpeedIncrease;
};

USTRUCT(BlueprintType)
struct FF_SaveGame_GadgetCommon : public FF_SaveGame_Item
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGadgetCommonStats GadgetStats;
};

UCLASS()
class EXMACHINAREMAKE_API UGadgetCommon : public UGadget
{
	GENERATED_BODY()
public:
	virtual EItemType GetItemType() const override { return EItemType::GadgetCommon; }

	virtual int32 GetSizeX() const override { return 1; }
	virtual int32 GetSizeY() const override { return 1; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxSpeedMultiplier;

	UFUNCTION(BlueprintCallable, Category="Creation")
	static UGadgetCommon* CreateGadgetCommonFromTable(TSubclassOf<UGadgetCommon> Class, UDataTable* DataTable, FName RowName);

	void SetFromTable(UDataTable* DataTable, FName RowName) override;

	// Save Game

	UFUNCTION(BlueprintPure, Category = "SaveGame")
	FF_SaveGame_GadgetCommon SaveGadgetCommon() const;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void LoadGadgetCommon(const FF_SaveGame_GadgetCommon& SaveGadgetCommon);

	virtual FString Serialize() const override;
	virtual void Deserialize(FString Json) override;

};
