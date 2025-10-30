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

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Range = 150.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Direction = FVector::ZeroVector;

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
    /** Called when attack starts (animation / ability activation) */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
    void StartAttack(const FAttackData& AttackData);

    /** Called when attack hit should be detected (usually during montage notify) */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
    void PerformHitDetection(const FAttackData& AttackData);

    /** Called when attack ends */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
    void StopAttack();

    /** Returns true if currently in an attack window */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
    bool IsAttacking() const;

    /** Returns current base damage */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
    float GetBaseDamage() const; 

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
    AWeaponBase* GetWeapon() const;
};