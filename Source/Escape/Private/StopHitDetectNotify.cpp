// Fill out your copyright notice in the Description page of Project Settings.


#include "StopHitDetectNotify.h"

#include "AttackInterface.h"

void UStopHitDetectNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp) return;

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    if (Owner->GetClass()->ImplementsInterface(UAttackInterface::StaticClass()))
    {
        AWeaponBase* Weapon = IAttackInterface::Execute_GetWeapon(Owner);

        if (Weapon)
        {
            Weapon->StopHitDetection();
        }
    }
}