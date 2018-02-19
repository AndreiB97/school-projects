package main;

public final class Knight extends Hero {

    public Knight(int _x, int _y) {
        super(900, 'K',  _x, _y);
    }

    protected final void checkLvlUp() {
        // check if character can LVL up
        if (XP >= 250 + LVL*50 && HP > 0) {
            LVL++;
            maxHP += 80;
            HP = maxHP;
            checkLvlUp();
        }
    }

    public final void ability1(Hero enemy) {
        // use ability 1
        float HPLimit = 0.2f + 0.01f*LVL;

        if (HPLimit > 0.4f) {
            HPLimit = 0.4f;
        }

        if (enemy.HP/100 <= HPLimit) {
            enemy.damageA1Taken = enemy.HP;
            enemy.enemyA1RaceModifier = 1f;
        } else {
            enemy.damageA1Taken = 200 + 30*LVL;

            if (Main.map[x][y] == 'L') {
                enemy.damageA1Taken *= 1.15f;
            }

            enemy.enemyA1RaceModifier = raceModifierHandler(1, enemy);
        }
    }

    public final void ability2(Hero enemy) {
		// use ability 2
        enemy.damageA2Taken = 100 + 40*LVL;

        enemy.enemyA2RaceModifier = raceModifierHandler(2, enemy);

        if (Main.map[x][y] == 'L') {
            enemy.damageA2Taken *= 1.15f;
        }

        enemy.overTimeDamageRoundsLeft = 1;
        enemy.overTimeDamage = 0;
        enemy.isStunned = true;
    }

    protected final float raceModifierHandler(int num , Hero enemy) {
		// get race modifier for enemy
        if (num == 1) {
            if (enemy.heroClass == 'R') {
                return 1.15f;
            } else if (enemy.heroClass == 'K') {
                return 1f;
            } else if (enemy.heroClass == 'P') {
                return 1.1f;
            } else if (enemy.heroClass == 'W') {
                return 0.8f;
            }
        } else if (num == 2) {
            if (enemy.heroClass == 'R') {
                return 0.8f;
            } else if (enemy.heroClass == 'K') {
                return 1.2f;
            } else if (enemy.heroClass == 'P') {
                return 0.9f;
            } else if (enemy.heroClass == 'W') {
                return 1.05f;
            }
        }
        return 1f;
    }

}
