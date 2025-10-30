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
    UE_LOG(LogTemp, Error, TEXT("AttackSwipeAction is HERE."));

    UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);
    if (Input && AttackSwipeAction)
    {
        Input->BindAction(AttackSwipeAction, ETriggerEvent::Started, this, &AACombatPlayerController::OnAttackMousePressed);
        Input->BindAction(AttackSwipeAction, ETriggerEvent::Completed, this, &AACombatPlayerController::OnAttackMouseReleased);
    }

    if (!AttackSwipeAction)
    {
        UE_LOG(LogTemp, Error , TEXT("AttackSwipeAction is NULL! Please assign it in Blueprint."));
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("AttackSwipeAction is HERE."));
    }
   /* InputComponent->BindAction("IA_AttackSwipe", IE_Pressed, this, &AACombatPlayerController::OnAttackMousePressed);
    InputComponent->BindAction("AttackMouse", IE_Released, this, &AACombatPlayerController::OnAttackMouseReleased);*/
}


void AACombatPlayerController::OnAttackMousePressed()
{
    UE_LOG(LogTemp, Warning, TEXT("Mouse Pressed detected"));
    //bIsDragging = true;

    //// sTART timer 
    //SwipeStartTime = GetWorld()->GetTimeSeconds();


    PressTime = GetWorld()->GetTimeSeconds();
    float X, Y;
    GetMousePosition(X, Y);
    SwipeStartPos = FVector2D(X, Y);

    // Double click check
    if (PressTime - LastClickTime <= DoubleClickMaxGap)
    {
        UE_LOG(LogTemp, Log, TEXT("Double Click Detected → Strong Jab"));
        OnDoubleClick.Broadcast();
        LastClickTime = 0; // Reset
        return;
    }

    LastClickTime = PressTime;

    //// Start heavy attack charge timer
    //GetWorldTimerManager().SetTimerForNextTick([this]()
    //    {
    //        
    //        float Elapsed = GetWorld()->GetTimeSeconds() - PressTime;

    //        UE_LOG(LogTemp, Log, TEXT("Held for: %f seconds"), Elapsed);

    //        if (GetWorld()->GetTimeSeconds() - PressTime >= HeavyAttackChargeTime)
    //        {
    //            StartCharge();
    //        }
    //    });


    GetWorldTimerManager().SetTimer(
        ChargeHandle,
        [this]()
        {
            float Elapsed = GetWorld()->GetTimeSeconds() - PressTime;
            UE_LOG(LogTemp, Log, TEXT("Held for: %f seconds"), Elapsed);

            if (Elapsed >= HeavyAttackChargeTime)
            {
                StartCharge();
                GetWorldTimerManager().ClearTimer(ChargeHandle); // Stop checking
            }
        },
        0.1f,   // check every 0.1 seconds
        true    // looping
    );

}

void AACombatPlayerController::OnAttackMouseReleased()
{
    GetWorldTimerManager().ClearTimer(ChargeHandle); // Stop checking
    UE_LOG(LogTemp, Warning, TEXT("Mouse Release detected"));
    //if (!bIsDragging) return;
    //bIsDragging = false;
    
    //DetectSwipeDirection(SwipeStartPos, DragEnd);


    ReleaseTime = GetWorld()->GetTimeSeconds();
    float X, Y;
    GetMousePosition(X, Y);
    SwipeEndPos = FVector2D(X, Y);

    float HoldDuration = ReleaseTime - PressTime;

    // Heavy Attack Swipe
    if (bIsCharging)
    {
        bIsCharging = false;
        float SwipeDuration = HoldDuration - HeavyAttackChargeTime;
      

        if (SwipeDuration >= ShortSwipeMaxTime)
        {
            UE_LOG(LogTemp, Error, TEXT("Charge"));
            ESwipeDirection Dir = DetectSwipeDirection(SwipeStartPos, SwipeEndPos);
            OnHeavyAttackSwipe.Broadcast(Dir);
        }
        return;
    }

    // Short Swipe
    if (HoldDuration <= ShortSwipeMaxTime)
    {
        FVector2D Delta = SwipeEndPos - SwipeStartPos;
        if (Delta.Size() >= MinSwipeDistance)
        {
            ESwipeDirection Dir = DetectSwipeDirection(SwipeStartPos, SwipeEndPos);
            OnShortSwipe.Broadcast(Dir);
        }
        return;
    }
}



void AACombatPlayerController::StartCharge()
{
    bIsCharging = true;
    UE_LOG(LogTemp, Log, TEXT("Heavy attack charging started"));
    // TODO: Trigger charge animation

    OnChargeSwipeStart.Broadcast();
}

void AACombatPlayerController::CancelCharge()
{
    bIsCharging = false;
    UE_LOG(LogTemp, Log, TEXT("Heavy attack charging Canceled"));
    // TODO: Trigger charge animation

    OnHeavySwipeCancel.Broadcast();
}






ESwipeDirection AACombatPlayerController::DetectSwipeDirection(const FVector2D& Start, const FVector2D& End) const
{
    FVector2D Delta = End - Start;
    if (Delta.Size() < 50.f) return ESwipeDirection::None; // Ignore small movements

    float Angle = FMath::Atan2(Delta.Y, Delta.X) * 180.f / PI;
    ESwipeDirection Direction = ESwipeDirection::None;

    if (Angle >= -45.f && Angle <= 45.f)
        return ESwipeDirection::Right;
    else if (Angle > 45.f && Angle < 135.f)
        return  ESwipeDirection::Up;
    else if (Angle >= 135.f || Angle <= -135.f)
        return ESwipeDirection::Left;
    else if (Angle < -45.f && Angle > -135.f)
        return ESwipeDirection::Down;

    return ESwipeDirection::None;
}