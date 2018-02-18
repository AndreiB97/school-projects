package main;

public final class Pyromancer extends Hero {
    ////////////////////////
    // methods

    public Pyromancer(int _x, int _y) {
        // just init stuff
        super(500, 'P',  _x, _y);
    }

    protected final void checkLvlUp() {
        // check if character can LVL up and then check again just for the lulz
        if (XP >= 250 + LVL*50 && HP > 0) {
            LVL++;
            maxHP += 50;
            HP = maxHP;
            checkLvlUp();
        }
    }

    protected final void ability1(Hero enemy) {
        // calculate base damage
        enemy.damageA1Taken = 350 + 50*LVL;

        // get race modifier from handler
        enemy.enemyA1RaceModifier = raceModifierHandler(1, enemy);

        // check for land modifier
        if (Main.map[x][y] == 'V') {
            enemy.damageA1Taken *= 1.25f;
        }
    }

    protected final void ability2(Hero enemy) {
        // calculate base damage
        enemy.damageA2Taken = 150 + 20*LVL;
        float overTimeDamage = 50 + 30*LVL;

        // get race modifier from handler
        enemy.enemyA2RaceModifier = raceModifierHandler(2, enemy);
        overTimeDamage *= enemy.enemyA2RaceModifier;

        // check for land modifier
        if (Main.map[x][y] == 'V') {
            enemy.damageA2Taken *= 1.25;
            overTimeDamage *= 1.25f;
        }

        // apply damage over time and rounds
        enemy.overTimeDamage = Math.round(overTimeDamage);
        enemy.overTimeDamageRoundsLeft = 2;
    }

    protected final float raceModifierHandler(int num, Hero enemy) {
        // check enemy class
        if (enemy.heroClass == 'R') {
            return 0.8f;
        } else if (enemy.heroClass == 'K') {
            return 1.2f;
        } else if (enemy.heroClass == 'P') {
            return 0.9f;
        } else if (enemy.heroClass == 'W') {
            return 1.05f;
        }
        return 1f;
    }

    // end methods
    ////////////////////////
}
