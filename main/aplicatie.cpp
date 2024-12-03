#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <fstream>
#include <sstream>

#include "aplicatie.h"
#include "utilizator.h"
#include "angajat.h"
#include "admin.h"

extern unordered_map<unsigned, shared_ptr<Utilizator>> utilizatori;
extern Angajat angajati;
extern Admin admini;
extern Aplicatie sistem;

using namespace std;

Aplicatie::Aplicatie()
{
	this->user = "";
	this->parola = "";
	this->control = 0;
	this->cod = 0;
}

Aplicatie::~Aplicatie()
{

}

bool Aplicatie::initializare_sistem()
{

	ifstream file("date_angajati.txt");
	if (!file.is_open())
	{
		cerr << "Nu s-a putut deschide fisierul date_angajati.txt!";
		return false;
	}

	utilizatori.clear();
	string linie;

	while (getline(file, linie))
	{
        try
        {
            stringstream ss(linie);
            string nume, prenume, username, parola, departament;
            unsigned varsta, cod, vechime, risc;

            ss >> nume >> prenume >> username >> parola >> varsta >> cod >> vechime >> risc >> departament;

            if (ss.fail())
            {
                throw runtime_error("Date invalide în linie: " + linie);
            }
            
            if (utilizatori.find(cod) != utilizatori.end())
            {
                throw runtime_error("Cod duplicat: " + to_string(cod));
            }
                
            if (departament == "HR")
            {
                auto adminPtr = make_shared<Admin>(nume, prenume, username, parola, departament, varsta, cod, vechime, risc);
                utilizatori.insert({ cod, adminPtr });
            }
            else
            {
                auto angajatPtr = make_shared<Angajat>(nume, prenume, username, parola, departament, varsta, cod, vechime, risc);
                utilizatori.insert({ cod, angajatPtr });
            }
        }
        catch (const exception& e)
        {
            cerr << "Eroare: " << e.what() << endl;
        }
	}

	file.close();
	return true;
}

bool Aplicatie::login()
{
    control = 999;
    cod = 999;
    
    cout << "Introduceti username: ";
    cin >> user;
    cout << "Introduceti parola: ";
    cin >> parola;

    for (auto it = utilizatori.begin(); it != utilizatori.end(); ++it)
    {
        unsigned key = it->first;                      
        shared_ptr<Utilizator> utilizator = it->second; 

        if (utilizator->getUsername() == user && utilizator->getParola() == parola)
        {
            if (utilizator->getDepartament() == "HR")
            {
                control = 1;
            }
            else
            {
                control = 0;
            }

            cod = utilizator->getCod();
            cout << "Autentificare reusita! Bine ai venit, " << utilizator->getNume() << "!" << endl;
            return true;
        }
    }
    cout << "Autentificare esuata! Username sau parola incorecte." << endl;
    return false;
}

void Aplicatie::meniu()
{
    unsigned comanda = 999;

    // 0 = Angajat fara atributii de admin
    // 1 = Angajat cu atributii de admin (HR)

    cout << "Bine ati venit, introduceti userul si parola pentru autentificare:\n\n";
    if (login() == true)
    {
        switch (control)
        {
        case 0:
            //system("cls");
            cout << "Operatiuni posibile:\n\n";
            cout << "1.Vizualizare date disponibile\n";
            cout << "2.Schimbare username\n";
            cout << "3.Schimbare parola\n";
            cout << "4.Exit\n";
            do
            {
                cout << "Introduceti comanda: ";
                cin >> comanda;
                system("cls");
                switch (comanda)
                {
                case 1:
                    angajati.afisare_info_angajat(cod);
                    break;
                case 2:
                    angajati.schimbare_user(cod);
                    break;
                case 3:
                    angajati.schimbare_parola(cod);
                    break;
                default:
                    cout << "Comanda invalida\n";
                }
            } while (comanda > 4);
            break;

        case 1:
            //TO DO
            //system("cls");
            cout << "Operatiuni posibile:\n\n";
            cout << "1.Adaugare angajat\n";
            cout << "2.Eliminare angajat\n";
            cout << "3.Editare date angajat\n";
            cout << "4.Editare salariu angajat\n";
            cout << "5.Crestere salariala departament\n";
            cout << "6.Exit\n\n";
            do
            {
                cout << "Introduceti comanda: ";
                cin >> comanda;
                system("cls");
                switch (comanda)
                {
                case 1:
                    admini.adaugare_angajat();
                    break;
                case 2:
                    admini.eliminare_angajat();
                    break;
                case 3:
                    admini.editare_angajat();
                    break;
                case 4:
                    //salarizare.initializare_date_salariu();
                    //salarizare.editare_salariu();
                    break;
                case 5:
                    //salarizare.crestere_salariu_departamente();
                    break;
                case 6:

                    break;
                default:
                    cout << "Comanda invalida\n";
                }
            } while (comanda > 6);
            break;
        default:
            cout << "Eroare la stabilirea atributilor userului";
            break;
        }
    }
}

bool Aplicatie::salvare_date()
{
    ofstream file("date_angajati.txt");
    if (!file.is_open())
    {
        cerr << "Nu s-a putut deschide fisierul";
        return false;
    }

    for (const auto& pair :utilizatori)
    {
        const auto& utilizator = pair.second;
        
        if (auto adminPtr = dynamic_pointer_cast<Admin>(utilizator))
        {
            file << utilizator->getNume() << " " << utilizator->getPrenume() << " " << utilizator->getUsername() << " " << utilizator->getParola() << " " << adminPtr->getVarsta() << " " << adminPtr->getCod() << " " <<
                adminPtr->getVechimeAngajat() << " " << adminPtr->getClasaRisc() << " " << adminPtr->getDepartament()<<"\n";
        }

        if (auto angajatPtr = dynamic_pointer_cast<Angajat>(utilizator))
        {
            file << utilizator->getNume() << " " << utilizator->getPrenume() << " " << utilizator->getUsername() << " " << utilizator->getParola() << " " << angajatPtr->getVarsta() << " " << angajatPtr->getCod() << " " <<
                angajatPtr->getVechimeAngajat() << " " << angajatPtr->getClasaRisc() << " " << angajatPtr->getDepartament() << "\n";
        }
        
    }

    file.close();
    return true; // Operațiunea a reușit
    
}