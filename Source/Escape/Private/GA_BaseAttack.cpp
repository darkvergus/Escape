// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_BaseAttack.h"
#include "AttackInterface.h"




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


    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    AActor* Avatar = GetAvatarActorFromActorInfo();
    IAttackInterface* Attacker = Cast<IAttackInterface>(Avatar);
    if (!Attacker) { EndAbility(Handle, ActorInfo, ActivationInfo, true, false); return; }

    AWeaponBase* Weapon = Attacker->Execute_GetWeapon(Avatar);
    if (Weapon)
    {
        Weapon->OnWeaponHit.AddDynamic(this, &UGA_BaseAttack::OnAttackHit);
        Weapon->StartHitDetection();
    }



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


void UGA_BaseAttack::OnAttackHit(const FHitResult& HitResult)
{
    AActor* Avatar = GetAvatarActorFromActorInfo();
    if (!Avatar) return;

    // Retrieve attacker info
    IAttackInterface* Attacker = Cast<IAttackInterface>(Avatar);
    if (!Attacker) return;

    const AActor* InstigatorActor = Attacker->Execute_GetAttackInstigator(Avatar);
    AWeaponBase* Weapon = Attacker->Execute_GetWeapon(Avatar);

    // Compute damage
    float TotalDamage = Damage;
    if (Weapon)
        TotalDamage += Weapon->GetBaseDamage();

    TotalDamage *= Attacker->Execute_GetAttackMultiplier(Avatar);

    // Apply damage through GAS (if DamageEffect is set)
    if (DamageEffect)
    {
        FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffect, GetAbilityLevel());
        if (SpecHandle.IsValid())
        {
            SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage"), TotalDamage);

            FGameplayAbilityTargetDataHandle TargetDataHandle(
                new FGameplayAbilityTargetData_SingleTargetHit(HitResult)
            );
            ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SpecHandle, TargetDataHandle);
        }
    }
    else if (AActor* HitActor = HitResult.GetActor())
    {
        // fallback if not using GAS for target
       // UGameplayStatics::ApplyDamage(HitActor, TotalDamage, Avatar->GetInstigatorController(), Avatar, nullptr);
    }

    // Optional: stop on first valid hit
    if (Weapon)
    {
        Weapon->StopHitDetection();
    }
}

void UGA_BaseAttack::OnMontageCompleted(UAnimMontage* Montage, bool bInterrupted)
{
    if (AActor* Avatar = GetAvatarActorFromActorInfo())
    {
        if (IAttackInterface* Attacker = Cast<IAttackInterface>(Avatar))
        {
            if (AWeaponBase* Weapon = Attacker->Execute_GetWeapon(Avatar))
            {
                Weapon->StopHitDetection();
                Weapon->OnWeaponHit.RemoveDynamic(this, &UGA_BaseAttack::OnAttackHit);
            }
        }
    }



    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}