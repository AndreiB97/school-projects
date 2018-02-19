package main;

public final class Rogue extends Hero {
    
    private int backstabCount;
    private int wizardCount;

    public Rogue(int _x, int _y) {
        super(600, 'R',  _x, _y);
        backstabCount = 0;
        wizardCount = 0;
    }

    protected final void checkLvlUp() {
        // check if character can LVL up
        if (XP >= 250 + LVL*50 && HP > 0) {
            LVL++;
            maxHP += 40;
            HP = maxHP;
            checkLvlUp();
        }
    }

    public final void ability1(Hero enemy) {
        // use ability 1
        enemy.damageA1Taken = 200+20*LVL;

        if (backstabCount%3 == 0) {
            backstabCount = 0;
            if (Main.map[x][y] == 'W') {
                enemy.damageA1Taken *= 1.5f;
            }
        }

        if (enemy.heroClass != 'W') {
            backstabCount++;
        } else {
            wizardCount++;
            if (wizardCount == 2) {
                wizardCount = 0;
                backstabCount++;
            }
        }

        enemy.enemyA1RaceModifier = raceModifierHandler(1, enemy);

        if (Main.map[x][y] == 'W') {
            enemy.damageA1Taken *= 1.15f;
        }
    }

    public final void ability2(Hero enemy) {
        // use ability 2
        float damageOverTime = 40+10*LVL;

        if (Main.map[x][y] == 'W') {
            enemy.overTimeDamageRoundsLeft = 6;
            damageOverTime *= 1.15f;
        } else {
            enemy.overTimeDamageRoundsLeft = 3;
        }

        enemy.enemyA2RaceModifier = raceModifierHandler(2, enemy);
        enemy.damageA2Taken = damageOverTime;

        damageOverTime *= enemy.enemyA2RaceModifier;

        enemy.overTimeDamage = Math.round(damageOverTime);
        enemy.isStunned = true;
    }

    protected final float raceModifierHandler(int num, Hero enemy) {
        // get race modifier for enemy
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

}
