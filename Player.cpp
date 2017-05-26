/*
 * Player.cpp
 *
 *  Created on: Dec 19, 2016
 *      Author: matthew
 */

#include "Player.hpp"
#include "Foe.hpp"
#include <string>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <iostream>

#define YELLOW   "\033[33m"
#define RESET   "\033[0m"
#define CYAN "\033[36m"

using namespace std;

Player::Player()
{
	cout << "Mysterious Voice: Welcome Hero. What may I call you?" << endl;
	getline(cin, name);

	cout << "\nMysterious Voice: "<< name << "... A fair name, I suppose. "
			<< "Be you Mage, Warrior, or Archer?" << endl;

	bool ans = false;
	while(!ans)
	{
		getline(cin, classification);
		for (auto & c: classification)
			c = toupper(c);

		switch(classification[0]){
		case 'A' : {
			ans = true;
			classification = "Archer";
			break;
		}
		case 'W' : {
			ans = true;
			classification = "Warrior";
			break;
		}
		case 'M' : {
			ans = true;
			classification = "Mage";
			break;
		}

		}
		if(!ans)
		{
			cout << "\nMysterious Voice:"
					"I've never heard of that profession around here,"
					"perhaps you know of it by another name?" << endl;
		}
	}

	score = 0;
	maxHealth = 1000;
	currHealth = maxHealth;
	level= 1;
	speed = 1;
	brawn = 10;
	resistance = 30;
	penetration = 1;
	crit_chance = 0.01;
}

std::string Player::getName()
{
	return name;
}

std::string Player::getClass()
{
	return classification;
}

int Player::getScore()
{
	return score;
}

void Player::setScore(int newScore)//to be used when the player kills or is killed
{
	score = newScore;
}

double Player::health() //so monsters can see how much health u have
{
	return currHealth;
}

double Player::getMaxHealth()
{
	return maxHealth;
}

int Player::getResistance() //for monsters calculating how much dmg
{
	return resistance;
}

int Player::getLevel()
{
	return level;
}

int Player::getSpeed()
{
	return speed;
}

void Player::levelUp()//level up and prompt the user for which state they want to increase
{
	if(level >= MAX_LEVEL)
	{
		cout << "\n## You've leveled all the way up! While you can't increase your stats,"
			<< " your health will be refreshed before the next battle. ##" << endl
		currHealth = maxHealth;
		return;
	}
	cout << "\n########## LEVEL  UP ##########\n" << endl;
	level++;
	maxHealth += 100;
	currHealth = maxHealth;
	stats(); //display the player's stats.

	//Base leveling of stats by class
	if (classification == "Archer")
	{
		cout << "Critical Hit Chance:\n     " << crit_chance << " --->";
		crit_chance += 0.1;
		cout << crit_chance << endl;
		cout << "Penetration:\n     " << penetration << " –––> ";
		penetration *= 1.1;
		cout << penetration << endl;
	}
	else if (classification == "Mage")
	{
		cout << "Brawn:\n     " << brawn << " --->";
		brawn *= 1.1;
		cout << brawn << endl;
		cout << "Speed:\n     " << speed << " –––> ";
		speed += 1;
		cout << speed << endl;
	}
	else
	{
		cout << "Health:\n     " << maxHealth << " --->";
		maxHealth *= 1.05;
		cout << maxHealth << endl;

		cout << "Resistance:\n     " << resistance << " –––> ";
		resistance *= 1.05;
		cout << resistance << endl;

		cout << "Brawn:\n     " << brawn << " --->";
		brawn *= 1.1;
		cout << brawn << endl;
	}

	sleep(1);

	bool ans = false;
	while(!ans)
	{
		cout << "Which stat would you like to level up?\n\t";
		string statistic;
		getline(cin, statistic);
		cout << endl;

		for (auto & c: statistic)
			c = toupper(c);

		switch(statistic[0])
		{
		case 'H' : {
			cout << "Health:\n     " << maxHealth << " –––> ";
			maxHealth += 200;
			currHealth = maxHealth;
			cout << maxHealth << endl;
			ans = true;
			break;
		}
		case 'S' : {
			cout << "Speed:\n     " << speed << " –––> ";
			speed++;
			cout << speed << endl;
			ans = true;
			break;
		}
		case 'B' : {
			cout << "Brawn:\n     " << brawn << " –––> ";
			brawn+=5;
			cout << brawn << endl;
			ans = true;
			break;
		}
		case 'R' : {
			cout << "Resistance:\n     " << resistance << " –––> ";
			resistance+=5;
			cout << resistance << endl;
			ans = true;
			break;
		}
		case 'P' : {
			cout << "Penetration:\n     " << penetration << " –––> ";
			penetration+=5;
			cout << penetration << endl;
			ans = true;
			break;
		}
		case 'C' : {
			cout << "Critical Hit Chance: " << crit_chance << " –––> ";
			crit_chance += 0.11;
			cout << crit_chance << endl;
			ans = true;
			break;
		}
		}
		if(!ans)
			cout << "What? I didn't understand that." << endl;
	}
}

/* Prints all of the player's stats */
void Player::stats()
{
	cout << name << "'s Stats:" << endl;
	cout << "     Level " << level << " " << classification << endl;
	cout << "     Health:      " << currHealth << " / " << maxHealth << endl;
	cout << "     Speed:       " << speed << endl;
	cout << "     Brawn:       " << brawn << endl;
	cout << "     Resistance:  " << resistance << endl;
	cout << "     Penetration: " << penetration << endl;
	cout << "     Crit Chance: " << crit_chance << endl;
}

/*--------- Battle Methods --------*/

void Player::takeDamage(double ow) //calculates how much damage the enemy does
{
	cout << "You've been hit for " << ow << " damage!" << endl;
	if(currHealth - ow > 0)
	{
		currHealth -= ow;
		cout << "You have " << currHealth << "/" << maxHealth << " left."
				<< endl << endl;
	}
	else{
		currHealth = 0;
	}

}

const double Player::damage(Foe enemy)
{
	double RNG = (rand() % 99 + 1)/100.0; //somewhere between 1 and 100
	double dmg = ((2 * level + 10) / 10) * (brawn / enemy.getResistance())*100
			*100 / (100 + enemy.getResistance() - penetration) + 2;
	double dealt = (RNG < crit_chance) ?  2 * dmg :  dmg; //crits deal double dmg

	cout << YELLOW << "You deal " << dealt << " damage to " << CYAN <<
			enemy.getName() << YELLOW << "!"<< RESET << endl << endl;
	return dealt;
}

Player::~Player(){};

