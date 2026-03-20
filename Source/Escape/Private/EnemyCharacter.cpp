
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



// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Optional: auto-equip default weapon
    if (WeaponComponent && WeaponComponent->DefaultWeaponClass)
        WeaponComponent->EquipWeapon(WeaponComponent->DefaultWeaponClass);


}


bool AEnemyCharacter::ActivateAbilitiesWithTag(FGameplayTagContainer AbilityTags, bool AllowRemoteActivation)
{
    if (!AbilitySystem)
    {
        return false;
    }

    return AbilitySystem->TryActivateAbilitiesByTag(AbilityTags, AllowRemoteActivation);
}






/** IAttackInterface */
AWeaponBase* AEnemyCharacter::GetWeapon_Implementation() const {



    return WeaponComponent ? WeaponComponent->GetCurrentWeapon(): nullptr;
}


float AEnemyCharacter::GetAttackMultiplier_Implementation() const {

    return CombatComponent ? CombatComponent->GetAttackMultiplier() : 1.0f;

}


void AEnemyCharacter::GetCurrentAttackTags_Implementation(FGameplayTagContainer& OutTags) const {

    return ;

}

AActor* AEnemyCharacter::GetAttackInstigator_Implementation() {

    return  this ;

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




UAbilitySystemComponent* AEnemyCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystem;
}