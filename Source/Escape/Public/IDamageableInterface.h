// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDamageableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UIDamageableInterface : public UInterface
{
    GENERATED_BODY()
};

class ESCAPE_API IIDamageableInterface
{
    GENERATED_BODY()

public:
    /** Core damage application function */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
    void ApplyDamage(const FDamageInfo& DamageInfo);
};