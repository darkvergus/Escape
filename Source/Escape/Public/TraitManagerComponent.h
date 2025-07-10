// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "TraitManagerComponent.generated.h"

class UAbilitySystemComponent;
class UArchetypeDefinition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArchetypeApplied, UArchetypeDefinition*, Archetype);

UCLASS(ClassGroup=(Character), meta=(BlueprintSpawnableComponent))
class ESCAPE_API UTraitManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTraitManagerComponent();

	UPROPERTY(EditAnywhere, Category = "Trait System")
	TArray<TSoftObjectPtr<UArchetypeDefinition>> AvailableArchetypes;

	UPROPERTY(BlueprintReadOnly, Category = "Trait System")
	TArray<UArchetypeDefinition*> ActiveArchetypes;

	UFUNCTION(BlueprintCallable, Category = "Trait System")
	void RollArchetypes(int32 NumToPick = 1);

	UFUNCTION(BlueprintCallable, Category = "Trait System")
	void ApplyArchetype(UArchetypeDefinition* Archetype);

	UPROPERTY(BlueprintAssignable, Category = "Trait System")
	FOnArchetypeApplied OnArchetypeApplied;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	UAbilitySystemComponent* CachedASC = nullptr;

	UAbilitySystemComponent* GetASC();
};
