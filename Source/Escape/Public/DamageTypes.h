// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DamageTypes.generated.h"


/**
 * 
 */

USTRUCT(BlueprintType)
struct FDamageInfo
{
    GENERATED_BODY()

    /** Final or base damage */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage = 0.f;

    /** Who caused the damage */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AActor* Instigator = nullptr;

    /** Optional: weapon used */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AActor* SourceActor = nullptr;

    /** Full hit data (VERY IMPORTANT) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FHitResult HitResult;

    /** Attack type (light, heavy, stab, etc.) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag AttackTag;

    /** Optional flags */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanBeBlocked = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanBeParried = true;
};

UENUM(BlueprintType)
enum class EDamageResult : uint8
{
    Ignored,
    Damaged,
    Blocked,
    Parried,
    Killed
};