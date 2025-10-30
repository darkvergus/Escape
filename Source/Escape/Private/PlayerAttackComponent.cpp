// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAttackComponent.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "DrawDebugHelpers.h"


UPlayerAttackComponent::UPlayerAttackComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerAttackComponent::BeginPlay()
{
    Super::BeginPlay();
    InitializeAttackAbilities();

}

void UPlayerAttackComponent::StartAttack_Implementation(const FAttackData& AttackData)
{
    bIsAttacking = true;
    UE_LOG(LogTemp, Log, TEXT("[%s] StartAttack: %s"), *GetOwner()->GetName(), *AttackData.AttackTag.ToString());
}

void UPlayerAttackComponent::PerformHitDetection_Implementation(const FAttackData& AttackData)
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    FVector Start = Owner->GetActorLocation();
    FVector End = Start + AttackData.Direction * AttackData.Range;
    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Owner);

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, Params))
    {
        UE_LOG(LogTemp, Log, TEXT("[%s] Hit: %s for %.1f damage"),
            *Owner->GetName(),
            *Hit.GetActor()->GetName(),
            AttackData.Damage);

        // (Optional) Here you can apply a GameplayEffect if using GAS
    }

    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.f, 0, 1.f);
}

void UPlayerAttackComponent::StopAttack_Implementation()
{
    bIsAttacking = false;
    UE_LOG(LogTemp, Log, TEXT("[%s] StopAttack"), *GetOwner()->GetName());
}

void UPlayerAttackComponent::InitializeAttackAbilities() {
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
