// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DamageTypes.generated.h"



UENUM(BlueprintType)
enum class EDamageType : uint8
{
    Heavy     UMETA(DisplayName = "Heavy"),
    Light      UMETA(DisplayName = "Light"),
    Push         UMETA(DisplayName = "Push"),
    GuardBreak      UMETA(DisplayName = "GuardBreak"),
    Unblockable          UMETA(DisplayName = "Unblockable")
    //TrueDamage   UMETA(DisplayName = "True Damage"), // Ignores armor/resistance
    //Custom       UMETA(DisplayName = "Custom")
};


/**
 * 
 */

USTRUCT(BlueprintType)
struct FDamageInfo
{
    GENERATED_BODY()

    /** Raw damage to apply */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
    float Damage = 0.0f;

    /** The type of damage (optional logic usage) */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
    EDamageType DamageType = EDamageType::Light;

    /** Critical hit bonus multiplier (e.g. 2.0x) */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
    float CritMultiplier = 1.0f;

    /** Was this a critical hit? */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
    bool bIsCritical = false;

    /** Source of the damage (instigating actor) */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
    AActor* Instigator = nullptr;

    /** Actual actor or projectile that caused the hit */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
    AActor* DamageCauser = nullptr;

    /** The location where the hit occurred */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
    FVector HitLocation = FVector::ZeroVector;

    /** The impact direction (e.g. for knockback) */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
    FVector HitDirection = FVector::ForwardVector;

    /** The component that was hit, if known */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
    UPrimitiveComponent* HitComponent = nullptr;

    /** Gameplay tags associated with the damage (e.g., "Fire", "Bleed") */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
    FGameplayTagContainer DamageTags;

    /** Status effects that should be applied with this damage */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
    TArray<FGameplayTag> StatusEffects;

    FDamageInfo() {}

    FDamageInfo(float InDamage, EDamageType InType = EDamageType::Light)
        : Damage(InDamage), DamageType(InType) {
    }
};