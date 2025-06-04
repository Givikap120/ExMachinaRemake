#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8 {
	None,
	TradeItem,
	Weapon,
	GadgetCommon,
	GadgetWeapon
};

USTRUCT(BlueprintType)
struct FItemStats : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		double Price;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TObjectPtr<UTexture2D> Icon;
};

USTRUCT(BlueprintType)
struct FF_SaveGame_Item
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	double Price;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture2D> Icon;
};

UCLASS(Abstract, Blueprintable)
class EXMACHINAREMAKE_API UItem : public UObject
{
	GENERATED_BODY()
	
public:	
	UItem();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		double Price;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TObjectPtr<UTexture2D> Icon;

	UFUNCTION(BlueprintPure)
		void GetSize(int32& SizeX, int32& SizeY) const;

	UFUNCTION(BlueprintPure)
	virtual EItemType GetItemType() const { return EItemType::None; }

	UFUNCTION(BlueprintPure)
	virtual int32 GetSizeX() const PURE_VIRTUAL(UItem::GetSizeX, return 0;);

	UFUNCTION(BlueprintPure)
	virtual int32 GetSizeY() const PURE_VIRTUAL(UItem::GetSizeY, return 0;);

	UFUNCTION(BlueprintCallable)
	virtual void SetFromTable(UDataTable* DataTable, FName RowName);

	// Save Game

	UFUNCTION(BlueprintPure, Category = "SaveGame")
	FF_SaveGame_Item SaveItem() const;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void LoadItem(const FF_SaveGame_Item& SaveItem);

	UFUNCTION(BlueprintPure, Category = "SaveGame")
	virtual FString Serialize() const;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	virtual void Deserialize(FString Json);
};

