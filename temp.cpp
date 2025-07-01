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
#define ll long long
int** graph = nullptr; // wskaźnik globalny
std::vector<std::vector<std::pair<ll, ll>>> lista;
std::ifstream otworzPlik(){
    std::cout<<"Podaj nazwe pliku: ";
    std::string nazwaPliku;
    std::cin >> nazwaPliku;
    std::ifstream plik(nazwaPliku);
    if(!plik) {
        std::cerr << "Nie mozna otworzyc pliku: " << nazwaPliku << std::endl;
        return std::ifstream(); // zwraca pusty ifstream;
    }else{
        std::cout << "Plik otwarty pomyslnie: " << nazwaPliku << std::endl;
    }
    return plik;
}
void zamknijPlik(std::ifstream &plik) {
    if (plik.is_open()) {
        plik.close();
        std::cout << "Plik zostal zamkniety." << std::endl;
    } else {
        std::cerr << "Plik nie jest otwarty." << std::endl;
    }
}
void utworzMacierz(int liczbaWierzcholkow) {
    // Tworzenie wektora o zadanej liczbie wierzchołków
    graph = new int*[liczbaWierzcholkow];
    for (int i = 0; i < liczbaWierzcholkow; ++i) {
        graph[i] = new int[liczbaWierzcholkow];
    
        for (int j = 0; j < liczbaWierzcholkow; ++j) {
            graph[i][j] = -1; // Inicjalizacja macierzy z wartością -1 (brak krawędzi)
        }
    }
}
void usunMacierz(int liczbaWierzcholkow) {
    // Usuwanie macierzy
    for (int i = 0; i < liczbaWierzcholkow; ++i) {
        delete[] graph[i];
    }
    delete[] graph;
    graph = nullptr; // Ustawienie wskaźnika na nullptr po usunięciu
}
// utworz vector par
void utworzListe(int liczbaWierzcholkow) {
        lista.clear();
        lista.resize(liczbaWierzcholkow); // Inicjalizacja listy sąsiedztwa
}
void usunListe(std::vector<std::pair<int, int>>* lista) {
    lista->clear(); // Usuwa wszystkie elementy z listy
}


void wypiszGraf(int liczbaWierzcholkow){
    std::cout << "Macierz sasiedztwa grafu:\n";
    for (int i = 0; i < liczbaWierzcholkow; ++i) {
        for (int j = 0; j < liczbaWierzcholkow; ++j) {
            std::cout.width(6);
            std::cout << graph[i][j];
        }
        std::cout << "\n";
    }
}
void wypiszListe(int liczbaWierzcholkow){
       for(int i=0;i<liczbaWierzcholkow;i++){
        std::cout<<"vertex "<<i<<" has";
        if(lista[i].size()==0){
            std::cout<<" no neighbours";
        }
        else{
            std::cout<<" neighbours ";
            for(int j=0;j<lista[i].size();j++){
                std::cout<<lista[i][j].first<<" weight "<< lista[i][j].second<<"; ";
            }
        }
        std::cout<<"\n";
    }
}
//utworzenie grafu w reprezentacji macierzowej
void utworzGrafMacierz(std::ifstream &plik) {
    //wczytanie liczby wierzchołków i krawędzi z pliku
    int liczbaWierzcholkow, liczbaKrawedzi;
    std::string linia;
    std::string tempString;

    plik >> liczbaKrawedzi >> liczbaWierzcholkow;
    utworzMacierz(liczbaWierzcholkow); // Inicjalizacja macierzy
    // Wczytanie krawędzi z pliku
    int a, b, c;
    for(int i = 0; i < liczbaKrawedzi; i++) {
        plik >> a >> b >> c;
        if (a < liczbaWierzcholkow && b < liczbaWierzcholkow) {
            graph[a][b] = c; // Ustawienie wagi krawędzi
        } else {
            std::cerr << "Bład: Wierzcholki poza zakresem." << std::endl;
        }
    }
    std::cout << "Liczba wierzcholkow: " << liczbaWierzcholkow << std::endl;
    std::cout << "Liczba krawedzi: " << liczbaKrawedzi << std::endl;
    wypiszGraf(liczbaWierzcholkow); // Wyświetlenie macierzy sąsiedztwa
}
//utworzenie grafu w reprezentacji listowej
void utworzGrafLista(std::ifstream &plik){
    int liczbaWierzcholkow, liczbaKrawedzi;
    std::string linia;
    std::string tempString;
    plik >> liczbaKrawedzi >> liczbaWierzcholkow;
    utworzListe(liczbaWierzcholkow); // Inicjalizacja listy
    int a, b, c;
       for (int i=0;i<liczbaKrawedzi;i++){
        plik>>a>>b>>c;
        lista[a].push_back({b,c});
       }
       wypiszListe(liczbaWierzcholkow); // Wyświetlenie listy sąsiedztwa
    }
void utworzGrafLosowy(){
    std::cout<<"Podaj liczbe wierzcholkow: ";
    int liczbaWierzcholkow;
    std::cin >> liczbaWierzcholkow;
    std::cout<<"Podaj gestosc (w procentach): ";
    int gestosc;
    std::cin >> gestosc;
    if(gestosc < 0 || gestosc > 100) {
        std::cerr << "Błąd: Gęstość musi być w zakresie od 0 do 100." << std::endl;
        return;
    }

    utworzMacierz(liczbaWierzcholkow); // Inicjalizacja macierzy
    utworzListe(liczbaWierzcholkow); // Inicjalizacja listy
    int maxLiczbaKrawedzi = liczbaWierzcholkow * (liczbaWierzcholkow - 1);
    int liczbaKrawedzi = (maxLiczbaKrawedzi * gestosc) / 100;
    int maxWaga = 100; // Maksymalna waga krawędzi
    std::cout << "Liczba krawedzi: " << liczbaKrawedzi << std::endl;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> krawedzDist(0, liczbaWierzcholkow-1);
    std::uniform_int_distribution<> wagaDist(1, maxWaga);
    std::set<std::pair<int, int>> istniejaceKrawedzie;

    while((int)istniejaceKrawedzie.size() < liczbaKrawedzi) {
        int a = krawedzDist(gen);
        int b = krawedzDist(gen);
        std::cout<<"a: "<<a<<" b: "<<b<<std::endl;
        if (a != b && istniejaceKrawedzie.count({a, b}) == 0) { // Sprawdzenie, czy krawędź już istnieje
            int waga = wagaDist(gen); // Losowanie wagi krawędzi
            graph[a][b] = waga; // Ustawienie wagi krawędzi w macierzy
            lista[a].push_back({b, waga}); // Dodanie krawędzi do listy sąsiedztwa
            istniejaceKrawedzie.insert({a, b});
        }
    }
    std::cout << "Liczba wierzcholkow: " << liczbaWierzcholkow << std::endl;
    std::cout << "Liczba krawedzi: " << liczbaKrawedzi << std::endl;
    wypiszGraf(liczbaWierzcholkow); // Wyświetlenie macierzy sąsiedztwa
    wypiszListe(liczbaWierzcholkow); // Wyświetlenie listy sąsiedztwa
}
std::vector<ll> dijkstra(
    const std::vector<std::vector<std::pair<ll, ll>>>& adj,
    int liczbaWierzcholkow,
    int source,
    int target
) {
    std::vector<ll> dist(liczbaWierzcholkow, std::numeric_limits<ll>::max());
    std::vector<int> prev(liczbaWierzcholkow, -1);
    using pli = std::pair<ll, int>; // (distance, vertex)
    std::priority_queue<pli, std::vector<pli>, std::greater<pli>> pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        ll d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;

        for (const auto& edge : adj[u]) {
            int v = edge.first;
            ll w = edge.second;
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    // Reconstruct path
    std::vector<ll> path;
    if (dist[target] == std::numeric_limits<ll>::max()) {
        return path; // No path
    }
    for (int v = target; v != -1; v = prev[v])
        path.push_back(v);
    std::reverse(path.begin(), path.end());
    return path;
}

int main(){
    std::ifstream plik = otworzPlik();
    if(!plik) {
        return 1; // Zakończ program, jeśli plik nie został otwarty
    }
    
    utworzGrafMacierz(plik);
    // plik.clear();
    plik.seekg(0, std::ios::beg);
    utworzGrafLista(plik);
   
    zamknijPlik(plik);
    return 0;
}