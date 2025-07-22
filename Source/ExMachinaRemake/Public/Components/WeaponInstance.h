#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/HitResult.h"
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
	// Sets default values for this component's properties
	UWeaponInstance();

	void TryStartShooting();
	void TryStopShooting();

	void StartReload();

	void DestroyWeapon();
	AActor* GetAimTarget() const;

	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnAmmoChanged OnAmmoChanged;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* HitEffect;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* ReloadSound;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* HitSound;

	UFUNCTION(BlueprintImplementableEvent)
	void VisualsStartLoop();

	UFUNCTION(BlueprintImplementableEvent)
	void VisualsEndLoop();

	UFUNCTION(BlueprintImplementableEvent)
	void VisualsPlayOnce();

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	UAudioComponent* AudioComponent;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	UWeapon* Weapon;

	float FireDelay;
	bool bIsShooting;
	bool bIsReloading;

	// Can deactivate weapon completely without removing it's reference
	bool bIsActive; 

	// Used to control if player can start shooting
	bool bCanStartShooting;

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

	void InitializeComponent(USkeletalMeshComponent* NewMeshComponent, UAudioComponent* NewAudioComponent, UWeapon* NewWeapon);
	void InitializeFromWeapon(UWeapon* NewWeapon);

	void ActivateWeapon();
	void DeactivateWeapon();
	
	//void RotateGun(FVector AimPoint, FRotator CarRotation);

	bool IsWeaponValid() const;
	bool EnsureValidAndHasAmmo();

	
};
