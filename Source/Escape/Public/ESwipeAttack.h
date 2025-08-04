// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "ESwipeAttack.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ESwipeDirection : uint8
{
    None     UMETA(DisplayName = "None"),
    Left     UMETA(DisplayName = "Left"),
    Right    UMETA(DisplayName = "Right"),
    Up       UMETA(DisplayName = "Up"),
    Down     UMETA(DisplayName = "Down")
};
