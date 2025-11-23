// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_AttackWindow.h"
#include "CoreMinimal.h"
#include "AttackInterface.h"




void UAnimNotifyState_AttackWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (!MeshComp) return;



    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;


    if (Owner->GetClass()->ImplementsInterface(UAttackInterface::StaticClass()))
    {

        AWeaponBase* Weapon = IAttackInterface::Execute_GetWeapon(Owner);

        if (Weapon)
        {

            Weapon->StartHitDetection();
        }
    }

}



void UAnimNotifyState_AttackWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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