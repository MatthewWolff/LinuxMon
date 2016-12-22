/*
 * Foe.h
 *
 *  Created on: Dec 18, 2016
 *      Author: matthew
 */

#ifndef FOE_HPP_
#define FOE_HPP_

#include "Player.hpp"
#include <string>

class Player;

using namespace std;

class Foe {
private:
	string name;
	double maxHealth;
	double health;
	int speed;
	double strength;
	double resistance;
	double penetration;
	double crit_chance;

public:
	//Constructor
	Foe(string Name, double Health, int Speed,
			double Damage, double Resistance, double PENETRATION, double CRIT);
			
	//Accessors
	const string getName() const;
	const double getResistance() const;
	const double getHealth() const;
	const double getMaxHealth() const;
	const int getSpeed() const;
	const double getStrength() const;
	
	//Battle methods
	void takeDamage(double ow);
	const double damage(Player enemy) ;
	
	
	//Destructor
	virtual ~Foe();
};


#endif /* FOE_HPP_ */
