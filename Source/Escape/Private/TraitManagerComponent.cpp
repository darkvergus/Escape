// Fill out your copyright notice in the Description page of Project Settings.


#include "TraitManagerComponent.h"
#include "ArchetypeDefinition.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

UTraitManagerComponent::UTraitManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTraitManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	CachedASC = GetASC();
}

void UTraitManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

UAbilitySystemComponent* UTraitManagerComponent::GetASC()
{
	if (CachedASC) 
	{
		return CachedASC;
	}
	return GetOwner() ? GetOwner()->FindComponentByClass<UAbilitySystemComponent>() : nullptr;
}

void UTraitManagerComponent::RollArchetypes(int32 NumToPick)
{
	TArray<UArchetypeDefinition*> Loaded;
	for (TSoftObjectPtr<UArchetypeDefinition>& SoftRef : AvailableArchetypes)
	{
		if (UArchetypeDefinition* Arch = SoftRef.LoadSynchronous())
		{
			Loaded.Add(Arch);
		}
	}

	if (Loaded.Num() == 0) 
	{
		return;
	}

	int32 TotalWeight = 0;
	for (const UArchetypeDefinition* Arch : Loaded)
	{
		TotalWeight += FMath::Max(Arch->RarityWeight, 1);
	}

	for (int32 i = 0; i < NumToPick && TotalWeight > 0; ++i)
	{
		int32 Roll = FMath::RandRange(1, TotalWeight);
		for (UArchetypeDefinition* Arch : Loaded)
		{
			Roll -= FMath::Max(Arch->RarityWeight, 1);
			if (Roll <= 0)
			{
				ApplyArchetype(Arch);
				TotalWeight -= FMath::Max(Arch->RarityWeight, 1);
				break;
			}
		}
	}
}

void UTraitManagerComponent::ApplyArchetype(UArchetypeDefinition* Arch)
{
	if (!Arch) 
	{
		return;
	}

	UAbilitySystemComponent* ASC = GetASC();
	if (!ASC) 
	{
		return;
	}

	for (TSubclassOf<UGameplayEffect> GEClass : Arch->PassiveEffects)
	{
		if (GEClass)
		{
			ASC->ApplyGameplayEffectToSelf(GEClass->GetDefaultObject<UGameplayEffect>(), 1.f, ASC->MakeEffectContext());
		}
	}

	for (TSubclassOf<UGameplayAbility> AbilityClass : Arch->GrantedAbilities)
	{
		if (AbilityClass)
		{
			FGameplayAbilitySpec Spec(AbilityClass, 1);
			ASC->GiveAbility(Spec);
		}
	}

	for (TSubclassOf<UActorComponent> CompClass : Arch->LogicComponents)
	{
		if (CompClass)
		{
			GetOwner()->AddComponent(CompClass->GetFName(), false, FTransform::Identity, NewObject<UActorComponent>(GetOwner(), CompClass));
		}
	}

	if (Arch->ArchetypeTag.IsValid())
	{
		ASC->AddLooseGameplayTag(Arch->ArchetypeTag);
	}

	ActiveArchetypes.Add(Arch);
	OnArchetypeApplied.Broadcast(Arch);
}

