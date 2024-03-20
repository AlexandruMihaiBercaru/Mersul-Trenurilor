#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

std::vector<std::string> split(std::string s, const std::string &delimiter);

class Statie {
private:
    std::string denumire;
    int cod; //<->un nod in graf
public:
    explicit Statie(const std::string &denumire_ = "", const int cod_ = 0) : denumire(denumire_), cod(cod_) {}

    const std::string &getDenumire() const {
        return denumire;
    }

    [[nodiscard]] int getCod() const {
        return cod;
    }

    friend std::ostream &operator<<(std::ostream &out, const Statie &statie) {
        out << statie.denumire;
        return out;
    }
};


class Magistrala {
private:

    std::vector<Statie> listaStatii;
    std::vector<int> listaDistante;
    int codMag;
    int nr_statii;

public:
    Magistrala() = default;

//    Magistrala(int cod_, int nr_statii_, const std::vector<Statie> &statii_ , const std::vector<int> &distante_ ) {
//        this->codMag = cod_; this->nr_statii = nr_statii_; this->listaStatii = statii_; this->listaDistante = distante_; }
    Magistrala(const std::vector<Statie> &listaStatii, const std::vector<int> &listaDistante, int codMag, int nrStatii)
            : listaStatii(listaStatii), listaDistante(listaDistante), codMag(codMag), nr_statii(nrStatii) {}

    //Constructor mai specific?? - pentru citire magistrale din fisier + creare obiecte de tip statie (simultan)
    //intrebare daca este ok asa?
    Magistrala(std::string &linie, int &allStationCnt, std::vector<Statie> &allStations) {
        std::vector<std::string> line = split(linie, ", "), denumiriStatiiTemp;
        std::string denStatieCrt;
        std::vector<int> distanteTemp;
        std::vector<Statie> statiiMag; //statiile de pe o magistrala
        int codStatieTemp;
        this->codMag = std::stoi(line[0]);
        this->nr_statii = std::stoi(line[1]);
        for (int i = 0; i < nr_statii; i++) //procesez pe rand statiile din fiecare magistrala, le salvez intr-un vector
        { //creez vectorul de statii din magistrala si il actualizez pe cel cu toate statiile
            denStatieCrt = line[2 + i];
            denumiriStatiiTemp.push_back(denStatieCrt);
            bool existsCrt = false;
            for (const auto &elem: allStations)
                if (elem.getDenumire() == denStatieCrt)
                    existsCrt = true;
            if (!existsCrt) //statia nu a mai fost adaugata pana acum
            {
                allStationCnt++;
                codStatieTemp = allStationCnt;
                Statie statieTemp(denStatieCrt, codStatieTemp);
                allStations.push_back(statieTemp);
                statiiMag.push_back(statieTemp);
            } //am adaugat in vectorul cu toate statiile
            else //exista in vectorul cu toate statiile, o adaug in vectorul cu statiile pentru magistrala -- ii vreau codul? (caut in vector)
            {
                for (auto &elem: allStations) //elem = o statie
                    if (elem.getDenumire() == denStatieCrt)
                        codStatieTemp = elem.getCod();
                Statie statieTemp(denStatieCrt, codStatieTemp);
                statiiMag.push_back(statieTemp);
            }
        }
        this->listaStatii = statiiMag;
        distanteTemp.reserve(nr_statii - 1);
        for (int i = 0; i < nr_statii - 1; i++)
            distanteTemp.push_back(std::stoi(line[2 + nr_statii + i]));
        this->listaDistante = distanteTemp;
    }

    int lungime() const {
        int sum = 0;
        for (int i: listaDistante)
            sum += i;
        return sum;
    }

    int getCodMag() const {
        return codMag;
    }

    friend std::ostream &operator<<(std::ostream &out, const Magistrala &mag) {
        out << "\nMAGISTRALA " << mag.codMag << ": \nLUNGIME: " << mag.lungime() << " km; \nTRASEU: ";
        for (int i = 0; i < mag.nr_statii - 1; i++)
            out << mag.listaStatii[i].getDenumire() << " - ";
        out << mag.listaStatii[mag.nr_statii - 1].getDenumire() << "\n";
        return out;
    }

    friend class Harta;
};


class Harta {
private:
    std::vector<Magistrala> magistrale;
    int nrTotalStatii;
    std::vector<Statie> allStations;
    std::vector<std::vector<int> > legaturi, distante; //matrice!
public:
    Harta(const std::vector<Magistrala> &magistrale_, std::vector<std::vector<int> > &legaturi_,
          std::vector<std::vector<int> > &distante_, int nrTotalStatii_, const std::vector<Statie> &allStations_) :
            magistrale(magistrale_), legaturi(legaturi_), distante(distante_), nrTotalStatii(nrTotalStatii_),
            allStations(allStations_) {}

    void addLegaturi_Costuri() {
        for (auto &elem: magistrale) //pentru fiecare elem <-> pentru fiecare magistrala
        {
            for (int i = 0; i < elem.nr_statii - 1; i++) {
                std::string statiePlecare = elem.listaStatii[i].getDenumire();
                std::string statieSosire = elem.listaStatii[i + 1].getDenumire();
                int codStatiePlecare, codStatieSosire;
                for (auto &elem2: allStations) //elem = o Statie
                {
                    if (elem2.getDenumire() == statiePlecare)
                        codStatiePlecare = elem2.getCod(); //devine nod in graf
                    if (elem2.getDenumire() == statieSosire)
                        codStatieSosire = elem2.getCod(); //devine nod in graf
                }
                this->distante[codStatiePlecare][codStatieSosire] = elem.listaDistante[i];
                this->distante[codStatieSosire][codStatiePlecare] = elem.listaDistante[i];
                this->legaturi[codStatiePlecare][codStatieSosire] = 1;
                this->legaturi[codStatieSosire][codStatiePlecare] = 1;
            }
        }
        //completez matricea de costuri, adica adaug infinit acolo unde nu am arce si i!=j
        for (int i = 1; i <= nrTotalStatii; i++) {
            for (int j = 1; j <= nrTotalStatii; j++)
                if (distante[i][j] == 0 && i != j)
                    distante[i][j] = 0x3F3F3F3F;
        }
    }

    void updateLegaturi_calculDistMinima() //matricea de drumuri + matricea de distante actualizata
    {
        for (int k = 1; k <= nrTotalStatii; k++)
            for (int i = 1; i <= nrTotalStatii; i++)
                for (int j = 1; j <= nrTotalStatii; j++) {
                    if (i != j && legaturi[i][j] == 0)
                        legaturi[i][j] = legaturi[i][k] * legaturi[k][j];
                    if (distante[i][j] > distante[i][k] + distante[k][j])
                        distante[i][j] = distante[i][k] + distante[k][j];
                }
    }

    friend std::ostream &operator<<(std::ostream &out, const Harta &harta) {
        out << "\nDistantele pe legaturi feroviare:\n";
        for (int i = 1; i <= harta.nrTotalStatii; i++) {
            out << "DE LA: " << harta.allStations[i - 1].getDenumire() << " LA: \n";
            for (int j = 1; j <= harta.nrTotalStatii; j++)
                if (harta.distante[i][j] != 0 && harta.distante[i][j] != 0x3F3F3F3F)
                    out << " - " << harta.allStations[j - 1].getDenumire() << ": " << harta.distante[i][j] << " km;\n";
            out << "\n";
        }
        return out;
    }

    void distanteStatie(const std::string &numeStatie) {
        bool found = false;
        int codStatie;
        for (auto &elem: allStations) {
            if (elem.getDenumire() == numeStatie) {
                found = true;
                codStatie = elem.getCod();
            }
        }
        if (!found)
            std::cout << "\nStatia nu a fost gasita!";
        else {
            std::cout << "\n\t----DISTANTELE PE LEGATURI FEROVIARE----\n\tDE LA " << numeStatie << " LA: \n";
            for (int j = 1; j <= nrTotalStatii; j++)
                if (distante[codStatie][j] != 0 && distante[codStatie][j] != 0x3F3F3F3F)
                    std::cout << "\t - " << allStations[j - 1].getDenumire() << ": " << distante[codStatie][j]
                              << " km;\n";
        }
    }
};

//class Vagon{
//    -------TO DO---------
//    std::string confort;
//    int numarLocuri;
//};

class Tren {
    int codTren;
    std::string categorie;
    Magistrala magistrala;
    std::vector<Statie> opriri;
public:
    Tren(int codTren_, std::string &categorie_, Magistrala &magistrala_, std::vector<Statie> &opriri_) :
            codTren(codTren_), categorie(categorie_), magistrala(magistrala_), opriri(opriri_) {}

    Tren(const Tren &trenInit) {
        this->codTren = trenInit.codTren;
        this->categorie = trenInit.categorie;
        this->magistrala = trenInit.magistrala;
        this->opriri = trenInit.opriri;
    }

    Tren &operator=(const Tren &trenInit) {
        this->codTren = trenInit.codTren;
        this->categorie = trenInit.categorie;
        this->magistrala = trenInit.magistrala;
        this->opriri = trenInit.opriri;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &out, const Tren &tren) {
        out << "\nTrenul de categorie " << tren.categorie << " cu indicativul " << tren.codTren
            << " circula pe magistrala " << tren.magistrala.getCodMag() << " si efectueaza opriri in:\n";
        for (auto &elem: tren.opriri)
            out << " - " << elem.getDenumire() << "\n";
        out << "\n";
        return out;
    }

    const std::vector<Statie> &getOpriri() const {
        return opriri;
    }

    int getCodTren() const {
        return codTren;
    }

    ~Tren() = default;


};

class Rutele {
    std::vector<Tren> allTrains;

public:
    explicit Rutele(const std::vector<Tren> &allTrains_) : allTrains(allTrains_) {}

    void arrivals(std::string &stationName) {
        std::cout << "----SOSIRI IN STATIA: " << stationName << "----\n";
        bool exists = false;
        for (auto &crtTrain: allTrains) {
            auto it = std::find_if(crtTrain.getOpriri().begin(), crtTrain.getOpriri().end(),
                                   [&stationName](const Statie &obj) { return obj.getDenumire() == stationName; });
            if (it != crtTrain.getOpriri().end() && stationName !=
                                                    crtTrain.getOpriri()[0].getDenumire()) //statia apare intre opririle trenului si nu este prima
            {
                exists = true;
                std::cout << crtTrain.getCodTren() << " din directia: ";
                auto index = std::distance(crtTrain.getOpriri().begin(), it);
                for (int i = 0; i < index - 1; i++)
                    std::cout << crtTrain.getOpriri()[i].getDenumire() << " - ";
                std::cout << crtTrain.getOpriri()[index - 1].getDenumire() << "\n";
            }
        }
        if (!exists)
            std::cout << "Nu exista sosiri!\n";
    }

    void departures(std::string &stationName) {
        std::cout << "\n----PLECARI DIN STATIA: " << stationName << "----\n";
        bool exists = false;
        for (auto &crtTrain: allTrains) {
            auto it = std::find_if(crtTrain.getOpriri().begin(), crtTrain.getOpriri().end(),
                                   [&stationName](const Statie &obj) { return obj.getDenumire() == stationName; });
            int nrst = crtTrain.getOpriri().size();
            if (it != crtTrain.getOpriri().end() && stationName != crtTrain.getOpriri()[nrst -
                                                                                        1].getDenumire()) //statia apare intre opririle trenului si nu este ultima
            {
                exists = true;
                std::cout << crtTrain.getCodTren() << " in directia: ";
                auto index = std::distance(crtTrain.getOpriri().begin(), it);
                for (int i = index + 1; i < nrst - 1; i++)
                    std::cout << crtTrain.getOpriri()[i].getDenumire() << " - ";
                std::cout << crtTrain.getOpriri()[nrst - 1].getDenumire() << "\n";
            }
        }
        if (!exists)
            std::cout << "Nu exista plecari!\n";
    }

    const std::vector<Tren> &getAllTrains() const {
        return allTrains;
    }
};

//class Bilet{
//   ----------TO DO--------
//};

void printMenu();

void printNetworkInfos(const std::vector<Magistrala> &retea, Harta &harta);

void printArrivals(Rutele &myRoutes);

void printDepartures(Rutele &myRoutes);

int main() {
    std::cout << "\n\t----BALKANIK TRANSRAIL----";
    std::ifstream fin;
    std::vector<Magistrala> railways; //salvez obiecte de tip magistrala, le citesc din fisierul magistrale.in
    std::vector<Statie> toateStatiile; //toate statiile din harta retelei
    std::vector<std::vector<int> > legaturi(200, std::vector<int>(200, 0)), distante(200, std::vector<int>(200, 0));
    std::string file_line;
    std::vector<Tren> trenuri;
    int numarTotalStatii = 0;

    fin.open("magistrale.in");
    if (!fin) {
        std::cerr << "Error in opening the file" << std::endl;
        return 1;
    }
    while (getline(fin, file_line)) {
        Magistrala magTemp(file_line, numarTotalStatii, toateStatiile);
        railways.push_back(magTemp);
    }
    fin.close();
    //am terminat cu fisierul magistrale.in. Acum, in vectorul 'railways' am magistralele

    Harta railwayMap(railways, legaturi, distante, numarTotalStatii, toateStatiile);
    railwayMap.addLegaturi_Costuri();
    railwayMap.updateLegaturi_calculDistMinima();

    fin.open("trenuri.in");
    if (!fin) {
        std::cerr << "Error in opening the file" << std::endl;
        return 1;
    }
    while (getline(fin, file_line)) {
        std::string oprireTemp;
        Magistrala crtRailway;
        std::vector<Statie> opriri;
        std::vector<std::string> line = split(file_line, ", ");
        int codTemp = std::stoi(line[0]);
        std::string catTemp = line[1];
        int codMagTemp = std::stoi(line[2]);
        auto it = std::find_if(railways.begin(), railways.end(),
                               [codMagTemp](const Magistrala &railway) { return railway.getCodMag() == codMagTemp; });
        if (it != railways.end())
            crtRailway = *it;
        for (int i = 3; i < line.size(); i++) {
            oprireTemp = line[i];
            auto it2 = std::find_if(toateStatiile.begin(), toateStatiile.end(),
                                    [&oprireTemp](const Statie &obj) { return obj.getDenumire() == oprireTemp; });
            if (it2 != toateStatiile.end())
                opriri.push_back(*it2);
        }
        Tren trenTemp(codTemp, catTemp, crtRailway, opriri);
        trenuri.push_back(trenTemp);
    }
    fin.close();
    Rutele myRetea(trenuri);

    while (true) {
        std::string keyboardInput;
        std::string mesaj[4] = {"Exit", "1", "2", "3"};
        printMenu();
        getline(std::cin, keyboardInput);

        if (keyboardInput == mesaj[0])
            break;
        else if (keyboardInput == mesaj[1])
            printNetworkInfos(railways, railwayMap);
        else if (keyboardInput == mesaj[2])
            printArrivals(myRetea);
        else if (keyboardInput == mesaj[3])
            printDepartures(myRetea);
        else {
            std::cout << "Input gresit... mai incearca." << std::endl;
            getline(std::cin, keyboardInput);
        }
    }
    return 0;
}


//Functii auxiliare
std::vector<std::string> split(std::string s, const std::string &delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;
    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(s.substr(pos_start));
    return res;
}

void printMenu() {
    std::cout << "\t\t\t\n\nAlegeti o optiune:\n\n";
    std::cout << "\t1. Informatii despre reteaua feroviara (vezi magistralele + distante intre localitati)\n";
    std::cout << "\t2. Sosirile intr-o statie aleasa\n";
    std::cout << "\t3. Plecarile dintr-o statie aleasa\n";
    std::cout << "\n\n\t [Tasteaza 'Exit' pentru a iesi.]\n";
}

void printNetworkInfos(const std::vector<Magistrala> &retea, Harta &harta) {
    for (const auto &elem: retea)
        operator<<(std::cout, elem);
    std::cout << "\nAlegeti o statie pentru care sa aflati distantele:\n";
    std::string statiaMea;
    std::getline(std::cin, statiaMea);
    harta.distanteStatie(statiaMea);
}

void printArrivals(Rutele &myRoutes) {
    std::cout << "\nAlegeti o statie:\n";
    std::string statiaMea;
    std::getline(std::cin, statiaMea);
    myRoutes.arrivals(statiaMea);
    std::cout
            << "\nDaca doriti sa aflati mai multe informatii despre un tren, introduceti-i codul, sau 'Back' pentru meniul principal.\n";
    std::getline(std::cin, statiaMea);
    if (statiaMea == (std::string) "Back")
        return;
    else {
        int codIntrodus = std::stoi(statiaMea);
        for (auto &tren: myRoutes.getAllTrains())
            if (codIntrodus == tren.getCodTren())
                std::cout << tren;
    }
}

void printDepartures(Rutele &myRoutes) {
    std::cout << "\nAlegeti o statie:\n";
    std::string statiaMea;
    std::getline(std::cin, statiaMea);
    myRoutes.departures(statiaMea);
    std::cout
            << "\nDaca doriti sa aflati mai multe informatii despre un tren, introduceti-i codul, sau 'Back' pentru meniul principal.\n";
    std::getline(std::cin, statiaMea);
    if (statiaMea == (std::string) "Back")
        return;
    else {
        int codIntrodus = std::stoi(statiaMea);
        for (auto &tren: myRoutes.getAllTrains())
            if (codIntrodus == tren.getCodTren())
                std::cout << tren;
    }
}

