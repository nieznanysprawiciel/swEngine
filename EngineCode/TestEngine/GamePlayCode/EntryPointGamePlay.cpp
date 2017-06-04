#include "TestEngine/stdafx.h"
#include "EntryPointGamePlay.h"

#include "swCommonLib/Serialization/PropertySerialization/EngineSerializationContext.h"
#include "swCommonLib/Serialization/Serializer.h"
#include "EngineCore/MainEngine/SWEngineInclude.h"

#include "PlayerSignalTest.h"

#include "EngineCore/EventsManager/Events/KeyDownEvent.h"



EntryPointGamePlay::EntryPointGamePlay()
{
}


EntryPointGamePlay::~EntryPointGamePlay()
{
}


void EntryPointGamePlay::ProceedGameLogic( float time_interval )
{

}

/*Funkcja wywo�ywana w momencie wczytywania levelu.
Zako�czona poprawnie powinna zwr�ci� warto�� 0, w innych wypadkach inn�.*/
int EntryPointGamePlay::LoadLevel( )
{
	const wchar_t CLONE_FIGHTER[] = L"tylko_do_testow/ARC.FBX";
	m_engine->Assets.Meshes.LoadSync( CLONE_FIGHTER );
	
	m_engine->Actors.RegisterClass< PlayerSignalTest >( sw::GetTypeidName< PlayerSignalTest >(), PlayerSignalTest::Create );

	auto actor1 = m_engine->Actors.CreateActor< PlayerSignalTest >( sw::GetTypeidName< PlayerSignalTest >(), sw::EnableDisplay );
	auto actor2 = m_engine->Actors.CreateActor< PlayerSignalTest >( sw::GetTypeidName< PlayerSignalTest >(), sw::EnableDisplay );

	actor1->SetOtherPlayer( actor2 );
	actor2->SetOtherPlayer( actor1 );


	DirectX::XMVECTOR position1 = DirectX::XMVectorSet( 4000.0, 0.0, 8000.0, 0.0 );
	actor1->Teleport( position1 );
	actor1->SetModel( m_engine->Assets.Meshes.GetSync( CLONE_FIGHTER ) );

	DirectX::XMVECTOR position2 = DirectX::XMVectorSet( 8000.0f, 0.0, 4000.0, 0.0 );
	actor2->Teleport( position2 );
	actor2->SetModel( m_engine->Assets.Meshes.GetSync( CLONE_FIGHTER ) );

	// Events
	auto layer = m_engine->Input.GetStandardAbstractionLayer( sw::STANDARD_ABSTRACTION_LAYER::PROTOTYPE_BUTTONS );
	layer->DemandDownEvent( sw::STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS1 );

	m_engine->Actors.Communication.AddListenerDelayed< sw::KeyDownEvent, PlayerSignalTest >( actor1, &PlayerSignalTest::InitJob );

	// Serialization test
	auto context = std::make_unique< EngineSerializationContext >();
	context->SaveWholeMap = true;

	ISerializer* ser = new ISerializer( std::move( context ) );

	actor1->Serialize( ser );

	ser->SaveFile( "levels/SerializationTest.swmap", WritingMode::Readable );

	return 0;
}

/*Funkcja wywo�ywana w momencie zako�czenia levelu do usuni�cia niepotrzebnych obiekt�w.
Zako�czona poprawnie powinna zwr�ci� warto�� 0, w innych wypadkach inn�.*/
int EntryPointGamePlay::UnloadLevel( )
{
	//m_engine->Actors.Communication.RemoveListenerDelayed< KeyDownEvent >( actor1 );


	return 0;
}
