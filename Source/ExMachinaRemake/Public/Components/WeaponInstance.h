#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/HitResult.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "Items/Weapon.h"
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
	void AimTo(const FVector& TargetLocation);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AActor* GetAimTarget() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void InitializeComponent(USkeletalMeshComponent* NewMeshComponent, UAudioComponent* NewAudioComponent, UWeapon* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void InitializeFromWeapon(UWeapon* NewWeapon);

	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnAmmoChanged OnAmmoChanged;

	inline bool IsWeaponValid() const { return IsValid(Weapon) && bIsActive; }
	inline UWeapon* GetWeapon() const { return Weapon; }

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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

	UFUNCTION(BlueprintImplementableEvent)
	void RotateGunMesh(float Yaw, float Pitch);

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RotationSpeed = 180.0f;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* HitEffect;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* BarrelEffect;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* ReloadSound;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* HitSound;

protected:
	void RestoreAmmo();
	void ConsumeAmmo();

	FHitResult TraceShot() const;

	void Shoot();

	void StartShooting();
	void StopShooting();

	void FinishReload();

	bool EnsureValidAndHasAmmo();

	bool bIsShooting;
	bool bIsReloading;

	// Can deactivate weapon completely without removing it's reference
	bool bIsActive;

private:

	float FireDelay;

	FVector AimTargetLocation;

	float CurrentYaw = 0;
	float CurrentPitch = 0;

	float DesiredYaw = 0;
	float DesiredPitch = 0;

	FTimerHandle ReloadTimerHandle;
	FTimerHandle ShootTimerHandle;
};
