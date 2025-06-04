#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "VehiclePartsCache.generated.h"

USTRUCT(BlueprintType)
struct EXMACHINAREMAKE_API FVehiclePartsCacheData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	double MinimalPrice;
};

UCLASS(Blueprintable)
class EXMACHINAREMAKE_API UVehiclePartsCache : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FVehiclePartsCacheData> Data;

	UFUNCTION(BlueprintCallable)
	double GetMinCost() const;
};
