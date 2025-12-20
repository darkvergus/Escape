// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "IDamageableInterface.generated.h"


/**
 * Blueprintable, minimal damageable interface.
 * Implement on actors (enemies, breakable props) or on a health component and forward calls.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UIDamageableInterface : public UInterface
{
    GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EBlockResult : uint8
{
    NotBlocking    UMETA(DisplayName = "NotBlocking"),
    Blocked        UMETA(DisplayName = "Blocked"),
    Parried        UMETA(DisplayName = "Parried")
};



class ESCAPE_API IIDamageableInterface
{
    GENERATED_BODY()

public:
    /** Called to apply damage / hit. Implementation MUST handle health deduction, hit reactions, block checks, etc. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
    void ReceiveDamage(const FDamageInfo& DamageInfo);

    /**
     * Optional: Query whether the object can currently be damaged (e.g. invulnerable states).
     * Return true if damage should be processed.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
    bool CanBeDamaged() const;

    /**
     * Optional: Let the target decide if the incoming hit is blocked or parried.
     * Return EBlockResult::NotBlocking / Blocked / Parried.
     * Ability/weapon can use this to react (e.g., cancel attack).
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
    EBlockResult TryBlock(const FDamageInfo& DamageInfo);

    /** Query current health (useful for UI or logic). */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
    float GetCurrentHealth() const;

    /** Query maximum health. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
    float GetMaxHealth() const;
};