package main;

public final class Wizard extends Hero {

    public Wizard(int _x, int _y) {
        super(400, 'W',  _x, _y);
    }

    protected final void checkLvlUp() {
        // check if character can LVL up
        if (XP >= 250 + LVL*50 && HP > 0) {
            LVL++;
            maxHP += 30;
            HP = maxHP;
            checkLvlUp();
        }
    }

    public final void ability1(Hero enemy) {
        // use ability 1
        float percent = 0.2f + 0.05f*LVL;

        if (enemy.maxHP * 0.3f < enemy.HP) {
            enemy.damageA1Taken = percent * enemy.maxHP * 0.3f;
        } else {
            enemy.damageA1Taken = percent * enemy.HP;
        }

        enemy.enemyA1RaceModifier = raceModifierHandler(1, enemy);

        if (Main.map[x][y] == 'D') {
            enemy.damageA1Taken *= 1.1f;
        }
    }

    public final void ability2(Hero enemy) {
        // use ability 2
        if (enemy.heroClass != 'W') {
            float damagePercent = 0.35f + 0.02f * LVL;

            Wizard tmp = new Wizard(-1, -1);

            enemy.attack(tmp);

            enemy.damageA2Taken = damagePercent * (Math.round(tmp.damageA1Taken) + Math.round(tmp.damageA2Taken));

            if (Main.map[x][y] == 'D') {
                enemy.damageA2Taken *= 1.1f;
            }

            enemy.enemyA2RaceModifier = raceModifierHandler(2, enemy);
        } else {
            enemy.damageA2Taken += 0;
        }
    }

    protected final float raceModifierHandler(int num, Hero enemy) {
        // returns race modifier for enemy
        if (num == 1) {
            if (enemy.heroClass == 'R') {
                return 0.8f;
            } else if (enemy.heroClass == 'K') {
                return 1.2f;
            } else if (enemy.heroClass == 'P') {
                return 0.9f;
            } else if (enemy.heroClass == 'W') {
                return 1.05f;
            }
        } else {
            if (enemy.heroClass == 'R') {
                return 1.2f;
            } else if (enemy.heroClass == 'K') {
                return 1.4f;
            } else if (enemy.heroClass == 'P') {
                return 1.3f;
            }
        }
        return 1f;
    }

}
