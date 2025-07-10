#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GASAbilitySystemComponent.h"
#include "ArchetypeDefinition.h"
#include "TraitManagerComponent.h"
#include "MentalStateResolverComponent.h"
#include "GASCharacter.generated.h"

UCLASS()
class ESCAPE_API AGASCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGASCharacter();

	UFUNCTION(BlueprintCallable, Category = "GAS Attribute")
	virtual int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "GAS Attribute")
	virtual float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GAS Attribute")
	virtual float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GAS Attribute")
	virtual float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "GAS Attribute")
	virtual float GetMaxStamina() const;

	/*Try to activate ALL abilities that match tag*/
	UFUNCTION(BlueprintCallable, Category = "GAS Attribute")
	bool ActivateAbilitiesWithTag(FGameplayTagContainer AbilityTags, bool AllowRemoteActivation = true);

	/*Called when Health Changes
	* @param DeltaValue -  Will be negative for health decrease , positive for healing
	* @param Origin - Cause of Stamina Change.
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaValue, AActor* Origin);

	/*Called when Health Changes
	* @param DeltaValue -  Will be negative for stamina decrease , positive for healing
	* @param Origin - Cause of Health Change.
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnStaminaChanged(float DeltaValue, AActor* Origin);

	/*Called when character runs out of Health*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnDead();

	/*UFUNCTION(BlueprintCallable,  Category="GAS Abilities|Attack")
	bool ActivateAttackAbility(bool AllowRemoteActivation = true);*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UGASCharacterAttributeSet* AttributeSet;

	/*the level of the character .Cant be changed directly once character spawn. */
	UPROPERTY(EditAnywhere, Category = "GAS Attribute")
	int32 CharacterLevel;

	//TSubclassOf<class UGameplayEffect> DefaultAttributeEffects;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS Attribute")
	TArray <TSubclassOf<class UGameplayEffect>> DefaultAttributeEffects;

	/*Set this to true if you want to add test abilities to the character */
	UPROPERTY(EditAnywhere, Category = "GAS Attribute|Debug")
	bool EnableTestAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS Attribute|Debug")
	TArray <TSubclassOf<class UGameplayAbility>> TestAbilities;

	virtual void SetTestAbilities();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS Abilities|AttackList")
	TArray <TSubclassOf<class UGameplayAbility>> AttackAbilityList;

	virtual void SetAttackAbilities();

	UPROPERTY()
	FGameplayAbilitySpecHandle AttackAbilitySpecHandle;

	//virtual void SetAttackAbility();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Traits")
    UTraitManagerComponent* TraitManager;

	UPROPERTY(EditDefaultsOnly, Category = "Traits")
	TSoftObjectPtr<UArchetypeDefinition> Asset;

	UFUNCTION()
	void HandleArchetypeApplied(UArchetypeDefinition* Arch);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Traits|Mental")
	UMentalStateResolverComponent* MentalResolver;
public:
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	// Inherited via IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void HandleHealthChange(float DeltaValue, AActor* Origin);
	virtual void HandleStaminaChange(float DeltaValue, AActor* Origin);

	virtual void ApplyDefaultAttributeEffects();
	virtual void RemoveDefaultAttributeEffects();
};