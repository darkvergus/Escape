// Fill out your copyright notice in the Description page of Project Settings.


#include "GASCharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "GASCharacter.h"

UGASCharacterAttributeSet::UGASCharacterAttributeSet()
{


}

void UGASCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(UGASCharacterAttributeSet, Health);
	DOREPLIFETIME(UGASCharacterAttributeSet, MaxHealth);
	DOREPLIFETIME(UGASCharacterAttributeSet, Stamina);
	DOREPLIFETIME(UGASCharacterAttributeSet, MaxStamina);

}

void UGASCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float DeltaValue = 0;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Additive) {
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	AActor* TargetActor = nullptr;
	AGASCharacter* GASCharacter = nullptr;

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		GASCharacter = Cast<AGASCharacter>(TargetActor);
	}


	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) {
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
		if (GASCharacter) {
			GASCharacter->HandleHealthChange(DeltaValue, Data.EffectSpec.GetContext().GetInstigator());
		}


	}

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute()) {

		bool StaminaOverflow = GetStamina() > GetMaxStamina() ? FMath::IsNearlyEqual(GetStamina() - DeltaValue, GetMaxStamina()) : false;
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
		if (GASCharacter && !StaminaOverflow) {
			GASCharacter->HandleStaminaChange(DeltaValue, Data.EffectSpec.GetContext().GetInstigator());
		}
	}



}

void UGASCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetMaxHealthAttribute()) {
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	if (Attribute == GetMaxStaminaAttribute()) {
		AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetStaminaAttribute());
	}
}

void UGASCharacterAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilitySystemComponent = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();

	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilitySystemComponent) {

		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = CurrentValue > 0.f ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;
		AbilitySystemComponent->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);

	}


}

void UGASCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCharacterAttributeSet, Health, OldHealth);
}

void UGASCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCharacterAttributeSet, MaxHealth, OldMaxHealth);
}

void UGASCharacterAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCharacterAttributeSet, Stamina, OldStamina);
}

void UGASCharacterAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCharacterAttributeSet, MaxStamina, OldMaxStamina);
}

