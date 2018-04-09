#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

class World
{
public:
	friend class Organism;
	friend class Doodlebug;
	friend class Ant;
	friend ostream& operator<<(ostream& o, World instance);
	friend istream& operator>>(istream& i, World instance);
	World();
	~World();
	void printGrid();
	void timeStep();
	void setGrid(int x, int y, Organism* pOrg);
	Organism* getGrid(int x, int y);

private:
	int x;
	int y;
	Organism* grid[20][20];
};
class Organism
{
public:
	friend class World;
	friend ostream& operator<<(ostream& o, Organism instance);
	friend istream& operator>>(istream& i, Organism instance);
	Organism();
	Organism(World *wrld, int x, int y);
	~Organism();
	char getType();
	virtual void move();
	virtual void breed();
	virtual bool starved();
	

protected:
	char type;
	int x;
	int y;
	int breedCount;
	bool moved;
	World* world;
	
};
class Ant:public Organism
{
public:
	friend class World;
	Ant();
	Ant(World *world, int x, int y);
	~Ant();
	void move();
	void breed();

private:
	bool moved;
};
class Doodlebug:public Organism
{
public:
	friend class World;
	Doodlebug();
	Doodlebug(World *world, int x, int y);
	~Doodlebug();
	void move();
	void breed();
	bool starved();
	
private:
	bool moved;
	int starveCount;
};
//World Constructor
World::World()
{
	srand(time(0));
	for (int i = 0; i <20; i++)
	{
		for (int j = 0; j <20; j++)
		{
			grid[i][j] = NULL;
		}
	}
	for (int i = 0; i <99; i++)
	{
		x = (rand() % 19);
		y = (rand() % 19);
		grid[x][y] = new Ant(this, x, y);
	}
	for (int i = 0; i < 5; i++)
	{
		x = (rand() % 19);
		y = (rand() % 19);
		grid[x][y] = new Doodlebug(this, x, y);
	}
	//cout << "The world is constructed!" << endl;
}
//World Deconstructor
World::~World()
{
	int i, j;
	for (i = 0; i<20; i++)
	{
		for (j = 0; j<20; j++)
		{
			if (grid[i][j] != NULL) delete (grid[i][j]);
		}
	}
	//cout << "The world is deconstructed!" << endl;
}
//Organism Constructor
Organism::Organism()
{
	world = NULL;
	moved = false;
	breedCount = 0;
	x = 0;
	y = 0;
	type = 'O';
	//cout << "The organism is constructed!" << endl;
}
Organism::Organism(World *world, int x, int y)
{
	world->setGrid(x, y, this);
	this->world = world;
	moved = false;
	breedCount = 0;
	this->x = x;
	this->y = y;
	type = 'O';
}
//Organism Deconstructor
Organism::~Organism()
{
	//cout << "The organism is deconstructed!" << endl;
}
//Ant Constructor
Ant::Ant()
{

	type = 'o';
	//cout << "The ant is constructed!" << endl;
}
Ant::Ant(World *world, int x, int y) : Organism(world, x, y)
{
	type = 'o';
}
//Ant Deconstructor
Ant::~Ant()
{
	//cout << "The ant is deconstructed!" << endl;
}
//Doodlebug Constructor
Doodlebug::Doodlebug()
{
	type = 'X';
	//cout << "The doodlebug is constructed!" << endl;
}
Doodlebug::Doodlebug(World *world, int x, int y) : Organism(world, x, y)
{
	type = 'X';
}
//Doodlebug Deconstructor
Doodlebug::~Doodlebug()
{
	//cout << "The doodlebug is deconstructed!" << endl;
}
//World << overload
ostream& operator<<(ostream& o, World instance)
{
	o << instance.x;
	return o;
}
//World >> overload
istream& operator >> (istream& i, World instance)
{
	i >> instance.x;
return i;
}
//Organism << overload
ostream& operator<<(ostream& o, Organism instance)
{
	o << instance.type;
	return o;
}
//Organism >> overload
istream& operator >> (istream& i, Organism instance)
{
	i >> instance.x;
	return i;
}
//World Functions
void World::printGrid()
{
	for (int i = 0; i <20; i++)
	{
		for (int j = 0; j <20; j++)
		{
			if (grid[i][j] == NULL)
				cout << '-';
			else cout << (*grid[i][j]);
		}
		cout << endl;
	}
}
Organism* World::getGrid(int x, int y)
{
	return grid[x][y];
}
void World::setGrid(int x, int y, Organism* pOrg)
{
	grid[x][y] = pOrg;
}
void World::timeStep()
{
	//set moved to false
	for (int i = 0; i<20; i++)
		for (int j = 0; j<20; j++)
		{
			if (grid[i][j] != NULL) grid[i][j]->moved = false;
		}
	//move Doodlebugs
	for (int i = 0; i<20; i++)
		for (int j = 0; j<20; j++)
		{
			if ((grid[i][j] != NULL) && (grid[i][j]->getType() == 'X'))
			{
				if (grid[i][j]->moved == false)
				{
					grid[i][j]->moved = true; 
					grid[i][j]->move();
				}
			}
		}
	//move Ants
	for (int i = 0; i<20; i++)
		for (int j = 0; j<20; j++)
		{
			if ((grid[i][j] != NULL) && (grid[i][j]->getType() == 'o'))
			{
				if (grid[i][j]->moved == false)
				{
					grid[i][j]->moved = true; 
					grid[i][j]->move();
				}
			}
		}
	//Starve Doodlebugs
	for (int i = 0; i<20; i++)
		for (int j = 0; j<20; j++)
		{
			if ((grid[i][j] != NULL) &&
				(grid[i][j]->getType() == 'X'))
			{
				if (grid[i][j]->starved())
				{
					delete (grid[i][j]);
					grid[i][j] = NULL;
				}
			}
		}
	//Breed
	for (int i = 0; i<20; i++)
		for (int j = 0; j<20; j++)
		{
			if ((grid[i][j] != NULL) && (grid[i][j]->moved == true))
			grid[i][j]->breed();
		}
}
//Organism functions
char Organism::getType()
{
	return type;
}
void Organism::move()
{

}
void Organism::breed()
{

}
bool Organism::starved()
{
	return 0;
}
//Ant functions
void Ant::move()
{
	int direction = rand() % 4;
	if (direction == 0)
	{
		if ((y>0) && (world->getGrid(x, y - 1) == NULL))
		{
			world->setGrid(x, y - 1, this);  
			world->setGrid(x, y, NULL);
			y--;
		}
	}
	else if (direction == 1)
	{
		if ((y<19) && (world->getGrid(x, y + 1) == NULL))
		{
			world->setGrid(x, y + 1, this);  
			world->setGrid(x, y, NULL);  
			y++;
		}
	}

	else if (direction == 2)
	{
		if ((x>0) && (world->getGrid(x - 1, y) == NULL))
		{
			world->setGrid(x - 1, y, this);  
			world->setGrid(x, y, NULL);  
			x--;
		}
	}
	else
	{
		if ((x<19) && (world->getGrid(x + 1, y) == NULL))
		{
			world->setGrid(x + 1, y, this); 
			world->setGrid(x, y, NULL);  
			x++;
		}
	}

}
void Ant::breed()
{
	breedCount++;
	if (breedCount == 3)
	{
		breedCount = 0;
		if ((y>0) && (world->getGrid(x, y - 1) == NULL))
		Ant *a1 = new Ant(world, x, y - 1);
		else if ((y<19) && (world->getGrid(x, y + 1) == NULL))
		Ant *a1 = new Ant(world, x, y + 1);
		else if ((x>0) && (world->getGrid(x - 1, y) == NULL))
		Ant *a1 = new Ant(world, x - 1, y);
		else if ((x<19) && (world->getGrid(x + 1, y) == NULL))
		Ant *a1 = new Ant(world, x + 1, y);
	}
}
//Doodlebug functions
//eat ants
void Doodlebug::move()
{
	if ((y > 0) && (world->getGrid(x, y - 1) != NULL) &&
		(world->getGrid(x, y - 1)->getType() == 'o'))
	{
		delete (world->grid[x][y - 1]);
		world->grid[x][y - 1] = this;
		world->setGrid(x, y, NULL);
		starveCount = 0;
		y--;
		return;
	}
	else if ((y < 19) && (world->getGrid(x, y + 1) != NULL) &&
		(world->getGrid(x, y + 1)->getType() == 'o'))
	{
		delete (world->grid[x][y + 1]);
		world->grid[x][y + 1] = this;
		world->setGrid(x, y, NULL);
		starveCount = 0;
		y++;
		return;
	}
	else if ((x > 0) && (world->getGrid(x - 1, y) != NULL) &&
		(world->getGrid(x - 1, y)->getType() == 'o'))
	{
		delete (world->grid[x - 1][y]);
		world->grid[x - 1][y] = this;
		world->setGrid(x, y, NULL);
		starveCount = 0;
		x--;
		return;
	}
	else if ((x < 19) && (world->getGrid(x + 1, y) != NULL) &&
		(world->getGrid(x + 1, y)->getType() == 'o'))
	{
		delete (world->grid[x + 1][y]);
		world->grid[x + 1][y] = this;
		world->setGrid(x, y, NULL);
		starveCount = 0;
		x++;
		return;
	}
}
//breed
void Doodlebug::breed()
{
	breedCount++;
	if (breedCount == 8)
	{
		breedCount = 0;
		if ((y>0) && (world->getGrid(x, y - 1) == NULL))
		Doodlebug *d1 = new Doodlebug(world, x, y - 1);
		else if ((y<19) && (world->getGrid(x, y + 1) == NULL))
		Doodlebug *d1 = new Doodlebug(world, x, y + 1);
		else if ((x>0) && (world->getGrid(x - 1, y) == NULL))
		Doodlebug *d1 = new Doodlebug(world, x - 1, y);
		else if ((x<19) && (world->getGrid(x + 1, y) == NULL))
		Doodlebug *d1 = new Doodlebug(world, x + 1, y);
	}
}
//starve
bool Doodlebug::starved()
{
	if (starveCount > 3)
		return true;
	else return false;
}

int main()
{

	//cout << "World:" << endl;
	//World w1;
	//cout << "w1.x:" << w1 << endl;
	//cout << "Organism" << endl;
	//Organism o1;
	//cout << "o1.type:" << o1 << endl;
	//cout << endl;
	//cout << "Ant" << endl;
	//Ant a1;
	//cout << endl;
	//cout << "Doodlebug" << endl;
	//Doodlebug d1;
	//cout << endl;
	
	//grid[1][1] = new Ant;
	//cout << (*grid[1][1]);
	string s;
	World w;
	
	while (true)
	{
		w.printGrid();
		w.timeStep();
		cout << endl << "Press enter for next step" << endl;
		getline(cin, s);	
	}
			return 0;
}