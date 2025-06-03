// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomFunctionLibrary.h"

static float Heuristic(const FVector& A, const FVector& B)
{
    return FVector::Dist2D(A, B);
}

static bool IsWalkable(const FVector& Current, const FVector& Neighbor, float MaxSlopeDegrees, const FVector& CharacterSize)
{
    FVector Delta = Neighbor - Current;
    FVector Flat = FVector(Delta.X, Delta.Y, 0.0f);

    if (Flat.SizeSquared() == 0) return false;

    float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Delta.GetSafeNormal(), Flat.GetSafeNormal())));
    return AngleDegrees <= MaxSlopeDegrees;
}

static void ReconstructPath(TSharedPtr<FPathfindingNode> Node, TArray<FVector>& OutPath)
{
    OutPath.Reset();
    while (Node.IsValid())
    {
        OutPath.Insert(Node->Location, 0);
        Node = Node->Parent;
    }
}

bool UCustomFunctionLibrary::AStarPathfinding(
    const TArray<FPointRow>& GridPoints,
    const FVector& Start,
    const FVector& End,
    const FVector& CharacterSize,
    float MaxSlopeDegrees,
    TArray<FVector>& OutPath)
{
    const int32 Rows = GridPoints.Num();
    if (Rows == 0) return false;
    const int32 Cols = GridPoints[0].Row.Num();

    auto GetIndex = [&](const FVector& Point, int32& OutX, int32& OutY) -> bool {
        for (int32 y = 0; y < Rows; ++y)
        {
            for (int32 x = 0; x < Cols; ++x)
            {
                if (GridPoints[y].Row[x].Equals(Point, 0.1f))
                {
                    OutX = x;
                    OutY = y;
                    return true;
                }
            }
        }
        return false;
        };

    int32 StartX, StartY, EndX, EndY;
    if (!GetIndex(Start, StartX, StartY) || !GetIndex(End, EndX, EndY))
        return false;

    TArray<TSharedPtr<FPathfindingNode>> OpenList;
    TSet<FIntPoint> ClosedSet;

    auto MakeNode = [&](int32 X, int32 Y, TSharedPtr<FPathfindingNode> Parent = nullptr) {
        TSharedPtr<FPathfindingNode> Node = MakeShared<FPathfindingNode>();
        Node->X = X;
        Node->Y = Y;
        Node->Location = GridPoints[Y].Row[X];
        Node->Parent = Parent;
        return Node;
        };

    TSharedPtr<FPathfindingNode> StartNode = MakeNode(StartX, StartY);
    StartNode->G = 0;
    StartNode->H = Heuristic(Start, End);
    OpenList.Add(StartNode);

    const TArray<FIntPoint> Offsets = {
        { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 },
        { 1, 1 }, { 1, -1 }, { -1, -1 }, { -1, 1 }
    };

    while (OpenList.Num() > 0)
    {
        OpenList.Sort([](const TSharedPtr<FPathfindingNode>& A, const TSharedPtr<FPathfindingNode>& B)
            {
                return A->F() < B->F();
            });

        TSharedPtr<FPathfindingNode> Current = OpenList[0];
        OpenList.RemoveAt(0);

        if (Current->X == EndX && Current->Y == EndY)
        {
            ReconstructPath(Current, OutPath);
            return true;
        }

        ClosedSet.Add(FIntPoint(Current->X, Current->Y));

        for (const FIntPoint& Offset : Offsets)
        {
            int32 NX = Current->X + Offset.X;
            int32 NY = Current->Y + Offset.Y;

            if (NX < 0 || NY < 0 || NY >= Rows || NX >= Cols)
                continue;

            if (ClosedSet.Contains(FIntPoint(NX, NY)))
                continue;

            const FVector& Neighbor = GridPoints[NY].Row[NX];

            if (!IsWalkable(Current->Location, Neighbor, MaxSlopeDegrees, CharacterSize))
                continue;

            float StepCost = FVector::Dist(Current->Location, Neighbor);

            TSharedPtr<FPathfindingNode> NeighborNode = MakeNode(NX, NY, Current);
            NeighborNode->G = Current->G + StepCost;
            NeighborNode->H = Heuristic(Neighbor, End);

            bool bFoundBetter = false;
            for (const auto& Node : OpenList)
            {
                if (Node->X == NX && Node->Y == NY && Node->F() <= NeighborNode->F())
                {
                    bFoundBetter = true;
                    break;
                }
            }

            if (!bFoundBetter)
            {
                OpenList.Add(NeighborNode);
            }
        }
    }

    return false;
}

UObject* UCustomFunctionLibrary::CreateObject(TSubclassOf<UObject> Class)
{
    return NewObject<UObject>((UObject*)GetTransientPackage(), Class.Get());
}