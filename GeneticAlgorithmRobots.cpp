#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

// Function Prototypes
int getAction();
int getSensorData();
void setMap(int (&map)[10][10]);
void displayMap(int map[10][10]);

struct Gene
{
  int north;
  int south;
  int east;
  int west;

  int action;
};

class Robot
{
private:
  int energyScore;
  Gene genes[16];
  int power;
public:
  Robot(){
    power = 5;
    energyScore = 0;
  }
  int getEnergyScore() const { return energyScore; }
  void incrementEnergy() { energyScore++; }
  void decrementPower() { power--; }
  int getPower() const { return power; }
  void pickupBattery() { power = power + 5; }
  void resetPower() { power = 5; }
  void resetEnergyScore() { energyScore = 0; }
  void setGenes();
  void getGenes(int &index);
  void setTopHalf(int i, vector<Robot>& population);
  void setBottomHalf(int i, vector<Robot>& population);
  void mutate();

  int getNorth(int i) { return genes[i].north; }
  int getSouth(int i) { return genes[i].south; }
  int getEast(int i) { return genes[i].east; }
  int getWest(int i) { return genes[i].west; }
  int getMovement(int i) { return genes[i].action; }

  bool operator <(Robot robot) const {
    return (energyScore > robot.energyScore);
  }
};

void Robot:: setGenes()
{
  for(int i = 0; i < 16; i++){
    genes[i].north = getSensorData();
    genes[i].south = getSensorData();
    genes[i].east = getSensorData();
    genes[i].west = getSensorData();
  
    genes[i].action = getAction();
  }
}

void Robot:: getGenes(int &index){
  for(int i = 0; i < 16; i++){
    cout << endl;
    cout << "North: " << genes[i].north << endl;
    cout << "South: " << genes[i].south << endl;
    cout << "East: " << genes[i].east << endl;
    cout << "West: " << genes[i].west << endl;
    cout << "Action: " << genes[i].action << endl;
    cout << "Gene: " << index << endl;
    index++;
  }
}

void Robot:: setTopHalf(int i, vector<Robot>& population)
{
  for(int j = 0; j < 8; j++)
  {
    population[i].genes[j] = population[i - 100].genes[j];
  }
}

void Robot:: setBottomHalf(int i, vector<Robot>& population)
{
  for(int j = 8; j < 16; j++)
  {
    population[i].genes[j] = population[i - 99].genes[j];
  }
}

void Robot:: mutate()
{
  int specificCharacter;
  specificCharacter = (rand() % 5) + 1;

  int changeTo;
  changeTo = (rand() % 4) + 1;

  if(specificCharacter == 1)
  {
    for(int i = 0; i < 16; i++)
    {
      genes[i].north = changeTo;
    } 
  }

  else if(specificCharacter == 2)
  {
    for(int i = 0; i < 16; i++)
    {
      genes[i].south = changeTo;
    } 
  }

  else if(specificCharacter == 3)
  {
    for(int i = 0; i < 16; i++)
    {
      genes[i].east = changeTo;
    } 
  }

  else if(specificCharacter == 4)
  {
    for(int i = 0; i < 16; i++)
    {
      genes[i].west = changeTo;
    } 
  }

  else if(specificCharacter == 5)
  {
    for(int i = 0; i < 16; i++)
    {
      genes[i].action = changeTo;
    } 
  }
} // END OF MUTATION

void robotRun(int map[10][10], Robot &robot);



////////////////////////////////////////////////////////////////////////////////////////
///////////////         MAIN PROGRAM STARTS HERE                  //////////////////////
////////////////////////////////////////////////////////////////////////////////////////




int main() {

  int index = 0;

  unsigned seed;
  seed = time(0);
  srand(seed);

  // CHANGE THIS VARIABLE TO CHANGE NUMBER OF GENES
  int generations = 10000;

  int totalEnergyScore = 0;
  double averageEnergy = 0;

  double grandTotal = 0;
  double grandAverage = 0;

  int map[10][10];

  vector<Robot> population;
  population.resize(200);

  // Creating Initial Population
  for(int i = 0; i < 200; i++)
  {
    population[i].setGenes();

  }

  // MAIN PROGRAM LOOP 
  for(int i = 1; i < generations + 1; i++)
  {
    // RUNNING GENERATION
    for(int i = 0; i < population.size(); i++)
    {
      setMap(map);
      robotRun(map, population[i]);
    }
  
    // SORTING POPULATION BY ENERGY SCORE
    sort(population.begin(), population.end());
  
    // CALCULATES TOTAL ENERGY SCORE
    for(int i = 0; i < population.size(); i++)
    {
      totalEnergyScore = population[i].getEnergyScore() + totalEnergyScore;
    }
  
    // CALCULATES AVERAGE ENERGY SCORE
    averageEnergy = static_cast<double>(totalEnergyScore) / 200;
  
    // DISPLAYS AVERAGE ENERGY SCORE
    cout << "Gen " << i << ": " << averageEnergy << endl;

    // Reset Energy Scores
    for(int i = 0; i < 200; i++)
    {
      population[i].resetEnergyScore();
      population[i].resetPower();
    }

    grandTotal = grandTotal + averageEnergy;
    
    averageEnergy = 0;
    totalEnergyScore = 0;
  
    // KILLS BOTTOM HALF OF POPULATION
    population.resize(100);
    population.resize(200);

    // REPRODUCTION
    for(int i = 100; i < 200; i = i + 2)
    {
      population[i].setTopHalf(i, population);
      population[i].setBottomHalf(i, population);

      population[i + 1].setTopHalf(i, population);
      population[i + 1].setBottomHalf(i, population);
    }

    // MUTATION
    for(int i = 189; i < 200; i++)
    {
      population[i].mutate();
    }

  } // END OF MAIN LOOP

  // AVERAGE OF ALL GENERATIONS
  grandAverage = grandTotal / generations;
  cout << "\nAverage Generation Fitness: " << grandAverage << endl;

  return 0;
}


int getAction()
{
  int randomNumber;

  randomNumber = (rand() % 5) + 1;

  return randomNumber;
}

int getSensorData()
{
  int randomNumber;

  randomNumber = (rand() % 4) + 1;

  return randomNumber;
}

void setMap(int (&map)[10][10])
{
  int row = 0;
  int col = 0;
  int batteriesLeft = 26;
  bool robotInserted = false;

  // SETTING FIRST ROW (ALL WALL OBJECTS)
  while(col < 10){
    map[row][col] = 4;
    col++;
  }

  row++;
  col = 0;
  
  while(row < 9)
  {

    // THIS SETS THE WESTERN WALL
    if(col == 0)
    {
      map[row][col] = 4;
      col++;
    } // END OF IF ELSE

    // THIS SETS THE EASTERN WALL
    else if(col == 9)
    {
      map[row][col] = 4;
      col = 0;
      row++;
    } // END OF IF ELSE

    else if(col != 0 and col != 9)
    {
      map[row][col] = 1;
      col++;
    } // END OF IF ELSE
  
  } // THIS IS THE END OF THE WHILE LOOP

  // THIS SETS THE SOUTHERN WALL
  while(col < 10)
  {
    map[row][col] = 4;
    col++;
  } // FINISHES MAP

  // THIS WHILE LOOP INSERTS BATTERIES INTO MAP
  while(batteriesLeft > 0)
  {
    row = (rand() % 8) + 1;
    col = (rand() % 8) + 1;

    if(map[row][col] == 1)
    {
      map[row][col] = 2;
      batteriesLeft--;
    }
  }

  // THIS WHILE LOOP WILL INSERT THE ROBOT
  while(robotInserted == false)
  {
    row = (rand() % 8) + 1;
    col = (rand() % 8) + 1;

    if(map[row][col] == 1)
    {
      map[row][col] = 9;
      robotInserted = true;
    }
  }
  

} // THIS IS THE END OF THE FUNCTION




// THIS FUNCTION IS FOR DEBUGGING ONLY
void displayMap(int map[10][10])
{
  for(int i = 0; i < 10; i++)
  {
    for(int j = 0; j < 10; j++)
    {
      cout << " " << map[i][j] << " ";
    }
    cout << endl;
  }
}


void robotRun(int map[10][10], Robot &robot)
{
  int row;
  int col;
  int action;
  bool robotFound = false;
  bool actionFound = false;
  
  
  while(robot.getPower() > 0)
  {
    for(int i = 0; i < 10; i++)
    {
      for(int j = 0; j < 10; j++)
      { 
        if(map[i][j] == 9)
        {
          row = i;
          col = j;
          robotFound = true;
          break;
        }
        else
        {
          continue;
        }
      } // COLLUMN CHECKING FOR LOOP

      if(robotFound == true)
      {
        robotFound = false;
        break;
      }
      
    } // ROW CHECKING FOR LOOP

    for(int f = 0; f < 15; f++)
    {
      // GENE CHECK
      if((map[row - 1][col] == robot.getNorth(f) or map[row - 1][col] == 3) and (map[row + 1][col] == robot.getSouth(f) or map[row + 1][col] == 3) and (map[row][col + 1] == robot.getEast(f) or map[row][col + 1] == 3) and (map[row][col - 1] == robot.getWest(f) or map[row][col - 1] == 3))
      {
        action = robot.getMovement(f);
        actionFound = true;
        break;
      }

      else
      {
        continue;
      }
      
    }

    if(actionFound == false)
    {
      action = robot.getMovement(15);
    }

    actionFound = false;

    // MOVE NORTH
    if(action == 1)
    {
      if(map[row - 1][col] == 4)
      {
        robot.decrementPower();
        continue;
      }
      else if(map[row - 1][col] == 2)
      {
        robot.pickupBattery();
        robot.decrementPower();
        robot.incrementEnergy();
        map[row][col] = 1;
        map[row - 1][col] = 9;
      }
      else if(map[row - 1][col] == 1)
      {
        robot.decrementPower();
        robot.incrementEnergy();
        map[row][col] = 1;
        map[row - 1][col] = 9;
      }
      
    }

    // MOVE SOUTH
    else if(action == 2)
    {
      if(map[row + 1][col] == 4)
      {
        robot.decrementPower();
        continue;
      }
      else if(map[row + 1][col] == 2)
      {
        robot.pickupBattery();
        robot.decrementPower();
        robot.incrementEnergy();
        map[row][col] = 1;
        map[row + 1][col] = 9;
      }
      else if(map[row + 1][col] == 1)
      {
        robot.decrementPower();
        robot.incrementEnergy();
        map[row][col] = 1;
        map[row + 1][col] = 9;
      }
    }

    // MOVE EAST
    else if(action == 3)
    {
      if(map[row][col + 1] == 4)
      {
        robot.decrementPower();
        continue;
      }
      else if(map[row][col + 1] == 2)
      {
        robot.pickupBattery();
        robot.decrementPower();
        robot.incrementEnergy();
        map[row][col] = 1;
        map[row][col + 1] = 9;
      }
      else if(map[row][col + 1] == 1)
      {
        robot.decrementPower();
        robot.incrementEnergy();
        map[row][col] = 1;
        map[row][col + 1] = 9;
      }
    }

    // MOVE WEST
    else if(action == 4)
    {
      if(map[row][col - 1] == 4)
      {
        robot.decrementPower();
        continue;
      }
      else if(map[row][col - 1] == 2)
      {
        robot.pickupBattery();
        robot.decrementPower();
        robot.incrementEnergy();
        map[row][col] = 1;
        map[row][col - 1] = 9;
      }
      else if(map[row][col - 1] == 1)
      {
        robot.decrementPower();
        robot.incrementEnergy();
        map[row][col] = 1;
        map[row][col - 1] = 9;
      }
    }

    // MOVE RANDOM DIRECTION
    else if(action == 5)
    {
      action = (rand() % 4) + 1;

      // RANDOM NORTH
      if(action == 1)
      {
        if(map[row - 1][col] == 4)
        {
          robot.decrementPower();
          continue;
        }
        else if(map[row - 1][col] == 2)
        {
          robot.pickupBattery();
          robot.decrementPower();
          robot.incrementEnergy();
          map[row][col] = 1;
          map[row - 1][col] = 9;
        }
        else if(map[row - 1][col] == 1)
        {
          robot.decrementPower();
          robot.incrementEnergy();
          map[row][col] = 1;
          map[row - 1][col] = 9;
        }
      }

      // RANDOM SOUTH
      else if(action == 2)
      {
        if(map[row + 1][col] == 4)
        {
          robot.decrementPower();
          continue;
        }
        else if(map[row + 1][col] == 2)
        {
          robot.pickupBattery();
          robot.decrementPower();
          robot.incrementEnergy();
          map[row][col] = 1;
          map[row + 1][col] = 9;
        }
        else if(map[row + 1][col] == 1)
        {
          robot.decrementPower();
          robot.incrementEnergy();
          map[row][col] = 1;
          map[row + 1][col] = 9;
        }
      }

      // RANDOM EAST
      else if(action == 3)
      {
        if(map[row][col + 1] == 4)
        {
          robot.decrementPower();
          continue;
        }
        else if(map[row][col + 1] == 2)
        {
          robot.pickupBattery();
          robot.decrementPower();
          robot.incrementEnergy();
          map[row][col] = 1;
          map[row][col + 1] = 9;
        }
        else if(map[row][col + 1] == 1)
        {
          robot.decrementPower();
          robot.incrementEnergy();
          map[row][col] = 1;
          map[row][col + 1] = 9;
        }
      }

      // RANDOM WEST
      else if(action == 4)
      {
        if(map[row][col - 1] == 4)
        {
          robot.decrementPower();
          continue;
        }
        else if(map[row][col - 1] == 2)
        {
          robot.pickupBattery();
          robot.decrementPower();
          robot.incrementEnergy();
          map[row][col] = 1;
          map[row][col - 1] = 9;
        }
        else if(map[row][col - 1] == 1)
        {
          robot.decrementPower();
          robot.incrementEnergy();
          map[row][col] = 1;
          map[row][col - 1] = 9;
        }
      }
      
    } // END OF RANDOM DIRECTION

    
  } // MAIN WHILE LOOP

  
} // END OF FUNCTION


 