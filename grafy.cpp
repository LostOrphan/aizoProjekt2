#include <iostream>
#include <vector>
#include <utility> 
#include <fstream>
#include <random>
#include <string>
#include <set>
#include <queue>
#include <limits>
#include <algorithm>
#include <chrono>
#define ll long long

std::vector<std::vector<int>> graph; // Macierz sąsiedztwa
std::vector<std::vector<std::pair<ll, ll>>> lista; // Lista sąsiedztwa
const int INF = std::numeric_limits<int>::max(); //Maksymalna wartość dla int
const long long INFLL = std::numeric_limits<long long>::max(); // Maksymalna wartość dla long long

// Funkcja do wyboru trybu działania programu
// parametry: int *dzialanieTryb - wskaźnik do zmiennej, która będzie przechowywać wybrany tryb działania
void tryb(int *dzialanieTryb){
    int input=10;
    while(input!=0){
        std::cout<<"[1] Wczytaj graf z plkiku\n";
        std::cout<<"[2] Wygeneruj graf losowy\n";
        std::cout<<"[3] Wyswietl graf\n";
        std::cout<<"[4] Usun graf z pamieci\n";
        std::cout<<"[5] Dijkstra\n";
        std::cout<<"[6] Test\n";
        std::cout<<"[0] Wyjscie\n";
        std::cin>>input;
        switch(input){
            case 1:
                *dzialanieTryb=1;
                return;
            case 2:
                *dzialanieTryb=2;
                return;
            case 3:
                std::cout<<"[1] Macierz\n";
                std::cout<<"[2] Lista\n";
                std::cout<<"[3] Macierz i Lista\n";
                std::cout<<"[0] Cofnij\n";
                std::cin>>input;
                if(input>=0 || input<=3){
                    switch(input){
                        case 1:
                            *dzialanieTryb=31;
                            return;
                        case 2:
                            *dzialanieTryb=32;
                            return;
                        case 3:
                            *dzialanieTryb=33;
                            return;
                        case 0:
                            input=10; // Cofnij do głównego menu
                            break;
                        default:
                            std::cout<<"Niepoprawny wybor\n";
                        }
                        continue;
                return;
            case 4:
                *dzialanieTryb=4;
                return;
            case 5:
                *dzialanieTryb=5;
                return;
            case 6:
                *dzialanieTryb=6;
                return;
            case 0:
                exit(0);
            default:
                std::cout<<"Niepoprawny wybor\n";
                continue;
        }
    }
}
}
// Funkcja do otwierania pliku
// Zwraca ifstream, który można użyć do odczytu z pliku
std::ifstream otworzPlik(){
    std::cout<<"Podaj nazwe pliku: ";
    std::string nazwaPliku;
    std::cin >> nazwaPliku;
    std::ifstream plik(nazwaPliku);
    if(!plik) {
        std::cerr << "Nie mozna otworzyc pliku: " << nazwaPliku << std::endl;
        return std::ifstream(); // zwraca pusty ifstream;
    }else{
        std::cout << "Plik otwarty pomyslnie" << std::endl;
    }
    return plik;
}
// Funkcja do zamykania pliku
// Parametr: plik - referencja do ifstream, który ma zostać zamknięty
void zamknijPlik(std::ifstream &plik) {
    if (plik.is_open()) {
        plik.close();
        std::cout << "Plik zostal zamkniety." << std::endl;
    } else {
        std::cerr << "Plik nie jest otwarty." << std::endl;
    }
}
// Funkcja czyszczaca macierz i liste sasiedztwa
// Parametr: liczbaWierzcholkow - liczba wierzchołków w grafie
void usunMacierz(int liczbaWierzcholkow) {
    graph.clear();
    lista.clear();
    std::cout << "Graf zostal usuniety z pamieci." << std::endl;
}
// Funkcja tworzaca strukture macierzy sasiedztwa
// Parametr: liczbaWierzcholkow - liczba wierzchołków w grafie
void utworzMacierz(int liczbaWierzcholkow) {
    // Jeśli macierz już istnieje, usuń ją
    if (!graph.empty()) {
        graph.clear();
    }
    // Tworzenie wektora o zadanej liczbie wierzchołków
    graph.resize(liczbaWierzcholkow, std::vector<int>(liczbaWierzcholkow, -1));
}
// Funkcja tworzaca strukture listy sasiedztwa
// Parametr: liczbaWierzcholkow - liczba wierzchołków w grafie
void utworzListe(int liczbaWierzcholkow) {
        lista.clear();
        lista.resize(liczbaWierzcholkow); // Inicjalizacja listy sąsiedztwa
}
// Funkcja do tworzenia grafu z pliku w reprezentacji macierzowej
// Parametry: plik - referencja do ifstream, liczbaWierzcholkow - liczba wierzchołków, liczbaKrawedzi - liczba krawędzi
void utworzGrafMacierz(std::ifstream &plik, int &liczbaWierzcholkow, int &liczbaKrawedzi) {
    plik >> liczbaKrawedzi >> liczbaWierzcholkow;
    utworzMacierz(liczbaWierzcholkow); // Inicjalizacja macierzy
    // Wczytanie krawędzi z pliku
    int a, b, c;
    for(int i = 0; i < liczbaKrawedzi; i++) {
        plik >> a >> b >> c;
        if (a < liczbaWierzcholkow && b < liczbaWierzcholkow) {
            graph[a][b] = c; // Ustawienie wagi krawędzi
        } else {
            std::cerr << "Blad: Wierzcholki poza zakresem." << std::endl;
        }
    }
    std::cout<<"Utworzono graf w reprezentacji macierzowej.\n";
}
// Funkcja do tworzenia grafu z pliku w reprezentacji listowej
// Parametry: plik - referencja do ifstream, liczbaWierzcholkow - liczba wierzchołków, liczbaKrawedzi - liczba krawędzi
void utworzGrafLista(std::ifstream &plik,int liczbaWierzcholkow, int liczbaKrawedzi){
    plik >> liczbaKrawedzi >> liczbaWierzcholkow;
    utworzListe(liczbaWierzcholkow); // Inicjalizacja listy
    int a, b, c;
       for (int i=0;i<liczbaKrawedzi;i++){
        plik>>a>>b>>c;
        lista[a].push_back({b,c});
       }
    std::cout<<"Utworzono graf w reprezentacji listowej.\n";
}
// Funkcja do tworzenia grafu losowego w postaci macierzy i listy sąsiedztwa
// Parametry: liczbaWierzcholkow - liczba wierzchołków, gestosc - gęstość grafu, maxWaga - maksymalna waga krawędzi
void utworzGrafLosowy(int &liczbaWierzcholkow, int &gestosc, int &maxWaga){ 
    utworzMacierz(liczbaWierzcholkow); // Inicjalizacja macierzy
    utworzListe(liczbaWierzcholkow); // Inicjalizacja listy
    int maxLiczbaKrawedzi = liczbaWierzcholkow * (liczbaWierzcholkow - 1);
    int liczbaKrawedzi = (maxLiczbaKrawedzi * gestosc) / 100;
    //Przygotowanie generatora liczb losowych
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> krawedzDist(0, liczbaWierzcholkow-1);
    std::uniform_int_distribution<> wagaDist(1, maxWaga);

    std::set<std::pair<int, int>> istniejaceKrawedzie;

    while((int)istniejaceKrawedzie.size() < liczbaKrawedzi) {
        int a = krawedzDist(gen);
        int b = krawedzDist(gen);
        if (a != b && istniejaceKrawedzie.count({a, b}) == 0) { // Sprawdzenie, czy krawędź już istnieje
            int waga = wagaDist(gen); // Losowanie wagi krawędzi
            graph[a][b] = waga; // Ustawienie wagi krawędzi w macierzy
            lista[a].push_back({b, waga}); // Dodanie krawędzi do listy sąsiedztwa
            istniejaceKrawedzie.insert({a, b});
        }
    }
   
}
// Funkcja do wypisywania grafu w postaci macierzy sąsiedztwa
// Parametr: liczbaWierzcholkow - liczba wierzchołków w grafie
void wypiszMacierz(int liczbaWierzcholkow){
    if(graph.empty()) {
        std::cout << "Graf nie został zainicjalizowany." << std::endl;
        return;
    }
    std::cout << "Macierz sasiedztwa grafu:\n";
    for (int i = 0; i < liczbaWierzcholkow; ++i) {
        for (int j = 0; j < liczbaWierzcholkow; ++j) {
            std::cout.width(6);
            std::cout << graph[i][j];
        }
        std::cout << "\n";
    }
}
// Funkcja do wypisywania grafu w postaci listy sąsiedztwa
// Parametr: liczbaWierzcholkow - liczba wierzchołków w grafie
void wypiszListe(int liczbaWierzcholkow){
        if(lista.empty()) {
            std::cout << "Lista sasiedztwa nie została zainicjalizowana." << std::endl;
        return;
        }
        std::cout << "Lista sasiedztwa grafu:\n";
        for(int i=0;i<liczbaWierzcholkow;i++){
            std::cout<<"Krawedz "<<i<<" ma";
            if(lista[i].size()==0){
                std::cout<<" 0 sasiadow";
            }
            else{
                std::cout<<" sasiadow: ";
                for(int j=0;j<lista[i].size();j++){
                    std::cout<<lista[i][j].first<<" waga: "<< lista[i][j].second<<"| ";
                }
            }
            std::cout<<"\n";
    }
}
// Funkcja wykonujaca algorytm Dijkstry dla macierzy sąsiedztwa
// Parametry: graph - macierz sąsiedztwa, n - liczba wierzchołków, start - wierzchołek startowy
// Zwraca parę wektorów: dystansów i poprzedników
std::pair<std::vector<int>, std::vector<int>> dijkstraMatrix(const std::vector<std::vector<int>>& graph, int n, int start) {
    std::vector<int> dystans(n, INF);
    std::vector<int> poprzedni(n, -1);
    std::vector<bool> odwiedzony(n, false);

    dystans[start] = 0;
    using P = std::pair<int, int>; // {dystans, wierzchołek}
    std::priority_queue<P, std::vector<P>, std::greater<>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (odwiedzony[u]) continue;
        odwiedzony[u] = true;

        for (int v = 0; v < n; ++v) {
            int waga = graph[u][v];

            if (waga != -1 && !odwiedzony[v]) {  // -1 oznacza brak krawędzi
                if (dystans[u] + waga < dystans[v]) {
                    dystans[v] = dystans[u] + waga;
                    poprzedni[v] = u;
                    pq.push({dystans[v], v});
                }
            }
        }
    }

    return {dystans, poprzedni};
}
// Funkcja wykonujaca algorytm Dijkstry dla listy sąsiedztwa
// Parametry: lista - lista sąsiedztwa, n - liczba wierzchołków, start - wierzchołek startowy
// Zwraca parę wektorów: dystansów i poprzedników
std::pair<std::vector<long long>, std::vector<int>> dijkstraLista(const std::vector<std::vector<std::pair<long long, long long>>>& lista,int n, int start)
{
    std::vector<long long> dystans(n, INFLL);
    std::vector<int> poprzedni(n, -1);
    std::vector<bool> odwiedzony(n, false);

    dystans[start] = 0;
    using P = std::pair<long long, int>; // {dystans, krawedz}
    std::priority_queue<P, std::vector<P>, std::greater<>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (odwiedzony[u]) continue;
        odwiedzony[u] = true;

        for (const auto& [v, waga] : lista[u]) {
            if (!odwiedzony[v] && dystans[u] + waga < dystans[v]) {
                dystans[v] = dystans[u] + waga;
                poprzedni[v] = u;
                pq.push({dystans[v], v});
            }
        }
    }

    return {dystans, poprzedni};
}
// Funkcja do wypisywania ścieżki z wierzchołka startowego do docelowego dla macierzy sąsiedztwa
// Parametry: graph - macierz sąsiedztwa, poprzedni - wektor poprzedników, dystans - wektor dystansów, start - wierzchołek startowy, cel - wierzchołek docelowy
void wypiszSciezke( std::vector<std::vector<int>>& graph, std::vector<int>& poprzedni, std::vector<int>& dystans, int start, int cel) {
    std::vector<int> sciezka;
    int aktualny = cel;

    // Jeśli nie ma ścieżki
    if (poprzedni[aktualny] == -1 && aktualny != start) {
        std::cout << "Brak sciezki z " << start << " do " << cel << std::endl;
        return;
    }

    // Składanie ścieżki w odwrotnej kolejności
    while (aktualny != -1) {
        sciezka.push_back(aktualny);
        if (aktualny == start) break;
        aktualny = poprzedni[aktualny];
    }

    reverse(sciezka.begin(), sciezka.end());

    std::cout << "Sciezka z " << start << " do " << cel << ":\n";
    int total = 0;
    for (size_t i = 0; i + 1 < sciezka.size(); ++i) {
        int u = sciezka[i];
        int v = sciezka[i + 1];
        int waga = graph[u][v];
        std::cout << u << " --(" << waga << ")--> ";
        total += waga;
    }
    std::cout << sciezka.back() << std::endl;
    std::cout << "Laczna waga sciezki: " <<total << "\n";
}
// Funkcja do wypisywania ścieżki z wierzchołka startowego do docelowego dla listy sąsiedztwa
// Parametry: lista - lista sąsiedztwa, poprzedni - wektor poprzedników, dystans - wektor dystansów, start - wierzchołek startowy, cel - wierzchołek docelowy
void wypiszSciezke(const std::vector<std::vector<std::pair<long long, long long>>>& lista,const std::vector<int>& poprzedni,const std::vector<long long>& dystans,int start, int cel)
{
    std::vector<int> sciezka;
    int aktualny = cel;
    // Jeśli nie ma ścieżki
    if (poprzedni[aktualny] == -1 && aktualny != start) {
        std::cout << "Brak sciezki z " << start << " do " << cel << std::endl;
        return;
    }
    // Składanie ścieżki w odwrotnej kolejności
    while (aktualny != -1) {
        sciezka.push_back(aktualny);
        if (aktualny == start) break;
        aktualny = poprzedni[aktualny];
    }
    std::reverse(sciezka.begin(), sciezka.end());
    std::cout << "Sciezka z " << start << " do " << cel << ":\n";
    long long total = 0;

    for (size_t i = 0; i + 1 < sciezka.size(); ++i) {
        int u = sciezka[i];
        int v = sciezka[i + 1];
        long long waga = -1;
        // Szukamy wagi krawędzi u -> v w liście sąsiedztwa
        for (const auto& [sasiad, koszt] : lista[u]) {
            if (sasiad == v) {
                waga = koszt;
                break;
            }
        }
        if (waga == -1) {
            std::cout << "Blad: nie znaleziono krawedzi " << u << " -> " << v << std::endl;
            return;
        }
        std::cout << u << " --(" << waga << ")--> ";
        total += waga;
    }
    std::cout << sciezka.back() << std::endl;
    std::cout << "Laczna waga sciezki: " << total << std::endl;
}
// Funkcja do przeprowadzania testów czasowych algorytmu Dijkstry
// Parametry: liczbaWierzcholkow - liczba wierzchołków, gestosc - gęstość grafu, maxWaga - maksymalna waga krawędzi
void test(int &liczbaWierzcholkow, int &gestosc, int &maxWaga) {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::microseconds;
    std::vector<std::vector<double>> czasyMacierz(5, std::vector<double>(3, 0.0)); // 5 rozmiarów, 3 gęstości
    std::vector<std::vector<double>> czasyLista(5, std::vector<double>(3, 0.0)); // 5 rozmiarów, 3 gęstości
    double czasSredniMacierz=0.0;
    double czasSredniLista=0.0;
    int wielkosci[]={100,500,1000,3000,5000};
    int gestosci[]={25,50,75};
    int start = 0; // Wierzchołek startowy
    for(int i=0;i<5;i++){
        liczbaWierzcholkow = wielkosci[i];
        maxWaga=wielkosci[i]+10;
        for(int j=0;j<3;j++){
            gestosc=gestosci[j];
            for(int k=0;k<20;k++){
                utworzGrafLosowy(liczbaWierzcholkow, gestosc, maxWaga);
                auto t1 = high_resolution_clock::now();
                auto [dystansM, poprzedniM] = dijkstraMatrix(graph, liczbaWierzcholkow, start);
                auto t2 = high_resolution_clock::now();
                auto czasMacierz = duration_cast<microseconds>(t2 - t1).count();
                czasSredniMacierz += czasMacierz;
                auto t3 = high_resolution_clock::now();
                auto [dystansL, poprzedniL] = dijkstraLista(lista, liczbaWierzcholkow, start);
                auto t4 = high_resolution_clock::now();
                auto czasLista = duration_cast<microseconds>(t4 - t3).count();
                czasSredniLista += czasLista;
                std::cout<<"Ukonczono test nr: "<<k+1<<" z 20 dla liczby wierzcholkow: "<<wielkosci[i]<<" i gestosci: "<<gestosci[j]<<"%"<<std::endl;
            }
            czasyMacierz[i][j] = czasSredniMacierz / 20.0; // Średni czas dla macierzy
            czasyLista[i][j] = czasSredniLista / 20.0; // Średni czas dla listy
            czasSredniMacierz = 0.0; // Resetowanie średniego czasu dla macierzy
            czasSredniLista = 0.0; // Resetowanie średniego czasu dla listy
        }
        std::cout<<"Ukonczono test dla wielkosci: "<<wielkosci[i]<<std::endl;
    }
    //Wypisanie wyników do pliku
    std::ofstream wyniki("wyniki_testu.txt");
    if (wyniki.is_open()) {
        wyniki << "Wyniki testu Dijkstry:\n";
        wyniki << "Liczba Wierzcholkow;Gestosc;Czas Macierz (μs);Czas Lista (μs)\n";
        for(int i=0;i<5;i++){
            for(int j=0;j<3;j++){
                wyniki << wielkosci[i] << ";" << gestosci[j] << ";" 
                       << czasyMacierz[i][j] << ";" 
                       << czasyLista[i][j] << "\n";
            }
        }
        wyniki.close();
        std::cout<<"Wyniki testu zapisano do pliku wyniki_testu.txt\n";
    } else {
        std::cerr << "Nie mozna otworzyc pliku do zapisu wynikow." << std::endl;
    }
}
int main(){
    int dzialanieTryb = 0;
    int finish = 0;
    int liczbaWierzcholkow = 0;
    int liczbaKrawedzi = 0;
    int start = 0, cel = 0; 
    int gestosc = 0, maxWaga = 0;
    while(true){
        tryb(&dzialanieTryb);
        switch(dzialanieTryb){
            case 1:{
                std::ifstream plik=otworzPlik();
                if(!plik) {
                    break;
                }
                utworzGrafMacierz(plik, liczbaWierzcholkow, liczbaKrawedzi);
                plik.seekg(0, std::ios::beg);
                utworzGrafLista(plik, liczbaWierzcholkow, liczbaKrawedzi);
                zamknijPlik(plik);
                break;
            }
            case 2:{
                std::cout<<"Podaj liczbe wierzcholkow: ";
                std::cin >> liczbaWierzcholkow;
                std::cout<<"Podaj gestosc (w procentach): ";
                std::cin >> gestosc;
                if(gestosc < 0 || gestosc > 100) {
                    std::cerr << "Bląd: Gestosc musi być w zakresie od 0 do 100." << std::endl;
                    break;
                }
                std::cout<<"Podaj maksymalna wage krawedzi: ";
                std::cin >> maxWaga;
                if(maxWaga <= 0) {
                    std::cerr << "Blad: Maksymalna waga krawedzi musi być wieksza od 0." << std::endl;
                    break;
                }
                utworzGrafLosowy(liczbaWierzcholkow, gestosc,maxWaga);
                std::cout<<"Utworzono graf losowy o liczbie wierzcholkow: " << liczbaWierzcholkow << " i liczbie krawedzi: " << liczbaKrawedzi << std::endl;
                break;
            }
            case 31:{
                wypiszMacierz(liczbaWierzcholkow);
                break;
            }
            case 32:{
                wypiszListe(liczbaWierzcholkow);
                break;
            }
            case 33:{
                wypiszMacierz(liczbaWierzcholkow);
                wypiszListe(liczbaWierzcholkow);
                break;
            }
            case 4:
                usunMacierz(liczbaWierzcholkow);
                liczbaWierzcholkow = 0; // Resetowanie liczby wierzchołków
                liczbaKrawedzi = 0; // Resetowanie liczby krawędzi
                break;
            case 5:{
                std::cout << "Podaj wierzcholek startowy: ";
                std::cin >> start;
                std::cout << "Podaj wierzcholek docelowy: ";
                std::cin >> cel;
                if (start < 0 || start >= liczbaWierzcholkow || cel < 0 || cel >= liczbaWierzcholkow) {
                    std::cerr << "Blad: Wierzcholki poza zakresem." << std::endl;
                    break;
                }
                if (graph.empty()) {
                    std::cerr << "Blad: Graf nie został zainicjalizowany." << std::endl;
                    break;
                }
                std::cout<<"Dijkstra dla macierzy:\n";
                auto [dystansM, poprzedniM] = dijkstraMatrix(graph, liczbaWierzcholkow, start);
                wypiszSciezke(graph, poprzedniM, dystansM, start, cel);
                std::cout<<"Dijkstra dla listy:\n";
                auto [dystansL, poprzedniL] = dijkstraLista(lista, liczbaWierzcholkow, start);
                wypiszSciezke(lista, poprzedniL, dystansL, start, cel);
                break;
            }
            case 6:
                test(liczbaWierzcholkow, liczbaKrawedzi, liczbaKrawedzi);
                break;       
        }
    }
    std::cout << "Wybrano tryb: " << dzialanieTryb << std::endl;
    return 0;
}