#include <iostream>
#include <random>
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <iomanip>
#include <exception>

using namespace std;

//easy medium hard

bool attack(int& fighters)
{
    int temp;
    int ShipsLost;
    random_device rd;
    uniform_real_distribution<double> dist(0, 100);
    temp = dist(rd);
    if (temp > 30)
    {
        temp = dist(rd);
        if (temp * fighters / (fighters+ 1) < 30)
            return false;
        else
        {
            ShipsLost = fighters * temp/100;
            cout << "Attack won, ships lost: " << ShipsLost <<"\n";
            fighters -= ShipsLost;
            return true;
        }
    }
    else
        return true;
}
template <typename T>
class fleet {
protected:
    int cost, count;
    T attribute;

public:
    fleet(int x, T attr)
    {
        cost = x;
        count = 0;
        attribute = attr;
    }
    virtual void construction(int x) = 0;
    virtual T getInfo() = 0;

    int getCost() {
        return cost;
    }
    int getCount() {
        return count;
    }
    virtual ~fleet() {}
};

template <typename T>
class fighters : public fleet<T> {
public:
    fighters() : fleet<T>(500, 1000) {}
    void construction(int x) {
        this->count += x;
        cout << "The fleet has " << x << " more fighter";
        if (x != 1)
            cout << "s!\n";
        else
            cout << "!\n";
    }
    T getInfo() {
        cout << "Nothing yet...";
        return this->attribute;
    }
    friend class planet;
    friend class PlanetNode;
};

template <typename T>
class transporters : public fleet<T> {
public:
    transporters() : fleet<T>(300, 10000) {}
    void construction(int x) {
        this->count += x;
        cout << "The fleet has " << x << " more cargo ship";
        if (x != 1)
            cout << "s!\n";
        else
            cout << "!\n";
    }
    T getInfo() {
        return this->attribute;
    }
    friend class planet;
    friend class PlanetNode;
};

template <typename T>
class civil : public fleet<T> {
public:
    civil() : fleet<T>(200, 100) {}
    void construction(int x) {
        this->count += x;
        cout << "The fleet has " << x << " more civil ship";
        if (x != 1)
            cout << "s!\n";
        else
            cout << "!\n";
    }
    T getInfo() {
        return this->attribute;
    }
    friend class planet;
    friend class PlanetNode;
};

class notEnoughResources : public exception {
public:
    const char* what() const throw() {
        return "Not enough resources!\n";
    }
};

class planet {
private:
    double distance;
    int resources, population;
    int DefaultResources = 10000, DefaultPopulation = 1000;
    fighters<int> s_fight;
    transporters<int> s_transport;
    civil<int> s_civil;
public:
    planet()
    {
        distance = 0;
        resources = DefaultResources;
        population = DefaultPopulation;
    }

    planet(double RandomDistance, int Cargos, int Civils, int Fighters)
    {
        distance = RandomDistance;
        resources = s_transport.getInfo() * Cargos;
        population = s_civil.getInfo() * Civils;
        s_transport.count = Cargos;
        s_civil.count = Civils;
        s_fight.count = Fighters;
    }
    ~planet(){}
    int getResources() {
        return resources;
    }
    void AsteroidMining(int Fighters, int Cargos);
    void NewCargo(int x);
    void NewFighter(int x);
    void NewCivil(int x);
    
    friend ostream& operator << (ostream& COUT, planet& x);
    friend class PlanetNode;
    friend class home;
    friend class shipyard;
    friend void populationIncrease();
};
class Disasters {
public:
    int Strenght;
    int Duration;
    int Kills;
    Disasters() {
        Strenght = 1;
        Duration = 0;
        Kills = 0;
    }
    Disasters(int x, int y, int z) {
        Strenght = x;
        Duration = y;
        Kills = z;
    }
    ~Disasters()
    {
    }
};
class PlanetNode {
public:
    static int PlanetCounter;
    planet x;
    PlanetNode* next;
    Disasters disaster;
public:
    PlanetNode()
    {
        PlanetCounter++;
        next = NULL;
    }
    PlanetNode(const PlanetNode& otherPlanet)
    {
        x = otherPlanet.x;
        disaster = otherPlanet.disaster;
        disaster.Duration = 0;
        PlanetCounter++;
        next = NULL;
    }
    ~PlanetNode()
    {
        PlanetCounter--;
    }
    static int getPlanetCounter() {
        return PlanetCounter;
    }
    void Colony(int x, int y, int z);
    void addPlanet(planet x);
    void Transport(int cargos, int destination);
    void Disaster();
    static void printPlanets();
    void populationIncrease();
    void colonyCheck();

    friend class planet;
};

int PlanetNode::PlanetCounter = 0;
PlanetNode* earth = new PlanetNode();

void PlanetNode::Transport(int cargos, int y)
{
    PlanetNode* n = earth;
    earth->x.resources -= cargos * earth->x.s_transport.getInfo();
    for (int i = 1; i <= y; i++)
    {
        n = n->next;
    }
    n->x.resources += cargos * n->x.s_transport.getInfo();
    cout << "Transport DONE!\n";
}
void PlanetNode::addPlanet(planet y) {
    PlanetNode* newPlanet = new PlanetNode();
    newPlanet->x = y;
    newPlanet->next = NULL;
    PlanetNode* n = this;
    while (n->next != NULL)
    {
        n = n->next;
    }
    n->next = newPlanet;
}
void PlanetNode::Colony(int x, int y, int z)
{

    if (this->x.s_transport.count >= y && this->x.s_civil.count >= z && this->x.s_fight.count >= x)
    {
        if (this->x.resources >= y * this->x.s_transport.getInfo())
        {
            if (this->x.population >= z * this->x.s_civil.getInfo())
            {
                this->x.resources -= y * this->x.s_transport.getInfo();
                this->x.population -= z * this->x.s_civil.getInfo();
                this->x.s_transport.count -= y;
                this->x.s_civil.count -= z;
                this->x.s_fight.count -= x;
                if (attack(x) == true)
                {
                    double Distance;
                    random_device rd;
                    uniform_real_distribution<double> dist(0.1, 5.0);
                    Distance = dist(rd);
                    planet newPlanet(Distance, z, y, x);
                    addPlanet(newPlanet);
                    cout << "Colonization successful!\n\n";
                }
                else
                    cout << "Colonization failed, fleet lost!\n\n";
            }
            else
                cout << "Not enough people!\n\n";
        }
        else
            cout << "Not enough resources!\n\n";
    }
    else
    {
        cout << "Not enough ships!\n\n";
    }

}
void PlanetNode::Disaster()
{
    random_device rd;
    uniform_int_distribution<int> dist(1, 100);
    int chance = dist(rd);
    uniform_int_distribution<int> dist3(1, 60);
    int kills = dist3(rd);
    if (chance > 96)
    {
        random_device rd;
        uniform_int_distribution<int> dist2(10, 30);
        int duration = dist2(rd);
        int strenght = dist2(rd);
        this->disaster.Duration = duration;
        this->disaster.Kills = kills;
        this->disaster.Strenght = strenght/2;
    }
}

static void printPlanets()
{
    PlanetNode* n = earth;
    while (n != NULL)
    {
        if (n->disaster.Duration != 0)
            cout << "\033[1;31m" << n->x << "\033[0m";
        else
        {
            n->Disaster();
            cout << n->x;
        }
        n = n->next;
        for (int i = 1; i < 20; i++)
        {
            cout << char(196);
        }
        cout<< "\n";
    }
}
void populationIncrease()
{
    PlanetNode* n = earth;
    random_device rd;
    uniform_real_distribution<double> dist(1.015, 1.025);
    double temp = dist(rd);
    while (n != NULL)
    {
        n->x.population *= temp;
        n->x.resources -= n->disaster.Strenght * n->x.population / 100;
        if (n->disaster.Kills != 0)
        {
            n->x.population -= n->x.population * n->disaster.Kills / 100;
            n->disaster.Kills = 0;
        }
        if (n->disaster.Duration != 0)
            n->disaster.Duration--;
        else
            n->disaster.Strenght = 1;
        n = n->next;
    }
}
void colonyCheck()
{
    PlanetNode* n = earth;
    while (n->next != NULL)
    {
        if ((n->next)->x.getResources() < 0)
        {
            PlanetNode* temp = (n->next)->next;
            delete n->next;
            n->next = temp;
        }
        else
            n = n->next;
    }
}

void planet::AsteroidMining(int x, int y)
{
    if (s_transport.count >= y && s_fight.count >= x)
    {
        int x_copy = x;
        if (attack(x) == true)
        {
            double temp;
            int temp_2;
            temp_2 = x_copy - x;
            s_fight.count -= temp_2;
            random_device rd;
            uniform_real_distribution<double> dist(0.0, 1.0);
            temp = dist(rd);
            temp_2= y * s_transport.getInfo() * temp;
            resources += temp_2;
            cout << "Expedition successful: +"<<temp_2<<" resources\n";
            cout << "\n";
        }
        else
        {
            s_fight.count -= x;
            s_transport.count -= y;
            cout << "Attack lost. The fleet has been lost\n";
            cout << "\n";
        }
    }
    else
        cout << "Not enough ships!\n\n";
}

void planet::NewCargo(int x)
{
    try {
        if (resources < x * s_transport.cost)
            throw notEnoughResources();
        resources -= x * s_transport.cost;
        s_transport.construction(x);
    }
    catch (notEnoughResources& x) {
        cout << x.what();
    }
}
void planet::NewCivil(int x)
{
    try {
        if (resources < x * s_civil.cost)
            throw notEnoughResources();
        resources -= x * s_civil.cost;
        s_civil.construction(x);
    }
    catch (notEnoughResources& x) {
        cout << "Not enough resources!\n";
    }
}
void planet::NewFighter(int x)
{
    try {
        if (resources < x * s_fight.cost)
            throw notEnoughResources();
        resources -= x * s_fight.cost;
        s_fight.construction(x);
    }
    catch (notEnoughResources& x) {
        cout << "Not enough resources!\n";
    }
}

ostream& operator<< (ostream& COUT, planet& x)
{
    if (x.distance == 0)
        COUT << "HOME\n";
    else
    {
        COUT << "COLONY: "<< setprecision(3) << x.distance <<" Light-Years\n";
    }
    COUT << "Population: " << x.population <<"\n";
    COUT << "Resources: " << x.resources << "\n";
    COUT << "Fighters: " << x.s_fight.getCount() << "\n";
    COUT << "Cargos: " << x.s_transport.getCount() << "\n";
    COUT << "Civil: " << x.s_civil.getCount() << "\n";
    return COUT;
}

class menu {
public:
    virtual void page() = 0;
};

class home : public menu {
public:
    static home* getInstance();

    void page()
    {
        cout << R"(   ___ _                  _
  / _ \ | __ _ _ __   ___| |_ ___
 / /_)/ |/ _` | '_ \ / _ \ __/ __|
/ ___/| | (_| | | | |  __/ |_\__ \
\/    |_|\__,_|_| |_|\___|\__|___/)" << "\n\n";
        cout << "No. of planets: " << PlanetNode::getPlanetCounter() << "\n\n";
        cout << "Cargo_Bay = " << earth->x.s_transport.getInfo() << " | Civil_Ship_Seats = " << earth->x.s_civil.getInfo() << "\n\n";
        populationIncrease();
        printPlanets();
        if (PlanetNode::getPlanetCounter() > 1) {
            colonyCheck();
        }
        cout << "\nInput_Structure: [abcd]\n\n" << "a = the desired action: 1 - shipyard | 2 - transport | 3 - colony | 4 - asteroid mining\n";
        cout << "b = the planet affected ( the planet of destination for the transport, none for the colony ), indexed from 0\n";
        cout << "b, c, d = the number of ships sent, cargos for transport, cargos and fighters for asteroid mining, all for colonies\n\n";
        cout << "Your input here: ";
    }

private:
    static home* instance;
    home() = default;
    home(const home&) = delete;
    home& operator=(const home&) = delete;
};

home* home::instance = nullptr;

home* home::getInstance() {
    if (instance == nullptr) {
        instance = new home();
    }
    return instance;
}

class shipyard : public menu {
private:
    int PlanetIndex;
    PlanetNode* Planet;
public:
    shipyard(int x, PlanetNode* y) {
        PlanetIndex = x;
        Planet = y;
    }
    void page() {
        int x, ok = 1, z;
        while(ok==1)
        {
            system("CLS");
            cout << R"( __ _     _                           _ 
/ _\ |__ (_)_ __  _   _  __ _ _ __ __| |
\ \| '_ \| | '_ \| | | |/ _` | '__/ _` |
_\ \ | | | | |_) | |_| | (_| | | | (_| |
\__/_| |_|_| .__/ \__, |\__,_|_|  \__,_|
           |_|    |___/                 

)";
            cout << "Planet: " << PlanetIndex << "\n";
            cout << "Resources: " << Planet->x.resources << "\n\n";
            cout << "Cargo_Cost = " << earth->x.s_transport.getCost() << " | Civil_Cost = " << earth->x.s_civil.getCost() << " | Fighter_Cost = " << earth->x.s_fight.getCost() << "\n\n";
            cout << "Fighters: " << Planet->x.s_fight.getCount() << "\n";
            cout << "Cargos: " << Planet->x.s_transport.getCount() << "\n";
            cout << "Civil: " << Planet->x.s_civil.getCount() << "\n\n";
            cout << "Ship type: "; cin >> x;
            switch (x) {
            case 0:
                ok = 0;
                break;
            case 1:
                cout << "Number: "; cin >> z;
                Planet->x.NewFighter(z);
                Sleep(1500);
                break;
            case 2:
                cin >> z;
                Planet->x.NewCargo(z);
                Sleep(1500);
                break;
            case 3:
                cin >> z;
                Planet->x.NewCivil(z);
                Sleep(1500);
            }
        }
    }
};
class game_over : public menu {
public:
    void page() {
        PlanetNode::PlanetCounter = 0;
        cout << "\033[1;31m" << R"(   ___                         ___                 
  / _ \__ _ _ __ ___   ___    /___\__   _____ _ __ 
 / /_\/ _` | '_ ` _ \ / _ \  //  //\ \ / / _ \ '__|
/ /_\\ (_| | | | | | |  __/ / \_//  \ V /  __/ |   
\____/\__,_|_| |_| |_|\___| \___/    \_/ \___|_|   )" << "\033[0m\n";
        cout << "\nTry Again? [y/n]\n";
    }
    bool gameOver() {
        char c;
        cin >> c;
        system("CLS");
        if (c == 'y')
        {
            PlanetNode* x = new(PlanetNode);
            earth = x;
            return true;
        }
        return false;
    }
    game_over(){}
    ~game_over(){}
};

void display()
{
    home* myHome = home::getInstance();
    bool ok = 1;
    char input[5];
    while (ok == 1)
    {
        if (earth->x.getResources() < 0)
        {
            system("CLS");
            game_over GameOverScreen;
            GameOverScreen.page();
            if (GameOverScreen.gameOver() != true)
                break;
        }
        PlanetNode* n = earth;
        input[0] = '0';
        myHome->page();
        Sleep(500);
        if (_kbhit()!=0)
        {
            cin >> input;
            cout << "\n";
        }
        switch (input[0])
        {
        case '1':
            if (int(input[1]) - 48 < PlanetNode::PlanetCounter)
            {
                for (int i = 1; i <= int(input[1]) - 48; i++)
                {
                    n = n->next;
                }
                shipyard ShipyardScreen(int(input[1]) - 48, n);
                ShipyardScreen.page();
            }
            else
                cout << "Wrong input!";
            Sleep(1500);
            break;
        case '2':
            if ((int(input[1]) - 48) < PlanetNode::PlanetCounter){
                earth->Transport(int(input[2]) - 48, (int(input[1]) - 48));
            }
            else
                cout << "Wrong input!";
            Sleep(1500);
            break;
        case '3':
            earth->Colony((int(input[1]) - 48), int(input[2]) - 48, int(input[3]) - 48);
            Sleep(1500);
            break;
        case '4':
            if (int(input[1]) - 48 < PlanetNode::PlanetCounter)
            {
                for (int i = 1; i <= int(input[1]) - 48; i++)
                {
                    n = n->next;
                }
                n->x.AsteroidMining(int(input[2]) - 48, int(input[3]) - 48);
            }
            else
                cout << "Wrong input!";
            Sleep(1500);
        }
        system("CLS");
    }
}

int main()
{
    display();
}