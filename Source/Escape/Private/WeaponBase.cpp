#include "WeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"



AWeaponBase::AWeaponBase()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));


    Mesh->SetupAttachment(Root);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


    HitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollision"));
    HitCollision->SetupAttachment(Mesh);
    HitCollision->SetCollisionObjectType(ECC_GameTraceChannel1); 
    HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    HitCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    HitCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);


   


}

void AWeaponBase::BeginPlay()
{
    Super::BeginPlay();
    if (UPrimitiveComponent* Collision = Cast<UPrimitiveComponent>(HitCollision))
    {
        Collision->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnHitOverlap);
    }
    

   
    

    IgnorePlayerCollisions();



}

void AWeaponBase::IgnorePlayerCollisions() {

    if (AActor* MyOwner = GetOwner())
    {

        


        if (Mesh)
        {
            
            Mesh->IgnoreActorWhenMoving(MyOwner, true);
            //Mesh->IgnoreActorWhenMoving(this, true);
            UE_LOG(LogTemp, Error, TEXT("[%s] Ignoring the following : %s  บบ %s"), *Mesh->GetName(), *this->GetName(), *MyOwner->GetName());

        }

        if (HitCollision)
        {
            HitCollision->IgnoreActorWhenMoving(MyOwner, true);
            HitCollision->IgnoreActorWhenMoving(this, true);
            UE_LOG(LogTemp, Error, TEXT("[%s] Ignoring the following : %s  บบ %s"), *HitCollision->GetName(), *this->GetName(), *MyOwner->GetName());
        }
    }
    if (WeaponOwner)
    {
        if (Mesh)
        {
            Mesh->IgnoreActorWhenMoving(WeaponOwner, true);
        }

        if (HitCollision)
        {
            HitCollision->IgnoreActorWhenMoving(WeaponOwner, true);
            HitCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
        }
    }


}


void AWeaponBase::StartHitDetection()
{

    UE_LOG(LogTemp, Warning, TEXT("Start Detect "));

    if (!HitCollision) return;

    bIsDetectingHits = true;

    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
    Mesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);

    HitCollision->IgnoreActorWhenMoving(GetOwner(), true);
    HitCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    HitCollision->bHiddenInGame = false;

    UE_LOG(LogTemp, Display, TEXT("ASASAS"));

    TArray<AActor*> IgnoredActors = HitCollision->MoveIgnoreActors;

    if (IgnoredActors.IsEmpty()) {
        
        UE_LOG(LogTemp, Warning, TEXT("Ignored Actors: EMPTY"));
    }
    
    
    for (AActor* Ignored : IgnoredActors)
    {
        if (IsValid(Ignored))
        {
            UE_LOG(LogTemp, Warning, TEXT("Ignored Actor: %s"), *Ignored->GetName());
        }
    }



    AlreadyHitActors.Empty();
    bIsDetectingHits = true;

    UE_LOG(LogTemp, Log, TEXT("[%s] Hit detection started"), *GetName());
}

void AWeaponBase::StopHitDetection()
{
    if (!HitCollision) return;
    if (!bIsDetectingHits) return;

    
    HitCollision->bHiddenInGame = true;

    bIsDetectingHits = false;
    HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    UE_LOG(LogTemp, Log, TEXT("[%s] Hit detection stopped"), *GetName());
}

void AWeaponBase::OnHitOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)


{


    //UE_LOG(LogTemp, Log, TEXT("[%s] Hit ___actor: %s  บบ %s"), *this->GetName(), *OtherActor->GetName(), *GetOwner()->GetName());
   
    if (!bIsDetectingHits || !OtherActor || AlreadyHitActors.Contains(OtherActor))
        return;

    AlreadyHitActors.Add(OtherActor);
    UE_LOG(LogTemp, Log, TEXT("[%s] Hit actor: %s"), *GetName(), *OtherActor->GetName());

    OnWeaponHit.Broadcast(SweepResult);
}

void AWeaponBase::ResetHitList()
{
    AlreadyHitActors.Empty();
}