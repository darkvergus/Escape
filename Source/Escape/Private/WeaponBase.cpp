#include "WeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>



AWeaponBase::AWeaponBase()
{
    PrimaryActorTick.bCanEverTick = true;
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));


    Mesh->SetupAttachment(Root);
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    Mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
    Mesh->SetCollisionResponseToChannel(ECC_WorldStatic , ECR_Block);


    HitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollision"));
    HitCollision->SetupAttachment(Mesh);
    HitCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    HitCollision->SetGenerateOverlapEvents(false);
    HitCollision->SetHiddenInGame(true);
}


void AWeaponBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bIsDetectingHits)
    {
        PerformHitTrace();
    }
}



void AWeaponBase::BeginPlay()
{
    Super::BeginPlay();
    if (UPrimitiveComponent* Collision = Cast<UPrimitiveComponent>(HitCollision))
    {
        Collision->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnHitOverlap);
    }
    

   
    

    //IgnorePlayerCollisions();



}


void AWeaponBase::PerformHitTrace() {

    UE_LOG(LogTemp, Log, TEXT("Tracing....."));
    FVector Start = PrevLocation;
    FVector End = HitCollision->GetComponentLocation();
    UE_LOG(LogTemp, Log, TEXT("Tracing..... (%s)"), *Start.ToString());

    FCollisionShape Shape; 

    if (UBoxComponent* Box = Cast<UBoxComponent>(HitCollision))
    {
        Shape = FCollisionShape::MakeBox(Box->GetScaledBoxExtent());
    }
    else if (USphereComponent* Sphere = Cast<USphereComponent>(HitCollision))
    {
        Shape = FCollisionShape::MakeSphere(Sphere->GetScaledSphereRadius());
    }

    if (bDebugTraces) DebugTrace(Start, End, debugDuration );


    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetOwner());
    Params.AddIgnoredActor(this);


    bool bHit = GetWorld()->SweepSingleByChannel(
        Hit,
        Start,
        End,
        HitCollision->GetComponentQuat(),
        ECC_GameTraceChannel1,   // Your custom "WeaponHit" channel
        Shape,
        Params
    );

    

    if (bHit && !AlreadyHitActors.Contains(Hit.GetActor()))
    {
        AlreadyHitActors.Add(Hit.GetActor());
        OnWeaponHit.Broadcast(Hit);
        if (bDebugTraces) DebugHit(Hit, debugDuration);

    }


    PrevLocation = End;
    PrevRotation = HitCollision->GetComponentQuat();
}


void AWeaponBase::DebugTrace(FVector Start, FVector End,float duration = 0.05f) {


    if (UBoxComponent* Box = Cast<UBoxComponent>(HitCollision))
    {
        FVector Extent = Box->GetScaledBoxExtent();

        // Draw start box
        DrawDebugBox(
            GetWorld(),
            Start,
            Extent,
            PrevRotation,
            FColor::Green,
            false,
            0.05f // stays visible for 0.05 sec
        );

        // Draw end box
        DrawDebugBox(
            GetWorld(),
            End,
            Extent,
            HitCollision->GetComponentQuat(),
            FColor::Blue,
            false,
            0.05f
        );

        // Draw sweep line
        DrawDebugLine(
            GetWorld(),
            Start,
            End,
            FColor::Yellow,
            false,
            0.05f
        );
    }

    if (USphereComponent* Sphere = Cast<USphereComponent>(HitCollision))
    {
        float Radius = Sphere->GetScaledSphereRadius();

        DrawDebugSphere(
            GetWorld(),
            Start,
            Radius,
            12,
            FColor::Green,
            false,
            0.05f
        );

        DrawDebugSphere(
            GetWorld(),
            End,
            Radius,
            12,
            FColor::Blue,
            false,
            0.05f
        );

        DrawDebugLine(
            GetWorld(),
            Start,
            End,
            FColor::Yellow,
            false,
            0.05f
        );
    }

}

void AWeaponBase::DebugHit(FHitResult Hit, float duration) {


}





void AWeaponBase::StartHitDetection()
{
    if (!HitCollision) return;

    PrevLocation = HitCollision->GetComponentLocation();
    PrevRotation = HitCollision->GetComponentQuat();
       
    AlreadyHitActors.Empty();
    
    bIsDetectingHits = true;

    UE_LOG(LogTemp, Log, TEXT("[%s] Hit detection started"), *GetName());
}

void AWeaponBase::StopHitDetection()
{
    if (!HitCollision) return;
    if (!bIsDetectingHits) return;
   
    bIsDetectingHits = false ;

    UE_LOG(LogTemp, Log, TEXT("[%s] Hit detection stopped"), *GetName());
}

void AWeaponBase::OnHitOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{


    UE_LOG(LogTemp, Log, TEXT("[%s_____%s] Hit ___actor: %s  บบ %s"), *this->GetName(), *OverlappedComp->GetName(), *OtherActor->GetName(), *GetOwner()->GetName());
   
    //if (!bIsDetectingHits || !OtherActor || AlreadyHitActors.Contains(OtherActor))
    //    return;

    //AlreadyHitActors.Add(OtherActor);
    //UE_LOG(LogTemp, Log, TEXT("[%s] Hit actor: %s"), *GetName(), *OtherActor->GetName());

    //OnWeaponHit.Broadcast(SweepResult);
}

void AWeaponBase::ResetHitList()
{
    AlreadyHitActors.Empty();
}