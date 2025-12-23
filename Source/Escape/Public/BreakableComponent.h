// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IDamageableInterface.h"
#include "BreakableComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UBreakableComponent : public UActorComponent, public IIDamageableInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBreakableComponent();

    /** Called once when health reaches zero */
    UPROPERTY(BlueprintAssignable, Category = "Breakable")
    FOnDeathSignature OnDeath;

protected:
    virtual void BeginPlay() override;

    /** Max durability before breaking */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breakable")
    float MaxHealth = 50.f;

    /** Current durability */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Breakable")
    float CurrentHealth;

    /** If true, destroy owner when health reaches zero */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breakable")
    bool bDestroyOnBreak = true;

    /** Can this object currently receive damage */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breakable")
    bool bCanBeDamaged = true;

public:
    // ───────── IDamageableInterface ─────────

    virtual void ReceiveDamage_Implementation(
        const FDamageInfo& DamageInfo) override;

    virtual bool CanBeDamaged_Implementation() const override;

    virtual EBlockResult TryBlock_Implementation(
        const FDamageInfo& DamageInfo) override;

    virtual float GetCurrentHealth_Implementation() const override;
    virtual float GetMaxHealth_Implementation() const override;

protected:
    /** Called when object breaks */
    void Break(const FDamageInfo& DamageInfo);
};