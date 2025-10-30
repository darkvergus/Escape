// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackInterface.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "WeaponBase.h"
#include "PlayerAttackComponent.generated.h"


UCLASS(ClassGroup = (Combat), meta = (BlueprintSpawnableComponent))
class ESCAPE_API UPlayerAttackComponent : public UActorComponent, public IAttackInterface
{
    GENERATED_BODY()

public:
    UPlayerAttackComponent();

protected:
    virtual void BeginPlay() override;

    /** Whether currently in an attack */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
    bool bIsAttacking = false;

    /** Base damage for attacks */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    float BaseDamage = 10.f;

    /** Attack abilities to grant at BeginPlay (like GA_LightAttack_01, GA_Stab, etc.) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack|Abilities")
    TArray<TSubclassOf<class UGameplayAbility>> AttackAbilityClasses;

    /** Handles to track granted abilities (optional, for cleanup) */
    UPROPERTY(VisibleAnywhere, Category = "Attack|Abilities")
    TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;

    /** Base damage for attacks */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Abilities")
    AWeaponBase* Weapon;

public:
    /** IAttackInterface implementation */
    virtual void StartAttack_Implementation(const FAttackData& AttackData) override;
    virtual void PerformHitDetection_Implementation(const FAttackData& AttackData) override;
    virtual void StopAttack_Implementation() override;
    virtual bool IsAttacking_Implementation() const override { return bIsAttacking; }
    virtual float GetBaseDamage_Implementation() const override { return BaseDamage; }


protected:
    /** Internal helper to grant attack abilities to the owning actor */
    void InitializeAttackAbilities();
};
