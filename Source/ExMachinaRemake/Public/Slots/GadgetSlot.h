#pragma once

#include "CoreMinimal.h"
#include <Items/Gadget.h>
#include "GadgetSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGadgetChangedSignature);

UCLASS(BlueprintType)
class EXMACHINAREMAKE_API UGadgetSlot : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UGadget> Gadget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	EItemType GadgetType;

	UFUNCTION(BlueprintPure)
	void GetSize(int32& SizeX, int32& SizeY) const;

	UFUNCTION(BlueprintPure)
	bool HasGadget() const { return IsValid(Gadget); }

	UFUNCTION(BlueprintCallable)
	bool TrySetGadget(UGadget* NewGadget);

	UFUNCTION(BlueprintCallable)
	bool TryRemoveGadget();

	UPROPERTY(BlueprintAssignable)
	FOnGadgetChangedSignature OnGadgetChanged;
};
