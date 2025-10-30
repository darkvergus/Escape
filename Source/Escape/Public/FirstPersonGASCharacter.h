// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASCharacter.h"
#include "ESwipeAttack.h" 
#include "WeaponComponent.h"
#include "PlayerAttackComponent.h"
#include "FirstPersonGASCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPE_API AFirstPersonGASCharacter : public AGASCharacter
{
	GENERATED_BODY()


protected: 
	void BeginPlay() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UPlayerAttackComponent* AttackComponent;
	

public : 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Weapon")
	UWeaponComponent* WeaponComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Weapon", meta = (ExposeOnSpawn = true))
	FName AttachToSocketName = "GripPoint";


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Weapon")
	//TSubclassOf<AWeaponBase> DefaultWeaponClass;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Weapon")
	//AWeaponBase* EquippedWeapon;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void HandleAttackSwipe(ESwipeDirection Direction);

	// Optional helper function
	//void EquipWeapon(TSubclassOf<AWeaponBase> WeaponClass);


	AFirstPersonGASCharacter();
};
