#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

const Position Position::npos = Position(-1, -1);
Map :: Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls) : num_rows(num_rows), num_cols(num_cols)
{
    map = new MapElement**[num_rows];
    for (int i = 0; i < num_rows; ++i)
    {
        map[i] = new MapElement*[num_cols];
        for (int j = 0; j < num_cols; ++j) 
                map[i][j] = new Path();
    }
    
        

    for (int i = 0; i < num_walls; ++i) 
    {
        int rw = array_walls[i].getRow();
        int cl = array_walls[i].getCol();
        if (rw >= 0 && rw < num_rows && cl >= 0 && cl < num_cols)
        {
            if (map[rw][cl] ->getType() != WALL)
            {
                delete map[rw][cl];
                map[rw][cl] = new Wall();
            }
        }
    }

    for (int i = 0; i < num_fake_walls; ++i) 
    {
        int rw = array_fake_walls[i].getRow();
        int cl = array_fake_walls[i].getCol();
        if (rw >= 0 && rw < num_rows && cl >= 0 && cl < num_cols)
        {
            delete map[rw][cl];
            map[rw][cl] = new FakeWall(rw, cl);
        }
    }

    

}

bool Map :: isValid ( const Position & pos , MovingObject * mv_obj ) const 
{
    string rfname =  mv_obj -> getName();
    if (pos.getCol() < 0 || pos.getRow() < 0 || pos.getCol() > num_cols - 1 || pos.getRow() > num_rows - 1)
        return 0;
    
    if (rfname == "Sherlock" && map[pos.getRow()][pos.getCol()] -> getType() == WALL)
        return 0; 

    if(rfname == "Sherlock" && map[pos.getRow()][pos.getCol()] -> getType() != WALL)
    {
        return 1;
    }

    if (rfname == "Criminal" && map[pos.getRow()][pos.getCol()] -> getType() == WALL)
        return 0;
    if(rfname == "Criminal" && map[pos.getRow()][pos.getCol()] -> getType() != WALL)
        return 1;

    if (rfname == "Watson")
    {
        
        switch (map[pos.getRow()][pos.getCol()] -> getType())
        {
        case WALL:
            return 0;
            break;
        case PATH:
            return 1;
            break;
        case FAKE_WALL:
            if (dynamic_cast<Character*>(mv_obj) -> getEXP() > dynamic_cast<FakeWall*>(map[pos.getRow()][pos.getCol()]) -> getReqExp())
            {   
                //cout << "flag neeeded" << endl; 
                return 1;
            }
            else
            {
                
                return 0;
            }
            break;
        }
    }
    
    if (rfname == "RobotC" || rfname == "RobotS" || mv_obj -> getName() == "RobotSW" || mv_obj -> getName() == "RobotW")
    {
        if (map[pos.getRow()][pos.getCol()] -> getType() == WALL)
            return 0;
    }
    return 1;
}

///////////////////////////



int Sherlock :: CountCallNextPos = 0;
Position Sherlock :: getNextPosition()
{
    char guide = moving_rule[CountCallNextPos++];
    if (CountCallNextPos >= rule_length)
        CountCallNextPos = 0;
    int next_R = pos.getRow();
    int next_C = pos.getCol();
    if (guide == 'L')
    {
        next_C--;
        return Position(next_R, next_C);
    }
    if (guide == 'R')
    {
        next_C++;
        return Position(next_R, next_C);
    }
    if (guide == 'U')
    {
        next_R--;
        return Position(next_R, next_C);
    }
    if (guide == 'D')
    {
        next_R++;
        return Position(next_R, next_C);
    }
    return Position::npos;
}

int Watson :: CountCallNextPos = 0;
Position Watson :: getNextPosition()
{
    char guide = moving_rule[CountCallNextPos++];
    if (CountCallNextPos >= rule_length)
        CountCallNextPos = 0;
    int next_R = pos.getRow();
    int next_C = pos.getCol();
    if (guide == 'L')
    {
        next_C--;
        return Position(next_R, next_C);
    }
    if (guide == 'R')
    {
        next_C++;
        return Position(next_R, next_C);
    }
    if (guide == 'U')
    {
        next_R--;
        return Position(next_R, next_C);
    }
    if (guide == 'D')
    {
        next_R++;
        return Position(next_R, next_C);
    }
    return Position::npos;
}
///////


Position Criminal :: getNextPosition()
{
    spos = sherlock->getCurrentPosition();
    wpos = watson->getCurrentPosition();
    int R = pos.getRow();
    int C = pos.getCol();
    int r_ori = R;
    int c_ori = C;
    if (R < max(spos.getRow(),wpos.getRow()) && R > min(spos.getRow(),wpos.getRow()))
    {
        if (C >= max(spos.getCol(), wpos.getCol()))
            C ++;
        else if (C  <= min(spos.getCol(), wpos.getCol()))
            C --;
        if (C != c_ori)
        {
            Position temp = Position(R, C);
            if (map -> isValid(temp, this))
                return temp;
        }
    }
    if (C < max(spos.getCol(),wpos.getCol()) && C > min(spos.getCol(),wpos.getCol()))
    {
        if (R >= max(spos.getRow(), wpos.getRow()))
            R ++;
        else if (R  <= min(spos.getRow(), wpos.getRow()))
            R --;
        if(R != r_ori)
        {
        Position temp = Position(R, C);
        if (map -> isValid(temp, this))
            return temp;
        }
    }


    if (R >= max(spos.getRow(),wpos.getRow()) && C >= max(spos.getCol(),wpos.getCol()))
    {
        Position temp = Position(R + 1, C);
        if (map -> isValid(temp, this))
            return temp;
        temp = Position(R, C + 1);
        if (map -> isValid(temp, this))
            return temp;
    }


    if (R >= max(spos.getRow(),wpos.getRow()) && C <= min(spos.getCol(),wpos.getCol()))
    {
        Position temp = Position(R, C - 1);
        if (map -> isValid(temp, this))
            return temp;
        temp = Position(R + 1, C);
        if (map -> isValid(temp, this))
            return temp;
    }


    if (R <= min(spos.getRow(),wpos.getRow()) && C >= max(spos.getCol(),wpos.getCol()))
    {
        Position temp = Position(R - 1, C);
        if (map -> isValid(temp, this))
            return temp;
        temp = Position(R, C + 1);
        if (map -> isValid(temp, this))
            return temp;
    }
    if (R <= min(spos.getRow(),wpos.getRow()) && C <= min(spos.getCol(),wpos.getCol()))
    {
        Position temp = Position(R, C - 1);
        if (map -> isValid(temp, this))
            return temp;
        temp = Position(R - 1, C);
        if (map -> isValid(temp, this))
            return temp;
    }

    for (int i = 0; i < 4; i++)
    {
        if(i == 0)
        {
            Position temp = Position(r_ori - 1, c_ori);
            if (map->isValid(temp, this))
                return temp;
        }
        if(i == 1)
        {
            Position temp = Position(r_ori , c_ori - 1);
            if (map->isValid(temp, this))
                return temp;
        }
        if(i == 2)
        {
            Position temp = Position(r_ori + 1, c_ori);
            if (map->isValid(temp, this))
                return temp;
        }
        if(i == 3)
        {
            Position temp = Position(r_ori, c_ori + 1);
            if (map->isValid(temp, this))
                return temp;
        }
    }
    
    return Position::npos;
}



////////////

Configuration::Configuration(const string & filepath) 
{
    
    ifstream inFile(filepath);
   
    string line;
    while (getline(inFile, line)) 
    {
        if (line.find("MAP_NUM_ROWS=") != string::npos)
            map_num_rows = stoi(line.substr(line.find("=") + 1));

        if (line.find("MAP_NUM_COLS=") != string::npos) 
            map_num_cols = stoi(line.substr(line.find("=") + 1));

        if (line.find("MAX_NUM_MOVING_OBJECTS=") != string::npos) 
            max_num_moving_objects = stoi(line.substr(line.find("=") + 1));

        if (line.find("ARRAY_WALLS=") != string::npos) 
        {
            string temp = line.substr(line.find("=") + 1);
            num_walls = 0;
            for (int i = 0; i < temp.length(); i++)
                if (temp[i] == '(')
                    num_walls++;
                
            if (num_walls != 0)
            {
                arr_walls = new Position[num_walls];
                stringstream ss(temp);
                char special;
                int num;
                int coordinates = 0;
                while (ss >> special) 
                {
                    if (isdigit(special)) 
                    {
                        ss.putback(special); 
                        ss >> num;      
                        arr_walls[coordinates].setRow(num);
                        ss >> special;       
                        ss >> num;      
                        arr_walls[coordinates].setCol(num); 
                        coordinates++;
                    }
                }
            }
            else 
            {
                arr_walls = nullptr;
            
            }
        }
        if (line.find("ARRAY_FAKE_WALLS=") != string::npos) 
        {
            string temp = line.substr(line.find("=") + 1);

            num_fake_walls = 0;
            for (int i = 0; i < temp.length(); i++)
                if (temp[i] == '(')
                    num_fake_walls++;
            if (num_fake_walls != 0)
            {
                arr_fake_walls = new Position[num_fake_walls]; 
                istringstream ss(temp);
                char special;
                int num;
                int coordinates = 0;
                while (ss >> special) 
                {
                    if (isdigit(special)) 
                    {
                        ss.putback(special); 
                        ss >> num;      
                        arr_fake_walls[coordinates].setRow(num); 
                        ss >> special;       
                        ss >> num;      
                        arr_fake_walls[coordinates].setCol(num);
                        coordinates++; 
                    }
                }
            }
            else {
                arr_fake_walls = nullptr;
            
            }
        }
        
        if (line.find("SHERLOCK_MOVING_RULE=") != string::npos) 
            sherlock_moving_rule = line.substr(line.find("=") + 1);

        if (line.find("WATSON_MOVING_RULE=") != string::npos) 
            watson_moving_rule = line.substr(line.find("=") + 1);

        if (line.find("SHERLOCK_INIT_POS=") != string::npos)
        {
            string a = line.substr(line.find("=") + 1);
            istringstream ss(a);
            int num;
            char characters;
            while (ss >> characters)
            {
                if (isdigit(characters))
                {
                    ss.putback(characters);
                    ss >> num;
                    sherlock_init_pos.setRow(num);
                    ss >> characters;
                    ss >> num;
                    sherlock_init_pos.setCol(num);
                }
            }

        }

        if (line.find("WATSON_INIT_POS=") != string::npos)
        {
            string temp = line.substr(line.find("=") + 1);

            istringstream ss(temp);
            int num;
            char characters;
            while (ss >> characters)
            {
                if (isdigit(characters))
                {
                    ss.putback(characters);
                    ss >> num;
                    watson_init_pos.setRow(num);
                    ss >> characters;
                    ss >> num;
                    watson_init_pos.setCol(num);
                }
            }
        }
        
        if (line.find("CRIMINAL_INIT_POS=") != string::npos)
        {
            string temp = line.substr(line.find("=") + 1);
            istringstream ss(temp);
            int num;
            char characters;
            while (ss >> characters)
            {
                if (isdigit(characters))
                {
                    ss.putback(characters);
                    ss >> num;
                    criminal_init_pos.setRow(num);
                    ss >> characters;
                    ss >> num;
                    criminal_init_pos.setCol(num);
                }
            }
        }
        if (line.find("SHERLOCK_INIT_HP=") != string::npos) 
            sherlock_init_hp = stoi(line.substr(line.find("=") + 1));
        if (line.find("SHERLOCK_INIT_EXP=") != string::npos) 
            sherlock_init_exp = stoi(line.substr(line.find("=") + 1));

        if (line.find("WATSON_INIT_HP=") != string::npos) 
            watson_init_hp = stoi(line.substr(line.find("=") + 1));

        if (line.find("WATSON_INIT_EXP=") != string::npos) 
            watson_init_exp = stoi(line.substr(line.find("=") + 1));

        if (line.find("NUM_STEPS=") != string::npos) 
            num_steps = stoi(line.substr(line.find("=") + 1));
                        
    }
    inFile.close();
    
}


string Configuration::str() const 
{
    string outConfig = "Configuration[\n";
    outConfig += "MAP_NUM_ROWS=" + to_string(map_num_rows) + "\n";
    outConfig += "MAP_NUM_COLS=" + to_string(map_num_cols) + "\n";
    outConfig += "MAX_NUM_MOVING_OBJECTS=" + to_string(max_num_moving_objects) + "\n";
    outConfig += "NUM_WALLS=" + to_string(num_walls) + "\n"; 

    outConfig += "ARRAY_WALLS=[";
    if(num_walls != 0) 
    {
        for(int i = 0; i < num_walls; i++)
            outConfig += arr_walls[i].str() + ";";

        outConfig.pop_back();
        outConfig += "]\n";
    }
    else 
        outConfig +="]\n";

    outConfig = outConfig + "NUM_FAKE_WALLS=" + to_string(num_fake_walls) + "\n";

    outConfig += "ARRAY_FAKE_WALLS=[";
    if(num_fake_walls != 0) 
    {
        for(int i = 0; i < num_fake_walls; i++) 
                outConfig += arr_fake_walls[i].str() + ";";
            
        outConfig.pop_back();
        outConfig += "]\n";
    }
    else 
        outConfig += "]\n";

    outConfig += "SHERLOCK_MOVING_RULE=" + sherlock_moving_rule + "\n";
    outConfig += "SHERLOCK_INIT_POS=" + sherlock_init_pos.str() + "\n";
    outConfig += "SHERLOCK_INIT_HP=" + to_string(sherlock_init_hp) + "\n";
    outConfig += "SHERLOCK_INIT_EXP=" + to_string(sherlock_init_exp) + "\n";
    outConfig += "WATSON_MOVING_RULE=" + watson_moving_rule + "\n";
    outConfig += "WATSON_INIT_POS=" + watson_init_pos.str() + "\n";
    outConfig += "WATSON_INIT_HP=" + to_string(watson_init_hp) + "\n";
    outConfig += "WATSON_INIT_EXP=" + to_string(watson_init_exp) + "\n";
    outConfig += "CRIMINAL_INIT_POS=" + criminal_init_pos.str() + "\n";
    outConfig += "NUM_STEPS=" + to_string(num_steps) + "\n";

    outConfig += "]";
    return outConfig;
}


/////////
Position RobotS :: getNextPosition()
{
    int R = pos.getRow();
    int C = pos.getCol();
    int dist = manhattanDistance(sherlock->getCurrentPosition(), pos);
    Position temp = Position(R - 1, C);
    for (int i = 0; i < 4; i++)
    {
        switch (i)
        {
        case 0:
            if (manhattanDistance(sherlock->getCurrentPosition(), temp) < dist && map->isValid(temp, this))
                return temp;
            break;
        case 1:
            temp.setRow(R);
            temp.setCol(C + 1);
            if (manhattanDistance(sherlock->getCurrentPosition(), temp) < dist && map->isValid(temp, this))
                return temp;
            break;
        case 2:
            temp.setRow(R + 1);
            temp.setCol(C);
            if (manhattanDistance(sherlock->getCurrentPosition(), temp) < dist && map->isValid(temp, this))
                return temp;
            break;
        case 3:
            temp.setRow(R);
            temp.setCol(C - 1);
            if (manhattanDistance(sherlock->getCurrentPosition(), temp) < dist && map->isValid(temp, this))
                return temp;
            break;
        default:
            break;
        }
    }
    return Position::npos;
}


Position RobotW :: getNextPosition()
{
    int R = pos.getRow();
    int C = pos.getCol();
    int dist = manhattanDistance(watson->getCurrentPosition(), pos);
    Position temp = Position(R - 1, C);
    for (int i = 0; i < 4; i++)
    {
        switch (i)
        {
        case 0:
            if (manhattanDistance(watson->getCurrentPosition(), temp) < dist && map->isValid(temp, this))
                return temp;
            break;
        case 1:
            temp.setRow(R);
            temp.setCol(C + 1);
            if (manhattanDistance(watson->getCurrentPosition(), temp) < dist && map->isValid(temp, this))
                return temp;
            break;
        case 2:
            temp.setRow(R + 1);
            temp.setCol(C);
            if (manhattanDistance(watson->getCurrentPosition(), temp) < dist && map->isValid(temp, this))
                return temp;
            break;
        case 3:
            temp.setRow(R);
            temp.setCol(C - 1);
            if (manhattanDistance(watson->getCurrentPosition(), temp) < dist && map->isValid(temp, this))
                return temp;
            break;
        default:
            break;
        }
    }
    return Position::npos;
}


Position RobotSW :: getNextPosition()
{
    int R = pos.getRow();
    int C = pos.getCol();
    int dist = manhattanDistance(sherlock->getCurrentPosition(), pos) + manhattanDistance(watson->getCurrentPosition(), pos);
    Position temp = Position(R - 2, C);
    for (int i = 0; i < 8; i++)
    {
        switch (i)
        {
        case 0:
            if (manhattanDistance(sherlock->getCurrentPosition(), temp) + manhattanDistance(watson->getCurrentPosition(), temp) < dist && map->isValid(temp, this))
                return temp;
            break;
        case 1:
            temp.setRow(R - 1);
            temp.setCol(C + 1);
            if (manhattanDistance(sherlock->getCurrentPosition(), temp) + manhattanDistance(watson->getCurrentPosition(), temp) < dist && map->isValid(temp, this))
                return temp;
            break;
        case 2:
            temp.setRow(R);
            temp.setCol(C + 2);
            if (manhattanDistance(sherlock->getCurrentPosition(), temp) + manhattanDistance(watson->getCurrentPosition(), temp) < dist && map->isValid(temp, this))
                return temp;
            break;
        case 3:
            temp.setRow(R + 1);
            temp.setCol(C + 1);
            if (manhattanDistance(sherlock->getCurrentPosition(), temp) + manhattanDistance(watson->getCurrentPosition(), temp) < dist && map->isValid(temp, this))
                return temp;
            break;
        case 4:
            temp.setRow(R + 2);
            temp.setCol(C);
            if (manhattanDistance(sherlock->getCurrentPosition(), temp) + manhattanDistance(watson->getCurrentPosition(), temp) < dist && map->isValid(temp, this))
                return temp;
            break; 
        case 5:
            temp.setRow(R + 1);
            temp.setCol(C - 1);
            if (manhattanDistance(sherlock->getCurrentPosition(), temp) + manhattanDistance(watson->getCurrentPosition(), temp) < dist && map->isValid(temp, this))
                return temp;
            break;
        case 6:
            temp.setRow(R);
            temp.setCol(C - 2);
            if (manhattanDistance(sherlock->getCurrentPosition(), temp) + manhattanDistance(watson->getCurrentPosition(), temp) < dist && map->isValid(temp, this))
                return temp;
            break;
        case 7:
            temp.setRow(R - 1);
            temp.setCol(C - 1);
            if (manhattanDistance(sherlock->getCurrentPosition(), temp) + manhattanDistance(watson->getCurrentPosition(), temp) < dist && map->isValid(temp, this))
                return temp;
            break;
        default:
            break;
        }
    }
    return Position::npos;
    

}


string BaseItem:: getName()
{
    switch (item_type)
    {
    case MAGIC_BOOK:
        return "MagicBook";
        break;
    case ENERGY_DRINK:
        return "EnergyDrink";
        break;
    case FIRST_AID:
        return "FirstAid";
        break;
    case EXCEMPTION_CARD:
        return "ExcemptionCard";
        break;
    case PASSING_CARD:
        return "PassingCard";
        break;
    default:
        return "";
        break;
    }    
}
//////////////////////////////////////////////
// END OF STUDENT'S ANSWER
//////////////////////////////////////////////
