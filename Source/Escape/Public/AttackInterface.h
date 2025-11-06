#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "WeaponBase.h"
#include "AttackInterface.generated.h"

// Simple struct to describe an attack event
USTRUCT(BlueprintType)
struct FAttackData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage = 0.f;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite)
    //float Range = 150.f;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite)
    //FVector Direction = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag AttackTag;  // e.g. "Attack.Light", "Attack.Stab"

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AActor* InstigatorActor = nullptr;
};

UINTERFACE(MinimalAPI, Blueprintable)
class UAttackInterface : public UInterface
{
    GENERATED_BODY()
};

class ESCAPE_API IAttackInterface
{
    GENERATED_BODY()

public:

    /** Returns the currently equipped weapon */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
    AWeaponBase* GetWeapon() const;

    /** Attack power multiplier (character buffs, stats, scaling) */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
    float GetAttackMultiplier() const;

    /** Returns current attack tags (light, heavy, stab, finisher, etc) */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
    void GetCurrentAttackTags(FGameplayTagContainer& OutTags) const;

    /** Returns the actor performing damage (mostly self) */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
    AActor* GetAttackInstigator() const;
};