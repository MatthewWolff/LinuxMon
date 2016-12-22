/*
 * main.cpp
 *
 *  Created on: Dec 20, 2016
 *      Author: matthew
 */

#include "Player.hpp"
#include "Foe.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
#define CLEAR "\033[2J"  // clears the terminal screen
#define CYAN "\033[36m"

void battle(Player &p, Foe &e);
void printDeathMessage();
void importMonsters();

void my_handler(int s)
{
	printDeathMessage();
	exit(0);
}

vector<string> MONSTERS;

void battle(Player &p, Foe &e)
{
	//use this to decide who goes first
	cout << BOLDWHITE << "You've encountered a foe!" << endl;
	cout << "You engage " << CYAN << e.getName() << BOLDWHITE
			<< " in mortal combat." << endl << endl << RESET;
	sleep(1);
	bool p_goes_first = p.getSpeed() > e.getSpeed();

	while(p.health() > 0 && e.getHealth() > 0)
	{
		sleep(1);
		if (p_goes_first)
		{
			cout << "Your turn!" << endl;
			e.takeDamage(p.damage(e));
			cout << RED << "Enemy Remaining Health: " << e.getHealth()
																							<< RESET<<  endl;
		}
		else
		{
			p.takeDamage(e.damage(p));
			cout << BOLDWHITE << "Remaining Health: " << p.health()
																							<< endl << RESET << endl;
		}

		//First attack was done - now we need to check if either party died
		if (p.health() <= 0 || e.getHealth() <= 0)
		{
			break; //break from loop if ded
		}

		//now it's round 2: electric boogaloo
		if (p_goes_first)
		{
			p.takeDamage(e.damage(p));
			cout << BOLDWHITE << "Remaining Health: " << p.health()
																									<< endl << RESET << endl;
		}
		else
		{
			e.takeDamage(p.damage(e));
			cout << "Your turn!" << endl;
			cout << RED << "Enemy Remaining Health: " << e.getHealth()
																										<< RESET<<  endl;
		}
		sleep(1);
	}

	//battle is over
	cout << "-----BATTLE ENDED-----" << endl;
	//now figure out who died and such
	if (p.health() <= 0)
	{

		printDeathMessage();
		cout << "You've died!!! You certainly tried your best, " << p.getName()
																						<< ", but it appears as though you've been bested." << endl;
		cout << endl << "\t Your Score: " << p.getScore() << endl << endl;
		exit(0);//exiting program because you're ded
	}

	//you are not dead
	cout << "Congratulations! You've won the battle!" << endl;
	p.levelUp();
	p.setScore(p.getScore() + e.getMaxHealth()/10);

}

//I'm making this its own function because of ASCII art from the internet
//Credit to http://http://ascii.co.uk/art/tombstone
void printDeathMessage()
{
	cout << endl;
	cout << BOLDWHITE << "Oh dear, you are dead!" << RESET << endl;
	cout << RED << 
			"    .o oOOOOOOOo                                            OOOo" << endl <<
			"    Ob.OOOOOOOo  OOOo.      oOOo.                      .adOOOOOOO" << endl <<
			"    OboO\"\"\"\"\"\"\"\"\"\"\"\".OOo. .oOOOOOo.    OOOo.oOOOOOo..\"\"\"\"\"\"\"\"\"'OO" << endl <<
			"    OOP.oOOOOOOOOOOO \"POOOOOOOOOOOo.   `\"OOOOOOOOOP,OOOOOOOOOOOB'" << endl <<
			"    `O'OOOO'     `OOOOo\"OOOOOOOOOOO` .adOOOOOOOOO\"oOOO'    `OOOOo" << endl <<
			"    .OOOO'            `OOOOOOOOOOOOOOOOOOOOOOOOOO'            `OO" << endl <<
			"    OOOOO                 '\"OOOOOOOOOOOOOOOO\"`                oOO" << endl <<
			"   oOOOOOba.                .adOOOOOOOOOOba               .adOOOOo." << endl <<
			"  oOOOOOOOOOOOOOba.    .adOOOOOOOOOO@^OOOOOOOba.     .adOOOOOOOOOOOO" << endl <<
			" OOOOOOOOOOOOOOOOO.OOOOOOOOOOOOOO\"`  '\"OOOOOOOOOOOOO.OOOOOOOOOOOOOO" << endl <<
			" \"OOOO\"       \"YOoOOOOMOIONODOO\"`  .   '\"OOROAOPOEOOOoOY\"     \"OOO\"" << endl <<
			"    Y           'OOOOOOOOOOOOOO: .oOOo. :OOOOOOOOOOO?'         :`" << endl <<
			"    :            .oO%OOOOOOOOOOo.OOOOOO.oOOOOOOOOOOOO?         ." << endl <<
			"    .            oOOP\"%OOOOOOOOoOOOOOOO?oOOOOO?OOOO\"OOo" << endl <<
			"                 '%o  OOOO\"%OOOO%\"%OOOOO\"OOOOOO\"OOO':" << endl <<
			"                      `$\"  `OOOO' `O\"Y ' `OOOO'  o             ." << endl <<
			"    .                  .     OP\"          : o     ." << endl <<
			"                              :" << endl <<
			"                              ." << endl << RESET;

	cout << BOLDWHITE << "############################ GAME OVER ############################" << endl;
	cout << RESET;
	cout << "\a" << endl;
}
void importMonsters()
{
	string temp;
	ifstream myfile ("monsters.txt");
	if (myfile.is_open())
	{
		while (getline(myfile,temp))
		{
			MONSTERS.push_back(temp);
		}
		myfile.close();
	}
	else
		cerr << "Unable to open file";
}

int main()
{
	Player You = Player();

	//to be able to exit at any time and kill the character
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = my_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

	importMonsters();

	int numDefeated = 0;
	while(You.health() > 0)
	{
		int nextFoe = rand() % MONSTERS.size();
		Foe enemy = Foe(
				MONSTERS[nextFoe],
				150 + 100*numDefeated,
				0.5 + 0.25*numDefeated,
				10 + 5*numDefeated,
				20 + 5*numDefeated,
				numDefeated*2.5,
				0.05*numDefeated);
		numDefeated++; //monsters get harder. So do you.
		battle(You, enemy);
	}
	printDeathMessage();
}

