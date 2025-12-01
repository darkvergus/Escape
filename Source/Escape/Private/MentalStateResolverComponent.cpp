#include "MentalStateResolverComponent.h"
#include "MentalStateRow.h"

UMentalStateResolverComponent::UMentalStateResolverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMentalStateResolverComponent::Initialise(UAbilitySystemComponent* ASC)
{
	CachedASC = ASC;
}

void UMentalStateResolverComponent::Recompute()
{
    if (!CachedASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("[MentalState] CachedASC is null"));
        return;
    }

    if (!MentalStateTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("[MentalState] MentalStateTable is null"));
        return;
    }

    if (CurrentMental.IsValid())
    {
        UE_LOG(LogTemp, Display, TEXT("[MentalState] Removing old mental tag %s"), *CurrentMental.ToString());
        CachedASC->RemoveLooseGameplayTag(CurrentMental);
    }

    int32 RowIndex = 0;
    for (auto& Pair : MentalStateTable->GetRowMap())
    {
        ++RowIndex;

        const FMentalStateRow* Row = reinterpret_cast<FMentalStateRow*>(Pair.Value);
        if (!Row)
        {
            UE_LOG(LogTemp, Error, TEXT("[MentalState] Row %d is null (bad import?)"), RowIndex);
            continue;
        }

        const bool actionMatch = CachedASC->HasMatchingGameplayTag(Row->Action);
        const bool hpMatch = CachedASC->HasMatchingGameplayTag(Row->HP);
        const bool archetypeMatch = CachedASC->HasMatchingGameplayTag(Row->Archetype);

        UE_LOG(LogTemp, Display, TEXT("[MentalState] Row %d  |  A:%s(%d)  HP:%s(%d)  Arch:%s(%d) → Mental:%s"), 
            RowIndex,
            *Row->Action.ToString(), actionMatch,
            *Row->HP.ToString(), hpMatch,
            *Row->Archetype.ToString(), archetypeMatch,
            *Row->Mental.ToString());

        if (actionMatch && hpMatch && archetypeMatch)
        {
            CurrentMental = Row->Mental;
            CachedASC->AddLooseGameplayTag(CurrentMental);

            UE_LOG(LogTemp, Warning, TEXT("[MentalState] MATCH!  Applied mental tag %s"), *CurrentMental.ToString());
            return;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("[MentalState] No row matched  –  clearing tag"));
    CurrentMental = FGameplayTag();
}


void UMentalStateResolverComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMentalStateResolverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}