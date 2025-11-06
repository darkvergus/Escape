// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GASCharacter.h"
#include "GA_BaseAttack.generated.h"


//DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnMontageEnded, UAnimMontage*, Montage, bool, bInterrupted);


/**
 * 
 */
UCLASS()
class ESCAPE_API UGA_BaseAttack : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
    UGA_BaseAttack();

    // GAS required function
    virtual void ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData) override;

protected:

    /** Montage to play when this ability is triggered */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    UAnimMontage* AttackMontage;

    /** Damage to apply */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    float Damage = 10.f;

    /** GameplayEffect to apply to target (optional) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    TSubclassOf<UGameplayEffect> DamageEffect;

    /** Called when montage notify is hit */
    UFUNCTION()
    void OnAttackHit(const FHitResult& HitResult);

    /** Called when montage finishes */
    UFUNCTION()
    virtual void OnMontageCompleted(UAnimMontage* Montage, bool bInterrupted);

private:
    FDelegateHandle MontageEndHandle;
};