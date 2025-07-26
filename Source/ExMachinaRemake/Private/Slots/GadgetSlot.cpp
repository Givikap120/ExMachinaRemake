#include "Slots/GadgetSlot.h"

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

bool UGadgetSlot::TrySetGadget(UGadget* NewGadget)
{
	if (!IsValid(NewGadget) || NewGadget->GetItemType() != GadgetType)
		return false;

	Gadget = NewGadget;
	OnGadgetChanged.Broadcast();
	return true;
}

bool UGadgetSlot::TryRemoveGadget()
{
	if (!IsValid(Gadget)) return false;

	Gadget = nullptr;
	OnGadgetChanged.Broadcast();
	return true;
}
