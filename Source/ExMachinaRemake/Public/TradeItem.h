// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "TradeItem.generated.h"

/**
 * 
 */
UCLASS()
class EXMACHINAREMAKE_API UTradeItem : public UItem
{
	GENERATED_BODY()
public:
	virtual EItemType GetItemType() const override { return EItemType::TradeItem; }

	virtual int32 GetSizeX() const override { return 2; }
	virtual int32 GetSizeY() const override { return 2; }

	UFUNCTION(BlueprintCallable, Category = "Creation")
		static UTradeItem* CreateTradeItemFromTable(TSubclassOf<UTradeItem> Class, UDataTable* DataTable, FName RowName);
};
