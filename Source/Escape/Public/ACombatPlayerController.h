// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ESwipeAttack.h"

#include "InputAction.h"  // For UInputAction
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "ACombatPlayerController.generated.h"




DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShortSwipe, ESwipeDirection, SwipeDirection);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeavySwipe, ESwipeDirection, SwipeDirection);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoubleClick);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHeavySwipeCancel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChargeSwipeStart);

/**
 * 
 */
UCLASS()
class ESCAPE_API AACombatPlayerController : public APlayerController
{
    GENERATED_BODY()

public : 
    
    //Events, called on Input command
    UPROPERTY(BlueprintAssignable, Category = "Input|Combat")
    FOnShortSwipe OnShortSwipe;
    UPROPERTY(BlueprintAssignable, Category = "Input|Combat")
    FOnHeavySwipe OnHeavyAttackSwipe;
    UPROPERTY(BlueprintAssignable, Category = "Input|Combat")
    FOnChargeSwipeStart OnChargeSwipeStart;
    UPROPERTY(BlueprintAssignable, Category = "Input|Combat")
    FOnHeavySwipeCancel OnHeavySwipeCancel;
    UPROPERTY(BlueprintAssignable, Category = "Input|Combat")
    FOnDoubleClick OnDoubleClick;






    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputAction* AttackSwipeAction;

   
    


    // Constants /Parameters
    const float ShortSwipeMaxTime = 0.5f;
    const float HeavyAttackChargeTime = 1.0f;
    const float MinSwipeDistance = 50.0f;
    const float DoubleClickMaxGap = 0.3f;


    // stop the charging
    void CancelCharge();

protected:

    virtual void SetupInputComponent() override;


private:
    
    bool bIsDragging = false;
    FVector2D SwipeStartPos;
    FVector2D SwipeEndPos;


    // Swipe / click tracking
    float PressTime = 0.0f;
    float ReleaseTime = 0.0f;
    float LastClickTime = 0.0f;

    bool bIsCharging = false;

 

    void OnAttackMousePressed();
    void OnAttackMouseReleased();



    // Helper functions
    void StartCharge();
    ESwipeDirection DetectSwipeDirection(const FVector2D& Start, const FVector2D& End) const;
   // void DetectSwipe(bool bFromHeavyAttack);

    //FVector2D GetMousePosition2D() const;
	
};
