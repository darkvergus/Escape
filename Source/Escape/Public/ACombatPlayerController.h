// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ESwipeAttack.h"

#include "InputAction.h"  // For UInputAction
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "ACombatPlayerController.generated.h"




DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackSwipeSignature, ESwipeDirection, SwipeDirection);


/**
 * 
 */
UCLASS()
class ESCAPE_API AACombatPlayerController : public APlayerController
{
    GENERATED_BODY()

public : 
    UPROPERTY(BlueprintAssignable, Category = "Input")
    FOnAttackSwipeSignature OnAttackSwipe;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    class UInputAction* IA_AttackSwipe;

protected:
    virtual void SetupInputComponent() override;

private:
    bool bIsDragging = false;
    FVector2D DragStart;
 

    void OnAttackMousePressed();
    void OnAttackMouseReleased();
    void DetectSwipeDirection(const FVector2D& Start, const FVector2D& End);


    //FVector2D GetMousePosition2D() const;
	
};
