#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

const vector<string> WARTOSCI_KART = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
const vector<string> KOLORY_KART = { "T", "k", "K", "P" }; // Trefl, karo, Kier, Pik

struct Karta {
    string wartosc;
    string kolor;

    string toString() const {
        if (wartosc.empty()) return "[PUSTA]";
        return wartosc + kolor;
    }
};

//Tasowanie talii algorytmem Fisher-Yates
void potasujTalie(vector<Karta>& talia) {
    int n = talia.size();
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        Karta temp = talia[i];
        talia[i] = talia[j];
        talia[j] = temp;
    }
}

bool czyPoprawnaWartosc(const string& wartosc) {
    for (int i = 0; i < WARTOSCI_KART.size(); ++i) {
        if (WARTOSCI_KART[i] == wartosc) {
            return true;
        }
    }
    return false;
}

class GraKarciana {
private:
    vector<Karta> talia;
    vector<Karta> rekaGracza;
    vector<Karta> rekaKomputera;
    int stosyGracza;
    int stosyKomputera;

    void inicjalizujTalie() {
        talia.clear();
        for (int i = 0; i < WARTOSCI_KART.size(); ++i) {
            for (int j = 0; j < KOLORY_KART.size(); ++j) {
                Karta nowaKarta;
                nowaKarta.wartosc = WARTOSCI_KART[i];
                nowaKarta.kolor = KOLORY_KART[j];
                talia.push_back(nowaKarta);
            }
        }
        potasujTalie(talia);
    }

    Karta dobierzKarte() {
        if (talia.empty()) {
            return { "", "" };
        }
        Karta karta = talia.back();
        talia.pop_back();
        return karta;
    }

    void rozdajKarty(int iloscKart) {
        rekaGracza.clear();
        rekaKomputera.clear();
        for (int i = 0; i < iloscKart; ++i) {
            Karta kartaDlaGracza = dobierzKarte();
            if (!kartaDlaGracza.wartosc.empty()) rekaGracza.push_back(kartaDlaGracza);
            Karta kartaDlaKomputera = dobierzKarte();
            if (!kartaDlaKomputera.wartosc.empty()) rekaKomputera.push_back(kartaDlaKomputera);
        }
    }

    bool czyMaKarte(const vector<Karta>& reka, const string& wartosc) {
        for (int i = 0; i < reka.size(); ++i) {
            if (reka[i].wartosc == wartosc) {
                return true;
            }
        }
        return false;
    }

    vector<Karta> przekazKarty(vector<Karta>& rekaOddajaca, const string& wartosc) {
        vector<Karta> przekazane;
        for (int i = rekaOddajaca.size() - 1; i >= 0; --i) {
            if (rekaOddajaca[i].wartosc == wartosc) {
                przekazane.push_back(rekaOddajaca[i]);
                rekaOddajaca.erase(rekaOddajaca.begin() + i);
            }
        }
        return przekazane;
    }

    void wyswietlStanGracza() {
        cout << "----------------------------------------\n";
        cout << "Stosy Komputera: " << stosyKomputera << endl;
        cout << "Twoje karty: ";
        if (rekaGracza.empty()) {
            cout << "brak";
        }
        else {
            for (int i = 0; i < rekaGracza.size(); ++i) {
                cout << rekaGracza[i].toString() << " ";
            }
        }
        cout << "\nTwoje stosy: " << stosyGracza << endl;
        cout << "(Pozostalo kart w talii: " << talia.size() << ")" << endl;
        cout << "----------------------------------------\n";
    }

    void sprawdzStos(vector<Karta>& reka, int& licznikStosow) {
        for (int w = 0; w < WARTOSCI_KART.size(); ++w) {
            string sprawdzanaWartosc = WARTOSCI_KART[w];
            int licznikKart = 0;
            for (int i = 0; i < reka.size(); ++i) {
                if (reka[i].wartosc == sprawdzanaWartosc) {
                    licznikKart++;
                }
            }

            if (licznikKart == 4) {
                cout << "*** Zebrano stos kart o wartości: " << sprawdzanaWartosc << "! ***" << endl;
                licznikStosow++;

                for (int i = reka.size() - 1; i >= 0; --i) {
                    if (reka[i].wartosc == sprawdzanaWartosc) {
                        reka.erase(reka.begin() + i);
                    }
                }
            }
        }
    }

    void turaGracza() {
        cout << "\n--- Twoja Tura ---" << endl;
        wyswietlStanGracza();

        if (rekaGracza.empty()) {
            if (!talia.empty()) {
                cout << "Nie masz kart! Dobierasz karte." << endl;
                rekaGracza.push_back(dobierzKarte());
                wyswietlStanGracza();
            }
            else {
                cout << "Nie masz kart i talia jest pusta. Koniec tury." << endl;
            }
            return;
        }

        bool pytajDalej = true;
        while (pytajDalej) {
            if (rekaGracza.empty()) {
                cout << "Nie masz wiecej kart do pytania." << endl;
                break;
            }

            string zapytanie;
            bool poprawneZapytanie = false;
            while (!poprawneZapytanie) {
                cout << "Podaj wartosc karty, o ktora chcesz zapytac (np. 7, K, A): ";
                cin >> zapytanie;

                cin.ignore(10000, '\n');

                if (!czyPoprawnaWartosc(zapytanie)) {
                    cout << "Nieprawidlowa wartosc karty. Sprobuj ponownie." << endl;
                }
                else if (!czyMaKarte(rekaGracza, zapytanie)) {
                    cout << "Nie masz karty o tej wartosci w rece! Musisz miec przynajmniej jedna." << endl;
                }
                else {
                    poprawneZapytanie = true;
                }
            }

            cout << "Pytasz komputer o: " << zapytanie << endl;

            if (czyMaKarte(rekaKomputera, zapytanie)) {
                vector<Karta> otrzymane = przekazKarty(rekaKomputera, zapytanie);
                cout << "Komputer przekazuje Ci: ";
                for (int i = 0; i < otrzymane.size(); ++i) {
                    cout << otrzymane[i].toString() << " ";
                    rekaGracza.push_back(otrzymane[i]);
                }
                cout << endl;
                sprawdzStos(rekaGracza, stosyGracza);
                wyswietlStanGracza();
                pytajDalej = true;
                cout << "Masz prawo pytac dalej." << endl;
            }
            else {
                cout << "Komputer odpowiada: Nie mam!" << endl;
                if (!talia.empty()) {
                    Karta dobrana = dobierzKarte();
                    cout << "Dobierasz karte z talii: " << dobrana.toString() << endl;
                    rekaGracza.push_back(dobrana);
                    sprawdzStos(rekaGracza, stosyGracza);
                }
                else {
                    cout << "Talia jest pusta. Nie mozesz dobrac karty." << endl;
                }
                pytajDalej = false;
            }
        }
    }

    void turaKomputera() {
        cout << "\n--- Tura Komputera ---" << endl;
        cout << "Stosy Komputera: " << stosyKomputera << endl;
        cout << "(Komputer ma " << rekaKomputera.size() << " kart)" << endl;


        if (rekaKomputera.empty()) {
            if (!talia.empty()) {
                cout << "Komputer nie ma kart. Dobiera karte." << endl;
                rekaKomputera.push_back(dobierzKarte());
                sprawdzStos(rekaKomputera, stosyKomputera);
            }
            else {
                cout << "Komputer nie ma kart i talia jest pusta. Koniec tury." << endl;
            }
            return;
        }

        bool pytajDalej = true;
        while (pytajDalej) {
            if (rekaKomputera.empty()) {
                cout << "Komputerowi skonczyly sie karty w trakcie tury." << endl;
                break;
            }

            int losowyIndex = rand() % rekaKomputera.size();
            string zapytanie = rekaKomputera[losowyIndex].wartosc;
            cout << "Komputer pyta o: " << zapytanie << endl;

            if (czyMaKarte(rekaGracza, zapytanie)) {
                vector<Karta> otrzymane = przekazKarty(rekaGracza, zapytanie);
                cout << "Przekazujesz komputerowi: ";
                for (int i = 0; i < otrzymane.size(); ++i) {
                    cout << otrzymane[i].toString() << " ";
                    rekaKomputera.push_back(otrzymane[i]);
                }
                cout << endl;
                sprawdzStos(rekaKomputera, stosyKomputera);
                cout << "Komputer pyta dalej." << endl;
                pytajDalej = true;
            }
            else {
                cout << "Odpowiadasz: Nie mam!" << endl;
                if (!talia.empty()) {
                    Karta dobrana = dobierzKarte();
                    cout << "Komputer dobiera karte z talii." << endl;
                    rekaKomputera.push_back(dobrana);
                    sprawdzStos(rekaKomputera, stosyKomputera);
                }
                else {
                    cout << "Talia jest pusta. Komputer nie moze dobrac karty." << endl;
                }
                pytajDalej = false;
            }
        }
    }

public:
    GraKarciana() {
        stosyGracza = 0;
        stosyKomputera = 0;
        inicjalizujTalie();
        rozdajKarty(7);
        sprawdzStos(rekaGracza, stosyGracza);
        sprawdzStos(rekaKomputera, stosyKomputera);
    }

    void rozpocznijGre() {
        cout << "=== ROZPOCZYNAMY ROZGRYWKE! ===" << endl;

        while (stosyGracza + stosyKomputera < 13) {

            if (stosyGracza + stosyKomputera == 13) {
                cout << "\nWszystkie 13 stosow zostalo zebranych! Koniec gry." << endl;
                break;
            }

            turaGracza();

            if (stosyGracza + stosyKomputera == 13) {
                cout << "\nWszystkie 13 stosow zostalo zebranych! Koniec gry." << endl;
                break;
            }

            cout << "\nNacisnij Enter, aby kontynuowac do tury komputera...";
            cin.get();

            turaKomputera();

            if (stosyGracza + stosyKomputera == 13) {
                cout << "\nWszystkie 13 stosow zostalo zebranych! Koniec gry." << endl;
                break;
            }
        }
            cout << "\n========== Koniec Gry ==========" << endl;
            cout << "Ostateczny wynik:" << endl;
            cout << "Twoje stosy: " << stosyGracza << endl;
            cout << "Stosy Komputera: " << stosyKomputera << endl;

            if (stosyGracza > stosyKomputera) {
                cout << "\n*** Gratulacje, wygrales! ***" << endl;
            }
            else if (stosyKomputera > stosyGracza) {
                cout << "\n--- Niestety, komputer wygral. ---" << endl;
            }
        }
};

int main() {
    srand(time(0));

    GraKarciana gra;
    gra.rozpocznijGre();

    cout << "\nNacisnij Enter, aby zakonczyc program...";
    cin.get();

    return 0;
}