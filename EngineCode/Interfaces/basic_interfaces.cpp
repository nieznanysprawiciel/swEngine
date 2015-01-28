#include "..\stdafx.h"
#include "basic_interfaces.h"
#include "..\Engine\Engine.h"


Engine* Object::engine = nullptr;		//po stworzeniu obiektu klasy Engine, zmienna ta jest uzupe�niana wskaxnikiem this



/*Funkcja pozwala wys�a� event, kt�ry b�dzie potem przetworzony przez klase FableEngine.
*Eventy s� metod� komunikacji pomiedzy silnikiem graficznym, silnikiem fizycznym, AI i silnikiem kolizji,
*a modu�em silnika odpowiedzialnym za fabu��. Istnieje szereg event�w wbudowanych, wysy�anych przez silnik,
*mo�na r�wnie� definiowa� w�asne nowe eventy poprzez dziedziczenie z klasy Event. Event mo�e by� wys�any przez dowolny
*objekt poprzez wywo�anie funkcji Object::event. Aby wys�a� w�asny event trzeba przeci��y� jedn� z funkcji klas wbudowanych,
*kt�ra jest potem wywo�ywana przez silnik i wywo�a� t� funkj�.
*
*Za zwolnienie pami�ci po klasie Event odpowiada klasa FabelEngine (jest to robione automatycznie po wywo�aniu funkcji obs�ugi,
*u�ytkownik nie musi si� tym przejmowac).*/
void inline Object::event(Event* new_event)
{
	engine->events_queue->push(new_event);
}


/*Funkcja wykonywana w ka�dym obiegu p�tli renderingu przez obiekt MovementEngine.
 *Dodajemy aktualny wektor pr�dko�ci i pr�dko�ci obrotowej do wektor�w po�o�enia i orientacji.
 *Poniewa� te wektory wyra�aj� przesuni�cie/rotacje na sekund�, to musimy to przemno�y� jeszcze przez time_interval.
 *
 *W momencie wykonania zosta�y juz uwzgl�dnione oddzia�ywania fizyczne oraz wp�yw kontroler�w.
 *Nale�y zauwa�y�, �e o ile najlepiej by by�o, �eby kontrolery tak�e u�ywa�y tych zmiennych do poruszania
 *obiektami na scenie, to nie jest to obowi�zek, bo maj� te� bezpo�redni dost�p do po�o�enia i orientacji.
 *
 *Sprawdzanie kolizji i ewentualne przesuni�cia z tym zwi�zane nast�puj� dopiero po zako�czeniu wywo�ywania tych funkcji.
 *
 *Do przeliczania u�ywamy biblioteki DirectXmath, kt�ra u�ywa zmiennych wektorowych i wykonuje obliczenia
 *na jednostkach SSE2. Do tego potrzebne jest wyr�wnanie zmiennych do 16 bit�w, czego nie oferuj� zmienne 
 *XMFloat, dlatego trzeba wykonywa� operacj� XMLoadFloat4 i XMStoreFloat4. 
 *Uwaga zmienne XMVECTOR i XMMATRIX nie mog� byc alokowane na stercie, poniewa� nie jest tam gwarantowane 
 *16bitowe wyr�wnanie. Po dok�adny opis odsy�am do MSDNu.*/
void Dynamic_object::move(float time_interval)
{
//translacja
	XMVECTOR pos = XMLoadFloat3(&position);
	XMVECTOR time = XMVectorReplicate(time_interval);
	XMVECTOR translate = XMLoadFloat3(&speed);
	translate *= time;
	pos = pos + translate;
	XMStoreFloat3(&position, pos);

//w docelowej wersji trzeba si� zdecydowa�, co lepsze i reszt� wywali�
#ifdef _QUATERNION_SPEED
//orientacja
	XMVECTOR orient = XMLoadFloat4(&orientation);
	XMVECTOR rot = XMLoadFloat4(&rotation_speed);

	//najpierw liczymy nowy kwaternion dla obrotu w czasie sekundy
	rot = XMQuaternionMultiply(orient, rot);
	//teraz interpolujemy poprzedni� orientacj� i orientacj� po sekundzie
	//ze wsp�czynnikiem r�wnym czasowi jaki up�yn�� faktycznie
	orient = XMQuaternionSlerp(orient, rot, time_interval);

	/*Du�o oblicze�, mo�e da si� to jako� za�atwi� bez interpolacji...*/

	XMStoreFloat4(&orientation, orient);
#else
	XMVECTOR orient = XMLoadFloat4( &orientation );		//pobieramy orientacj�
	XMVECTOR rot = XMLoadFloat4( &rotation_speed );		//pobieramy o� obrotu (k�t te�, ale on nie ma znaczenia)

	if ( !XMVector3Equal( rot, XMVectorZero( ) ) )
	{
		float rot_angle = rotation_speed.w * time_interval;	//liczymy k�t obrotu

		rot = XMQuaternionRotationAxis( rot, rot_angle );		//przerabiamy na kwaternion
		orient = XMQuaternionMultiply( orient, rot );			//liczymy nowy kwaternion orientacji

		XMStoreFloat4( &orientation, orient );
	}
	//else: wtedy orientacja si� nie zmienia
#endif
}

/*Funkcja o zastosowaniu tym samym co move, z t� r�nic�, �e wykonywana dla obiekt�w z�o�onych. Przesuni�cie
jest z�o�eniem ruchu rodzica i danegi obiektu.

Funkcja jest wirtualna i w klasie Complex_object (implementacja poni�ej) wywo�uje rekurencyjnie t� funkcj�
dla wszystkich swoich dzieci.

UWAGA!! nale�y pami�ta�, �e w klasie MovementEngine nie powinno wyst�pi� �adne dziecko klasy Complex_object.
W przeciwnym razie niekt�re przesuniecia i obroty zostan� zastosowane wielokrotnie do jednego obiektu.*/
void Dynamic_object::move_complex(float time_interval, const XMFLOAT3& parent_speed, const XMFLOAT4& parent_rotation)
{
	return;
	//DOKO�CZYC !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//translacja
	XMVECTOR pos = XMLoadFloat3(&position);
	XMVECTOR time = XMVectorReplicate(time_interval);
	XMVECTOR translate = XMLoadFloat3(&speed);
	XMVECTOR parent_translate = XMLoadFloat3(&parent_speed);
	
	translate += parent_translate;		//g��wna r�nica: dodajemy przesuni�cie rodzica
	translate *= time;
	pos = pos + translate;
	XMStoreFloat3(&position, pos);

	//orientacja
	XMVECTOR orient = XMLoadFloat4(&orientation);
	XMVECTOR rot = XMLoadFloat4(&rotation_speed);
	XMVECTOR parent_rotn = XMLoadFloat4(&parent_rotation);

	//najpierw liczymy nowy kwaternion dla obrotu w czasie sekundy
	rot = XMQuaternionMultiply(orient, rot);
	//teraz interpolujemy poprzedni� orientacj� i orientacj� po sekundzie
	//ze wsp�czynnikiem r�wnym czasowi jaki up�yn�� faktycznie
	orient = XMQuaternionSlerp(orient, rot, time_interval);

	/*Du�o oblicze�, mo�e da si� to jako� za�atwi� bez interpolacji...*/

	XMStoreFloat4(&orientation, orient);
}

void Complex_object::move_complex(float time_interval, const XMFLOAT3& parent_speed, const XMFLOAT4& parent_rotation)
{

}

//----------------------------------------------------------------------------------------------//
//									Static_object
//----------------------------------------------------------------------------------------------//

Static_object::Static_object()
{//inicjujemy obiekt w punkcie ( 0.0, 0.0, 0.0 ) i zorientowany tak jak jego mesh
	position.x = 0.0;
	position.y = 0.0;
	position.z = 0.0;

	XMVECTOR quaternion = XMQuaternionIdentity();
	XMStoreFloat4(&orientation,quaternion);
}

Static_object::Static_object(const XMFLOAT3& pos, const XMFLOAT4& orient)
{
	position = pos;
	orientation = orient;
}

//----------------------------------------------------------------------------------------------//
//									Dynamic_object
//----------------------------------------------------------------------------------------------//

Dynamic_object::Dynamic_object()
{
	speed.x = 0.0;
	speed.y = 0.0;
	speed.z = 0.0;

#ifdef _QUATERNION_SPEED
	XMVECTOR quaternion = XMQuaternionIdentity();
	XMStoreFloat4( &rotation_speed, quaternion );
#else
	rotation_speed.w = 0.0;
	rotation_speed.x = 0.0;
	rotation_speed.y = 0.0;
	rotation_speed.z = 0.0;
#endif
}

Dynamic_object::Dynamic_object( const XMFLOAT3& move_speed, const XMFLOAT4& rot_speed )
{
	speed = move_speed;
	rotation_speed = rot_speed;
}

//----------------------------------------------------------------------------------------------//
//									Physical_object
//----------------------------------------------------------------------------------------------//

Physical_object::Physical_object()
{
	mass = 1;
}

//----------------------------------------------------------------------------------------------//
//									Dynamic_mesh_object
//----------------------------------------------------------------------------------------------//

Dynamic_mesh_object::Dynamic_mesh_object()
{
	model_reference = nullptr;
	model_changed = false;
	vertex_buffer = nullptr;
#ifdef _SCALEABLE_OBJECTS
	scale = 1.0;
#endif
}

Dynamic_mesh_object::~Dynamic_mesh_object()
{
	//Kasujac obiekt nie wolno nam niczego usuwa�, bo nic nie nale�y do nas
	//jedynie kasujemy referencje

	//kasujemy referencje bezpo�rednie
	delete_all_references();
}

int Dynamic_mesh_object::set_model(Model3DFromFile* model)
{
	if( model == nullptr )
		return 1;

	//najpierw czy�cimy poprzedni� zawarto��
	//kasujemy referencje bezpo�rednie
	delete_all_references();

	//dodajemy now� zawarto��
	model_reference = model;
	model->add_object_reference();

	vertex_buffer = model->get_vertex_buffer();
	vertex_buffer->add_object_reference();

	unsigned int count = model->get_parts_count();
	model_parts.reserve( count );

	for (unsigned int i = 0; i < count; ++i)
	{//przepisujemy sobie wska�niki
		register const ModelPart* part = model->get_part( i );

		model_parts.push_back( *part );
		add_references(part);			//Dodajemy odwo�ania
	}

	//Pytanie czy to ma sens. Funkcja reserve ustawi�a wielko�� wektora na przynajmniej count.
	//Je�eli da�a wi�cej, to warto by by�o zmniejszy�, bo zawarto�� tych wektor�w si� ju� nie zmieni.
	//Problemem jest, �e obie te funkcje wed�ug dokumentacji nie s� wi���ce.
	model_parts.shrink_to_fit( );

	model_changed = false;		//Zawarto�� tablic odpowiada modelowi

	return 0;
}

/* #private
Dodajemy odwo�ania do wszystkich istniej�cych element�w w przekazanym wska�niku.*/
void Dynamic_mesh_object::add_references( const ModelPart* part )
{
	if ( part == nullptr )
		return;

	if ( part->material )
		part->material->add_object_reference();
	if ( part->index_buffer )
		part->index_buffer->add_object_reference();
	if ( part->mesh )
		part->mesh->add_object_reference();
	if ( part->pixel_shader )
		part->pixel_shader->add_object_reference();
	for ( int i = 0; i < ENGINE_MAX_TEXTURES; ++i )
		if ( part->texture )
			part->texture[i]->add_object_reference( );
	if ( part->vertex_shader )
		part->vertex_shader->add_object_reference();

}

/* #private
Kasuje odwo�ania do obiekt�w, kt�rych w�asno�ci� jest ModelsManager albo Model3DFromFile
w tablicy model_parts raz wska�niku model_reference i vertex_buffer;

�adne obiekty nie s� kasowane, poniewa� nie nale�� one do nas.
Wszystkie zmienne s� za to czyszczone.*/
void Dynamic_mesh_object::delete_all_references( )
{
	if ( model_reference != nullptr )
		model_reference->delete_object_reference( );
	model_reference = nullptr;

	if ( vertex_buffer )
		vertex_buffer->delete_object_reference();
	vertex_buffer = nullptr;

	for ( unsigned int k = 0; k < model_parts.size( ); ++k )
	{//ka�dy element mo�e by� nullptrem
		register ModelPart* part = &model_parts[k];

		//ka�dy element mo�e by� nullptrem
		if ( part->material )
			part->material->delete_object_reference();
		if ( part->index_buffer )
			part->index_buffer->delete_object_reference( );
		if ( part->mesh )
			part->mesh->delete_object_reference( );
		if ( part->pixel_shader )
			part->pixel_shader->delete_object_reference( );
		for ( int i = 0; i < ENGINE_MAX_TEXTURES; ++i )
			if ( part->texture )
				part->texture[i]->delete_object_reference( );
		if ( part->vertex_shader )
			part->vertex_shader->delete_object_reference( );
	}
	model_parts.clear();
}

//----------------------------------------------------------------------------------------------//
//									Base_input_controller
//----------------------------------------------------------------------------------------------//

Base_input_controller::Base_input_controller( InputAbstractionLayer_base* layer )
: abstraction_layer( layer )
{}

Base_input_controller::~Base_input_controller(){};

