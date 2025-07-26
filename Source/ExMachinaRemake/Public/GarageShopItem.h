#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GarageShopItem.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
class UGarageShopItem : public UInterface
{
	GENERATED_BODY()
};

class UTexture2D;

class EXMACHINAREMAKE_API IGarageShopItem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Garage Shop Item")
	virtual FString GetDisplayName() const = 0;

	UFUNCTION(BlueprintCallable, Category = "Garage Shop Item")
	virtual float GetPrice() const = 0;

	UFUNCTION(BlueprintCallable, Category = "Garage Shop Item")
	virtual UTexture2D* GetIcon() const = 0;
};
