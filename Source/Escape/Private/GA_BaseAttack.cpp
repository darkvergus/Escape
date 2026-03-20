// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_BaseAttack.h"
#include "AttackInterface.h"
#include "IDamageableInterface.h"
#include <Kismet/GameplayStatics.h>




UGA_BaseAttack::UGA_BaseAttack()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    // Ability is activated from input, not auto
    //ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Stunned")));
}

void UGA_BaseAttack::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{




    if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) {

        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    AActor* Avatar = GetAvatarActorFromActorInfo();



        IAttackInterface* Attacker = Cast<IAttackInterface>(Avatar); 


    if (Avatar && Avatar->GetClass()->ImplementsInterface(UAttackInterface::StaticClass()))
    {
        //UE_LOG(LogTemp, Warning, TEXT("Implements interface! %s"), *Avatar->GetName() );
        AWeaponBase* Weapon = IAttackInterface::Execute_GetWeapon(Avatar);
       // UE_LOG(LogTemp, Warning, TEXT("Avatar Multiplier %f"), IAttackInterface::Execute_GetAttackMultiplier(Avatar));
        
       
        
        if (Weapon)
        {
            //UE_LOG(LogTemp, Warning, TEXT("Implements interface! Weapon %s"), *Weapon->GetName());

            Weapon->OnWeaponHit.AddDynamic(this, &UGA_BaseAttack::OnAttackHit);
        }
        else UE_LOG(LogTemp, Error , TEXT("NO WEAPON! "))

    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Avatar does NOT implement interface"));
    }

    

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
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

    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}


void UGA_BaseAttack::OnAttackHit(const FHitResult& HitResult)
{
    
    if (!HitResult.GetActor() && !HitResult.GetComponent())
        return;


    AActor* HitActor = HitResult.GetActor();
    UPrimitiveComponent* HitComp = HitResult.GetComponent();


    if (!AlreadyHitActors.Contains(HitActor) )
    {
        AlreadyHitActors.Add(HitActor);
        UE_LOG(LogTemp, Log, TEXT("Hit -> %s "), *HitActor->GetName());
    }
    else {
       
        return;
    }
       
    AActor* Avatar = GetAvatarActorFromActorInfo();
    if (!Avatar || !Avatar->HasAuthority())
    {
        // If capabilities run on client, route to server RPC here
        return;
    }


    float CalcDamage = ComputeDamage(); 


    // Build FDamageInfo
    FDamageInfo DamageInfo;
    DamageInfo.Instigator = Avatar;
    DamageInfo.SourceActor = TryGetWeapon();
    DamageInfo.HitResult = HitResult;
    DamageInfo.Damage = CalcDamage;
    // DamageInfo.AttackTags = ...;




    // 1) Physics objects (simulate physics)

    if (HitComp && HitComp->IsSimulatingPhysics())
    {
        HandleHitDynamicObject(HitResult, DamageInfo);

        return;
    }

    // 2) World static / static geometry (walls) - interrupt
    // Use Hit.ImpactComponent->GetCollisionObjectType() or Hit.Actor->ActorHasTag
   
    if (HitComp && HitComp->GetCollisionObjectType() == ECC_WorldStatic)
    {
     
        HandleHitStaticObject(HitResult, DamageInfo);

        return;
    }



    // 3) Damageable actors (enemies, breakables)
    if (HitActor && 
        (HitActor->GetClass()->ImplementsInterface(UIDamageableInterface::StaticClass())
            || 
         HitActor->GetClass()->ImplementsInterface(UAbilitySystemInterface::StaticClass())
        
        ))
    {
        HandleHitEnemy(HitResult,DamageInfo);

        return;
    }


    // 4) Pawn that doesn't implement damageable (fallback)
    if (HitActor && HitActor->IsA(APawn::StaticClass()))
    {
        // You may choose to call ApplyDamage as fallback
        UGameplayStatics::ApplyDamage(HitActor, DamageInfo.Damage, /*InstigatorController*/ GetAvatarActorFromActorInfo()->GetInstigatorController(), DamageInfo.SourceActor, nullptr);
        return;
    }

    // 5) Default: treat as world hit
    //OnHitWorldDefault(Hit);


    
}



void UGA_BaseAttack::HandleHitDynamicObject(const FHitResult& HitResult, FDamageInfo DamageInfo) {

    AActor* HitActor = HitResult.GetActor();
    UPrimitiveComponent* HitComp = HitResult.GetComponent();


    UE_LOG(LogTemp, Log, TEXT("Dynamic Oject -> %s , push by %f"), *HitActor->GetName(), ImpactValue);

    //// push object
    const FVector Impulse = -HitResult.ImpactNormal * ImpactValue; // tune


    HitComp->AddImpulseAtLocation(Impulse, HitResult.ImpactPoint);

    if (HitActor && HitActor->GetClass()->ImplementsInterface(UIDamageableInterface::StaticClass()))
    {
        HandleHitEnemy(HitResult, DamageInfo);

    }


    // Do not treat as world-stop necessarily; maybe continue attack (design choice)
    return;

}


void UGA_BaseAttack::HandleHitEnemy(const FHitResult& HitResult, FDamageInfo DamageInfo) {

    AActor* HitActor = HitResult.GetActor();
    UPrimitiveComponent* HitComp = HitResult.GetComponent();

    UE_LOG(LogTemp, Log, TEXT("Hit charactr  -> %s , push by %f"), *HitActor->GetName(), ImpactValue);


    if (IAbilitySystemInterface* TargetASCI = Cast< IAbilitySystemInterface>(HitActor)) {
        UAbilitySystemComponent* TargetASC = TargetASCI->GetAbilitySystemComponent();
        UE_LOG(LogTemp, Log, TEXT("Hit charactr  -> has ASC"));



        if (HitActor->GetClass()->ImplementsInterface(UAttackInterface::StaticClass()))
        {
            
            FGameplayEventData EventData;
            EventData.Instigator = GetAvatarActorFromActorInfo();
            EventData.Target = HitResult.GetActor();
            FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
            EventData.TargetData.Add(TargetData);


            UE_LOG(LogTemp, Log, TEXT("Hit charactr  -> Attack interface"));

            if (TargetASC)
            {
                UE_LOG(LogTemp, Error, TEXT("send event to ->  Ability.StateTrigger.Stagger"));
                TargetASC->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Abilitiy.StateTrigger.Stagger")), &EventData);
            }
        }
    }

    

    if (HitActor->GetClass()->ImplementsInterface(UIDamageableInterface::StaticClass())) {


        // Check CanBeDamaged (target decides)
        bool bCanBeDamaged = IIDamageableInterface::Execute_CanBeDamaged(HitActor);
        if (!bCanBeDamaged)
        {
            // Target says no — maybe play a clang or ignore
            return;
        }



        // Let the Damager handle block logic 
        EDamageResult result  = IIDamageableInterface::Execute_ReceiveDamage(HitActor, DamageInfo);
        // Handle response to attack outcome 
        switch (result){

            case EDamageResult::Ignored:
                //Tr
                return;

            case EDamageResult::Damaged:
                //Trigger Attacker response on Successful hit

                return;

            case EDamageResult::Parried:

                // handle parry: cancel ability, play VFX/SFX, give attacker stun
                //HandleParry(HitActor, Hit);
                EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
                return;

            case EDamageResult::Blocked: 
                // handle block: play shield clang, stop attack
                //HandleBlock(HitActor, Hit);

                EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
                return;

            case EDamageResult::Killed:
                //Trigger Attacker response on Kill
                return; 
        }        

    }

   

}


void UGA_BaseAttack::HandleHitStaticObject (const FHitResult& HitResult, FDamageInfo DamageInfo) {


    UE_LOG(LogTemp, Warning , TEXT("STATIC Oject -> %s"), *HitResult.GetActor()->GetName());

    AActor* Avatar = GetAvatarActorFromActorInfo();


    ACharacter* Character = Cast<ACharacter>(Avatar);

    UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();


    AnimInstance->Montage_Stop(0.5f, AttackMontage);

    return;
}


void UGA_BaseAttack::HandleHitDamageItem(const FHitResult& HitResult, FDamageInfo DamageInfo) {

    //// Optionally: if object is destructible/damageable, route to ReceiveDamage
    //if (HitActor->GetClass()->ImplementsInterface(UIDamageableInterface::StaticClass()))
    //{
    //    // Ask if it can be damaged
    //    bool bCan = UIDamageableInterface::Execute_CanBeDamaged(HitActor);
    //    if (bCan)
    //    {
    //        // Could call TryBlock, but physics objects usually don't block
    //        UIDamageableInterface::Execute_ReceiveDamage(HitActor, DamageInfo);
    //    }
    //}


    return;
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


    AlreadyHitActors.Empty();
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}




float  UGA_BaseAttack::ComputeDamage() {


    AActor* Avatar = GetAvatarActorFromActorInfo();
    if (!Avatar) return -1;

    // Retrieve attacker info
    if (Avatar->GetClass()->ImplementsInterface(UAttackInterface::StaticClass()))
    {
        AWeaponBase* Weapon = IAttackInterface::Execute_GetWeapon(Avatar);

        // Compute damage
        float TotalDamage = Damage;
        if (Weapon)
            TotalDamage += Weapon->GetBaseDamage();

        TotalDamage *= IAttackInterface::Execute_GetAttackMultiplier(Avatar);
        return TotalDamage;

    }


    return 0.0f;
}



AWeaponBase* UGA_BaseAttack::TryGetWeapon() {


    if (AActor* Avatar = GetAvatarActorFromActorInfo())
    {
        if (IAttackInterface* Attacker = Cast<IAttackInterface>(Avatar))
        {
            return  Attacker->Execute_GetWeapon(Avatar);
        }
    }
    return NULL;
}

void UGA_BaseAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled) {

    // Makes sure the animation is stopped  the animation

    if (bWasCancelled) {
        AActor* Avatar = GetAvatarActorFromActorInfo();

        if (ACharacter* Character = Cast<ACharacter>(Avatar)) {
            UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

            if (AnimInstance)
                AnimInstance->Montage_Stop(0.5f, AttackMontage);
        }
    }
   
   




    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility,
        bWasCancelled);

}
