// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GASCharacterAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 * 
 */
UCLASS()
class ESCAPE_API UGASCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UGASCharacterAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)override;

	UPROPERTY(BlueprintReadOnly, Category = "GAS Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "GAS Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "GAS Attributes", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, Stamina);

	UPROPERTY(BlueprintReadOnly, Category = "GAS Attributes", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, MaxStamina);

	UPROPERTY(BlueprintReadOnly, Category = "GAS Attributes", ReplicatedUsing = OnRep_HearingRadius)
	FGameplayAttributeData HearingRadius;
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, HearingRadius)

	UPROPERTY(BlueprintReadOnly, Category = "GAS Attributes", ReplicatedUsing = OnRep_FootstepVolume)
	FGameplayAttributeData FootstepVolume;
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, FootstepVolume)

	UPROPERTY(BlueprintReadOnly, Category = "GAS Attributes", ReplicatedUsing = OnRep_BreathVolume)
	FGameplayAttributeData BreathVolume;
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, BreathVolume)

	UPROPERTY(BlueprintReadOnly, Category = "GAS Attributes", ReplicatedUsing = OnRep_SneakSpeedScalar)
	FGameplayAttributeData SneakSpeedScalar;
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, SneakSpeedScalar)

	UPROPERTY(BlueprintReadOnly, Category = "GAS Attributes", ReplicatedUsing = OnRep_OutgoingDamageScalar)
	FGameplayAttributeData OutgoingDamageScalar;
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, OutgoingDamageScalar)

	UPROPERTY(BlueprintReadOnly, Category = "GAS Attributes", ReplicatedUsing = OnRep_EnemyRecoveryScalar)
	FGameplayAttributeData EnemyRecoveryScalar;
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, EnemyRecoveryScalar)

	UPROPERTY(BlueprintReadOnly, Category = "GAS Attributes", ReplicatedUsing = OnRep_IncomingDamageScalar)
	FGameplayAttributeData IncomingDamageScalar;
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, IncomingDamageScalar)

	UPROPERTY(BlueprintReadOnly, Category = "GAS Attributes", ReplicatedUsing = OnRep_HitReactScalar)
	FGameplayAttributeData HitReactScalar;
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, HitReactScalar)

protected:

	UFUNCTION()
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute,
		const FGameplayAttributeData& MaxAttribute,
		float NewMaxValue,
		const FGameplayAttribute& AffectedAttributeProperty);


	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

	UFUNCTION()
	virtual void OnRep_HearingRadius(const FGameplayAttributeData& OldHearingRadius);

	UFUNCTION()
	virtual void OnRep_FootstepVolume(const FGameplayAttributeData& OldFootstepVolume);

	UFUNCTION()
	virtual void OnRep_BreathVolume(const FGameplayAttributeData& OldBreathVolume);

	UFUNCTION()
	virtual void OnRep_SneakSpeedScalar(const FGameplayAttributeData& OldSneakSpeed);

	UFUNCTION()
	virtual void OnRep_OutgoingDamageScalar(const FGameplayAttributeData& OldOutgoingDamage);

	UFUNCTION()
	virtual void OnRep_EnemyRecoveryScalar(const FGameplayAttributeData& OldEnemyRecovery);

	UFUNCTION()
	virtual void OnRep_IncomingDamageScalar(const FGameplayAttributeData& OldIncomingDamage);

	UFUNCTION()
	virtual void OnRep_HitReactScalar(const FGameplayAttributeData& OldHitReact);
};
