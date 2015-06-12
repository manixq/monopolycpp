#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

class gracz;
void clearscrn()
{
  cout << "\033[2J\033[1;1H"; //czysci ekran   (ansi escape code - znalezione na stackoverflow.com)
}

class pole
{
protected:
  gracz* wlasciciel;
  string nazwa;
  bool mogekupic;
  int hotel;
  int cena;
  int oplata;
  pole* nast;
  pole* pop;
public:
  pole():nast(NULL),pop(NULL){}
  pole(string n,bool m):wlasciciel(NULL),nazwa(n),mogekupic(m),hotel(-1),oplata(0),nast(NULL),pop(NULL){}
  
  friend class plansza;
  friend class menu;
};

class miasto:public pole
{
public:
  miasto(string n, int c)
  {
    nazwa = n;
    cena = c;
    hotel = 0;
    wlasciciel = NULL;
    mogekupic = true;
  }
  friend class plansza;
};

class polespecjalne:public pole
{
public:
  polespecjalne(string n,int c,int o,bool m)
  {
    cena = c;
    nazwa = n;
    oplata = o;
    mogekupic = m;
  }
  friend class plansza;
};

class plansza
{

  pole* start;
  public:
  plansza():start(NULL){}
  void dodajpole(string n,bool m)//nazwa; czy mozna kupic
  {
    pole* nowe;
      if(start == NULL)
      {
	start = new pole(n,m);
	start->nast = start;
	start->pop = start;
      }
    else 
      {
	pole* ptr;
	ptr=start;
	while(ptr->nast != start)
	  ptr = ptr->nast;
	nowe = new pole(n,m);
	nowe->pop = ptr;
	nowe->nast = start;
	ptr->nast = nowe;
	start->pop = nowe;
      }
  }
  void dodajmiasto(string n, int c)//nazwa; cena kupna
  {
    pole* nowe;
      if(start == NULL)
      {
	start = new miasto(n,c);
	start->nast = start;
	start->pop = start;
      }
    else 
      {	
	pole* ptr;
	nowe = new miasto(n,c);
	ptr=start;
	while(ptr->nast != start)
	  ptr = ptr->nast;
	nowe->pop = ptr;
	nowe->nast = start;
	ptr->nast = nowe;
	start->pop = nowe;
      }
  }
  void dodajpolespecjalne(string n,int c,int o,bool m)//nazwa;  cena kupna;  oplaty za postoj;  czy mozna kupic
  {
    pole* nowe;
      if(start == NULL)
      {
	start = new polespecjalne(n,c,o,m);
	start->nast = start;
	start->pop = start;
      }
    else 
      {	
	pole* ptr;
	nowe = new polespecjalne(n,c,o,m);
	ptr=start;
	while(ptr->nast != start)
	  ptr = ptr->nast;
	nowe->pop = ptr;
	nowe->nast = start;
	ptr->nast = nowe;
	start->pop = nowe;
      }
  }
  ~plansza()
  {
    pole* ptr;
    ptr= start;
    if(start != NULL) 
      {	
	while(ptr->nast != start)
	  {
	    ptr = ptr->nast;   
	    delete ptr->pop;	   
	  }
	delete ptr;	
      }
  }
  friend class menu;
};

class gracz
{
  pole* pozycja;
  pole* kupione;
  gracz* nast;
  int kasa;
  int nier;
  string nazwa;
public:
  gracz():nast(NULL){}
  gracz(string n):kupione(NULL),nast(NULL),kasa(2000),nier(0),nazwa(n){}
  friend class menu;
};

class menu
{
  plansza* map;
  gracz* gracze;
  int y;
public:
  menu():gracze(NULL){}
  void generatemap()
  {
    map = new plansza();
    map->dodajpole("Start",false);
    map->dodajmiasto("Saloniki",120);
    map->dodajmiasto("Ateny",120);
    map->dodajpolespecjalne("Parking strzeżony",400,100,true);
    map->dodajpolespecjalne("Linie Kolejowe Południowe",400,50,true);
    map->dodajmiasto("Neapol",200);
    map->dodajmiasto("Mediolan",200);
    map->dodajmiasto("Rzym",240);
    map->dodajpolespecjalne("Więzienie",0,0,false);
    map->dodajmiasto("Barcelona",280);
    map->dodajmiasto("Sewilla",280);
    map->dodajmiasto("Madryt",320);
    map->dodajpolespecjalne("Linie Kolejowe Zachodnie",400,300,true);
    map->dodajmiasto("Liverpool",360);
    map->dodajmiasto("Glasgow",360);
    map->dodajmiasto("Londyn",400);
    map->dodajmiasto("Rotterdam",440);
    map->dodajmiasto("Bruksela",440);
    map->dodajmiasto("Amsterdam",480);
    map->dodajpolespecjalne("Linie Kolejowe Północne",400,200,true);
    map->dodajmiasto("Malmo",520);
    map->dodajmiasto("Goterborg",520);
    map->dodajpolespecjalne("opłata za wode",0,100,false);
    map->dodajmiasto("Sztokholm",580);
    map->dodajpolespecjalne("Zawiniłeś! Idziesz do więzienia!",0,1000,false);
    map->dodajmiasto("Frankfurt",600);
    map->dodajmiasto("Kolonia",600);
    map->dodajmiasto("Bonn",640);
    map->dodajpolespecjalne("Linie Kolejowe wschodnie",400,100,true);
    map->dodajmiasto("Innsbruck",700);
    map->dodajmiasto("Wiedeń",800);
  }
  void start()
    {
      gracz* ptr;
      string nazwa;
      cout << "Witaj w grze Monopol!"<<endl;
      cout << " Podaj liczbe graczy: ";
      cin >> y;
      if(y < 2)
	{
	  cout << "zbyt mala liczba graczy :("<<endl;
	  ui1();
	}
      else
	{
	  cout << " Podaj nick gracza nr 1: ";
	  cin >> nazwa;
	  gracze = new gracz(nazwa);
	  ptr = gracze;
	  ptr -> pozycja = map->start;
	  for(int i = 1; i < y; i++)
	    {
	      cout << " Podaj nick gracza nr "<<i+1<<": ";
	      cin >> nazwa;
	      ptr->nast = new gracz(nazwa);
	      ptr = ptr->nast;
	      ptr -> pozycja = map->start;
	    }
	  ptr->nast = gracze;
	}
    }
  void pokazmape()
  { 
    gracz* player = gracze;
    pole* ptr;
    ptr = map->start;
    while(ptr->nast != map->start)
      {
	cout << ptr->nazwa;
	for(int i = 0; i < y; i++)
	  {
	    if(player->pozycja == ptr)
	      cout << "---|"<<player->nazwa<<"|---";
	    player = player->nast;
	  }
	cout << endl;
	ptr = ptr->nast;
      }
    cout << ptr->nazwa;
    for(int i = 0; i < y; i++)
      {
	if(player->pozycja == ptr)
	  cout << "---|player "<<i<<"|---";
      }
    cout << endl<<endl; 
  }
  void buy()
  {
    if(gracze->pozycja->mogekupic == true)
      {
	gracze->kasa = gracze->kasa-gracze->pozycja->cena;
	cout <<"Kupiłeś "<<gracze->pozycja->nazwa<<" za "<< gracze-> pozycja->cena<<"$"<<endl;
	gracze->pozycja->wlasciciel = gracze;
	gracze->pozycja->mogekupic = false;
	gracze->nier = gracze->nier + gracze->pozycja->cena;
	gracze->pozycja->oplata = 1/3*gracze->pozycja->cena;
      }
    else if(gracze->pozycja->wlasciciel == gracze)
      {
	if(gracze->pozycja->hotel >= 0 )
	  {
	    gracze->kasa = gracze->kasa - gracze->pozycja->cena*1/3;
	    cout << "Wydales "<<gracze->pozycja->cena*1/3<<"$ i Kupiles Hotel!"<<endl;
	    gracze->pozycja->hotel++;
	    gracze->nier =  gracze->nier + gracze->pozycja->cena*1/2;
	    gracze->pozycja->oplata = gracze->pozycja->oplata + gracze->pozycja->cena*1/2;
	  }
	else
	  cout << "niestety nic wiecej tu nie zdzialasz!"<<endl;
      }
    else
      cout << "nie mozna kupic!"<<endl;
  }
  void kostka()
  {
    srand( time( NULL) );
    int x = rand() % 6 + 1;
    cout <<" Wyrzuciłeś: "<<x<<"!"<<endl;
    for(int i = 0; i < x; i++)
      {
	gracze->pozycja = gracze->pozycja->nast;
	if(gracze->pozycja->nazwa == "Start")
	  {
	    cout << "przechodzisz przez start i otrzymujesz 400$"<<endl;
	    gracze->kasa =gracze->kasa + 400;
	  }
      }
    cout << "Odwiedzasz pole:  "<<gracze->pozycja->nazwa<<"!"<<endl;
    if(gracze->pozycja->oplata > 0 )
      {
	gracze->kasa = gracze->kasa - gracze->pozycja->oplata;
	cout << " Musisz zaplacic "<< gracze->pozycja->oplata<<" $ za pobyt"<<endl;
	if(gracze->pozycja->wlasciciel != NULL)
	  {
	    gracze->pozycja->wlasciciel->kasa = gracze->pozycja->wlasciciel->kasa + gracze->pozycja->oplata;
	  }
	if(gracze->pozycja->nazwa == "Zawiniłeś! Idziesz do więzienia!")
	  while(gracze->pozycja->nazwa != "Więzienie")
	    {
	      gracze->pozycja = gracze->pozycja->pop;
	    }
      }
  }
  void mojekonto()
  {
    cout << gracze->nazwa<<endl;
    cout << "Posiadasz na koncie "<<gracze->kasa<<"$"<<endl;
    cout<< "Całkowita wartość majątku: "<<gracze->nier+gracze->kasa<<"$"<<endl;
  }
  void info()
  {
    cout << gracze->pozycja->nazwa<<endl;
    if(gracze->pozycja->cena > 0)
      cout << "cena: "<<gracze->pozycja->cena<<endl;
    if(gracze->pozycja->oplata > 0)
      cout << "oplata: "<<gracze->pozycja->oplata<<endl;
    if(gracze->pozycja->hotel >= 0)
      cout <<"hotele: " << gracze->pozycja->hotel<<endl;
    if(gracze->pozycja->wlasciciel == NULL)
      {
      cout << "wlasciciel: brak"<<endl;
      }
    else
      cout << "wlasciciel: "<<gracze->pozycja->wlasciciel->nazwa<<endl;
  }
  void ui1()
  {
    int x;
    cout << "---Menu Główne---"<<endl;
    cout << "1: Nowa Gra"<<endl;
    cout << "2: Exit"<<endl;
    cout << "Wybierz: ";
    cin >> x;
    clearscrn();
    switch(x)
      {
      case 1:
	clearscrn();
	 generatemap();
	 start();
	 clearscrn();
	 ui2();
	break;
      case 2:
	break;
      default:
	break;
      }
  }
   void ui2()
  {
    int x;    
    cout << "--Menu Gry--"<<endl<<endl;
    cout << "Tura gracza "<< gracze->nazwa<<endl<<"------"<<endl<<endl;
    cout<< "1: Pokaz Mape"<<endl;
    cout << "2: ---Rzuc Kostka---"<<endl;
    cout << "3: Kup(miasto/hotel)"<<endl;
    cout << "4: Twój stan konta"<<endl;
    cout <<"5: informacje o polu"<<endl;
    cout <<"6: Wyczysc ekran"<<endl;
    cout << "9: Wyjdz z gry"<<endl;
    cout <<"Wybierz: ";
    cin >> x;
    clearscrn();
    switch(x)
      {
      case 1:
	pokazmape();
	ui2();
	break;
      case 2: 
	kostka();
	ui3();
	break;
      case 3:
	buy();
	ui2();
	break;
      case 4:
	mojekonto();
	ui2();
	break;
      case 5:
	info();
	ui2();
	break;
      case 6: 
	clearscrn();
	ui2();
	break;
      case 9:
	cout << "Wychodzisz z gry."<<endl;
	break;
      default:
	ui2();
	break;
      }
  }
  void gameover()
  {
    if(gracze->kasa < 0)
      {
	cout << "graczu "<< gracze->nazwa<<" jestes bankrutem! Wylatujesz!"<<endl;
	gracze->nazwa = "bank";
	for(int i = 1; i < y; i++)
	  gracze = gracze->nast;
	gracze->nast = gracze->nast->nast;
      }
    if(gracze->nast == gracze)
      {
	cout << "gratulacje! "<<gracze->nazwa<< "wygrales!"<<endl;
	ui1();
      }
  }
 void ui3()
  {
    int x;     
    cout << "--Menu Gry--"<<endl<<endl;
    cout << "Tura gracza "<< gracze->nazwa<<endl<<"------"<<endl<<endl;
    cout<< "1: Pokaz Mape"<<endl;
    cout << "2: Kup(miasto/hotel)"<<endl;
    cout << "3: Twój stan konta"<<endl;
    cout <<"4: informacje o polu"<<endl;
    cout <<"5: Wyczysc ekran"<<endl;
    cout <<"6: ---Koniec tury---"<<endl;
    cout << "9: Wyjdz z gry"<<endl;
    cout <<"Wybierz: ";
    cin >> x;
    clearscrn();
    switch(x)
      {
      case 1:
	pokazmape();
	ui3();
	break;
      case 2:
	buy();
	ui3();
	break;
      case 3:
	mojekonto();
	ui3();
	break;
      case 4:
	info();
	ui3();
	break;
      case 5: 
	clearscrn();
	ui3();
	break;
      case 6:
      	gracze = gracze->nast;
	gameover();
	ui2();
	break;
      case 9:
	cout << "Wychodzisz z gry."<<endl;
	break;
      default:
	ui3();
	break;
      }
  }
  ~menu()
  {
    gracz* ptr;
    ptr = gracze;
    if(gracze != NULL)
      {
	gracz* dilit;
	while(ptr->nast != gracze)
	  { 
	    dilit = ptr;
	    ptr = ptr->nast;
	    delete dilit;  
	  } 
	delete ptr;
      }
  }
};

int main()
{
  clearscrn();
  menu lecimy;
  lecimy.ui1();
}
