// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ArchetypeDefinition.generated.h"

class UGameplayEffect;
class UGameplayAbility;

/**
 * 
 */
UCLASS(BlueprintType)
class UArchetypeDefinition : public UPrimaryDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, Category = "Archetype")
    FText DisplayName;

    UPROPERTY(EditDefaultsOnly, Category = "Archetype")
    int32 RarityWeight = 100;

    UPROPERTY(EditDefaultsOnly, Category = "Archetype")
    TArray<TSubclassOf<UGameplayEffect>> PassiveEffects;

    UPROPERTY(EditDefaultsOnly, Category = "Archetype")
    TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;

    UPROPERTY(EditDefaultsOnly, Category = "Archetype")
    TArray<TSubclassOf<UActorComponent>> LogicComponents;

    UPROPERTY(EditDefaultsOnly, Category = "Archetype")
    FGameplayTag ArchetypeTag;
};
