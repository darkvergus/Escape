#include "HealthComponent.h"
#include "GameplayTagContainer.h"
#include "MentalStateResolverComponent.h"
#include "GASAbilitySystemComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentHealth = MaxHealth;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
}

void UHealthComponent::TakeDamage(float DamageAmount)
{
	if (DamageAmount <= 0.0f || !IsAlive())
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.0f)
	{
		Die();
	}

	BroadcastHPState();
}

void UHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f || !IsAlive())
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	BroadcastHPState();
}

void UHealthComponent::SetHealth(float NewHealth)
{
	CurrentHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.0f)
	{
		Die();
	}

	BroadcastHPState();
}

float UHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

bool UHealthComponent::IsAlive() const
{
	return CurrentHealth > 0.0f;
}

void UHealthComponent::BroadcastHPState()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	UAbilitySystemComponent* ASC = Owner->FindComponentByClass<UAbilitySystemComponent>();
	if (!ASC)
	{
		return;
	}

	const float Percent = (MaxHealth > 0.f) ? (CurrentHealth / MaxHealth) : 0.f;

	static const FGameplayTag tagHpHealthy = FGameplayTag::RequestGameplayTag(TEXT("HP.Healthy"));
	static const FGameplayTag tagHpInjured = FGameplayTag::RequestGameplayTag(TEXT("HP.Injured"));
	static const FGameplayTag tagHpLow = FGameplayTag::RequestGameplayTag(TEXT("HP.Low"));

	FGameplayTagContainer OldTags;
	OldTags.AddTag(tagHpHealthy);
	OldTags.AddTag(tagHpInjured);
	OldTags.AddTag(tagHpLow);
	ASC->RemoveLooseGameplayTags(OldTags);

	const FGameplayTag NewHP = (Percent > 0.75f) ? tagHpHealthy : (Percent > 0.30f) ? tagHpInjured : tagHpLow;

	ASC->AddLooseGameplayTag(NewHP);

	if (UMentalStateResolverComponent* Resolver = Owner->FindComponentByClass<UMentalStateResolverComponent>())
	{
		Resolver->Recompute();
	}
}

void UHealthComponent::Die()
{
	OnDeath.Broadcast();
}