// Fill out your copyright notice in the Description page of Project Settings.


#include "ACombatPlayerController.h"
#include "ESwipeAttack.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "InputTriggers.h"


void AACombatPlayerController::SetupInputComponent()
{

    Super::SetupInputComponent();


    UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);
    if (Input && IA_AttackSwipe)
    {
        Input->BindAction(IA_AttackSwipe, ETriggerEvent::Started, this, &AACombatPlayerController::OnAttackMousePressed);
        Input->BindAction(IA_AttackSwipe, ETriggerEvent::Completed, this, &AACombatPlayerController::OnAttackMouseReleased);
    }

    if(IA_AttackSwipe) UE_LOG(LogTemp, Warning, TEXT("IA_AttackSwipe"));
   /* InputComponent->BindAction("IA_AttackSwipe", IE_Pressed, this, &AACombatPlayerController::OnAttackMousePressed);
    InputComponent->BindAction("AttackMouse", IE_Released, this, &AACombatPlayerController::OnAttackMouseReleased);*/
}

void AACombatPlayerController::OnAttackMousePressed()
{
    UE_LOG(LogTemp, Warning, TEXT("Mouse Pressed detected"));
    bIsDragging = true;
    float X, Y;
    GetMousePosition(X, Y);
    DragStart = FVector2D(X, Y);
}

void AACombatPlayerController::OnAttackMouseReleased()
{

    UE_LOG(LogTemp, Warning, TEXT("Mouse Release detected"));
    if (!bIsDragging) return;
    bIsDragging = false;

    float X, Y;
    GetMousePosition(X, Y);
    FVector2D DragEnd(X, Y);

    DetectSwipeDirection(DragStart, DragEnd);
}

void AACombatPlayerController::DetectSwipeDirection(const FVector2D& Start, const FVector2D& End)
{
    FVector2D Delta = End - Start;
    if (Delta.Size() < 50.f) return; // Ignore small movements

    float Angle = FMath::Atan2(Delta.Y, Delta.X) * 180.f / PI;
    ESwipeDirection Direction = ESwipeDirection::None;

    if (Angle >= -45.f && Angle <= 45.f)
        Direction = ESwipeDirection::Right;
    else if (Angle > 45.f && Angle < 135.f)
        Direction = ESwipeDirection::Up;
    else if (Angle >= 135.f || Angle <= -135.f)
        Direction = ESwipeDirection::Left;
    else if (Angle < -45.f && Angle > -135.f)
        Direction = ESwipeDirection::Down;

    OnAttackSwipe.Broadcast(Direction);
}