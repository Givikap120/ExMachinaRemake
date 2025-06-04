// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Engine/SkeletalMesh.h"
#include <Sound/SoundWave.h>
#include "Weapon.generated.h"

/** Type of the gun by size in inventory */
UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	None,
	SmallGun,
	BigGun,
	GiantGun,
	SideGun,
	Special
};

/** Type of the gun by damage type */
UENUM(BlueprintType)
enum class EDamageType : uint8 {
	None,
	Bullet,
	Laser,
	Blast
};

/** Type of the barrel: all at once or alternate */
UENUM(BlueprintType)
enum class EBarrelType : uint8 {
	Normal,
	Alternate
};

/** Stats of the weapon to make a Table */
USTRUCT(BlueprintType)
struct FWeaponStats : public FItemStats
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EWeaponType WeaponType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EDamageType DamageType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		double FiringRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		double FiringRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 ChargeSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float RecharingTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Durability;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsAuto;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FName> Barrels;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EBarrelType BarrelType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TObjectPtr<USoundWave> FireSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TObjectPtr<USoundWave> TailSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TObjectPtr<USkeletalMesh> Model;
};

/** Socket names for fire algorithm */
USTRUCT(BlueprintType)
struct FBarrelSockets
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
		FName Trace;

	UPROPERTY(BlueprintReadOnly)
		TArray<FName> Particle;

	FBarrelSockets() : Trace(""), Particle({}) {}
	FBarrelSockets(FName _Trace, TArray<FName> _Particle) : Trace(_Trace), Particle(_Particle) {}
};

USTRUCT(BlueprintType)
struct FF_SaveGame_Weapon : public FF_SaveGame_Item
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FWeaponStats WeaponStats;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 CurrentCharge;
};

UCLASS(Abstract)
class EXMACHINAREMAKE_API UWeapon : public UItem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FWeaponStats BaseStats;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MakeStructureDefaultValue = "0"))
		int32 CurrentCharge;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MakeStructureDefaultValue = "0"))
		int32 Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EWeaponType WeaponType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MakeStructureDefaultValue = "0.000000"))
		double FiringRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MakeStructureDefaultValue = "0"))
		double FiringRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MakeStructureDefaultValue = "0"))
		int32 ChargeSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MakeStructureDefaultValue = "0.000000"))
		float RecharingTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MakeStructureDefaultValue = "False"))
		bool IsAuto;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FName> Barrels;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EBarrelType BarrelType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MakeStructureDefaultValue = "None"))
		TObjectPtr<USoundBase> FireSound;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MakeStructureDefaultValue = "None"))
		TObjectPtr<USoundBase> TailSound;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MakeStructureDefaultValue = "None"))
		TObjectPtr<USkeletalMesh> Model;

private:
	int32 CurrentBarrel = -1; // used if alternate barrel weapon

public:
	UFUNCTION(BlueprintCallable)
		FName GeTraceBarrel() const;

	UFUNCTION(BlueprintCallable)
		TArray<FName> GetParticleBarrels() const;

	UFUNCTION(BlueprintCallable)
		FBarrelSockets GetUseBarrels();

	UFUNCTION(BlueprintCallable)
		void ApplyStats(const FWeaponStats& Stats);

	UFUNCTION(BlueprintCallable)
		static void GetSizeForWeaponType(EWeaponType Type, int32& SizeX, int32& SizeY);

	UFUNCTION(BlueprintCallable)
		static int32 GetSizeXForWeaponType(EWeaponType Type);

	UFUNCTION(BlueprintCallable)
		static int32 GetSizeYForWeaponType(EWeaponType Type);

	UFUNCTION(BlueprintCallable, Category = "Creation")
		static UWeapon* CreateWeaponFromTable(TSubclassOf<UWeapon> Class, UDataTable* DataTable, FName RowName);

	virtual EItemType GetItemType() const override { return EItemType::Weapon; }
	void SetFromTable(UDataTable* DataTable, FName RowName) override;

	virtual int32 GetSizeX() const override;
	virtual int32 GetSizeY() const override;

	// Save Game

	UFUNCTION(BlueprintPure, Category = "SaveGame")
	FF_SaveGame_Weapon SaveWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void LoadWeapon(const FF_SaveGame_Weapon& SaveWeapon);

	virtual FString Serialize() const override;
	virtual void Deserialize(FString Json) override;
};
