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



    IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(Owner);
    if (!AbilityInterface)
    {
        UE_LOG(LogTemp, Error,
            TEXT("[%s] Owner does not implement AbilitySystemInterface."),
            *Owner->GetName());
        return;
    }

    UAbilitySystemComponent* ASC = AbilityInterface->GetAbilitySystemComponent();
    if (!ASC)
    {
        UE_LOG(LogTemp, Error,
            TEXT("[%s] AbilitySystemComponent is null."),
            *Owner->GetName());
        return;
    }

    // VERY IMPORTANT: ensure ASC is initialized
    if (!ASC->AbilityActorInfo.IsValid())
    {
        UE_LOG(LogTemp, Error,
            TEXT("[%s] AbilityActorInfo not initialized yet. Delaying ability grant."),
            *Owner->GetName());
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


