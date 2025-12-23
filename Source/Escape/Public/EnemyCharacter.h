// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AttackInterface.h"
#include "WeaponComponent.h"
#include "EnemyCombatComponent.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class ESCAPE_API AEnemyCharacter :	public ACharacter,
									public IAbilitySystemInterface,
									public IAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();


	/** GAS */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** IAttackInterface */
	virtual AWeaponBase* GetWeapon_Implementation() const override;
	virtual float GetAttackMultiplier_Implementation() const override;
	virtual void GetCurrentAttackTags_Implementation(FGameplayTagContainer& OutTags) const override;
	virtual AActor* GetAttackInstigator_Implementation()  override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(BlueprintReadOnly)
	UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Weapon")
	UWeaponComponent* WeaponComponent;


	//  Handles most of the Damaging,  Ability Registration etc.... 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UEnemyCombatComponent* CombatComponent;




	/*  TO DO 

	
	// handles the getting hit portion
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UDamageableComponent* DamageableComponent;

	*/


};
