// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableComponent.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "DamageTypes.h"

// Sets default values for this component's properties
UBreakableComponent::UBreakableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UBreakableComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}


// ───────── Damageable Interface ─────────

bool UBreakableComponent::CanBeDamaged_Implementation() const
{
    return bCanBeDamaged && CurrentHealth > 0.f;
}

EDamageResult UBreakableComponent::TryBlock_Implementation(
    const FDamageInfo& DamageInfo)
{
    // Breakables NEVER block
    return EDamageResult::Damaged;
}

float UBreakableComponent::GetCurrentHealth_Implementation() const
{
    return CurrentHealth;
}

float UBreakableComponent::GetMaxHealth_Implementation() const
{
    return MaxHealth;
}

EDamageResult UBreakableComponent::ReceiveDamage_Implementation(
    const FDamageInfo& DamageInfo)
{
    if (!CanBeDamaged_Implementation())
        return EDamageResult::Ignored;

    CurrentHealth -= DamageInfo.Damage;
    // 🔊 TODO: play on hit sound (use DamageInfo / PhysMaterial)
    // 💥 TODO: spawn  on hit VFX
 

    if (CurrentHealth <= 0.f)
    {
        Break(DamageInfo);

        return EDamageResult::Killed;
    }
    return EDamageResult::Damaged;
}

void UBreakableComponent::Break(const FDamageInfo& DamageInfo) 
{
    bCanBeDamaged = false;

    AActor* Owner = GetOwner();
    if (!Owner) return;

    // 🔊 TODO: play break sound (use DamageInfo / PhysMaterial)
    // 💥 TODO: spawn  break VFX


    OnDeath.Broadcast();


    if (bDestroyOnBreak)
    {
        Owner->Destroy();
    }

}