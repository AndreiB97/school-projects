package main;

public abstract class Hero {

    // map position
    protected int x;
    protected int y;

    // character attributes
    protected int maxHP;
    protected int HP;
    protected int XP;
    protected int LVL;
    protected char heroClass;

    // fight info
    protected float damageA1Taken;
    protected float damageA2Taken;
    protected float enemyA1RaceModifier;
    protected float enemyA2RaceModifier;

    // debuffs
    protected int overTimeDamage;
    protected int overTimeDamageRoundsLeft;
    protected boolean isStunned;

     public Hero(int _HP, char _heroClass, int _x, int _y) {
         // character attributes
         maxHP = _HP;
         HP = _HP;
         XP = 0;
         LVL = 0;
         heroClass = _heroClass;

         // map position
         x = _x;
         y = _y;

         // fight info
         damageA2Taken = 0f;
         damageA1Taken = 0f;
         enemyA2RaceModifier = 1f;
         enemyA1RaceModifier = 1f;

         // debuffs
         overTimeDamage = 0;
         overTimeDamageRoundsLeft = 0;
         isStunned = false;
    }

    public void move(char direction) {
        // move the character in a direction
        switch (direction) {
            case 'U':
                x--;
                break;
            case 'D':
                x++;
                break;
            case 'L':
                y--;
                break;
            case 'R':
                y++;
                break;
        }
    }

    public void addXP(int enemyLVL) {
         // add XP based on LVL difference then check for LVL up
        int newXP = 200 - (LVL - enemyLVL)*40;
        if (newXP > 0) {
            XP += newXP;
            checkLvlUp();
        }
    }

    public void applyOverTimeDamage() {
        // apply over time damage and decrement stun rounds
        if(overTimeDamageRoundsLeft > 0) {
            HP -= overTimeDamage;
            overTimeDamageRoundsLeft--;
            if (overTimeDamageRoundsLeft == 0) {
                isStunned = false;
            }
        }
    }

    protected void attack(Hero enemy) {
        // use abilities
        ability1(enemy);
        ability2(enemy);
    }

    protected void takeDamage() {
        // calculates damage taken
        HP -= Math.round(damageA1Taken * enemyA1RaceModifier);
        HP -= Math.round(damageA2Taken * enemyA2RaceModifier);
    }

    protected abstract void checkLvlUp();

    protected abstract void ability1(Hero enemy);

    protected abstract void ability2(Hero enemy);

    protected abstract float raceModifierHandler(int num, Hero enemy);

}
