// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Templates/SubclassOf.h"
#include "CustomFunctionLibrary.generated.h"

USTRUCT(BlueprintType)
struct FPathfindingNode
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FVector Location;

    int32 X;
    int32 Y;
    float G;
    float H;
    float F() const { return G + H; }
    TSharedPtr<FPathfindingNode> Parent;

    FPathfindingNode() : X(0), Y(0), G(0), H(0) {}
};

USTRUCT(BlueprintType)
struct FPointRow
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    TArray<FVector> Row;
};

/**
 * 
 */
UCLASS()
class EXMACHINAREMAKE_API UCustomFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Pathfinding")
    static bool AStarPathfinding(
        const TArray<FPointRow>& GridPoints,
        const FVector& Start,
        const FVector& End,
        const FVector& CharacterSize,
        float MaxSlopeDegrees,
        TArray<FVector>& OutPath
    );

    UFUNCTION(BlueprintCallable, Category = "Creation")
    static UObject* CreateObject(TSubclassOf<UObject> Class);
};
