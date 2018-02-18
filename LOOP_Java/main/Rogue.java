package main;

public final class Rogue extends Hero {
    ////////////////////////
    // variables

    private int backstabCount;
    private int wizardCount;

    // end variables
    ////////////////////////



    ////////////////////////
    // methods

    public Rogue(int _x, int _y) {
        // just init stuff
        super(600, 'R',  _x, _y);
        backstabCount = 0;
        wizardCount = 0;
    }

    protected final void checkLvlUp() {
        // check if character can LVL up and then check again just for the lulz
        if (XP >= 250 + LVL*50 && HP > 0) {
            LVL++;
            maxHP += 40;
            HP = maxHP;
            checkLvlUp();
        }
    }

    public final void ability1(Hero enemy) {
        // calculate base damage
        enemy.damageA1Taken = 200+20*LVL;

        // check for backstab multiplier
        if (backstabCount%3 == 0) {
            backstabCount = 0;
            if (Main.map[x][y] == 'W') {
                enemy.damageA1Taken *= 1.5f;
            }
        }

        // bug fix for Wizard deflect
        if (enemy.heroClass != 'W') {
            backstabCount++;
        } else {
            wizardCount++;
            if (wizardCount == 2) {
                wizardCount = 0;
                backstabCount++;
            }
        }

        // get the race modifier from handler
        enemy.enemyA1RaceModifier = raceModifierHandler(1, enemy);

        // check for land multiplier
        if (Main.map[x][y] == 'W') {
            enemy.damageA1Taken *= 1.15f;
        }
    }

    public final void ability2(Hero enemy) {
        // calculate base damage
        float damageOverTime = 40+10*LVL;

        // check for land multiplier and set rounds for over time
        if (Main.map[x][y] == 'W') {
            enemy.overTimeDamageRoundsLeft = 6;
            damageOverTime *= 1.15f;
        } else {
            enemy.overTimeDamageRoundsLeft = 3;
        }

        // get race modifier from handler and set the fight damage
        enemy.enemyA2RaceModifier = raceModifierHandler(2, enemy);
        enemy.damageA2Taken = damageOverTime;

        // apply race modifier for over time damage
        damageOverTime *= enemy.enemyA2RaceModifier;

        // set over time damage and stun
        enemy.overTimeDamage = Math.round(damageOverTime);
        enemy.isStunned = true;
    }

    protected final float raceModifierHandler(int num, Hero enemy) {
        // check ability number and enemy class
        if (num == 1) {
            if (enemy.heroClass == 'R') {
                return 1.2f;
            } else if (enemy.heroClass == 'K') {
                return 0.9f;
            } else if (enemy.heroClass == 'P') {
                return 1.25f;
            } else if (enemy.heroClass == 'W') {
                return 1.25f;
            }
        } else {
            if (enemy.heroClass == 'R') {
                return 0.9f;
            } else if (enemy.heroClass == 'K') {
                return 0.8f;
            } else if (enemy.heroClass == 'P') {
                return 1.2f;
            } else if (enemy.heroClass == 'W') {
                return 1.25f;
            }
        }
        return 1f;
    }

    // end methods
    ////////////////////////

}
