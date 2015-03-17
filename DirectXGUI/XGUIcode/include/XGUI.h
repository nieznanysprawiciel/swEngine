/**
@file XGUI.h
@brief Ten plik nale�y za��czy� w projekcie u�ywaj�cym biblioteki DirectXGUI.
Zawiera podstawowe deklaracje.*/

//#include <D3DX11.h>
#include <DirectXMath.h>
#include "FastDelegate.h"
#include "GUIEvent.h"


namespace XGUI
{
	class Control;
	class Root;

	/**@typedef fastdelegate::FastDelegate1<GUIEvent*> XGUIEventDelegate
	@brief Definicja delegata u�ywanego przez XGUI.

	Aby zdarzenia generowane wewnatrz GUI mog�y zosta� obs�u�one na zewn�trz,
	u�ytkownik GUI musi przypisa� temu delegatowi odpowiedni� funkcj� obs�ugi.
	Funkcja ta dostaje w parametrze wska�nik na struktur� @ref Event, kt�ra zawiera
	wszystkie informacje o danym zdarzeniu. Wska�nik ten trzeba sobie zrzutowa� w funkcji
	obs�ugi na wska�nik na jeden z obiekt�w pochodnych odpowiadaj�cych konkretnemu typowi zdarzenia.

	Ustawienie delegata mo�na przeprowadzi� przypisuj�c mu po prostu odpowiedni� funkcj� (delegaci s� sk�adowymi publicznymi).
	Dla funkcji globalnych i metod statycznych dost�pne s� opcje:
	- Delegat.bind(&Funkcja);
	- Delegat.bind(&Klasa::MetodaStatyczna);
	- Delegat = &FunkcjaGlobalna;
	- Delegat = &Klasa::MetodaStatyczna;

	Dla normalnych metod sk�adowych:
	- Delegat.bind(&ObjektKlasy, &Klasa::Metoda);
	- Delegat = fastdelegate::MakeDelegate(&ObjektKlasy, &Klasa::Metoda);

	@attention W przypadku implementowania w�asnych klas GUI, nie nale�y u�ywa� delegat�w do
	implementowania wewn�trzych funkcjonalno�ci jakiej� kontrolki. Do takiego celu s� przygotowane
	specjalne funkcje virtualne, kt�re mo�na napisa�.

	Konwencja nazewnictwa jest nast�puj�ca:
	- delegaci maj� nazwy postaci event[nazwa_zdarzenia]
	- funkcje wewn�trzne obs�ugi zdarze� dla kontrolek maj� nazwy on[nazwa_zdarzenia]

	Oba typy funkcji s� wywo�ywane w tych samych okoliczno�ciach. Najpierw jest wywo�ywana obs�uga funkcji
	wewn�trznej, a dopiero potem zdarzenie zewn�trzne (zak�adaj�c, �e oba typy b��ugi s� przez kontrolk� u�ywane).

	@attention Je�eli jakie� niuanse sprawi�, �e zdarzenia o tej samej nazwie nie b�d� wywo�ywane w tych samych
	okoliczno�ciach to trzeba nada� im inne nazwy.
	*/
	typedef fastdelegate::FastDelegate1<GUIEvent*> XGUIEventDelegate;

	/**@brief Struktura opisuj�ca czworok�t.*/
	struct Rect
	{
		DirectX::XMFLOAT2 top_left;		///<Wsp�rz�dne lewego g�rnego rogu.
		DirectX::XMFLOAT2 bottom_right;	///<Wsp�rz�dne prawego dolnego rogu.
	};


	/**@brief Enumeracja opisuj�ca mo�liwe wyr�wnanie kontrolek wzgl�dem rodzica.
	
	Zmienne enumeracj� mo�na sk�ada� za pomoca operatora sumy bitowej.
	U�ytkownik jest odpowiedzialny za sk�adanie zmiennej z sensem.*/
	enum ALIGNMENT
	{
		LEFT				= 1,
		RIGHT				= 2,
		TOP					= 4,
		BOTTOM				= 8,
		VERTICAL_CENTER		= 16,
		HORIZONTAL_CENTER	= 32,
		TOP_LEFT			= LEFT | TOP,
		TOP_RIGHT			= RIGHT | TOP,
		CENTER_LEFT			= VERTICAL_CENTER | LEFT,
		CENTER_RIGHT		= VERTICAL_CENTER | RIGHT,
		BOTTOM_LEFT			= BOTTOM | LEFT,
		BOTTOM_RIGHT		= BOTTOM | RIGHT,
		TOP_CENTER			= TOP | HORIZONTAL_CENTER,
		BOTTOM_CENTER		= BOTTOM | HORIZONTAL_CENTER
	};


	/**@brief Klasa bazowa dla wszystkich kontrolek.*/
	class Control
	{
	private:
		static Root*			root;		///<Korze� drzewa kontrolek b�d�cy jednocze�nie zarz�dc� ca�ego GUI

	protected:
		Control*				parent;		///<Rodzic kontrolki.

		bool					visible;	///<Widoczno�� kontrolki
		bool					mouse_on;	///<Przy ostatnim sprawdzaniu myszka by�a w obszarze kontrolki.
		bool					focus_change_order;	///<W przypadku dostania focusa kontrolka mo�e zosta� przeniesiona na pocz�tek listy, �eby by� renderowan� na wierzchu.

		DirectX::XMFLOAT2		relative_position;	///<Pozycja wzgl�dem rodzica (we wsp�rz�dnych ekranu [-1;1])
		DirectX::XMFLOAT2		dimension;	///<Rozmiar kontrolki (we wsp�rz�dnych ekranu [-1;1])
		ALIGNMENT				align;		///<Identyfikuje wzgl�dem czego s� podawane wsp�rz�dne position
	public:
		Control( Control* set_parent ) { parent = set_parent; }

		XGUIEventDelegate		eventMouseOn;
		XGUIEventDelegate		eventMouseOut;
		XGUIEventDelegate		eventLeftClick;
		XGUIEventDelegate		eventLeftUnClick;
		XGUIEventDelegate		eventRightClick;
		XGUIEventDelegate		eventRightUnClick;
		XGUIEventDelegate		eventFocusSet;
		XGUIEventDelegate		eventFocusLost;


		inline bool isMouseOn() { return mouse_on; }	///<Funkcja zwraca warto�� zmiennej @ref mouse_on
		inline bool isVisible() { return visible; }		///<Funkcja zwraca warto�� pola @ref visible
		inline bool isChangeOrder() { return focus_change_order; } ///<Zwraca warto�� pola @ref focus_change_order
		inline Control* getParent() { return parent; }	///<Zwraca rodzica kontrolki
		inline ALIGNMENT getAlignment() { return align; }	///<Zwraca enum opisuj�cy wyr�wnanie kontrolki wzgl�dem rodzica
		inline DirectX::XMFLOAT2 getDimension() { return dimension; }	///<Zwraca rozmiar kontrolki
		inline DirectX::XMFLOAT2 getRelativePosition() { return relative_position; }	///<Zwraca pozycj� kontrolki wzgl�dem rodzica


		/**@brief Funkcja sprawdza czy mysz znajduje si� wewn�trz obszaru danej kontrolki.

		Domy�lnie zaimplementowane jest sprawdzanie czy kursor znajduje si� w odpowiednim
		prostok�cie. Je�eli kszta�t kontrolki jest inny, mo�na t� funkcj� przeimplementowa�
		dla tego kszta�tu.

		Po�o�enie kontrolki jest obliczane wzgledem podanego prostok�ta w parametrze clipping_rect.
		Zmienna align m�wi o tym, wzgl�dem kt�rej kraw�dzi zdefiniowane jest po�o�enie
		w wektorze relative_position.

		@attention
		Obiekt nie sprawdza czy mysz znalaz�a si� w jego obszarze, je�eli nie jest widoczny.

		@note
		Funkcja nie powinna zmienia� stanu obiektu. Po sprawdzeniu czy mysz jest w obszarze
		kontrolki nast�puj� najcz�ciej wywo�anie funkcji on_mouse_reaction. Tam nale�y implementowa�
		reakcj� na mysz.

		@note
		Nie zawsze musimy zwr�ci� prawdziw� informacj�. Je�eli chcemy zaimplementowa� zmian�
		rozmiar�w kontrolki, to mysz cz�sto b�dzie wyje�d�a� poza jej obszar. Wtedy musimy
		zwr�ci� true, mimo �e mysz nie znajduje si� w naszym obszarze.

		@param point Punkt w pikselach, w kt�rych znajduje si� mysz.
		@param clipping_rect Obszar rodzica wzgl�dem kt�rego jest liczone po�o�enie kontrolki.
		@return Zwraca true, je�eli mysz znajduje si� w obszarze zajmowanym prze kontrolk�.
		*/
		virtual bool test_mouse( const DirectX::XMFLOAT2& point, const Rect& clipping_rect);

		/**@brief Funkcja implementuje reakcj� kontrolki na to, �e mysz znalaz�a si� w jej obszarze.

		Najpierw kontrolka jest sprawdzana funkcj� test_mouse. Je�eli zwr�ci true, to powinna zosta�
		wywo�ana funkcja on_mouse_reaction, w kt�rej znajduje si� reakcja na najechanie mysz�.

		@param point Punkt w pikselach, w kt�rych znajduje si� mysz.
		@param clipping_rect Obszar rodzica wzgl�dem kt�rego jest liczone po�o�enie kontrolki.
		@param buttons Tablica przycisk�w. @todo: Trzeba wymy�le� format tej tablicy
		*/
		virtual void on_mouse_reaction( const DirectX::XMFLOAT2& point, const Rect& clipping_rect, const char* buttons );

		/**@brief Funkcja zostaje wywo�ana, kiedy mysz najedzie na kontrolk�. Wywo�ywana tylko
		raz za pierwszym razem, kiedy zostanie wykryta mysz nad kontrolk�.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiej� typowej dla siebie funkcjonalno�ci,
		wywo�ywanej przez najechanie mysz�, bez przedefiniowywania funkcji on_mouse_reaction.
		Tak� funkcjonalno�ci� mo�e by� np. zmiana wygl�du kontrolki po najechaniu i po zjechaniu myszy
		z jej obszaru ( @ref onMouseOut ).

		@attention
		Do renderingu przeznaczona jest tylko i wy��cznie funkcja @ref draw_clipped. W tej funkcji mo�na
		co najwy�ej ustawi� odpowiedni stan obiektu, �eby p�niej zosta� on poprawnie wyrenderowany.

		@note
		Informacja o zaj�ciu zdarzenia zostaje r�wnie� przekazana na zewn�trz GUI w postaci wywo�ania
		odpowiedniej funkcji obs�ugi lub wys�ania eventu do kolejki komunikat�w. Nie nale�y samodzielnie
		implementowa� takiego zachowania, poniewa� jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param point Punkt w pikselach, w kt�rych znajduje si� mysz.
		@param clipping_rect Obszar rodzica wzgl�dem kt�rego jest liczone po�o�enie kontrolki.
		@param buttons Tablica przycisk�w. @todo: Trzeba wymy�le� format tej tablicy
		*/
		virtual void onMouseOn( const DirectX::XMFLOAT2& point, const Rect& clipping_rect, const char* buttons ) = 0;
		
		/**@brief Funkcja wywo�ywana w momencie przekazania kontrolce focusa.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiej� typowej dla siebie funkcjonalno�ci
		w reakcji na otrzymanie focusa.

		@note
		Informacja o zaj�ciu zdarzenia zostaje r�wnie� przekazana na zewn�trz GUI w postaci wywo�ania
		odpowiedniej funkcji obs�ugi lub wys�ania eventu do kolejki komunikat�w. Nie nale�y samodzielnie
		implementowa� takiego zachowania, poniewa� jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param prev_focus Zwraca wska�nik na kontrolk�, kt�ra poprzednio mia�a ustawionego focusa.
		*/
		virtual void onFocusSet( Control* prev_focus ) = 0;

		/**@brief Funkcja wywo�ywana w momencie, gdy kontrolka traci focusa.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiej� typowej dla siebie funkcjonalno�ci
		w reakcji na utrat� focusa.

		@attention
		Funkcja nie powinna pr�bowa� przywraca� sobie focusa. Istanieje niebezpiecze�stwo zap�tlenia
		lub nieprzewidywalnego zachowania, je�eli dwie kotrolki b�d� pr�bowa�y wykona� t� operacj�.

		@note
		Informacja o zaj�ciu zdarzenia zostaje r�wnie� przekazana na zewn�trz GUI w postaci wywo�ania
		odpowiedniej funkcji obs�ugi lub wys�ania eventu do kolejki komunikat�w. Nie nale�y samodzielnie
		implementowa� takiego zachowania, poniewa� jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param next_focus Wska�nik na kontrolk�, kt�ra otrzyma�a focusa.
		*/
		virtual void onFocusLost( Control* next_focus ) = 0;

		/**@brief Funkcja wywo�ywana w momencie, gdy kontrolka zostanie klikni�ta lewym przyciskiem myszy.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiej� typowej dla siebie funkcjonalno�ci
		w reakcji na klikni�cie.

		@note
		Informacja o zaj�ciu zdarzenia zostaje r�wnie� przekazana na zewn�trz GUI w postaci wywo�ania
		odpowiedniej funkcji obs�ugi lub wys�ania eventu do kolejki komunikat�w. Nie nale�y samodzielnie
		implementowa� takiego zachowania, poniewa� jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param point Punkt w pikselach, w kt�rych znajduje si� mysz.
		@param clipping_rect Obszar rodzica wzgl�dem kt�rego jest liczone po�o�enie kontrolki.
		@param buttons Tablica przycisk�w. @todo: Trzeba wymy�le� format tej tablicy
		*/
		virtual void onLeftClick( const DirectX::XMFLOAT2& point, const Rect& clipping_rect, const char* buttons );

		/**@brief Lewy przycisk myszy zosta� puszczony.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiej� typowej dla siebie funkcjonalno�ci
		w reakcji na puszczenie przycisku.

		@attention
		Funkcja nie zostanie wywo�ana, je�eli puszczenie przycisku nie nast�pi nad kontrolk�. W przypadku
		przeniesienia si� znad obszaru jednej kontrolki nad inn� z wci�ni�tym przyciskiem, pierwsza kontrolka
		dostanie zdarzenie click, a druga un_click.

		@note
		Informacja o zaj�ciu zdarzenia zostaje r�wnie� przekazana na zewn�trz GUI w postaci wywo�ania
		odpowiedniej funkcji obs�ugi lub wys�ania eventu do kolejki komunikat�w. Nie nale�y samodzielnie
		implementowa� takiego zachowania, poniewa� jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param point Punkt w pikselach, w kt�rych znajduje si� mysz.
		@param clipping_rect Obszar rodzica wzgl�dem kt�rego jest liczone po�o�enie kontrolki.
		@param buttons Tablica przycisk�w. @todo: Trzeba wymy�le� format tej tablicy
		*/
		virtual void onLeftUnClick( const DirectX::XMFLOAT2& point, const Rect& clipping_rect, const char* buttons );

		/**@brief Funkcja wywo�ywana w momencie, gdy kontrolka zostanie klikni�ta prawym przyciskiem myszy.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiej� typowej dla siebie funkcjonalno�ci
		w reakcji na klikni�cie.

		@note
		Informacja o zaj�ciu zdarzenia zostaje r�wnie� przekazana na zewn�trz GUI w postaci wywo�ania
		odpowiedniej funkcji obs�ugi lub wys�ania eventu do kolejki komunikat�w. Nie nale�y samodzielnie
		implementowa� takiego zachowania, poniewa� jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param point Punkt w pikselach, w kt�rych znajduje si� mysz.
		@param clipping_rect Obszar rodzica wzgl�dem kt�rego jest liczone po�o�enie kontrolki.
		@param buttons Tablica przycisk�w. @todo: Trzeba wymy�le� format tej tablicy
		*/
		virtual void onRightClick( const DirectX::XMFLOAT2& point, const Rect& clipping_rect, const char* buttons );

		/**@brief Prawy przycisk myszy zosta� puszczony.

		Funkcja pozwala dowolnej kontrolce na zaimplementowanie jakiej� typowej dla siebie funkcjonalno�ci
		w reakcji na puszczenie przycisku.

		@attention
		Funkcja nie zostanie wywo�ana, je�eli puszczenie przycisku nie nast�pi nad kontrolk�. W przypadku 
		przeniesienia si� znad obszaru jednej kontrolki nad inn� z wci�ni�tym przyciskiem, pierwsza kontrolka
		dostanie zdarzenie click, a druga un_click.

		@note
		Informacja o zaj�ciu zdarzenia zostaje r�wnie� przekazana na zewn�trz GUI w postaci wywo�ania
		odpowiedniej funkcji obs�ugi lub wys�ania eventu do kolejki komunikat�w. Nie nale�y samodzielnie
		implementowa� takiego zachowania, poniewa� jest ono zaimplementowane w funkcji on_mouse_reaction.

		@param point Punkt w pikselach, w kt�rych znajduje si� mysz.
		@param clipping_rect Obszar rodzica wzgl�dem kt�rego jest liczone po�o�enie kontrolki.
		@param buttons Tablica przycisk�w. @todo: Trzeba wymy�le� format tej tablicy
		*/
		virtual void onRightUnClick( const DirectX::XMFLOAT2& point, const Rect& clipping_rect, const char* buttons );

		/**@brief Funkcja u�ywana do zakomunikowania dzieciom jakiej� kontrolki, �e mysz opu�ci�a
		obszar rodzica.

		Funkcja jest wywo�ywana w trzech sytuacjach:
		- Rodzic kontrolki przy poprzednim wywo�aniu test_mouse mia� nad sob� kursor myszy, a w kolejnym
		go utraci�. Wtedy musi on wywo�a� onMouseOut dla wszystkich swoich dzieci, kt�re tak�e mia�y
		nad sob� kursor, sprawdzaj�c warto�� zmiennej mouse_on.
		- Rodzic kontrolki otrzyma� komunikat mouse_out, wi�c przekazuje je wszystkim potomkom, kt�rzy
		maj� ustawion� zmienn� mouse_on na true.
		- Rodzic odpytuj�c swoje dzieci znalaz� ju� kontrolk� w kt�rej obszarze znajduje si� mysz.
		Musi teraz poinformowa� rodze�stwo tamtej kontrolki, �e utraci�y mysz je�eli j� mia�y.

		Mysz mo�e si� znajdowa� jednocze�nie w obszarze tylko jednej kontrolki spo�r�d dzieci.
		Jednak poniewa� kotrolki mog� si� przys�ania�, to funkcja test_mouse mo�e wielokrotnie
		zwr�ci� warto�� true. Z tego wzgl�du bierze si� pod uwag� zawsze pierwsz� kontrolk� na li�cie,
		kt�ra zwr�ci te warto��.

		@param point Wsp�rz�dne myszy.
		*/
		virtual void onMouseOut( const DirectX::XMFLOAT2& point );
		void draw_clipped( const Rect& clipping_rect );

	private:
		/**@brief Funkcja rysuj�ca, kt�ra powinna zosta� zaimplementowana w klasach potomnych.

		Rysowanie kontrolki zaczyna si� od funkcji draw_clipped, kt�ra jest wywo�ywana przez rodzica
		kontrolki.
		Wykonywane s� nast�puj�ce czynno�ci:
		- Sprawdzane jest czy kontrolka jest widoczna. Nie jest renderowana, je�eli visible jest ustawione na false.
		- Obszar obcinania podany w parametrze jest ustawiany w zmiennych DirectX.
		- Wywo�ywana jest funkcja onDraw, kt�ra powinna byc zaimplementowana w klasie pochodnej.

		Funkcja ta powinna wywo�a� funkcje draw_clipped wszystkich kontrolek, kt�re s� jej dzie�mi i powinny
		si� narysowa�.

		Co prawda kontrolki musz� by� wewn�trz obszaru rodzica, ale nie ma gwarancji,
		�e na siebie nie wchodz�. Je�eli w funkcji test_mouse schodzono rekurencyjnie od pocz�tku listy
		dzieci, to rysowa� wypada w odwrotnej kolejno�ci.

		Wewn�trz funkcji nie ma konieczno�ci pilnowania prostok�ta obcinania, poniewa� jest to robione
		sprz�towo przez DirectXa. Mimo to podawany jest w parametrze prostok�t, wewn�trz kt�rego odbywa si�
		rysowanie. Mo�na to wykorzysta� w celu optymalizacji, aby nie rysowa� kontrolek, kt�re i tak zostan�
		obci�te.

		@param clipping_rect Czworok�t, wewn�trz kt�rego ma zosta� narysowana kontrolka.
		*/
		virtual void onDraw( const Rect& clipping_rect ) = 0;
	};


	/**@brief Korze� drzewa kontrolek i jednocze�nie klasa zarz�dzaj�ca ca�ym GUI.
	*/
	class Root	:	public Control
	{
	private:
		Control*		focus;				///<Wskazuje na kontrolk�, kt�ra posiada aktualnie focusa lub nullptr

	protected:

	public:
		void set_focus( Control* control );		///<Ustawia focus dla podanej w parametrze kontrolki.

	};

}