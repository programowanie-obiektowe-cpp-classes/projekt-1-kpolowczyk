#include "PrintHello.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>


// Klasa bazowa Pracownik
class Pracownik {
protected:
    std::string imie;

public:
    virtual void print() const = 0;
    virtual ~Pracownik() = default;
    virtual double getPensja() const=0;
    //virtual double getWydajnosc() const=0;
};

// Inżynier
class Inz : public Pracownik {

double pensja=5000;
//static const double wydajnosc=10;

public:
    std::string wydzial;

    Inz(std::string cin_imie="Kamil", std::string cin_wydzial="Meil") {
        imie = cin_imie;
        wydzial = cin_wydzial;
    }

    void print() const override {
        std::cout << "Inzynier: " << imie << ", wydzial: " << wydzial << std::endl;
    }

    double getPensja() const override {return pensja;}
    //double getWydajnosc() const override {return wydajnosc;}
};

// Magazynier
class Mag : public Pracownik {
double pensja=4000;
//static const double wydajnosc=10;

public:
    bool obsl_widl;
    
    Mag(std::string cin_imie="Kamil", bool cin_obsl_widl=false) {
        imie = cin_imie;
        obsl_widl = cin_obsl_widl;
    }

    void print() const override {
        std::cout << "Magazynier: " << imie << ", obsługa wózka widłowego: " << (obsl_widl ? "tak" : "nie") << std::endl;
    }
    double getPensja() const override {return pensja;}
    //double getWydajnosc() const override {return wydajnosc;}
};

// Marketer
class Mkt : public Pracownik {
double pensja=4000;
//static const double wydajnosc=10;

public:
    int follows;


    Mkt(std::string cin_imie="Kamil", int cin_follows=100) {
        imie = cin_imie;  
        follows = cin_follows;  
    }

    void print() const override {
        std::cout << "Marketer: " << imie << ", liczba followersów: " << follows << std::endl;
    }
    double getPensja() const override {return pensja;}
    //double getWydajnosc() const override {return wydajnosc;}
};

// Robotnik
class Rob : public Pracownik {
double pensja=3000;
//static const double wydajnosc=10;

public:
    double but;

    Rob(std::string cin_imie="Kamil", double cin_but=44) {
        imie = cin_imie;  
        but = cin_but;  
    }

    void print() const override {
        std::cout << "Robotnik: " << imie << ", rozmiar buta: " << but << std::endl;
    }
    double getPensja() const override {return pensja;}
    //double getWydajnosc() const override {return wydajnosc;}
};

// Klasa Kredyt
class Kredyt {
public:
    double dlug;
    int pozostale_raty;

    Kredyt(double dlug, int pozostale_raty) : dlug(dlug), pozostale_raty(pozostale_raty) {}

    double splac_rate() {
        return dlug / pozostale_raty;
    }
};

// Klasa Firma
class Firma {
private:
    double stan_konta;
    int n_kredytow;
    std::vector<std::unique_ptr<Kredyt>> kredyty;//po prostu wektor kredytu
    std::vector<std::unique_ptr<Pracownik>> prac;

    double l_prac[4]={0,0,0,0};
    double wydajnosc[4]={10,10,10,10};
    double przychod=0;
    double pojemnosc_magazynu=0;
    double rata=0;
    double wyplata=0;


public:



    Firma(double poczatkowy_stan) : stan_konta(poczatkowy_stan), n_kredytow(0) {}

    void zatrudnij(std::unique_ptr<Pracownik> p) 
    {
        if (dynamic_cast<Inz*>(p.get()) != nullptr) {l_prac[0]++;} 
        if (dynamic_cast<Mag*>(p.get()) != nullptr) {l_prac[1]++;}
        if (dynamic_cast<Mkt*>(p.get()) != nullptr) {l_prac[2]++;}
        if (dynamic_cast<Rob*>(p.get()) != nullptr) {l_prac[3]++;}
    
        prac.push_back(std::move(p));//dopisać metody specjalne 
    }

    void drukuj_pracownikow() const {
        for (const auto& p : prac) {p->print();}
    }

    void wez_kredyt(double kwota, int czas_splaty) {
        kredyty.push_back(std::make_unique<Kredyt>(kwota, czas_splaty));
        n_kredytow++;
        stan_konta += kwota;
    }

    void zaplac_wynagrodzenie() {
        

        for (const auto& p : prac) {
            double wyplata_pracownika=p->getPensja();
            wyplata+=wyplata_pracownika;}
        stan_konta-=wyplata;
    }

    void otrzymaj_przychod() {
        double cena_produktu=l_prac[0]*wydajnosc[0];//Inżynier
        double popyt=l_prac[1]*wydajnosc[1];//Marketing
        pojemnosc_magazynu=l_prac[2]*wydajnosc[2];//Magazynier
        double teoretyczna_liczba_produktow=l_prac[3]*wydajnosc[3];//Robotnik

        double liczba_produktow = std::min(teoretyczna_liczba_produktow, pojemnosc_magazynu);//ile można wyprodukować znając ograniczenie magazynowe
        double sprzedane = std::min(popyt, liczba_produktow);
        przychod=sprzedane*cena_produktu;
    }

    void splac_raty() {
        for (auto& kredyt : kredyty) {
            stan_konta -= kredyt->splac_rate();
            rata=kredyt->splac_rate();
        }
    }

    double get_stan_konta() const {
        
        return stan_konta;
    }

    void podsumowanie()
    {
        std::cout<<"------------------"<<std::endl;
        std::cout<<"STAN KONTA: "<<stan_konta<<std::endl;
        std::cout<<"PRZYCHOD:   "<<przychod<<"  (wielkosc magazynu: "<<pojemnosc_magazynu<<")"<<std::endl;
        std::cout<<"WYPLATY:    "<<wyplata<<std::endl;
        std::cout<<"RATA KREDYTU    "<<rata<<std::endl;
        std::cout<<"------------------"<<std::endl;
    }
};

// Klasa Gra
class Gra {
private:
    bool stan_gry;
    bool tura=true;
    Firma firma;
    int miesiac=1;
    double stawka_wygranej=500000;

public:
    Gra() : firma(100000), stan_gry(true) {}

    void akcja_gracza() {
        tura=true;
        while(tura){
        int wybor;
        std::cout << "Miesiac:  " <<miesiac<< std::endl;
        std::cout << "Wybierz akcje: " << std::endl;
        std::cout << "1. Zatrudnij pracownika" << std::endl;
        std::cout << "2. Wez kredyt" << std::endl;
        std::cout << "3. Drukuj pracownikow" << std::endl;
        std::cout << "4. Nastepny miesiac" << std::endl;
        std::cout << "5. Zakoncz gre" << std::endl;
        std::cin >> wybor;

        switch (wybor) {
            case 1: {
                int typ_pracownika;
                std::cout << "Wybierz typ pracownika do zatrudnienia: " << std::endl;
                std::cout << "1. Inzynier" << std::endl;
                std::cout << "2. Marketer" << std::endl;
                std::cout << "3. Magazynier" << std::endl;
                std::cout << "4. Robotnik" << std::endl;
                std::cin >> typ_pracownika;

                std::string imie;
                std::cout << "Podaj imie pracownika: ";
                std::cin >> imie;

                switch (typ_pracownika) {
                    case 1: {
                        std::string wydzial;
                        std::cout << "Podaj wydzial: ";
                        std::cin >> wydzial;
                        firma.zatrudnij(std::make_unique<Inz>(imie, wydzial));

                        break;
                    }
                    case 2: {
                        int follows;
                        std::cout << "Podaj liczbe obserwujacych: ";
                        std::cin >> follows;
                        firma.zatrudnij(std::make_unique<Mkt>(imie, follows));

                        break;
                    }
                    case 3: {
                        bool obsl_widl;
                        std::cout << "Czy obsluguje wozek widlowy? (1 - tak, 0 - nie): ";
                        std::cin >> obsl_widl;
                        firma.zatrudnij(std::make_unique<Mag>(imie, obsl_widl));

                        break;
                    }
                    case 4: {
                        double but;
                        std::cout << "Podaj rozmiar buta: ";
                        std::cin >> but;
                        firma.zatrudnij(std::make_unique<Rob>(imie, but));

                        break;
                    }
                    default:
                        std::cout << "Niepoprawny wybor." << std::endl;
                        break;
                }
                break;
            }
            case 2: {
                double kwota;
                int czas_splaty;
                std::cout << "Podaj kwote kredytu: ";
                std::cin >> kwota;
                std::cout << "Podaj czas splaty (w miesiacach): ";
                std::cin >> czas_splaty;
                firma.wez_kredyt(kwota, czas_splaty);
                break;
            }
            case 3:
                firma.drukuj_pracownikow();
                break;

            case 4:
                tura = false;
                miesiac++;
                break;//firma.nast_miesiac
                //break>>return 

            case 5:
                stan_gry = false;
                tura = false;
                break;
            default:
                std::cout << "Niepoprawny wybor." << std::endl;
                break;
        }
        }
    }

    bool get_stan(){
        if (stan_gry==true){
            if (firma.get_stan_konta()<0){
                std::cout << "PORAZKA" << std::endl; 
                return false;
                }
            else if (firma.get_stan_konta()>stawka_wygranej) {
                std::cout << "WYGRANA" << std::endl; 
                return false;}
            else return true;
        }
        else return false;
    }

    void tick() {
        firma.otrzymaj_przychod();
        firma.zaplac_wynagrodzenie();
        firma.splac_raty();
        firma.podsumowanie();
    }

    
};

// Główna funkcja gry
int main() {
    Gra gra;
    while (gra.get_stan()) {
        gra.akcja_gracza();
        gra.tick();
    }
    return 0;


}
