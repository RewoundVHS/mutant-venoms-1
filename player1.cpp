// Neve Laughery
// CSCI330

#include "player1.h"

const string PlayerClass::DEFAULT_NAME = "DEFAULT"; 
const PlayerClass::PlayerType PlayerClass::DEFAULT_TYPE = PlayerClass::HUNTER;

const int PlayerClass::STATS[MAX_STATS][MAX_TYPES] = {
//   Hu  Sc  Fr  Sp  Vi
    {43, 82, 61, 51, 21},    // Max health
    {43, 82, 61, 51, 21},    // Health
    { 8, 10, 10,  9,  8},    // Willpower
    { 9,  6,  9, 16,  3},    // Max speed
    { 9,  6,  9, 16,  3},    // Current speed
    { 9,  6,  9, 16,  3},    // Momentum
    { 7, 10, 11, 12,  6},    // Power
    { 1,  3,  3,  2,  2},    // Physical damage rolls
    { 6,  6,  6,  6,  6},    // Faces on damage roll dice
    { 1,  9,  9,  8, 11},    // Defense value
    {-1, -1, -1, -1, -1},    // Row
    {-1, -1, -1, -1, -1},    // Col
    { 0,  0,  0,  0,  0},    // Active
    { 0,  0,  0,  0,  0}};   // Direction

PlayerClass::PlayerClass(string initName, PlayerType initType) {
    if (initName.size() <= 0)
        name = DEFAULT_NAME;
    else
        name = initName;

    if (initType >= HUNTER && initType < MAX_TYPES)
        type = initType;
    else
        type = DEFAULT_TYPE;

    for (int i=0; i<MAX_STATS; i++) {
        playerStats[i] = STATS[i][type];
    }
}

string PlayerClass::Name() const {
    return name;
}

int PlayerClass::Type() const {
    return type;
}

string PlayerClass::TypeStr() const {
    string typeString;

    switch (type) {
        case HUNTER:
            typeString = "HUNTER";
            break;
        case SCAVENGER:
            typeString  = "SCAVENGER";
            break;
        case FROG:
            typeString = "FROG";
            break;
        case SPIDER:
            typeString = "SPIDER";
            break;
        case VIPER:
            typeString = "VIPER";
            break;
    }
    return typeString;
}

int PlayerClass::Row() const{
    return playerStats[ROW];
}

int PlayerClass::Col() const {
    return playerStats[COL];
}

char PlayerClass::Dir() const {
    return GameSpace::DIR_CHAR[playerStats[DIR]];  
}

int PlayerClass::Health() const {
    return playerStats[HEALTH];
}

int PlayerClass::WillPower() const {
    return playerStats[WILLPOWER];
}

int PlayerClass::MaxSpeed() const {
    return playerStats[MAX_SPEED];
}

int PlayerClass::CurrentSpeed() const {
    return playerStats[CUR_SPEED];
}

int PlayerClass::Momentum() const {
    return playerStats[MOMENTUM];
}

int PlayerClass::Power() const {
    return playerStats[POWER];
}

int PlayerClass::RollPhysDam() const {
    return Dice::Roll(name, GameSpace::DAMAGE, playerStats[DAMAGE_ROLLS], playerStats[ROLL_FACES]);
}

int PlayerClass::Defense() const {
    return playerStats[DEFENSE];
}

bool PlayerClass::Human() const {  
    bool isHuman;
    switch (type) {
        case HUNTER:
            isHuman = true;
            break;
        case SCAVENGER:
            isHuman = true;
            break;
        default:
            isHuman = false;
            break;
    }
    return isHuman;
}

bool PlayerClass::Venom() const {  
    bool isVenom;
    switch (type) {
        case FROG:
            isVenom = true;
            break;
        case SPIDER:
            isVenom = true;
            break;
        case VIPER:
            isVenom = true;
            break;
        default:
            isVenom = false;
            break;
    }
    return isVenom;
}

bool PlayerClass::IsAlive() const {
    return (playerStats[HEALTH] > 0 || playerStats[WILLPOWER] > 0);
}

bool PlayerClass::IsDead() const {
    return (playerStats[HEALTH] <= 0 || playerStats[WILLPOWER] <= 0);
}

bool PlayerClass::IsActive() const { 
    return (playerStats[ACTIVE] == 1); 
}

bool PlayerClass::IsMyEnemy(PlayerClass* p) const {
    bool isEnemy = false;
    if (p != NULL) 
        isEnemy = ((Human()) && !(p->Human()) || (Venom() && (type != p->Type())));

    return (isEnemy);
}

bool PlayerClass::MakeActive() {
    bool set = false;

    if (IsAlive()) {
        playerStats[ACTIVE] = 1;
        set = true;
    }
    return set;
}

bool PlayerClass::MakeInActive() {
    playerStats[ACTIVE] = 0;
    return true;
}

bool PlayerClass::MakeDead() {
    if (IsActive() || IsDead()) {
        playerStats[HEALTH] = 0;
        playerStats[WILLPOWER] = 0;
        playerStats[CUR_SPEED] = 0;
        playerStats[MOMENTUM] = 0;
        playerStats[ACTIVE] = 0;
    }
    return IsDead();
}

int PlayerClass::Wounded(int damage) {
    if (IsActive() && damage >= 0) 
        playerStats[HEALTH] -= damage;

    if (IsDead())
        MakeDead();

    return playerStats[HEALTH];
}

int PlayerClass::Invigorate(int vigor) {
    if ((IsActive()) && (vigor >= 0)) {
        playerStats[HEALTH] += vigor;
        if (playerStats[HEALTH] > playerStats[MAX_HEALTH])
            playerStats[HEALTH] = playerStats[MAX_HEALTH];
    }
    return playerStats[HEALTH];
}

bool PlayerClass::Heal() {
    int healAmount = 10;
    bool healed = false;

    if (IsActive() && IsAlive()) {
        playerStats[CUR_SPEED] = playerStats[MAX_SPEED];
        playerStats[HEALTH] += healAmount;
        if (playerStats[HEALTH] > playerStats[MAX_HEALTH])
            playerStats[HEALTH] = playerStats[MAX_HEALTH];
        healed = true;
    }
    return healed;
}

int PlayerClass::WillDamage(int willDmg) {
    if (IsActive() && (willDmg >= 0))
        playerStats[WILLPOWER] -= willDmg;

    if (playerStats[WILLPOWER] <= 0)
        MakeDead();
}

int PlayerClass::AddWill(int addWill) {
    if (IsActive() && (addWill >= 0))
        playerStats[WILLPOWER] += addWill;

    return playerStats[WILLPOWER];
}

int PlayerClass::SlowDown(int hazard) {
    if (IsActive() && hazard >= 0) {
        playerStats[CUR_SPEED] -= hazard;
        if (playerStats[CUR_SPEED] < 0)
            playerStats[CUR_SPEED] = 0;
        if (playerStats[MOMENTUM] > playerStats[CUR_SPEED])
            ResetMomentum();
    }
    return playerStats[CUR_SPEED];
}

int PlayerClass::ResetCurrentSpeed() {
    if (IsActive()) 
        playerStats[CUR_SPEED] = playerStats[MAX_SPEED];
    
    return playerStats[CUR_SPEED];
}

int PlayerClass::ResetMomentum() {
    if (IsActive()) 
        playerStats[MOMENTUM] = playerStats[CUR_SPEED];

    return playerStats[MOMENTUM];
}

int PlayerClass::UseMomentum(int num) {
    if (IsActive() && num >= 0) {
        playerStats[MOMENTUM] -= num;
        if (playerStats[MOMENTUM] < 0) 
            playerStats[MOMENTUM] = 0; 
    }
    return playerStats[MOMENTUM];
}

bool PlayerClass::SetCell(int newRow, int newCol) {
    bool set = false;

    if ((newRow >= 0 && newCol >= 0) || (newRow == -1 && newCol == -1)) {
        playerStats[ROW] = newRow;
        playerStats[COL] = newCol;
        set = true;
    }
    return set;
}

bool PlayerClass::SetDir(DirType newDir) {
    bool set = false;
    if (newDir >= GameSpace::NORTH && newDir <= GameSpace::WEST) {
        playerStats[DIR] = newDir;
        set = true;
    }
    return set;
}

void PlayerClass::Write(ostream& out) const {
    out << TypeStr() << '#';
    if (IsActive())
        out << '1';
    else
        out << '0';
    out << '#' << Name() << '#' << '(' << Row() << ',' << Col() << ',' << Dir()
    << ")#" << Health() << '#' << WillPower() << '#' << Power() << '#' 
    << CurrentSpeed() << '#' << Momentum() << '#' << playerStats[DAMAGE_ROLLS] 
    << 'D' << playerStats[ROLL_FACES] << '#' << Defense() << '#' << endl;
}

ostream& operator<<(ostream& out, const PlayerClass *p) {
    if (p != NULL)
        p->Write(out);
    return out;
}
