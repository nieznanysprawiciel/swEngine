#pragma once
/**@file Renderer.h

Ten plik wybiera renderer, kt�ry b�dzie u�ywany do renderowania.
Ka�dy renderer definiuje siebie jako typ Renderer. W zale�no�ci od prze��cznika ENABLE_RENDERER
za��czany jest plik z odpowiednim rendererem.

@attention Nigdy nie nale�y includowa� renderera bezpo�rednio. Zawsze za po�rednictwem tego pliku.*/

#include "macros_switches.h"
#include "RendererConstants.h"

#define RENDERER_H_INCLUDE


#if ENABLE_RENDERER == DX11
	#include "DisplayEngine\Renderer\DX11Renderer.h"
#else
	#error Renderer nie zosta� ustawiony. Zdefiniuj makro ENABLE_RENDERER w pliku macros_switches.h.
#endif

#undef RENDERER_H_INCLUDE
