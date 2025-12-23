// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCombatComponent.h"

// Sets default values for this component's properties
UEnemyCombatComponent::UEnemyCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

}




void UEnemyCombatComponent::BeginPlay()
{
    Super::BeginPlay(); 
    InitializeAttackAbilities();
}

void UEnemyCombatComponent::InitializeAttackAbilities(){

    AActor* Owner = GetOwner();
    if (!Owner) return;

    UAbilitySystemComponent* ASC = nullptr;

    if (IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(Owner))
    {
        ASC = AbilityInterface->GetAbilitySystemComponent();
    }

    if (!ASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s] No AbilitySystemComponent found, cannot grant attack abilities."), *Owner->GetName());
        return;
    }

    // Grant each ability in AttackAbilityClasses
    for (TSubclassOf<UGameplayAbility> AbilityClass : AttackAbilityClasses)
    {
        if (AbilityClass)
        {
            FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, INDEX_NONE, Owner);
            FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(AbilitySpec);
            GrantedAbilityHandles.Add(Handle);

            UE_LOG(LogTemp, Error, TEXT("[%s] Granted attack ability: %s"), *Owner->GetName(), *AbilityClass->GetName());
        }
    }
}


