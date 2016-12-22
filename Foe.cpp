/*
 * Foe.cpp
 *
 *  Created on: Dec 18, 2016
 *      Author: matthew
 */

#include "Foe.hpp"
#include "Player.hpp"
#include <string>
#include <iostream>

using namespace std;

#define YELLOW   "\033[33m"
#define RESET   "\033[0m"
#define CYAN "\033[36m"

Foe::Foe(string Name, double Health, int Speed,
		double Damage, double Resistance, double Penetration, double Crit)
{
	name = Name;
	maxHealth = Health;
	health = maxHealth;
	speed = Speed;
	strength = Damage;
	resistance = Resistance;
	penetration = Penetration;
	crit_chance = Crit;
}
const string Foe::getName() const
{
	return name;
}
const double Foe::getResistance() const
{
	return resistance;
}
const double Foe::getHealth() const
{
	return health;
}
const double Foe::getMaxHealth() const
{
	return maxHealth;
}
const int Foe::getSpeed() const
{
	return speed;
}

void Foe::takeDamage(double ow)
{
	if(ow <= health)
		health -= ow;
	else
		health = 0;
}

const double Foe::damage(Player enemy)
{
	double RNG = (rand() % 99 + 1) / 100.0; //somewhere between 1 and 100
	double dmg = ((2 * enemy.getLevel() + 10) / 10) * (strength/enemy.getResistance()) * 100
			* 100 / (100 + enemy.getResistance()-penetration)+2;
	double dealt = (RNG < crit_chance) ?  2 * dmg :  dmg; //crits deal double dmg

	cout << CYAN << name << YELLOW << " deals " << dealt << " damage to you!"
			RESET << endl << endl;
	return dealt;
}

Foe::~Foe() {}

