﻿# projektTowerDefense
# projektTowerDefense
# TowerDefense – gra strategiczna w C++ z SFML

**TowerDefense** to gra typu tower defense z bohaterem sterowanym przez gracza, stworzona w C++ z użyciem biblioteki **SFML** i środowiska **Qt Creator**.

---

## Opis rozgrywki

Gracz broni zamku przed kolejnymi falami przeciwników. Może:
- budować wieże i generatory zasobów,
- kupować i ulepszać bohatera (Knight, Archer, Mage, Samurai),
- walczyć w czasie rzeczywistym,
- zbierać złoto i inwestować w obronę.

Gra zawiera różne typy przeciwników, system fal, sklep, pasek złota oraz rozbudowane animacje postaci i wież.

---

## Sterowanie

### Klawiatura:
- `W`, `A`, `S`, `D` – ruch bohatera
- `LShift` – dash(tylko Archer i Knight)
- `Q`, `E` – ataki specjalne (Mage, Samurai)

### Mysz:
- kliknięcie w `Shop` – otwiera/zamyka sklep
- kliknięcie w pole – budowa wieży/generatora lub ulepszenie

---

## Budowle

- **Wieża Łucznika** – szybki dystansowy atak
- **Wieża Maga** – magiczne pociski
- **Wieża z Katapulta** - duży zasięg, duże obrażenia
- **Generator** – pasywnie zwiększa zasoby

---

## Bohaterowie

- **Knight** – silny wojownik wręcz z losowymi animacjami ataku
- **Archer** – atak strzałami, szybki ruch
- **Mage** – atak wręcz + fireball
- **Samurai** – szybki, dwa typy ataku (Q i E)

Każdy bohater posiada:
- pasek życia,
- system ulepszania,
- automatyczną regenerację HP co 5 sekund (10% max HP).

---

## Sklep

W sklepie możesz:
- kupić bohatera,
- postawić wieżę lub generator,
- ulepszyć zamek,
- ulepszyć bohatera.

---

## Kompilacja i uruchomienie

### Wymagania:
- C++17
- Qt Creator (zestaw MinGW 64-bit)
- SFML 2.5 (zainstalowana i dodana do projektu)

---

## Struktura projektu

- `main.cpp` – punkt wejścia
- `GameEngine.*` – logika gry i pętla renderująca
- `Field.*` i pochodne – zarządzanie polami mapy
- `HeroBase.*` i `Knight`, `Archer`, `Mage`, `Samurai` – klasy bohaterów
- `Enemy.*`, `EnemyBoss`, `EnemyFast`, `EnemyArcher`, ... – klasy przeciwników
- `Tower*` – wieże
- `Shop.*` – interfejs sklepu
- `LevelLoader.*` – ładowanie rund i ścieżek z pliku `levels.txt`
- `assets/` – grafiki, animacje, dane poziomów

---

## Dystrybucja `.exe`

Do uruchomienia gry potrzebne są:
- `TowerDefence.exe`
- folder `assets/`
- pliki DLL z SFML i MinGW (np. `sfml-graphics-2.dll`, `libgcc_s_seh-1.dll` itd.)

---

## Autorzy

Projekt tworzony jako gra edukacyjna z wykorzystaniem SFML i Qt na potrzeby zaliczenia i nie czerpiemy żadnych korzyści z niego. Wszystkie tekstury były brane ze stron typu Itch.io lub Craftpix, wszystkie były darmowe.
https://craftpix.net/freebies/free-enemy-pixel-pack-for-top-down-defense/
https://craftpix.net/freebies/free-flame-effects-sprite-pack/?num=1&count=14&sq=fireball&pos=6
https://craftpix.net/freebies/free-shinobi-sprites-pixel-art/?num=1&count=8&sq=samurai&pos=3
https://jumpbutton.itch.io/girlknightasset
---

