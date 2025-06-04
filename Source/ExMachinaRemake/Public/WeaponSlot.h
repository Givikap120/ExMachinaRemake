#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.h"
#include "WeaponSlot.generated.h"

/** Stats of the weapon to make a Table */
USTRUCT(BlueprintType)
struct FWeaponSlotStats : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
		EWeaponType SlotType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
		float PosX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
		float PosY;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true", MakeStructureDefaultValue = "None"))
		FName SocketName;
};

UCLASS()
class EXMACHINAREMAKE_API UWeaponSlot : public UObject
{
	GENERATED_BODY()
	
public:	
	UWeaponSlot();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
		EWeaponType SlotType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
		float PosX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
		float PosY;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true", MakeStructureDefaultValue = "None"))
		FName SocketName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
		TObjectPtr<UWeapon> WeaponPtr;

	UFUNCTION(BlueprintPure)
		void GetSize(int32& SizeX, int32& SizeY) const;

	UFUNCTION(BlueprintCallable)
		void SetFromTable(UDataTable* DataTable, FName RowName);

	UFUNCTION(BlueprintCallable)
		static UWeaponSlot* CreateWeaponSlotFromTable(TSubclassOf<UWeaponSlot> Class, UDataTable* DataTable, FName RowName);
};
