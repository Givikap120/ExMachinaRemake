#include "Components/WeaponInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Items/Weapon.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "CustomFunctionLibrary.h"
#include "Animation/AnimInstance.h"
#include "DrawDebugHelpers.h"

UWeaponInstance::UWeaponInstance()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponInstance::TryStartShooting()
{
	if (!IsWeaponValid()) return;
	bIsShooting = true;

	UWorld* World = GetWorld();
	if (!World) return;

	if (ShootTimerHandle.IsValid())
	{
		UCustomFunctionLibrary::Delay(
			this,
			&UWeaponInstance::StartShooting,
			World->GetTimerManager().GetTimerRemaining(ShootTimerHandle)
		);
	}
	else
	{
		StartShooting();
	}
}

void UWeaponInstance::StartShooting()
{
	if (!bCanStartShooting) return;

	bCanStartShooting = false;

	UCustomFunctionLibrary::SetTimer(
		ShootTimerHandle,
		this,
		&UWeaponInstance::Shoot,
		FireDelay,
		true,
		0
	);

	if (EnsureValidAndHasAmmo() && Weapon->IsAuto)
	{
		VisualsStartLoop();
	}
}

void UWeaponInstance::TryStopShooting()
{
	if (!IsWeaponValid()) return;
	bIsShooting = false;

	UWorld* World = GetWorld();
	if (!World) return;

	if (ShootTimerHandle.IsValid())
	{
		UCustomFunctionLibrary::Delay(
			this,
			&UWeaponInstance::StopShooting,
			World->GetTimerManager().GetTimerRemaining(ShootTimerHandle)
		);
	}
	else
	{
		StopShooting();
	}
}

void UWeaponInstance::StopShooting()
{
	bCanStartShooting = true;
	UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, ShootTimerHandle);

	if (IsWeaponValid() && Weapon->IsAuto)
	{
		VisualsEndLoop();
	}
}

void UWeaponInstance::StartReload()
{
	if (!IsWeaponValid() || bIsReloading) return;
	if (Weapon->CurrentCharge >= Weapon->ChargeSize) return;

	bIsReloading = true;

	if (IsValid(SkeletalMeshComponent)) 
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReloadSound, SkeletalMeshComponent->GetComponentLocation());
		VisualsEndLoop();
	}

	UCustomFunctionLibrary::SetTimer(
		ReloadTimerHandle,
		this,
		&UWeaponInstance::FinishReload,
		Weapon->RecharingTime,
		false
	);
}

void UWeaponInstance::FinishReload()
{
	RestoreAmmo();
	bIsReloading = false;

	if (IsWeaponValid() && bIsShooting && Weapon->IsAuto) VisualsStartLoop();
}

void UWeaponInstance::RestoreAmmo()
{
	if (!IsWeaponValid()) return;

	Weapon->CurrentCharge = Weapon->ChargeSize;
	OnAmmoChanged.Broadcast(Weapon->CurrentCharge);
}

void UWeaponInstance::ConsumeAmmo()
{
	if (!IsWeaponValid()) return;

	--Weapon->CurrentCharge;
	OnAmmoChanged.Broadcast(Weapon->CurrentCharge);
}

FHitResult UWeaponInstance::TraceShot() const
{
	FHitResult HitResult;

	if (!IsWeaponValid() || !IsValid(SkeletalMeshComponent)) return HitResult;

	UWorld* World = GetWorld();
	if (!IsValid(World)) return HitResult;

	const FVector Start = SkeletalMeshComponent->GetSocketLocation(TEXT("Gun"));

	FRotator GunRotation = SkeletalMeshComponent->GetSocketRotation(TEXT("Gun"));
	GunRotation = FRotator(GunRotation.Roll - 90.0f, GunRotation.Yaw - 90.0f, 0);

	const FVector End = Start + GunRotation.Vector() * Weapon->FiringRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
	DrawDebugLine(World, Start, End, FColor::Red, false, 1, 0, 1);

	return HitResult;
}

void UWeaponInstance::Shoot()
{
	// Don't shoot if there's no active command to shoot or there's no ammo
	if (!bIsShooting || !EnsureValidAndHasAmmo()) return;

	ConsumeAmmo();

	// Do hit trace
	FHitResult Hit = TraceShot();

	FBarrelSockets Barrels = Weapon->GetUseBarrels();
	for (FName BarrelSocket : Barrels.Particle)
	{
		UGameplayStatics::SpawnEmitterAttached(
			BarrelEffect, 
			SkeletalMeshComponent, 
			BarrelSocket, 
			FVector::ZeroVector, 
			FRotator(0, 90, 90),
			EAttachLocation::SnapToTarget
		);
	}

	if (!Weapon->IsAuto) VisualsPlayOnce();

	AActor* HitActor = Hit.GetActor();
	if (!IsValid(HitActor)) return;

	UGameplayStatics::ApplyDamage(HitActor, Weapon->Damage, nullptr, GetOwner(), UDamageType::StaticClass());

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Hit.ImpactPoint);
}

void UWeaponInstance::InitializeComponent(USkeletalMeshComponent* NewMeshComponent, UAudioComponent* NewAudioComponent, UWeapon* NewWeapon)
{
	SkeletalMeshComponent = NewMeshComponent;
	AudioComponent = NewAudioComponent;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
	AudioComponent->AttachToComponent(SkeletalMeshComponent, AttachmentRules, TEXT("Gun"));

	InitializeFromWeapon(NewWeapon);
}

void UWeaponInstance::InitializeFromWeapon(UWeapon* NewWeapon)
{
	Weapon = NewWeapon;

	if (!IsValid(Weapon))
	{
		DeactivateWeapon();
		return;
	}

	ActivateWeapon();

	// Cabin adjust stats

	FireDelay = 60.0f / Weapon->FiringRate;
	if (IsValid(AudioComponent)) AudioComponent->SetSound(Weapon->FireSound);
	if (IsValid(SkeletalMeshComponent))
	{
		SkeletalMeshComponent->SetSkeletalMesh(Weapon->Model);
		SkeletalMeshComponent->SetAnimInstanceClass(Weapon->Model->PostProcessAnimBlueprint);
	}

	OnAmmoChanged.Broadcast(Weapon->CurrentCharge);
}

void UWeaponInstance::ActivateWeapon()
{
	bIsActive = true;
	if (IsValid(SkeletalMeshComponent)) SkeletalMeshComponent->SetVisibility(true, true);
}

void UWeaponInstance::DeactivateWeapon()
{
	bIsActive = false;
	bIsShooting = false;
	bIsReloading = false;
	VisualsEndLoop();

	UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, ShootTimerHandle);
	UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, ReloadTimerHandle);

	if (IsValid(SkeletalMeshComponent)) SkeletalMeshComponent->SetVisibility(false, true);
}

//void UWeaponInstance::RotateGun(FVector AimPoint, FRotator CarRotation)
//{
//	/*if (!SkeletalMeshComponent) return;
//
//	FVector MuzzleLocation = SkeletalMeshComponent->GetSocketLocation("Muzzle");
//	FVector Direction = (AimPoint - MuzzleLocation).GetSafeNormal();
//	FRotator AimRotation = Direction.Rotation();
//
//	SkeletalMeshComponent->SetWorldRotation(AimRotation + CarRotation);*/
//}

AActor* UWeaponInstance::GetAimTarget() const
{
	return TraceShot().GetActor();
}

bool UWeaponInstance::IsWeaponValid() const
{
	return IsValid(Weapon) && bIsActive;
}

bool UWeaponInstance::EnsureValidAndHasAmmo()
{
	if (!IsWeaponValid() || bIsReloading) return false;

	if (Weapon->CurrentCharge > 0)
	{
		return true;
	}
	else
	{
		StartReload();
		return false;
	}
}