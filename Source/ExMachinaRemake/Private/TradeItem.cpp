#include "TradeItem.h"
#include "CustomFunctionLibrary.h"

UTradeItem* UTradeItem::CreateTradeItemFromTable(TSubclassOf<UTradeItem> Class, UDataTable* DataTable, FName RowName)
{
	auto Result = Cast<UTradeItem>(UCustomFunctionLibrary::CreateObject(Class));
	Result->SetFromTable(DataTable, RowName);
	return Result;
}