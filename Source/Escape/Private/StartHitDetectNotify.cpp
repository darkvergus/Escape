// Fill out your copyright notice in the Description page of Project Settings.


#include "StartHitDetectNotify.h"

#include "AttackInterface.h"

void UStartHitDetectNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    UE_LOG(LogTemp, Error, TEXT("INIT HIT DETECT"));
    if (!MeshComp) return;

   

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;
    

    if (Owner->GetClass()->ImplementsInterface(UAttackInterface::StaticClass()))
    {
        UE_LOG(LogTemp, Error, TEXT("INIT HIT DETECT"));
        AWeaponBase* Weapon = IAttackInterface::Execute_GetWeapon(Owner);

        if (Weapon)
        {
            UE_LOG(LogTemp, Warning, TEXT("Implements interface! Weapon %s"), *Weapon->GetName());
            Weapon->StartHitDetection();
        }
    }
}