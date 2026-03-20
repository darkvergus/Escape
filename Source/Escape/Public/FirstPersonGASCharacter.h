
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



	UFUNCTION(BlueprintCallable, Category = "Combat")
	void HandleAttackSwipe(ESwipeDirection Direction);

	AFirstPersonGASCharacter();
};
