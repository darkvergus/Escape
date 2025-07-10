#include "GASCharacter.h"
#include "GASCharacterAttributeSet.h" 

AGASCharacter::AGASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UGASCharacterAttributeSet>("AttributeSet");
	TraitManager = CreateDefaultSubobject<UTraitManagerComponent>("TraitManager");
	MentalResolver = CreateDefaultSubobject<UMentalStateResolverComponent>("MentalResolver");

	CharacterLevel = 1;
	DefaultAttributeEffects = TArray<TSubclassOf<UGameplayEffect>>();
	AttackAbilityList = TArray<TSubclassOf<UGameplayAbility>>();
}

int32 AGASCharacter::GetCharacterLevel() const
{
	return CharacterLevel;
}

float AGASCharacter::GetHealth() const
{
	if (!AttributeSet) {
		return 0.0f;
	}

	return AttributeSet->GetHealth();
}

float AGASCharacter::GetMaxHealth() const
{
	if (!AttributeSet) 
	{
		return 0.0f;
	}

	return AttributeSet->GetMaxHealth();
}

float AGASCharacter::GetStamina() const 
{
	if (!AttributeSet) 
	{
		return 0.0f;
	}

	return AttributeSet->GetStamina();
}

float AGASCharacter::GetMaxStamina() const 
{
	if (!AttributeSet) 
	{
		return 0.0f;
	}

	return AttributeSet->GetMaxStamina();
}

bool AGASCharacter::ActivateAbilitiesWithTag(FGameplayTagContainer AbilityTags, bool AllowRemoteActivation)
{
	if (!AbilitySystemComponent)
	{
		return false;
	}

	return AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags, AllowRemoteActivation);
}

void AGASCharacter::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if (MentalResolver)
	{
		MentalResolver->Initialise(AbilitySystemComponent);
	}

	if (HasAuthority() && TraitManager)
	{
		TraitManager->OnArchetypeApplied.AddDynamic(this, &AGASCharacter::HandleArchetypeApplied);

		if (Asset.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("[Traits] Applying direct archetype %s"), *Asset.GetAssetName());
			TraitManager->ApplyArchetype(Asset.Get());
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("[Traits] Rolling archetype from list"));
			TraitManager->RollArchetypes(1);
		}
	}
}

void AGASCharacter::SetTestAbilities() 
{
	if (!AbilitySystemComponent) 
	{
		return;
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		for (TSubclassOf<UGameplayAbility>& TestAbility : TestAbilities) 
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(TestAbility, GetCharacterLevel(), INDEX_NONE, this));
		}
	}
}

void AGASCharacter::SetAttackAbilities()
{
	if (!AbilitySystemComponent) 
	{
		return;
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		for (TSubclassOf<UGameplayAbility>& AttackAbility : AttackAbilityList) 
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AttackAbility, GetCharacterLevel(), INDEX_NONE, this));
		}
	}
}

void AGASCharacter::HandleArchetypeApplied(UArchetypeDefinition* Arch)
{
	if (!Arch)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[Traits] Archetype applied: %s"), *Arch->GetName());

	if (MentalResolver)
	{
		MentalResolver->Recompute();
	}
}

void AGASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!AbilitySystemComponent) 
	{
		return;
	}
	if (EnableTestAbilities) 
	{
		SetTestAbilities();
	}

	ApplyDefaultAttributeEffects();
	SetAttackAbilities();
}

UAbilitySystemComponent* AGASCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGASCharacter::HandleHealthChange(float DeltaValue, AActor* Origin)
{
	OnHealthChanged(DeltaValue, Origin);
	if (GetHealth() <= 0.f) 
	{
		OnDead();
	}
}

void AGASCharacter::HandleStaminaChange(float DeltaValue, AActor* Origin)
{
	OnStaminaChanged(DeltaValue, Origin);
	//if (GetHealth() <= 0.f) {
	//	OnDead();
	//}
}

void AGASCharacter::ApplyDefaultAttributeEffects()
{
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent is null!"));
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf <class UGameplayEffect>& DefaultEffect : DefaultAttributeEffects) 
	{
		//FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffects, CharacterLevel, EffectContext);

		if (!IsValid(DefaultEffect))
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid Gameplay Effect in DefaultAttributeEffects!"));
			continue;
		}

		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultEffect, CharacterLevel, EffectContext);
		if (NewHandle.IsValid()) 
		{
			FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
	}
}

void AGASCharacter::RemoveDefaultAttributeEffects() 
{
	FGameplayEffectQuery Query;
	Query.EffectSource = this;
	AbilitySystemComponent->RemoveActiveEffects(Query);
}