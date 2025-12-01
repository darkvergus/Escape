#pragma once
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "MentalStateRow.generated.h"

USTRUCT(BlueprintType)
struct FMentalStateRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag Action;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag HP;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag Archetype;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag Mental;
};