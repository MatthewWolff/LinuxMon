/*
 * Player.hpp
 *
 *  Created on: Dec 17, 2016
 *      Author: matthew
 */

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <string>
#include "Foe.hpp"
class Foe; //Required to stop errors stemming from two classes interacting.

using namespace std;
class Player{
private:
	string name;
	string classification;
	int score;
	double maxHealth;
	double currHealth;
	int level;
	int speed;
	double brawn;
	double resistance;
	double penetration;
	double crit_chance;
	//strength, speed, defense, magic resistance

public:
	//Globals
	const int MAX_LEVEL = 18;
	
	//Constructor
	Player();
	
	//Accessors and mutators
	string getName();
	string getClass();
	int getScore();
	void setScore(int newScore); //to be used when the player kills or is killed
	double health(); //so monsters can see how much health you have
	double getMaxHealth();
	int getResistance(); //for monsters calculating how much dmg
	int setResist();
	int getSpeed();
	int setSpeed();
	int getLevel();
	void levelUp(); //level up and prompt the user for which state they want to increase
	void stats(); //Prints all stats to console

	//Battle methods
	void takeDamage(double ow); // how much damage the enemy does
	const double damage(Foe enemy); //determines how much dmg to do

	virtual ~Player(); //deconstructor
};

#endif /* PLAYER_HPP_ */
