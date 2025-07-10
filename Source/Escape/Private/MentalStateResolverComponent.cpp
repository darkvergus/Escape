#include "MentalStateResolverComponent.h"

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
    if (!CachedASC || !MentalStateTable)
    {
        return;
    }

    const FGameplayTagContainer& Tags = CachedASC->GetOwnedGameplayTags();

    if (CurrentMental.IsValid())
    {
        CachedASC->RemoveLooseGameplayTag(CurrentMental);
    }

    const UScriptStruct* RowStruct = MentalStateTable->GetRowStruct();
    for (auto& Pair : MentalStateTable->GetRowMap())
    {
        uint8* RowData = Pair.Value;
        FGameplayTag* ActionTag = RowStruct->FindPropertyByName("Action")->ContainerPtrToValuePtr<FGameplayTag>(RowData);
        FGameplayTag* HPTag = RowStruct->FindPropertyByName("HP")->ContainerPtrToValuePtr<FGameplayTag>(RowData);
        FGameplayTag* ArchetypeTag = RowStruct->FindPropertyByName("Archetype")->ContainerPtrToValuePtr<FGameplayTag>(RowData);
        FGameplayTag* MentalTag = RowStruct->FindPropertyByName("Mental")->ContainerPtrToValuePtr<FGameplayTag>(RowData);

        if (ActionTag && HPTag && ArchetypeTag && MentalTag && Tags.HasTag(*ActionTag) && Tags.HasTag(*HPTag) && Tags.HasTag(*ArchetypeTag))
        {
            CurrentMental = *MentalTag;
            CachedASC->AddLooseGameplayTag(CurrentMental);
            return;
        }
    }
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

