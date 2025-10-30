// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_BaseAttack.h"




UGA_BaseAttack::UGA_BaseAttack()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    // Ability is activated from input, not auto
    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Stunned")));
}

void UGA_BaseAttack::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) return;

    AGASCharacter* Character = Cast<AGASCharacter>(ActorInfo->AvatarActor.Get());
    if (Character && AttackMontage)
    {
        // Play montage
        UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            float MontageLength = AnimInstance->Montage_Play(AttackMontage, 1.f);
            if (MontageLength > 0.f)
            {
                // Bind notify / end
                FOnMontageEnded EndDelegate;
                EndDelegate.BindUObject(this, &UGA_BaseAttack::OnMontageCompleted);
                AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);
            }
        }
    }
    else
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
    }
}


void UGA_BaseAttack::OnAttackHit()
{
    if (DamageEffect)
    {
        // Example: apply effect to target(s) — you'd hook this up to hit detection
        // For now just log
        UE_LOG(LogTemp, Log, TEXT("Attack hit → applying %s"), *DamageEffect->GetName());
    }
}

void UGA_BaseAttack::OnMontageCompleted(UAnimMontage* Montage, bool bInterrupted)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}