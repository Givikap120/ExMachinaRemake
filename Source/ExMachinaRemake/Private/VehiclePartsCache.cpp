// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePartsCache.h"

double UVehiclePartsCache::GetMinCost() const
{
	double Result = TNumericLimits<double>::Max();

	for (const auto& DataUnit : Data)
	{
		Result = FMath::Min(Result, DataUnit.MinimalPrice);
	}

	return Result;
}
