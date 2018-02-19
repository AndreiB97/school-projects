package main;

import fileio.FileSystem;
import java.io.IOException;

public class Main {

    // map vars
    private static int n;
    private static int m;
    static char[][] map;

    // char vars
    private static int p;
    private static Hero[] characters;

    // i/o vars
    private static FileSystem fs;

    public static void main(String args[]) {
        try {
            fs = new FileSystem(args[0], args[1]);
        }
        catch (IOException e) {
            System.out.println("ERROR: Cannot open input/output file");
        }

        readMap();
        readCharacters();
        readRounds();

        printResults();

        try {
            fs.close();
        }
        catch (IOException e) {
            System.out.println("ERROR: Cannot close input/output file");
        }
    }

    private static void readMap() {
        // read and set the map
        try {
            n = fs.nextInt();
            m = fs.nextInt();

            map = new char[n][m];

            String line;

            for (int i = 0; i < n; i++) {
                line = fs.nextWord();
                for (int j = 0; j < m; j++) {
                    map[i][j] = line.charAt(j);
                }
            }
        }
        catch (IOException e) {
            System.out.println("ERROR: Cannot read map");
        }
    }

    private static void readCharacters() {
        // read and set the characters
        try {
            String heroClass;
            int x, y;

            p = fs.nextInt();

            characters = new Hero[p];

            for (int i = 0; i < p; i++) {
                heroClass = fs.nextWord();
                x = Integer.parseInt(fs.nextWord());
                y = Integer.parseInt(fs.nextWord());

                switch(heroClass) {
                    case "P":
                        characters[i] = new Pyromancer(x, y);
                        break;
                    case "K":
                        characters[i] = new Knight(x, y);
                        break;
                    case "W":
                        characters[i] = new Wizard(x, y);
                        break;
                    case "R":
                        characters[i] = new Rogue(x, y);
                        break;
                }
            }
        }
        catch (IOException e) {
            System.out.println("ERROR: Cannot read characters");
        }
    }

    private static void readRounds() {
        // move the characters and then check for encounters
        try {
            int r = fs.nextInt();
            String moves;

            for (int i = 0; i < r; i++) {
                moves = fs.nextWord();

                for (int j = 0; j < p; j++) {
                    if (characters[j].HP > 0 && !characters[j].isStunned) {
                        characters[j].move(moves.charAt(j));
                    }
                    characters[j].applyOverTimeDamage();
                }
                checkEncounters();
            }
        }
        catch (IOException e) {
            System.out.println("ERROR: Cannot read rounds");
        }
    }

    private static void checkEncounters() {
        // check if 2 heroes are on the same spot on the map
        for (int i = 0; i < p; i++) {
            for (int j = i+1; j < p; j++) {
                if (characters[i].x == characters[j].x && characters[i].y == characters[j].y &&
                        characters[i].HP > 0 && characters[j].HP > 0) {
                    fight(characters[i], characters[j]);
                    break;
                }
            }
        }
    }

    private static void fight(Hero hero1, Hero hero2) {
        // attack then apply the damage and give XP to the winner
        hero1.attack(hero2);
        hero2.attack(hero1);

        hero1.takeDamage();
        hero2.takeDamage();

        if (hero1.HP <= 0) {
            hero2.addXP(hero1.LVL);
        } else if (hero2.HP <= 0) {
            hero1.addXP(hero2.LVL);
        }
    }

    private static void printResults() {
        try {
            for (int i = 0; i < p; i++) {
                if (characters[i].HP > 0) {
                    fs.writeWord(characters[i].heroClass + " " +
                            characters[i].LVL + " " + characters[i].XP + " " +
                            characters[i].HP + " " + characters[i].x + " " +
                            characters[i].y);
                    fs.writeNewLine();
                } else {
                    fs.writeWord(characters[i].heroClass + " dead");
                    fs.writeNewLine();
                }
            }
        }
        catch (IOException e) {
            System.out.println("ERROR: Cannot write in file");
        }
    }

}
