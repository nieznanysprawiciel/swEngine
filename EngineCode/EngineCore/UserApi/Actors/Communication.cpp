#include "EngineCore/stdafx.h"
#include "Communication.h"

#include "EngineCore/MainEngine/EngineContext.h"


namespace Api { namespace Actors
{



/**@copydoc EventManager::AddListener*/
void Communication::AddListenerDelayed		( rttr::type eventType, const EngineObject* handlerOwner, EventDelegate handler )
{
	Context->eventsManager->AddListener( eventType, handlerOwner, handler );
}

/**@copydoc EventManager::RemoveListener*/
void Communication::RemoveListenerDelayed	( rttr::type eventType, const EngineObject* handlerOwner )
{
	Context->eventsManager->RemoveListener( eventType, handlerOwner );
}

/**@copydoc EventManager::ClearListeners*/
void Communication::ClearListenersDelayed	()
{
	Context->eventsManager->ClearListeners();
}

/**@brief EventManager::SendEvent*/
void Communication::SendEvent				( Event * evt )
{
	Context->eventsManager->SendEvent( evt );
}

}	// Actors
}	// Api