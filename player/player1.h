#include "Arduino.h"

class player
{
	private:
	uint8_t _game_score;
	uint8_t _set_score;
	uint8_t _player_id;
	char    _name[9]; //displayed vertically on both sides
	bool    _side; //side=FALSE means they are on hte left side
	bool    _serve; //serve=TRUE means they have the serve
	
	public:
	void game_increment();
	void set_increment();
	void game_decrement();
	void swap_side();
	void set_side(bool);
	void swap_serve();
	void set_serve(bool);
	void set_name(char [9]);

	
	uint8_t read_game_score();
	uint8_t read_set_score();
	uint8_t read_player_id();
	char*	read_name();
	bool	read_side();
	bool	read_serve();

	player();
	
};

uint8_t player::read_game_score()
{
	return _game_score;
}

uint8_t player::read_set_score()
{
	return _set_score;
}

uint8_t player::read_player_id()
{
	return _game_score;
}

char*  player::read_name()
{
	return _name;
}

bool player::read_serve()
{
	return _serve;
}

bool player::read_side()
{
	return _side;
}

//initialize to zero set later when created;
player::player()
{
	_game_score	=0;
	_set_score 	=0;
	_serve		=0;
	_side		=0;
}


void player::game_increment()
{
	_game_score++;
}

void player::set_increment()
{
	_set_score++;
}

void player::game_decrement()
{
	_game_score--;
}

void player::swap_side()
{
	_side= !_side;
}

void player::swap_serve()
{
	_serve= !_serve;
}

void player::set_serve(bool init)
{
	_serve= init;
}

void player::set_side(bool init)
{
	_side= init;
}

void player::set_name(char init[9])
{
	*_name=*init;
}
/*void main() 
{	
	rand_1=random() & 1; //who serves first
	rand_2=random() & 1;// what side they are on
	
	if (rand1)
	{
		print(player_a to serve);
	}
	else
	{
		print(player_b to serve);
	}
	
	if (rand2)
	{
	print(player a to left side);
	}
	else
	{
		print(player b to left side);
	}
	
	player	player_a(id_1,rand1,rand2);
	player	player_b(id_2,!rand1,!rand2);

	if game_score(player_a,player_b)
		
	if game_score(player_b,player_a)
	
}*/

