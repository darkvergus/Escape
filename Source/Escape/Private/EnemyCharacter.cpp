// Fill out your copyright notice in the Description page of Project Settings.


#include "GASAbilitySystemComponent.h"
#include "EnemyCharacter.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    AbilitySystem = CreateDefaultSubobject<UGASAbilitySystemComponent>("AbilitySystem");
    AbilitySystem->SetIsReplicated(true);


    WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));



    CombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>("CombatComponent");



    /*
    DamageableComponent = CreateDefaultSubobject<UDamageableComponent>("DamageableComponent");
    */

}


/** IAttackInterface */
AWeaponBase* AEnemyCharacter::GetWeapon_Implementation() const {



    return WeaponComponent ? WeaponComponent->GetCurrentWeapon(): nullptr;
}


float AEnemyCharacter::GetAttackMultiplier_Implementation() const {

     return  0.0f;

}


void AEnemyCharacter::GetCurrentAttackTags_Implementation(FGameplayTagContainer& OutTags) const {

    return ;

}

AActor* AEnemyCharacter::GetAttackInstigator_Implementation() {

    return  this ;

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


UAbilitySystemComponent* AEnemyCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystem;
}