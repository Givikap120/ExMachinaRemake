#include "Components/Cabin.h"
#include "Slots/GadgetSlot.h"
#include "Components/AudioComponent.h"
#include "Engine/DataTable.h"

TArray<UItem*> UCabin::GetAllItems() const
{
	TArray<UItem*> Items = UVehiclePart::GetAllItems();

	for (UGadgetSlot* GadgetSlot : GadgetCommonSlots)
	{
		if (IsValid(GadgetSlot) && IsValid(GadgetSlot->Gadget))
		{
			Items.Add(GadgetSlot->Gadget);
		}
	}

	for (UGadgetSlot* GadgetSlot : GadgetWeaponSlots)
	{
		if (IsValid(GadgetSlot) && IsValid(GadgetSlot->Gadget))
		{
			Items.Add(GadgetSlot->Gadget);
		}
	}

	return Items;
}

bool UCabin::HasItem(UItem* Item) const
{
	if (!IsValid(Item)) return false;
	if (UVehiclePart::HasItem(Item)) return true;

	for (UGadgetSlot* GadgetSlot : GadgetCommonSlots)
	{
		if (IsValid(GadgetSlot) && GadgetSlot->Gadget == Item) return true;
	}

	for (UGadgetSlot* GadgetSlot : GadgetWeaponSlots)
	{
		if (IsValid(GadgetSlot) && GadgetSlot->Gadget == Item) return true;
	}

	return false;
}

void UCabin::RemoveAllItems()
{
	UVehiclePart::RemoveAllItems();

	for (UGadgetSlot* GadgetSlot : GadgetCommonSlots)
	{
		if (IsValid(GadgetSlot)) GadgetSlot->TrySetGadget(nullptr);
	}

	for (UGadgetSlot* GadgetSlot : GadgetWeaponSlots)
	{
		if (IsValid(GadgetSlot)) GadgetSlot->TrySetGadget(nullptr);
	}
}

void UCabin::InitComponents()
{
	UVehiclePart::InitComponents();
	InitGadgetSlots();

	if (IsValid(GasSoundComponent))
	{
		USoundWave* EngineSound = CabinStats.EngineSound.LoadSynchronous();
		if (IsValid(EngineSound)) GasSoundComponent->SetWaveParameter(TEXT("EngineSound"), EngineSound);
	}
}

float UCabin::GetMaxSpeedMps() const
{
	float BaseSpeed = CabinStats.MaxSpeed / 0.036f;
	return AdjustSpeedWithGadgets(BaseSpeed);
}

FWeaponStats UCabin::AdjustWeaponStats(FWeaponStats Stats) const
{
	for (UGadgetSlot* GadgetSlot : GadgetWeaponSlots)
	{
		if (!IsValid(GadgetSlot)) continue;

		UGadgetWeapon* GadgetWeapon = Cast<UGadgetWeapon>(GadgetSlot->Gadget);
		if (!IsValid(GadgetWeapon)) continue;

		GadgetWeapon->ApplyToStats(Stats);
	}

	return Stats;
}

void UCabin::SetFromTable(UDataTable* DataTable, FName RowName)
{
	if (!IsValid(DataTable)) return;

	FCabinFullStats* Row = DataTable->FindRow<FCabinFullStats>(RowName, "");
	if (!Row) return;

	VehiclePartStats = Row->PartStats;
	CabinStats = Row->CabinStats;

	InitComponents();
}

void CopyGadgets(const TArray<UGadgetSlot*>& From, TArray<UGadgetSlot*>& To)
{
	int32 GadgetsCount = FMath::Min(From.Num(), To.Num());

	for (int32 i = 0; i < GadgetsCount; ++i)
	{
		if (!IsValid(From[i]) || !IsValid(To[i])) continue;
		To[i]->TrySetGadget(From[i]->Gadget);
	}
}

void UCabin::CopyTo(UVehiclePart* Target) const
{
	UCabin* TargetCabin = Cast<UCabin>(Target);
	if (!IsValid(TargetCabin)) return;

	TargetCabin->CabinStats = CabinStats;
	UVehiclePart::CopyTo(TargetCabin);

	CopyGadgets(GadgetCommonSlots, TargetCabin->GadgetCommonSlots);
	CopyGadgets(GadgetWeaponSlots, TargetCabin->GadgetWeaponSlots);
}

FSaveGame_Cabin UCabin::SaveCabin() const
{
	TArray<FF_SaveGame_GadgetCommon> GadgetCommonSaves;

	for (UGadgetSlot* GadgetSlot : GadgetCommonSlots)
	{
		if (!IsValid(GadgetSlot)) continue;
		
		UGadgetCommon* GadgetCommon = Cast<UGadgetCommon>(GadgetSlot->Gadget);
		if (IsValid(GadgetCommon))
		{
			GadgetCommonSaves.Add(GadgetCommon->SaveGadget());
		}
		else
		{
			GadgetCommonSaves.Add(FF_SaveGame_GadgetCommon());
		}
	}

	TArray<FF_SaveGame_GadgetWeapon> GadgetWeaponSaves;

	for (UGadgetSlot* GadgetSlot : GadgetWeaponSlots)
	{
		if (!IsValid(GadgetSlot)) continue;

		UGadgetWeapon* GadgetWeapon = Cast<UGadgetWeapon>(GadgetSlot->Gadget);
		if (IsValid(GadgetWeapon))
		{
			GadgetWeaponSaves.Add(GadgetWeapon->SaveGadget());
		}
		else
		{
			GadgetWeaponSaves.Add(FF_SaveGame_GadgetWeapon());
		}
	}

	FSaveGame_Cabin SaveData;

	SaveData.VehiclePartSave = SaveVehiclePart();
	SaveData.CabinStats = CabinStats;
	SaveData.GadgetCommonSaves = GadgetCommonSaves;
	SaveData.GadgetWeaponSaves = GadgetWeaponSaves;

	return SaveData;
}

void UCabin::LoadCabin(const FSaveGame_Cabin& Save)
{
	CabinStats = Save.CabinStats;
	LoadVehiclePart(Save.VehiclePartSave);
	
	ensure(Save.GadgetCommonSaves.Num() == GadgetCommonSlots.Num());
	int32 GadgetCommonSlotsCount = FMath::Min(Save.GadgetCommonSaves.Num(), GadgetCommonSlots.Num());

	for (int32 i = 0; i < GadgetCommonSlotsCount; ++i)
	{
		UGadgetSlot* GadgetSlot = GadgetCommonSlots[i];
		if (!IsValid(GadgetSlot)) continue;

		if (!Save.GadgetCommonSaves[i].Name.IsNone())
		{
			UGadgetCommon* NewGadget = NewObject<UGadgetCommon>(this);
			NewGadget->LoadGadget(Save.GadgetCommonSaves[i]);
			GadgetSlot->TrySetGadget(NewGadget);
		}
		else
		{
			GadgetSlot->TrySetGadget(nullptr);
		}
	}

	ensure(Save.GadgetWeaponSaves.Num() == GadgetWeaponSlots.Num());
	int32 GadgetWeaponSlotsCount = FMath::Min(Save.GadgetWeaponSaves.Num(), GadgetWeaponSlots.Num());

	for (int32 i = 0; i < GadgetWeaponSlotsCount; ++i)
	{
		UGadgetSlot* GadgetSlot = GadgetWeaponSlots[i];
		if (!IsValid(GadgetSlot)) continue;

		if (!Save.GadgetWeaponSaves[i].Name.IsNone())
		{
			UGadgetWeapon* NewGadget = NewObject<UGadgetWeapon>(this);
			NewGadget->LoadGadget(Save.GadgetWeaponSaves[i]);
			GadgetSlot->TrySetGadget(NewGadget);
		}
		else
		{
			GadgetSlot->TrySetGadget(nullptr);
		}
	}
}

float UCabin::AdjustSpeedWithGadgets(float Speed) const
{
	float Result = Speed;

	for (UGadgetSlot* GadgetSlot : GadgetCommonSlots)
	{
		if (!IsValid(GadgetSlot)) continue;

		UGadgetCommon* GadgetCommon = Cast<UGadgetCommon>(GadgetSlot->Gadget);
		if (IsValid(GadgetCommon)) Result *= GadgetCommon->MaxSpeedMultiplier;
	}

	return Result;
}

void UCabin::InitGadgetSlots()
{
	ResizeGadgetSlots(GadgetCommonSlots, CabinStats.GadgetCommonSlots, EItemType::GadgetCommon);
	ResizeGadgetSlots(GadgetWeaponSlots, CabinStats.GadgetWeaponSlots, EItemType::GadgetWeapon);
}

void UCabin::ResizeGadgetSlots(TArray<UGadgetSlot*>& Slots, int32 NewSize, EItemType GadgetType)
{
	if (Slots.Num() == NewSize) return;

	check(GadgetType == EItemType::GadgetCommon || GadgetType == EItemType::GadgetWeapon);

	if (Slots.Num() > NewSize)
	{
		for (int32 i = NewSize; i < Slots.Num(); ++i)
		{
			UGadgetSlot* Slot = Slots[i];
			if (!IsValid(Slot)) continue;

			if (GadgetType == EItemType::GadgetCommon)
			{
				Slot->OnGadgetChanged.RemoveDynamic(this, &UCabin::BroadcastGadgetCommonChanged);
			}
			else if (GadgetType == EItemType::GadgetWeapon)
			{
				Slot->OnGadgetChanged.RemoveDynamic(this, &UCabin::BroadcastGadgetWeaponChanged);
			}
		}

		Slots.SetNum(NewSize);
		return;
	}

	const int32 SlotsToAdd = NewSize - Slots.Num();
	for (int32 i = 0; i < SlotsToAdd; ++i)
	{
		UGadgetSlot* NewSlot = NewObject<UGadgetSlot>(this);
		NewSlot->GadgetType = GadgetType;

		if (GadgetType == EItemType::GadgetCommon)
		{
			NewSlot->OnGadgetChanged.AddDynamic(this, &UCabin::BroadcastGadgetCommonChanged);
		}
		else if (GadgetType == EItemType::GadgetWeapon)
		{
			NewSlot->OnGadgetChanged.AddDynamic(this, &UCabin::BroadcastGadgetWeaponChanged);
		}
		
		Slots.Add(NewSlot);
	}
}
