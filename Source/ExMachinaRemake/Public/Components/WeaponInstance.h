#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/HitResult.h"
#include <Sound/SoundBase.h>
#include <Particles/ParticleSystem.h>
#include "WeaponInstance.generated.h"

class USkeletalMeshComponent;
class UAudioComponent;
class UWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32, NewAmmo);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXMACHINAREMAKE_API UWeaponInstance : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponInstance();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void TryStartShooting();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void TryStopShooting();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartReload();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ActivateWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void DeactivateWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AActor* GetAimTarget() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void InitializeComponent(USkeletalMeshComponent* NewMeshComponent, UAudioComponent* NewAudioComponent, UWeapon* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void InitializeFromWeapon(UWeapon* NewWeapon);

	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnAmmoChanged OnAmmoChanged;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	UAudioComponent* AudioComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	UWeapon* Weapon;

	UFUNCTION(BlueprintImplementableEvent)
	void VisualsStartLoop();

	UFUNCTION(BlueprintImplementableEvent)
	void VisualsEndLoop();

	UFUNCTION(BlueprintImplementableEvent)
	void VisualsPlayOnce();

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* HitEffect;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* BarrelEffect;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* ReloadSound;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* HitSound;

private:

	float FireDelay;
	bool bIsShooting;
	bool bIsReloading;

	// Can deactivate weapon completely without removing it's reference
	bool bIsActive; 

	// Used to control if player can start shooting
	bool bCanStartShooting = true;

	FTimerHandle ReloadTimerHandle;
	FTimerHandle ShootTimerHandle;

private:
	void RestoreAmmo();
	void ConsumeAmmo();

	FHitResult TraceShot() const;

	void Shoot();

	void StartShooting();
	void StopShooting();

	void FinishReload();
	
	//void RotateGun(FVector AimPoint, FRotator CarRotation);

	bool IsWeaponValid() const;
	bool EnsureValidAndHasAmmo();

	
};
