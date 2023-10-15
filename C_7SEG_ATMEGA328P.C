XD

//------------------------------------------------------------------------------------------------------------------- DEKLARACJA ZMIENNYCH

      int wyswietlanie[] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111}; 	//Tablica na pierwszym polu cyfra "1", na drugim "2" itp.
      int tysiace, setki, dziesiatki, jednosci;		
      int wybieranie_wyswietlacza=0;


void wyswietl(int wartosc)
{ 
      //-------------------------------------------------------------------------------------------------------------------KONFIGURACJA WYJSC
      DDRC = 255;  				//Ustawienie PORTU C jako wyjścia
      DDRD = 255;  				//Ustawienie PORTU D jako wyjścia
      PORTC = 0;				//Ustawienie domyślnie wyjść C w stan niski  (wyłączona linia sygnałowa)
      PORTD = 255;				//Ustawienie domyślnie wyjść D w stan wysoki (wyłączone zasilanie wyswitlaczy) 
 
      //------------------------------------------------------------------------------------------------------------------- DZIELENIE  LICZBY NA POSZCZEGOLNE CYFRY
      jednosci   	= wartosc % 10;		//Wydobywam z podanej wartosci jednostki	(% 10 to reszta z dzielenia przez 10)
      dziesiatki 	= (wartosc/10) % 10 ;	//Wydobywam z podanej wartosci dziesiatki 
      setki 		= (wartosc/100) % 10 ;	//Wydobywam z podanej wartosci setki
      tysiace		= (wartosc/1000) % 10 ;	//Wydobywam z podanej wartosci tysiące
      
      //------------------------------------------------------------------------------------------------------------------- USTAWIENIE PRZERWAN
     sei();					//Włączenie przerwań globalnych
     TIMSK |= (1<<OCIE0);			//Włączenie porownania przerwania dla Timera0
     TCCR0 = 0b00001101;			//Ustawienie preskalera /1024 (3 najmlodsze bity), WDM01 (4 bit od prawej, wymusza zliczanie do wartosci OCR0)
     OCR0 = 5;					//Jezeli timer doliczy do tej wartosci nastepuje przerwanie i ponowne liczenie
	
     /*
      Zegar taktuje z 1MHz, preskaler zmienia taktowanie zliaczania do timera do 1MHz/prescaler, a przerwanie odbywa się co przepełnienie 1MHz/prescaler/OCR0
      Dla tego programu jest to 1 000 000 / 1024 / 5  = 195 Hz (co tyle wystąpi przerwanie) 
      Czyli każdy wyświetlacz odwieża się z częstotliwością 195Hz/5 = 39Hz
     */
}

ISR(TIMER0_COMP_vect )					//Co x czas przerywa program odswieza kolejny wyswietlacz i wraca do programu
{  
      switch(wybieranie_wyswietlacza)
	 {
	 case 0:
	       {
	       PORTD = 0b11111110;			//Podaje stan niski (ON) na wyświtlacz tysięcy
	       PORTC  = wyswietlanie[tysiace]; 		//Podaje na l. sygnalowa liczbe do wyswietlenia
	       wybieranie_wyswietlacza++;		//Dodaje do wybieranie_wyswietlacza +1, zeby przeszedl do nastepnego wyswietlacza
	       break;
	       }
	 case 1:
	       {
	       PORTD = 0b11111101;
	       PORTC  = wyswietlanie[setki]; 		
	       wybieranie_wyswietlacza++;
	       break;
	       }
	 case 2:
	       {
	       PORTD = 0b11111011;
	       PORTC  = wyswietlanie[dziesiatki]; 		
	       wybieranie_wyswietlacza++;
	       break;
	       }
	 case 3:
	       {
	       PORTD = 0b11110111;
	       PORTC  = wyswietlanie[jednosci]; 		
	       wybieranie_wyswietlacza=0;
	       break;
	       }
	}

   }
