/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class Position;
class Configuration;
class Map;

class Criminal;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
// class MagicBook;
// class EnergyDrink;
// class FirstAid;
// class ExcemptionCard;
// class PassingCard;

class BaseBag;
class SherlockBag;
class WatsonBag;

class TestStudyInPink;

enum ItemType { MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD };
enum ElementType { PATH, WALL, FAKE_WALL };
enum RobotType { C, S, W, SW };




class MapElement {
public:
    friend class TestStudyInPink;
protected:
    ElementType type;
public:
    MapElement(ElementType in_type) : type(in_type){};
    virtual ~MapElement(){};
    virtual ElementType getType() const
    {
       return type;
    };

};//done

class Path : public MapElement {

public:
    friend class TestStudyInPink;
    Path() : MapElement(PATH){};
};//done

class Wall : public MapElement {

public:
    friend class TestStudyInPink;
    Wall() : MapElement(WALL){};
};//done

class FakeWall : public MapElement {
public:
    friend class TestStudyInPink;
private:
    int ReqExp;
public:
    FakeWall(int r, int c) : MapElement(FAKE_WALL)
    {
       ReqExp = (r * 257 + c * 139 + 89) % 900 + 1;//////!!!!!!!!!!!
    };
    int getReqExp() const
    {
        return ReqExp;
    };
};//done

class Map {
public:
    friend class TestStudyInPink;
private:
    int num_rows, num_cols; 
    MapElement***  map;   //000000________0000000
public:
    Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls);
    ~Map()
    {
        //cout << "Map destructor called." << endl;
        for (int i = 0; i < num_rows; ++i) {
            for (int j = 0; j < num_cols; ++j) {
                    if (map[i][j] != nullptr) {
                        delete map[i][j];  // Only delete if it's not null
                        map[i][j] = nullptr;  // Set to null after deleting
                }
            }
            delete[] map[i];
        }
        //delete[] map;
    };
    bool isValid (const Position & pos , MovingObject * mv_obj) const ;
};// almost done

class Position {
public:
    friend class TestStudyInPink;
private:
    int r, c;
public:
    static const Position npos;
    Position(int r = 0, int c = 0) : r(r) , c(c) {};
    //const Position Position::npos(-1, -1);

    Position(const string & str_pos)
    {
        stringstream ss(str_pos);
        char removal; 
        ss >> removal >> r >> removal >> c >> removal;
    };
    int getRow() const
    {
        return r;
    };
    int getCol() const
    {
        return c;
    };
    void setRow(int r)
    {
        this->r = r;
    };
    void setCol(int c)
    {
        this->c = c;
    };

    string str() const
    {
        return "(" + to_string(r) + "," + to_string(c) + ")";
    };

    bool isEqual(int in_r, int in_c) const
    {
        return ((in_r == r) && (in_c == c));
    };
    bool isEqual(Position compare) const
    {
        return ((compare.getCol() == c) && (compare.getRow() == r));
    };
};//done  
//const Position Position::npos = Position(-1, -1);
inline int manhattanDistance(Position point1, Position point2) {
    return abs(point1.getRow() - point2.getRow()) + abs(point1.getCol() - point2.getCol());
}

class MovingObject {
public:
    friend class TestStudyInPink;
protected:
    int index;
    Position pos;
    Map * map;
    string name;
public:
    MovingObject(int index, const Position pos, Map * map, const string & name="") : index(index), pos(pos), map(map), name(name) {};
    ~MovingObject() = default;
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const
    {
        return pos;
    }
    virtual void move() = 0;
    virtual string str() const = 0;

    string getName() const
    {
        return name;
    }
};//
class Character : public MovingObject
{
public:
    friend class TestStudyInPink;
private:
    int Exp, Hp;
    void LimHp(int hp)
    {
        if(Hp < 0)
            Hp = 0;
        if(Hp > 500)
            Hp = 500;
    };
    void LimExp(int exp)
    {
        if(Exp < 0)
            Exp = 0;
        if(Exp > 900)
            Exp = 900; 
    };
public:
    Character(int index, const Position& init_pos, Map* map, const string& name, int Hp, int Exp) : MovingObject(index, init_pos, map, name), Exp(Exp), Hp(Hp)
    {
        LimHp(Hp);
        LimExp(Exp);
    };
    ~Character(){};
    virtual int getHP() const
    {
        return Hp;
    };
    virtual int getEXP() const
    {
        return Exp;
    };
    void setHP(int Hp_new)
    {
        LimHp(Hp_new);
        this->Hp = Hp_new;
    }
    void setEXP(int Exp_new)
    {
        LimHp(Exp_new);
        this->Exp = Exp_new;
    }

};



class Sherlock : public Character {
public:
    friend class TestStudyInPink;
private:
    string moving_rule;
    int rule_length = 0;
public:
    static int CountCallNextPos;
    Sherlock(int index, const string& moving_rule, const Position& init_pos, Map* map, int init_hp, int init_exp) : Character(index, init_pos, map, "Sherlock", init_hp, init_exp), moving_rule(moving_rule)
    {
        while (moving_rule[rule_length] != '\0')
        {
            rule_length++;
        }
    }
    ~Sherlock(){};
    Position getNextPosition() override; 
    void move() override
    {
        Position next_pos = getNextPosition();
           
        if (map -> isValid(next_pos, this) && getHP() != 0 && getEXP() != 0)
            pos = next_pos;
    }
    void move_end(Position newpos)
    {
        pos = newpos;
    }

    string str() const override 
    {
        return name + "[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
    }

};//done

class Watson : public Character {
public:
    friend class TestStudyInPink;
private:
    string moving_rule;
    int rule_length = 0;
public:
    static int CountCallNextPos;
    Watson(int index, const string& moving_rule, const Position& init_pos, Map* map, int init_hp, int init_exp) : Character(index, init_pos, map, "Watson", init_hp, init_exp), moving_rule(moving_rule)
    {
        while (moving_rule[rule_length] != '\0')
        {
            rule_length++;
        }
    }
    Position getNextPosition() override;
    ~Watson(){};
    void move() override
    {
        Position next_pos = getNextPosition();
        if (map -> isValid(next_pos, this) && getHP() != 0 && getEXP() != 0)
            pos = next_pos;
    }
    void move_end(Position newpos)
    {
        pos = newpos;
    }
    string str() const override 
    {
        return name + "[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
    }
};

class Criminal : public Character {
public:
    friend class TestStudyInPink;
private:

    Position spos;
    Position wpos;
    Position prePos;
    Sherlock* sherlock;
    Watson* watson;
public:
    Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson) : Character(index, init_pos, map, "Criminal", 0, 0), spos(sherlock->getCurrentPosition()), wpos(watson->getCurrentPosition()), prePos(), watson(watson), sherlock(sherlock){};
    Position getNextPosition() override; // cpp
    ~Criminal(){};
    void move() override
    {
        Position next_pos = getNextPosition();
        if (map -> isValid(next_pos,this))
        {
            prePos = pos;
            pos = next_pos;
        }
    }
    Position getprePos() const
    {
        return prePos;
    }
    string str() const override 
    {
        return name + "[index=" + to_string(index) + ";pos=" + pos.str() + "]";
    }
};//
//
class ArrayMovingObject {
public:
    friend class TestStudyInPink;
private:
    MovingObject** arr_mv_objs;
    int count = 0;
    int capacity;
public:
    ArrayMovingObject(int capacity) : count(0), capacity(capacity)
    {
        arr_mv_objs = new MovingObject*[capacity];
    };
    
    ~ArrayMovingObject() 
    {
        // for (int i = 0; i < capacity; i++)
        //     delete [] arr_mv_objs[i];
        delete[] arr_mv_objs;
    };
    bool isFull() const
    {
        return (count == capacity);
    };
    bool add(MovingObject * mv_obj)
    {
        if (isFull())
            return false;
        arr_mv_objs[count++] = mv_obj;
        return true;
    };

    MovingObject * get(int index) const
    {
        if (index >= 0 && index < count)
            return arr_mv_objs[index];
        else 
            return nullptr; 
    };
    int size() const
    {
        return count;
    };
    string str() const
    {
        stringstream ss;
        ss << "ArrayMovingObject[count=" << count << ";capacity=" << capacity << ";";
        for (int i = 0; i < count; i++) {
            ss << arr_mv_objs[i]->str(); 
            if (i != count - 1)
                ss << ";";
        }
        ss << "]";
        return ss.str();
    };
};

class Configuration {
public:
    friend class TestStudyInPink;
    friend class StudyPinkProgram;

private:
    int map_num_rows, map_num_cols;
    int max_num_moving_objects;
    int num_walls;
    Position* arr_walls;//
    int num_fake_walls;
    Position* arr_fake_walls;//
    ////////
    string sherlock_moving_rule;
    Position sherlock_init_pos;//
    int sherlock_init_hp;
    int sherlock_init_exp;
    ///////
    string watson_moving_rule;
    Position watson_init_pos;//
    int watson_init_hp;
    int watson_init_exp;
    ///////
    Position criminal_init_pos;
    int num_steps;

public:
    Configuration(const string & filepath);
    ~Configuration(){};
    string str() const;

};


class Robot : public MovingObject
{
public:
    friend class TestStudyInPink;
protected:
    Criminal* criminal;
    RobotType robot_type;
    
public:
    BaseItem* init_item;    
    Robot(int index, Position init, Map * map, RobotType rb_type, string name, Criminal* criminal): MovingObject(index, init, map, name), criminal(criminal), robot_type(rb_type)/////////////
    {
        if (robot_type == C)
            name = "RobotC";
        else if (robot_type == S)
            name = "RobotS";
        else if (robot_type == W)
            name = "RobotW";
        else if (robot_type == SW)
            name = "RobotSW";
    };
    virtual ~Robot(){};
    void move() override
    {
        Position next_pos = getNextPosition();
        if (!next_pos.isEqual(Position::npos))
            pos = next_pos;
    };
    
    int getDistance(){return 0;};
    BaseItem* takeItem()// hiện thực như là item bị mất khỏi robot
    {
        return init_item;
    }
};





class RobotC : public Robot{
public:
    friend class TestStudyInPink;
   
public:
    RobotC(int index, Position init,Map * map, Criminal* criminal): Robot(index, init, map, C, "RobotC",criminal){};//////////
    Position getNextPosition() override
    {
        return criminal->getprePos();
    }; 
    string str() const override
    {
        return "Robot[pos=" + pos.str() + ";type=C;dist=]";
    };
    int getDistance(Sherlock* sherlock)
    {
        return manhattanDistance(pos, sherlock->getCurrentPosition());
    }
    int getDistance(Watson* watson)
    {
        return manhattanDistance(pos, watson->getCurrentPosition());   
    }
};///// sửa lại constructor

class RobotS : public Robot{
public:
    friend class TestStudyInPink;
private:
    Sherlock* sherlock;
public:
    RobotS(int index, Position init, Map * map, Criminal* criminal , Sherlock* sherlock): Robot(index, init,map, S, "RobotS",criminal), sherlock(sherlock){};
    Position getNextPosition() override; 
    string str() const override
    {
        return "Robot[pos=" + pos.str() + ";type=S;dist=" + to_string(manhattanDistance(pos, sherlock->getCurrentPosition())) + "]";
    };
    int getDistance()
    {
        return manhattanDistance(pos, sherlock->getCurrentPosition());   
    }
};

class RobotW : public Robot{
public:
    friend class TestStudyInPink;
private:
    Watson* watson;
public:
    RobotW(int index, Position init,Map * map, Criminal* criminal , Watson* watson): Robot(index, init, map, W, "RobotW",criminal), watson(watson){};
    Position getNextPosition() override;
    string str() const override
    {
        return "Robot[pos=" + pos.str() + ";type=W;dist=" + to_string(manhattanDistance(pos, watson->getCurrentPosition())) + "]";
    }; 
    int getDistance()
    {
        return manhattanDistance(pos, watson->getCurrentPosition()); 
    }
};

class RobotSW : public Robot{
public:
    friend class TestStudyInPink;
private:
    Watson* watson;
    Sherlock* sherlock;
    
public:
    RobotSW(int index, Position init, Map * map, Criminal* criminal , Sherlock* sherlock , Watson* watson): Robot(index, init, map, SW, "RobotSW",criminal), sherlock(sherlock), watson(watson){};
    virtual Position getNextPosition() override; 
    
    virtual string str() const override
    {
        return "Robot[pos=" + pos.str() + ";type=SW;dist=" + to_string(manhattanDistance(pos, sherlock->getCurrentPosition()) + manhattanDistance(pos, watson->getCurrentPosition())) + "]";
    };
    int getDistance()
    {
        return manhattanDistance(pos, sherlock->getCurrentPosition()) + manhattanDistance(pos, watson->getCurrentPosition()); 
    }
};


////////



class BaseItem
{
public:
    friend class TestStudyInPink;
protected:
    ItemType item_type;
public:
    BaseItem(ItemType init_item): item_type(init_item){};
    virtual ~BaseItem(){};
    virtual bool canUse(Character* obj, Robot * robot) = 0;
    virtual void use(Character* obj, Robot * robot) = 0;
    virtual void setChallenge(const string& Challenge) {};
    ItemType getType()
    {
        return item_type;
    };
    string getName();
};


class MagicBook : public BaseItem
{
public:
    friend class TestStudyInPink;
private:
    
public:
    MagicBook() : BaseItem(MAGIC_BOOK){};
    ~MagicBook(){};
    bool canUse(Character* obj, Robot * robot) override
    {
        if (obj -> getName() == "Sherlock" ||obj -> getName() == "Watson")
            if (obj ->getEXP() <= 350)
                return 1;
        return 0;
    }
    void use(Character * obj, Robot* robot) override
    {
        if (canUse(obj, robot))
            obj -> setHP(obj->getEXP() * 0.25 + obj->getEXP());
    }
};


class EnergyDrink :public BaseItem
{
public:
    friend class TestStudyInPink;
private:
    /* data */
public:
    EnergyDrink() : BaseItem(ENERGY_DRINK){};
    ~EnergyDrink(){};
    bool canUse(Character* obj, Robot * robot) override
    {
        if (obj -> getName() == "Sherlock" ||obj -> getName() == "Watson")
            if (obj ->getHP() <= 100)
                return 1;
        return 0;
    }
    void use(Character * obj, Robot* robot) override
    {
        if (canUse(obj, robot))
            obj -> setHP(obj->getHP() * 0.2 + obj->getHP());
    }
};


class FirstAid : public BaseItem
{
public:
    friend class TestStudyInPink;
private:
    /* data */
public:
    FirstAid() : BaseItem(FIRST_AID){};
    ~FirstAid(){};
    bool canUse(Character* obj, Robot * robot) override
    {
        if (obj -> getName() == "Sherlock" ||obj -> getName() == "Watson")
            if (obj -> getHP() <= 100 || obj -> getEXP() <= 350)
                return 1;
        return 0;
    }
    void use(Character * obj, Robot* robot) override
    {
        if (canUse(obj, robot))
            obj -> setHP(obj->getHP() * 0.5 + obj->getHP());
    }
};

class ExcemptionCard : public BaseItem
{
public:
    friend class TestStudyInPink;
private:
    /* data */
public:
    ExcemptionCard() : BaseItem(EXCEMPTION_CARD){};
    ~ExcemptionCard(){};
    bool canUse(Character* obj, Robot * robot) override
    {
        if (obj -> getName() == "Sherlock")
            if (obj -> getHP() % 2 == 1)
                return 1;
        return 0;
    }
    void use(Character * obj, Robot* robot) override
    {
        if (canUse(obj, robot))
        {

        }
    }
};

class PassingCard : public BaseItem
{
public:
    friend class TestStudyInPink;
private:
    string Challenge;
public:
    PassingCard() : BaseItem(PASSING_CARD){};
    ~PassingCard(){};
    void setChallenge(const string& Challenge)override
    {
        this->Challenge= Challenge;
    }
    string getChallenge()
    {
        return Challenge;
    }
    bool canUse(Character* obj, Robot * robot) override
    {
        if (obj -> getName() == "Watson")
            if (obj -> getHP() % 2 == 0)
                return 1;
        return 0;
    }
    void use(Character * obj, Robot* robot) override
    {
        if (canUse(obj, robot))
        {
        }
    }
};



class BaseBag
{
public:
    friend class TestStudyInPink;
protected:
    Character* obj;
    int count;
    int maxitem;
    class Node
    {
    private:
        BaseItem* initem;
        Node* next;
        friend class BaseBag;
    public:
        Node(BaseItem* item, Node * node = nullptr): initem(item), next(node){};
    };
    Node* head;
public:
    BaseBag(Character* obj, int itemQuantity):obj(obj), count(0), maxitem(itemQuantity),head(nullptr){};
    ~BaseBag()
    {
        while (head) 
        {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    };
    virtual bool insert (BaseItem* item)
    {
        if (count >=  maxitem)
            return 0;
        
        Node* curr = head;
        head = new Node(item, curr);
       
        count++;
        return 1;
    };
    virtual BaseItem* get()
    {// get chỉ lấy các recovery item
        if (!head) 
            return nullptr;
        Node* curr = head;
        Node* temp = nullptr;
        while (!curr->initem->canUse(obj, nullptr) && curr && curr->initem->getType() == EXCEMPTION_CARD && curr->initem->getType() == PASSING_CARD)
        {
            temp = curr;
            curr = curr->next;
        }
        if (!curr)
            return nullptr;
        if (curr == head)
        {
            head = head->next;
            count--;
            return curr->initem;
        }
        temp->next = curr->next;
        count--;
        return curr->initem;
         
    }; 
    virtual BaseItem* get(ItemType itemType)
    {// chỉ lấy excemption và passing card
        if (!head) 
            return nullptr;
        Node* prev = nullptr;
        Node* curr = head;

        while (curr) {
        if (curr->initem->getType() == itemType) 
        {    
            if (!prev) 
            {
                head = curr->next;
            } else {
                prev->next = curr->next;
            }
            count--;
            return curr->initem;
        }
        prev = curr;
        curr = curr->next;
        }
        
        return nullptr;
    }; 
    
    
    virtual string str() const
    {
        ostringstream oss;
        oss << "Bag[count=" << count << ";";
        Node* temp = head;
        while (temp != NULL) 
        {
            oss << temp->initem->getName();
            if (temp->next != NULL)
                oss << ",";
            temp = temp->next;
        }
        oss << "]";
        return oss.str();
    };
};

class SherlockBag : public BaseBag
{
public:
    friend class TestStudyInPink;
private:

public:
    SherlockBag(Character* obj): BaseBag(obj, 13){};
    ~SherlockBag(){};
};

class WatsonBag : public BaseBag
{
public:
    friend class TestStudyInPink;
private:
    
public:
    WatsonBag(Character* obj): BaseBag(obj, 15){};
    ~WatsonBag(){};
};






class StudyPinkProgram {
public:
    friend class TestStudyInPink;
private:

    Configuration * config;
    
    Sherlock * sherlock;
    Watson * watson;
    Criminal * criminal;
    
    Map * map;
    ArrayMovingObject * arr_mv_objs;


public:
    StudyPinkProgram(const string & config_file_path) 
    {
        config = new Configuration(config_file_path);
        map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
        arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
        sherlock = new Sherlock(1,config->sherlock_moving_rule, config->sherlock_init_pos,map, config->sherlock_init_hp, config->sherlock_init_exp);
        watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
        criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);
        arr_mv_objs->add(criminal);
        arr_mv_objs->add(sherlock);
        arr_mv_objs->add(watson);
    };
    //~StudyPinkProgram(){};
    void setRobotItem(Position inputpos, Robot* inrobot)
    {
        int i = inputpos.getRow();
        int j = inputpos.getCol();
        int p = i * j;
        int s = 0;
        while (p > 9)
        {
            while (p != 0)
            {
                s += p%10;
                s /= 10;
            }
            p = s;
            s = 0;
        }
        s = p;
        if (s == 0 || s == 1)
            inrobot->init_item = new MagicBook();

        if (s == 2 || s == 3)
            inrobot->init_item = new EnergyDrink();

        if (s == 4 || s == 5)
            inrobot->init_item = new FirstAid();
        
        if (s == 6 || s == 7)
            inrobot->init_item = new ExcemptionCard();
        if (s == 8 || s == 9)
        {
            inrobot->init_item = new PassingCard();
            int t = (i * 11 + j)%4;
            switch (t)
            {
            case 0:
                inrobot->init_item->setChallenge("RobotS");
                break;
            case 1:
                inrobot->init_item->setChallenge("RobotC");
                break;
            case 2:
                inrobot->init_item->setChallenge("RobotSW");
                break;
            case 3:
                inrobot->init_item->setChallenge("All");
                break;
            default:
                break;
            }
        }
    }

    bool isStop() const
    {
        if (sherlock->getHP() == 0)
            return 1;
        if (watson->getHP() == 0)
           return 1;
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
            return 1;
        if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
            return 1;
        return 0;
    };

    void printResult() const {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
            cout << "Sherlock caught the criminal" << endl;
        }
        else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
            cout << "Watson caught the criminal" << endl;
        }
        else {
            cout << "The criminal escaped" << endl;
        }
    }

    void printStep(int si) const {
        cout << "Step: " << setw(4) << setfill('0') << si
            << "--"
            << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }

    void run(bool verbose) {
        SherlockBag* sherbag = new SherlockBag(sherlock);
        WatsonBag* watbag = new WatsonBag(watson);
        for (int istep = 0; istep < config->num_steps; ++istep) {
            for (int i = 0; i < arr_mv_objs->size(); ++i) {
                arr_mv_objs->get(i)->move();
                if (sherlock -> getCurrentPosition().isEqual(arr_mv_objs->get(i)->getCurrentPosition()))
                {
                    
                    string itsname = arr_mv_objs->get(i)->getName();
                    if (itsname == "RobotS")
                    {
                        //find excemption card to protect
                        BaseItem * usecard = sherbag->get(EXCEMPTION_CARD);
                        if (sherlock->getEXP() > 400)
                        {
                            Robot* temp = dynamic_cast<Robot*>(arr_mv_objs->get(i));
                            sherbag->insert(temp->takeItem());
                        }
                        else if(!usecard)
                        {
                            sherlock->setEXP(sherlock->getEXP() * 0.9);
                        }
                        delete usecard;
                        BaseItem* recoveritem = sherbag->get();
                        if (recoveritem)
                        {
                            recoveritem->use(sherlock, NULL);
                            delete recoveritem;
                        }
                    }
                    else if (itsname == "RobotSW")
                    {
                        BaseItem * usecard = sherbag->get(EXCEMPTION_CARD);
                        //find excemption card to protect
                        if (sherlock->getEXP() > 300 && sherlock->getHP() > 335)
                        {
                            Robot* temp = dynamic_cast<Robot*>(arr_mv_objs->get(i));
                            sherbag->insert(temp->takeItem());
                        }
                        else if(!usecard)
                        {
                            sherlock->setEXP(sherlock->getEXP() * 0.85);
                            sherlock->setHP(sherlock->getHP() * 0.85);
                        }
                        delete usecard;
                        BaseItem* recoveritem = sherbag->get();
                        if (recoveritem)
                        {
                            recoveritem->use(sherlock, NULL);
                            recoveritem = NULL;/// delete item bị sai
                        }
                    }
                    else if (itsname == "RobotC")
                    {
                        BaseItem * usecard = sherbag->get(EXCEMPTION_CARD);
                        // find excemption card to protect
                        if (sherlock->getEXP() > 500)
                            sherlock->move_end(criminal->getCurrentPosition());
                        else if(!usecard)
                        {
                            Robot* temp = dynamic_cast<Robot*>(arr_mv_objs->get(i));
                            sherbag->insert(temp->takeItem());
                        }
                        delete usecard;
                        BaseItem* recoveritem = sherbag->get();
                        if (recoveritem)
                        {
                            recoveritem->use(sherlock, NULL);
                            delete recoveritem;
                        }
                    }
                    else if (itsname == "Watson")
                    {
                        while(true)
                        {
                            BaseItem* usecard = sherbag->get(PASSING_CARD);
                            if(usecard)
                                watbag->insert(usecard);
                            else
                                break;
                        }
                        while(true)
                        {
                            BaseItem* usecard = watbag->get(EXCEMPTION_CARD);
                            if(usecard)
                                sherbag->insert(usecard);
                            else
                                break;
                        }
                    }
                }
                if (watson -> getCurrentPosition().isEqual(arr_mv_objs->get(i)->getCurrentPosition()))
                {
                    string itsname = arr_mv_objs->get(i)->getName();
                    if (itsname == "RobotW")
                    {
                        BaseItem ** usecard;
                        usecard[0] = watbag->get(PASSING_CARD);
                        int counting = 0;
                        while (usecard[counting])
                        {
                            if (dynamic_cast<PassingCard*>(usecard[counting])->getChallenge() == "RobotW")
                                break;
                            usecard[++counting] = watbag->get(PASSING_CARD);
                        }
                        for (int i = 0; i <= counting - 1; i++)
                        {
                            watbag->insert(usecard[i]);
                        }
                     
                        // find passing card
                        if (!usecard[counting] && watson->getHP() < 350)
                        {
                            watson->setHP(watson->getHP()*0.95);
                        }// failure
                        else 
                        {
                            Robot* temp = dynamic_cast<Robot*>(arr_mv_objs->get(i));
                            watbag->insert(temp->takeItem());
                            if (usecard[counting])
                                delete usecard[counting];
                        }
                        

                        BaseItem* recoveritem = watbag->get();
                        if (recoveritem)
                        {
                            recoveritem->use(watson, NULL);
                            delete recoveritem;
                        }
                    }
                    if (itsname == "RobotSW")
                    {
                        BaseItem ** usecard;
                        usecard[0] = watbag->get(PASSING_CARD);
                        int counting = 0;
                        while (usecard[i])
                        {
                            if (dynamic_cast<PassingCard*>(usecard[counting])->getChallenge() == "RobotSW")
                                break;
                            
                            usecard[++counting] = watbag->get(PASSING_CARD);
                        }
                        for (int i = 0; i <= counting - 1; i++)
                        {
                            watbag->insert(usecard[i]);
                        }

                        //find passing card
                        if (!usecard[counting] && (watson->getEXP() < 600 || watson->getHP() < 165))
                        {
                            watson->setHP(watson->getHP() * 0.85);
                            watson->setEXP(watson->getEXP() * 0.85);
                        }
                        else 
                        {
                            Robot* temp = dynamic_cast<Robot*>(arr_mv_objs->get(i));
                            watbag->insert(temp->takeItem());
                            if (usecard[counting])
                                delete usecard[counting];
                            
                        }
                
                        // find recovery item
                        BaseItem* recoveritem = watbag->get();
                        if (recoveritem)
                        {
                            recoveritem->use(watson, NULL);
                            delete recoveritem;
                        }
                    }
                    if (itsname == "RobotC")
                    {
                        BaseItem ** usecard;
                        usecard[0] = watbag->get(PASSING_CARD);
                        int counting = 0;
                        while (usecard[i])
                        {
                            if (dynamic_cast<PassingCard*>(usecard[counting])->getChallenge() == "RobotC")
                                break;
                            
                            usecard[++counting] = watbag->get(PASSING_CARD);
                        }
                        for (int i = 0; i <= counting - 1; i++)
                        {
                            watbag->insert(usecard[i]);
                        }/// bị sai như trên
                        // find passing card

                        //get robot item
                        if (!usecard[counting])
                        {
                            Robot* temp = dynamic_cast<Robot*>(arr_mv_objs->get(i));
                            watbag->insert(temp->takeItem());
                            temp = NULL;
                            delete temp;
                        }
                        else
                        {  
                            watson->move_end(criminal->getCurrentPosition());
                        }
                        // find recovery item
                        BaseItem* recoveritem = watbag->get();
                        if (recoveritem != NULL)
                        {
                            recoveritem->use(watson, NULL);
                            delete recoveritem;
                        }
                    } 
                }
                if (isStop()) {
                    printStep(istep);
                    break;
                }
                if (verbose) {
                    printStep(istep);
                }
            }
            
            
            if (!arr_mv_objs->isFull())
            {
                if (istep == 2)
                {
                    int counting = arr_mv_objs->size() - 1;
                    arr_mv_objs->add(new RobotC(counting,criminal->getprePos(),map,criminal));
                    setRobotItem(criminal->getprePos(), dynamic_cast<Robot*>(arr_mv_objs->get(counting)));
                }
                if (istep % 3 == 2)
                {
                    int sherdist = manhattanDistance(criminal->getCurrentPosition(), sherlock->getCurrentPosition());
                    int watdist = manhattanDistance(criminal->getCurrentPosition(), watson->getCurrentPosition());
                    if (sherdist < watdist)
                    {
                        int counting = arr_mv_objs->size() - 1;
                        arr_mv_objs->add(new RobotS(counting,criminal->getprePos(),map,criminal,sherlock));
                        setRobotItem(criminal->getprePos(), dynamic_cast<Robot*>(arr_mv_objs->get(counting)));
                    }
                    if (sherdist > watdist)
                    {    
                        int counting = arr_mv_objs->size() - 1;
                        arr_mv_objs->add(new RobotW(counting,criminal->getprePos(),map,criminal,watson));
                        setRobotItem(criminal->getprePos(), dynamic_cast<Robot*>(arr_mv_objs->get(counting)));
                    }
                    if (sherdist == watdist)
                    {
                        int counting = arr_mv_objs->size() - 1;
                        arr_mv_objs->add(new RobotSW(counting,criminal->getprePos(),map,criminal,sherlock, watson));
                        setRobotItem(criminal->getprePos(), dynamic_cast<Robot*>(arr_mv_objs->get(counting)));
                    }
                }
            }
            
        }

        printResult();

    }

    ~StudyPinkProgram(){};
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
