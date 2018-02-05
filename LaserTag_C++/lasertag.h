#ifndef LASERTAG_H_INCLUDED
#define LASERTAG_H_INCLUDED

#include "iostream"
#include "list"
#include "string"

using namespace std;

// converteste un string in int
int stringToInt(string s) {
    int num = 0;
    for (unsigned int i = 0; i < s.length(); i++) {
        num = num+s[i]-'0';
        num *= 10;
    }
    num /= 10;
    return num;
}

// functie folosita la sortarea stringurilor in mod alfabetic
bool sortString(string s1, string s2) {
    if (s1.compare(s2) < 0) {
        return true;
    }
    return false;
}

// structura de date pentru stocarea informatiilor senzorilor
class sensorInfo {
    int nrSensors;
    int **sensorDistance;

 public:
    sensorInfo() {
        sensorDistance = NULL;
    }

    void readSensors() {
        cin >> nrSensors;
        sensorDistance = new int*[nrSensors];
        for (int i = 0; i < nrSensors; i++) {
            sensorDistance[i] = new int[nrSensors];
            for (int j = 0; j < i; j++) {
                cin >> sensorDistance[i][j];
            }
            sensorDistance[i][i] = 0;
            for (int j = i+1; j < nrSensors; j++) {
                cin >> sensorDistance[i][j];
            }
        }
    }

    int getSensorDistance(int s1, int s2) {
        return sensorDistance[s1][s2];
    }

    ~sensorInfo() {
        if (sensorDistance) {
            for (int i = 0; i < nrSensors; i++) {
                delete[] sensorDistance[i];
            }
            delete[] sensorDistance;
        }
    }
} sensors;

// structura de date pentru stocarea informatiilor jucatorilor
class playerInfo {
    string name;
    int lives;
    list<string> peopleShot;
    list<int> sensorsTriggered;
    list<string> peopleShotTotal;
    int shootingScore;
    int explorationScore;

 public:
    playerInfo() {
        lives = 2;
        shootingScore = 0;
        explorationScore = 0;
    }

    void setPeopleShotTotal(list<string> l) {
        peopleShotTotal = l;
    }

    void setName(string name) {
        this->name = name;
    }

    string getName() {
        return name;
    }

    void resetPlayerData() {
        lives = 2;
        addExplorationScore();
        if (!peopleShot.empty()) {
            peopleShotTotal.splice(peopleShotTotal.end(), peopleShot);
        }
        peopleShot.clear();
        sensorsTriggered.clear();
    }

    bool isAlive() {
        return (bool)lives;
    }

    void gotShot() {
        lives--;
    }

    void addPeopleShot(string name) {
        peopleShot.push_back(name);
    }

    list<string> getPeopleShot() {
        return peopleShot;
    }

    list<string> getPeopleShotTotal() {
        return peopleShotTotal;
    }

    void addSensor(int sensor) {
        sensorsTriggered.push_back(sensor);
    }

    list<int> getSensorsTriggered() {
        return sensorsTriggered;
    }

    void addShootingScore(int s) {
        shootingScore += s;
    }

    int getShootingScore() {
        return shootingScore;
    }

    void addExplorationScore() {
        list<int>::iterator s1 = sensorsTriggered.begin(), s2;
        s2 = s1;
        s2++;
        while (s2 != sensorsTriggered.end()) {
            explorationScore += sensors.getSensorDistance(*s1, *s2);
            s1++;
            s2++;
        }
        sensorsTriggered.sort();
        sensorsTriggered.unique();
        explorationScore += 3*sensorsTriggered.size();
    }

    int getExplorationScore() {
        return explorationScore;
    }
};

// functie folosita la sortarea jucatorilor dupa nume
bool sortName(playerInfo p1, playerInfo p2) {
    if (p1.getName().compare(p2.getName()) < 0) {
        return true;
    }
    return false;
}

// functie folosita la sortarea jucatorilor dupa shooting score
bool compareExplorationScore(playerInfo p1, playerInfo p2) {
    if (p1.getExplorationScore() > p2.getExplorationScore()) {
        return true;
    }
    if (p1.getExplorationScore() < p2.getExplorationScore()) {
        return false;
    }
    if (p1.getName().compare(p2.getName()) < 0) {
        return true;
    }
    return false;
}

// functie folosita la sortarea jucatorilor dupa exploration score
bool compareShootingScore(playerInfo p1, playerInfo p2) {
    if (p1.getShootingScore() > p2.getShootingScore()) {
        return true;
    }
    if (p1.getShootingScore() < p2.getShootingScore()) {
        return false;
    }
    if (p1.getName().compare(p2.getName()) < 0) {
        return true;
    }
    return false;
}

// structura folosita pentru stocarea informatiilor echipelor
class teamInfo {
    int playersNr;
    int roundsWon;
    list<playerInfo> *players;

 public:
    teamInfo() {
        roundsWon = 0;
        players = NULL;
    }

    void readTeamSize() {
        cin >> playersNr;
    }

    void readPlayers() {
        string name;
        players = new list<playerInfo>[playersNr];

        for (int i = 0; i < playersNr; i++) {
            playerInfo player;
            cin >> name;
            player.setName(name);
            unsigned long H = hashPlayer(name);
            players[H%playersNr].push_back(player);
        }
    }

    void checkWin() {
        bool won = false;
        list<playerInfo>::iterator j;
        for (int i = 0; i < playersNr && !won; i++) {
            if (!players[i].empty()) {
                for (j = players[i].begin();
                     j != players[i].end() && !won; j++) {
                    if (j->isAlive()) {
                        roundsWon++;
                        won = true;
                    }
                }
            }
        }
    }

    int getRoundsWon() {
        return roundsWon;
    }

    void resetTeam() {
        list<playerInfo>::iterator j;
        for (int i = 0; i < playersNr; i++) {
            for (j = players[i].begin(); j != players[i].end(); j++) {
                j->resetPlayerData();
            }
        }
    }

    // functie de hash care foloseste algoritmul djb2 de Dan Bernstein
    unsigned long hashPlayer(string key) {
        unsigned long H = 5381;
        for (unsigned int i = 0; i < key.length(); i++) {
            H = H*33+i;
        }
        return H;
    }

    ~teamInfo() {
        if (players) {
            delete[] players;
        }
    }

    bool playerInTeam(string name) {
        bool found = false;
        int i = hashPlayer(name)%playersNr;
        list<playerInfo>::iterator it;
        for (it = players[i].begin(); it != players[i].end(); it++) {
            if (it->getName() == name) {
                found = true;
                break;
            }
        }
        return found;
    }

    void playerTriggeredSensor(string name, int s) {
        int i = hashPlayer(name)%playersNr;
        list<playerInfo>::iterator it;
        for (it = players[i].begin(); it != players[i].end(); it++) {
            if (it->getName() == name) {
                it->addSensor(s);
                break;
            }
        }
    }

    void fireExchange(string name1, string name2, char iff) {
        int H = hashPlayer(name1)%playersNr;
        list<playerInfo>::iterator i;
        for (i = players[H].begin(); i != players[H].end(); i++) {
            if (i->getName() == name1) {
                if (iff == 'f') {
                    i->addShootingScore(-5);
                } else {
                    i->addShootingScore(2);
                }
                i->addPeopleShot(name2);
                break;
            }
        }
    }

    void playerShot(string name) {
        int H = hashPlayer(name)%playersNr;
        list<playerInfo>::iterator i;
        for (i = players[H].begin(); i != players[H].end(); i++) {
            if (i->getName() == name) {
                i->gotShot();
                break;
            }
        }
    }

    list<playerInfo> getTeamInfo() {
        list<playerInfo> info;
        for (int i = 0; i < playersNr; i++) {
            if (!players[i].empty()) {
                list<playerInfo>::iterator j;
                for (j = players[i].begin(); j != players[i].end(); j++) {
                    info.push_back(*j);
                }
            }
        }
        return info;
    }
};

// structura folosita pentru stocarea de fire exchange
struct fireExchangeInfo {
    string players;
    int exchangeNr;
    fireExchangeInfo(string players, int exchangeNr) {
        this->players = players;
        this->exchangeNr = exchangeNr;
    }
};

// functie folosita la sortarea jucatorilor dupa fire exchange
bool sortFireExchange(fireExchangeInfo f1, fireExchangeInfo f2) {
    if (f1.exchangeNr > f2.exchangeNr) {
        return true;
    }
    if (f1.exchangeNr < f2.exchangeNr) {
        return false;
    }
    if (f1.players.compare(f2.players) < 0) {
        return true;
    }
    return false;
}


void laserTag() {
    // citire informatii senzori
    sensors.readSensors();

    // citire informatii echipe
    teamInfo teams[2];
    for (int i = 0; i < 2; i++) {
        teams[i].readTeamSize();
    }
    for (int i = 0; i < 2; i++) {
        teams[i].readPlayers();
    }

    // pornirea campionatului
    string op;
    cin >> op;
    while (true) {
        if (op == "END_CHAMPIONSHIP") {
            // reseteaza datele echipelor si incheie campionatul
            for (int i = 0; i < 2; i++) {
                teams[i].resetTeam();
            }
            break;
        }
        if (op.substr(0, 4) == "JOC_") {
            for (int i = 0; i < 2; i++) {
                // verifica cine a castigat runda trecuta si
                // reseteaza datele jucatorilor
                if (op != "JOC_1") {
                    teams[i].checkWin();
                }
                teams[i].resetTeam();
            }
        } else if (op[0] >= '0' && op[0] <= '9') {
            // verifica activarea unui senzor
            int s = stringToInt(op.substr(0, op.length()-1));
            string name;
            cin >> name;
            if (teams[0].playerInTeam(name)) {
                teams[0].playerTriggeredSensor(name, s);
            } else {
                teams[1].playerTriggeredSensor(name, s);
            }
        } else {
            // verifica un fire exchange
            string name1, name2;
            name1 = op;
            cin >> name2 >> name2;
            if (teams[0].playerInTeam(name1)) {
                if (teams[0].playerInTeam(name2)) {
                    teams[0].fireExchange(name1, name2, 'f');
                    teams[0].playerShot(name2);
                } else {
                    teams[0].fireExchange(name1, name2, 'e');
                    teams[1].playerShot(name2);
                }
            } else {
                if (teams[0].playerInTeam(name2)) {
                    teams[1].fireExchange(name1, name2, 'e');
                    teams[0].playerShot(name2);
                } else {
                    teams[1].fireExchange(name1, name2, 'f');
                    teams[1].playerShot(name2);
                }
            }
        }
        cin >> op;
    }

    // calcularea statisticilor
    list<playerInfo> team, team2;
    team = teams[0].getTeamInfo();
    team2 = teams[1].getTeamInfo();
    team.splice(team.end(), team2);
    list<playerInfo> bestShooter = team;

    // sortarea in functie de shooting score
    bestShooter.sort(compareShootingScore);

    // afisarea in functie de scor
    list<playerInfo>::iterator i = bestShooter.begin();
    cout << "I. Top shooters" << endl;
    int score, j;
    for (j = 1; j <= 5 && i != bestShooter.end(); j++) {
        score = i->getShootingScore();
        cout << j << ". " << i->getName() << " " << score << 'p' << endl;
        i++;
    }
    while (i != bestShooter.end() && i->getShootingScore() == score) {
        cout << j << ". " << i->getName() << " " << score << 'p' <<endl;
        j++;
        i++;
    }
    cout << endl;

    cout << "II. Top explorers" << endl;
    list<playerInfo> topExplorer = team;

    // sortarea dupa exploration score
    topExplorer.sort(compareExplorationScore);

    // afisarea in functie de scor
    i = topExplorer.begin();
    for (j = 1; j <= 5 && i != topExplorer.end(); j++) {
        score = i->getExplorationScore();
        cout << j << ". " << i->getName() << " " << score << 'p' << endl;
        i++;
    }
    while (i != topExplorer.end() && i->getExplorationScore() == score) {
        cout << j << ". " << i->getName() << " " << score << 'p' << endl;
        j++;
        i++;
    }
    cout << endl;

    cout << "III. Top fire exchange" << endl;
    list<string> targets, targets2;
    list<string>::iterator t, t2;
    list<fireExchangeInfo> exchange;
    list<playerInfo>::iterator iter;
    string name2;
    int x;

    // sortarea alfabetica
    team.sort(sortName);

    // generarea fire exchangeurilor
    for (i = team.begin(); i != team.end(); i++) {
        string name1 = i->getName();
        iter = i;
        iter++;
        targets = i->getPeopleShotTotal();
        targets.sort(sortString);
        while (iter != team.end()) {
            x = 0;
            string name2 = iter->getName();
            while (!targets.empty()) {
                if (targets.front() == name2) {
                    x++;
                    targets.pop_front();
                } else {
                    break;
                }
            }
            targets2 = iter->getPeopleShotTotal();
            targets2.sort(sortString);
            while (!targets2.empty()) {
                if (targets2.front() == name1) {
                    x++;
                    targets2.pop_front();
                } else {
                    break;
                }
            }
            iter->setPeopleShotTotal(targets2);
            exchange.push_back(fireExchangeInfo(name1 + " - " + name2, x));
            iter++;
        }
    }

    // sortarea dupa numarul de fire exchange
    exchange.sort(sortFireExchange);

    // afisarea in functie de numarul de fire exchange
    list<fireExchangeInfo>::iterator iterator = exchange.begin();
    for (j = 1; j <= 5 && iterator != exchange.end(); j++) {
        score = iterator->exchangeNr;
        cout << j << ". " << iterator->players << " " << score << endl;
        iterator++;
    }
    while (iterator != exchange.end() && iterator->exchangeNr == score) {
        cout << j << ". " << iterator->players << " " << score << endl;
        j++;
        iterator++;
    }
    cout << endl;

    cout << "IV. Final score" << endl;
    team = teams[0].getTeamInfo();
    team2 = teams[1].getTeamInfo();
    float winChance1 = 0, winChance2 = 0;
    i = team.begin();

    // calculare winning changge
    while (i != team.end()) {
        if (i->isAlive()) {
            iter = bestShooter.begin();
            while (iter != bestShooter.end()) {
                if (iter->getName() == i->getName()) {
                    break;
                }
                iter++;
            }
            if (bestShooter.front().getShootingScore() != 0) {
                winChance1 += (float)iter->getShootingScore()
                               /bestShooter.front().getShootingScore();
            } else {
                winChance1 += (float)iter->getShootingScore();
            }
            iter = topExplorer.begin();
            while (iter != topExplorer.end()) {
                if (iter->getName() == i->getName()) {
                    break;
                }
                iter++;
            }
            if (topExplorer.front().getExplorationScore()) {
                winChance1 += (float)iter->getExplorationScore()
                               /topExplorer.front().getExplorationScore();
            } else {
                winChance1 += (float)iter->getExplorationScore();
            }
            i++;
        }
    }
    i = team2.begin();
    while (i != team2.end()) {
        if (i->isAlive()) {
            iter = bestShooter.begin();
            while (iter != bestShooter.end()) {
                if (iter->getName() == i->getName()) {
                    break;
                }
                iter++;
            }
            if (bestShooter.front().getShootingScore() != 0) {
                winChance2 += (float)iter->getShootingScore()
                               /bestShooter.front().getShootingScore();
            } else {
                winChance2 += (float)iter->getShootingScore();
            }
            iter = topExplorer.begin();
            while (iter != topExplorer.end()) {
                if (iter->getName() == i->getName()) {
                    break;
                }
                iter++;
            }
            if (topExplorer.front().getExplorationScore() != 0) {
                winChance2 += (float)iter->getExplorationScore()
                               /topExplorer.front().getExplorationScore();
            } else {
                winChance2 += (float)iter->getExplorationScore();
            }
            i++;
        }
    }

    // afisare scor final
    if (winChance1 > winChance2) {
        cout << teams[0].getRoundsWon()+1 << " - "
             << teams[1].getRoundsWon() << endl;
    } else {
        cout << teams[0].getRoundsWon() << " - "
             << teams[1].getRoundsWon()+1 << endl;
    }
}

#endif  // LASERTAG_H_INCLUDED
