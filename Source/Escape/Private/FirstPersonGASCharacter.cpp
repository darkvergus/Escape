// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonGASCharacter.h"
#include "ACombatPlayerController.h"
#include "ESwipeAttack.h"


void AFirstPersonGASCharacter::BeginPlay() {

    Super::BeginPlay();

    AACombatPlayerController* PC = Cast<AACombatPlayerController>(GetController());
    if (PC)
    {
        PC->OnShortSwipe.AddDynamic(this, &AFirstPersonGASCharacter::HandleAttackSwipe);
    }
}


void AFirstPersonGASCharacter::HandleAttackSwipe( ESwipeDirection Direction)
{
    // Call AttackInterface function or trigger attack here
    UE_LOG(LogTemp, Log, TEXT("Swipe Attack: %d"), (int32)Direction);
}