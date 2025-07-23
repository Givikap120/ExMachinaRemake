#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/Weapon.h"
#include "WeaponSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponChanged);

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

UCLASS(Blueprintable)
class EXMACHINAREMAKE_API UWeaponSlot : public UObject
{
	GENERATED_BODY()
	
public:	
	inline UWeapon* GetWeapon() const { return Weapon; }

	UFUNCTION(BlueprintPure)
	void GetSize(int32& SizeX, int32& SizeY) const;

	UFUNCTION(BlueprintCallable)
	bool HasWeapon() const { return IsValid(Weapon); }

	UFUNCTION(BlueprintCallable)
	bool TrySetWeapon(UWeapon* NewWeapon);

	UFUNCTION(BlueprintCallable)
	bool TryDropWeapon(UWeapon* NewWeapon);

	UFUNCTION(BlueprintCallable)
	UWeapon* RemoveWeapon();

	UFUNCTION(BlueprintCallable)
	void SetFromTable(UDataTable* DataTable, FName RowName);

	UFUNCTION(BlueprintCallable)
	static UWeaponSlot* CreateWeaponSlotFromTable(TSubclassOf<UWeaponSlot> Class, UDataTable* DataTable, FName RowName);

	UPROPERTY(BlueprintAssignable)
	FOnWeaponChanged OnWeaponChanged;

	UFUNCTION(BlueprintCallable)
	void SetFromStats(FWeaponSlotStats Stats);

	UFUNCTION(BlueprintCallable)
	UWeapon* ValidateWeapon();

	UFUNCTION(BlueprintCallable)
	bool IsValidSlot() const;

protected:
	void SetWeapon(UWeapon* NewWeapon);

	UPROPERTY(BlueprintReadOnly)
		EWeaponType SlotType;

	UPROPERTY(BlueprintReadOnly)
		float PosX;

	UPROPERTY(BlueprintReadOnly)
		float PosY;

	UPROPERTY(BlueprintReadOnly)
		FName SocketName;

	UPROPERTY(BlueprintReadOnly)
		UWeapon* Weapon;
};
