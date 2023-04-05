# Programowanie Równoległe i Rozproszone lab 4 - sieciowo rozproszone procesy

## Zadanie
Proszę napisać program realizujący **rozproszone** zadanie obliczenia iloczynu macierzy i wektora. Program powinien składać się z:

- **serwera** - tworzącego obiekt typu Manager udostępniający dwie kolejki - jedną, do której przekazywane są dane do obliczeń oraz drugą - do której przekazywane są cząstkowe rozwiązania.
- **klienta** - wczytującego dane macierzy i wektora i dzielącego dane na liczbę zadań przekazaną jako argument programu. Klient dodaje zadania do pierwszej kolejki, odbiera wyniki wrzucone do kolejki z wynikami czątkowymi i łączy wynik w wektor w odpowiedniej kolejności.
- **workera** - program pobiera zadania z kolejki, tworzy określoną liczbę podprocesów (może odpowiadać np. liczbie procesorów dostępnych w danej maszynie), wykonuje obliczenia dla kolejnych odebranych zadań, wyniki dodaje do kolejki z wynikami czątkowymi. Program proszę uruchomić również na innej maszynie/ maszynach niż na tej na której znajduje się program serwera.

## Instrukcja uruchomienia
Serwer:
```shell
python server.py [ip] [port]
```

Klient + worker:
```shell
python main.py [plik_macierz] [plik_wektor] [ip] [port]
```

Wynik zapisywany w pliku `res.dat`
