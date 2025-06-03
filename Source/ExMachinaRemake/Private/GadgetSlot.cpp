// Fill out your copyright notice in the Description page of Project Settings.


#include "GadgetSlot.h"

void UGadgetSlot::GetSize(int32& SizeX, int32& SizeY) const
{
	switch (GadgetType)
	{
	case EItemType::GadgetCommon:
		SizeX = 1;
		SizeY = 1;
	case EItemType::GadgetWeapon:
		SizeX = 1;
		SizeY = 2;
	default:
		SizeX = 0;
		SizeY = 0;
	}
}

bool UGadgetSlot::TrySetGadget(UGadget* Gadget)
{
	if (!IsValid(Gadget) || Gadget->GetItemType() != GadgetType)
		return false;

	GadgetPtr = Gadget;
	OnGadgetChanged.Broadcast();
	return true;
}

bool UGadgetSlot::TryRemoveGadget()
{
	if (!IsValid(GadgetPtr)) return false;

	GadgetPtr = nullptr;
	OnGadgetChanged.Broadcast();
	return true;
}
