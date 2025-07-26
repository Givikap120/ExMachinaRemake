#include "Components/VehiclePart.h"
#include <Items/Weapon.h>
#include "Components/WeaponInstance.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

UVehiclePart::UVehiclePart()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UVehiclePart::InitComponents()
{
	InitMeshComponent();
	InitWeaponSlot();
	UpdateWeaponInstanceFromSlot();
}

void UVehiclePart::InitWeaponSlot()
{
	if (!IsValid(WeaponSlot))
	{
		WeaponSlot = NewObject<UWeaponSlot>(this, TEXT("WeaponSlot"));
		WeaponSlot->OnWeaponChanged.AddDynamic(this, &UVehiclePart::UpdateWeaponInstanceFromSlot);
	}

	WeaponSlot->SetFromStats(VehiclePartStats.WeaponSlots);
}

void UVehiclePart::InitMeshComponent()
{
	if (!IsValid(StaticMeshComponent)) return;

	UStaticMesh* Mesh = VehiclePartStats.Mesh.LoadSynchronous();
	StaticMeshComponent->SetStaticMesh(Mesh);

	if (!IsValid(Mesh)) return;

	const TArray<FStaticMaterial>& Materials = Mesh->GetStaticMaterials();

	for (int32 Index = 0; Index < Materials.Num(); ++Index)
	{
		UMaterialInterface* BaseMaterial = Materials[Index].MaterialInterface;

		if (IsValid(BaseMaterial))
		{
			UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
			StaticMeshComponent->SetMaterial(Index, DynMaterial);
		}
	}
}

float UVehiclePart::GetTotalCost() const
{
	float Cost = VehiclePartStats.Price;

	UWeapon* Weapon = GetWeapon();
	if (IsValid(Weapon)) Cost += Weapon->Price;

	return Cost;
}

UWeapon* UVehiclePart::GetWeapon() const
{
	// We take that weapon slot is the primary source, and assume that weapon instance has the same weapon
	if (IsValid(WeaponSlot)) return WeaponSlot->GetWeapon();
	return nullptr;
}

TArray<UItem*> UVehiclePart::GetAllItems() const
{
	TArray<UItem*> Result;

	UWeapon* Weapon = GetWeapon();
	if (IsValid(Weapon)) Result.Add(Weapon);

	return Result;
}

bool UVehiclePart::HasItem(UItem* Item) const
{
	if (!IsValid(Item)) return false;
	return GetWeapon() == Item;
}

void UVehiclePart::WeaponsAimTo(const FVector& Location)
{
	if (!IsValid(WeaponInstance) || !WeaponInstance->IsWeaponValid()) return;
	WeaponInstance->AimTo(Location);
}

void UVehiclePart::UpdateWeaponInstanceFromSlot()
{
	if (!IsValid(WeaponInstance)) return;
	WeaponInstance->InitializeFromWeapon(GetWeapon());
}

UWeapon* UVehiclePart::ValidateWeaponSlot()
{
	if (!IsValid(WeaponSlot)) return nullptr;
	return WeaponSlot->ValidateWeapon();
}

void UVehiclePart::RemoveAllItems()
{
	if (!IsValid(WeaponSlot)) return;
	WeaponSlot->RemoveWeapon();
}

bool UVehiclePart::TryDropWeaponToSlot(UWeapon* Weapon)
{
	if (!IsValid(WeaponSlot)) return false;
	return WeaponSlot->TryDropWeapon(Weapon);
}

FSaveGame_VehiclePart UVehiclePart::SaveVehiclePart() const
{
	FSaveGame_VehiclePart Result;

	Result.VehiclePartStats = VehiclePartStats;

	if (IsValid(WeaponSlot) && WeaponSlot->HasWeapon())
	{
		Result.WeaponStats = WeaponSlot->GetWeapon()->SaveWeapon();
	}

	return Result;
}

void UVehiclePart::LoadVehiclePart(const FSaveGame_VehiclePart& Save)
{
	VehiclePartStats = Save.VehiclePartStats;
	InitComponents();

	if (!IsValid(WeaponSlot)) return;

	if (Save.WeaponStats.Name.IsNone())
	{
		WeaponSlot->TrySetWeapon(nullptr);
	}
	else
	{
		UWeapon* Weapon = NewObject<UWeapon>(this, DefaultWeaponClass);
		Weapon->LoadWeapon(Save.WeaponStats);
		WeaponSlot->TrySetWeapon(Weapon);
	}
}

float UVehiclePart::GetFiringRange() const
{
	if (!IsValid(WeaponSlot) || !WeaponSlot->HasWeapon()) return 0;
	return WeaponSlot->GetWeapon()->FiringRange;
}

void UVehiclePart::CopyTo(UVehiclePart* Target) const
{
	if (!IsValid(Target)) return;

	Target->VehiclePartStats = VehiclePartStats;
	Target->InitComponents();

	if (!IsValid(WeaponSlot) || !IsValid(Target->WeaponSlot)) return;

	Target->WeaponSlot->TrySetWeapon(WeaponSlot->GetWeapon());
}

UVehiclePart* UVehiclePart::Clone() const
{
	UVehiclePart* Result = NewObject<UVehiclePart>(GetOuter(), GetClass());
	CopyTo(Result);
	return Result;
}
