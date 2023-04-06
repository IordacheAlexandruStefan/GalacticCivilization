#include <iostream>
#include <random>
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <iomanip>

using namespace std;

bool attack(int& fighters)
{
    double temp;
    int ShipsLost;
    random_device rd;
    uniform_real_distribution<double> dist(0.0, 2.5);
    temp = dist(rd);
    if (temp > 1)
    {
        temp = dist(rd);
        if (temp > 1)
            return false;
        else
        {
            ShipsLost = fighters * temp;
            cout << "Attack won, ships lost: " << ShipsLost <<"\n";
            fighters -= ShipsLost;
            return true;
        }
    }
    else
        return true;
}

class fleet {
private:
    int fighter, cargo, civil;
    int CostFighter=500, CostCargo=200, CostCivil=300;
public:
    

    fleet()
    {
        fighter = 0;
        cargo = 0;
        civil = 0;
    }
    friend fleet operator+ (fleet x);
    friend ostream& operator<< (ostream& COUT,fleet& x);

    friend class PlanetNode;
    friend class planet;
    friend void display();
};
class planet {
private:
    double distance;
    long long int resources, population;
    int CargoBay = 10000 , Seats=100 , DefaultResources=10000, DefaultPopulation=1000;
    fleet ships;
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
        resources = CargoBay * Cargos;
        population = Seats * Civils;
        ships.cargo = Cargos;
        ships.civil = Civils;
        ships.fighter = Fighters;
    }
    ~planet()
    {
    }
    friend ostream& operator << (ostream& COUT, planet& x);

    void AsteroidMining(int Fighters, int Cargos);
    void NewCargo(int x);
    void NewFighter(int x);
    void NewCivil(int x);
    
    friend class fleet;
    friend class PlanetNode;
    friend void display();
    friend void printPlanets(PlanetNode* n);
    friend void populationIncrease(PlanetNode* n);
    friend void colonyCheck(PlanetNode* n);
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
    void Colony(int x, int y, int z);
    void addPlanet(planet x);
    void Transport(int cargos, int destination);
    void Disaster();
    friend void display();
    friend void printPlanets(PlanetNode* n);
    friend void populationIncrease(PlanetNode* n);
    friend void colonyCheck(PlanetNode* n);
};

int PlanetNode::PlanetCounter = 1;

PlanetNode* earth = new PlanetNode();

void PlanetNode::Transport(int cargos, int y)
{
    PlanetNode* n = earth;
    earth->x.resources -= cargos * earth->x.CargoBay;
    for (int i = 1; i <= y; i++)
    {
        n = n->next;
    }
    n->x.resources+= cargos * n->x.CargoBay;
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
    PlanetCounter++;
}
void PlanetNode::Colony(int x, int y, int z)
{

    if (this->x.ships.cargo >= y && this->x.ships.civil >= z && this->x.ships.fighter >= x)
    {
        if (this->x.resources >= y * this->x.CargoBay)
        {
            if (this->x.population >= z * this->x.Seats)
            {
                this->x.resources -= y * this->x.CargoBay;
                this->x.population -= z * this->x.Seats;
                this->x.ships.cargo -= y;
                this->x.ships.civil -= z;
                this->x.ships.fighter -= x;
                if (attack(x) == true)
                {
                    double Distance;
                    random_device rd;
                    uniform_real_distribution<double> dist(0.1, 5.0);
                    Distance = dist(rd);
                    planet newPlanet(Distance, x, y, z);
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

void printPlanets(PlanetNode*n)
{
    while (n != NULL)
    {
        if (n->disaster.Duration != 0)
            cout << "\033[1;31m" << n->x <<char(31)<< "\n" << n->x.ships << "\033[0m";
        else
        {
            n->Disaster();
            cout << n->x << "\n" << n->x.ships;
        }
        n = n->next;
        for (int i = 1; i < 20; i++)
        {
            cout << char(196);
        }
        cout<< "\n";
    }
}
void populationIncrease(PlanetNode* n)
{
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
void colonyCheck(PlanetNode* n)
{
    while (n->next != NULL)
    {
        if ((n->next)->x.resources < 0)
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
    if (ships.cargo >= y && ships.fighter >= x)
    {
        int x_copy = x;
        if (attack(x) == true)
        {
            double temp;
            int temp_2;
            temp_2 = x_copy - x;
            ships.fighter -= temp_2;
            random_device rd;
            uniform_real_distribution<double> dist(0.0, 1.0);
            temp = dist(rd);
            temp_2= y * CargoBay * temp;
            resources += temp_2;
            cout << "Expedition successful: +"<<temp_2<<" resources\n";
            cout << "\n";
        }
        else
        {
            ships.fighter -= x;
            ships.cargo -= y;
            cout << "Attack lost. The fleet has been lost\n";
            cout << "\n";
        }
    }
    else
        cout << "Not enough ships!\n\n";
}
void planet::NewCargo(int x)
{
    if (resources >= x * ships.CostCargo)
    {
        ships.cargo += x;
        resources -= x * ships.CostCargo;
        cout << "The fleet has " << x << " more cargo ship";
        if (x != 1)
            cout << "s!\n";
        else
            cout << "!\n";
    }
    else
    {
        cout << "Not enough resources!\n";
    }
}
void planet::NewCivil(int x)
{
    if (resources >= x * ships.CostCivil)
    {
        ships.civil += x;
        resources -= x * ships.CostCivil;
        cout << "The fleet has " << x << " more civil ship";
        if (x != 1)
            cout << "s!\n";
        else
            cout << "!\n";
    }
    else
    {
        cout << "Not enough resources!\n";
    }
}
void planet::NewFighter(int x)
{
    if (resources >= x * ships.CostFighter)
    {
        ships.fighter += x;
        resources -= x * ships.CostFighter;
        cout << "The fleet has " << x << " more fighter";
        if (x != 1)
            cout << "s!\n";
        else
            cout << "!\n";
    }
    else
    {
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
    COUT <<"Population: " << x.population <<"\n";
    COUT << "Resources: " << x.resources;
    //COUT << x.ships << "\n";
    return COUT;
}
ostream& operator<< (ostream& COUT, fleet& x)
{
    COUT << "Cargo ships: " << x.cargo << endl;
    COUT << "Civil ships: " << x.civil << endl;
    COUT << "Fighters: " << x.fighter << endl;
    return COUT;
}

void display()
{
    bool ok = 1;
    char input[5];
    while (ok == 1)
    {
        cout << "Cargo_Bay = " << earth->x.CargoBay << " | Civil_Ship_Seats = " << earth->x.Seats<<"\n";
        cout << "Cargo_Cost = " << earth->x.ships.CostCargo << " | Civil_Cost = " << earth->x.ships.CostCivil << " | Fighter_Cost = " << earth->x.ships.CostFighter  << "\n\n";
        if (earth->x.resources < 0)
        {
            system("CLS");
            char c;
            cout << "\033[1;31mTry Again?\033[0m [y/n]\n";
            cin >> c;
            if (c == 'y')
            {
                PlanetNode* x = new(PlanetNode);
                earth = x;
            }
            else
                break;
        }
        populationIncrease(earth);
        colonyCheck(earth);
        printPlanets(earth);
        PlanetNode* n = earth;
        cout << "\nInput_Structure: [abcde]\n\n" << "a = the desired action: [1-3] - new ships | 4 - transport | 5 - asteroid mining | 6 - colony\n";
        cout << "b = the planet affected ( the planet of destination for the transport, none for the colony ), indexed from 0\n";
        cout << "c, d, e = the number of ships sent, cargos for transport, cargos and fighters for asteroid mining, all for colonies\n\n";
        cout << "Your input here: ";
        input[0] = '0';
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
                n->x.NewCargo(int(input[2]) - 48);
            }
            else
                cout << "Wrong input!";
            Sleep(1500);
            break;
        case '2':
            if (int(input[1]) - 48 < PlanetNode::PlanetCounter)
            {
                for (int i = 1; i <= int(input[1]) - 48; i++)
                {
                    n = n->next;
                }
                n->x.NewCivil(int(input[2]) - 48);
            }
            else
                cout << "Wrong input!";
            Sleep(1500);
            break;
        case '3':
            if (int(input[1]) - 48 < PlanetNode::PlanetCounter)
            {
                for (int i = 1; i <= int(input[1]) - 48; i++)
                {
                    n = n->next;
                }
                n->x.NewFighter(int(input[2]) - 48);
            }
            else
                cout << "Wrong input!";
            Sleep(1500);
            break;
        case '4':
            if (int(input[2]) - 48 < PlanetNode::PlanetCounter)
                earth->Transport(int(input[2]) - 48, (int(input[1]) - 48));
            else
                cout << "Wrong input!";
            Sleep(1500);
            break;
        case '6':
            earth->Colony((int(input[1]) - 48), int(input[2]) - 48, int(input[3]) - 48);
            Sleep(1500);
            break;
        case '5':
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
            break;
        }
        system("CLS");
    }
}

int main()
{
    display();
}