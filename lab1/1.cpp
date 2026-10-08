#include <random>
#include <iostream>
#include <fstream>

using namespace std;

class Interface
{
public:
    virtual unsigned roll() = 0;
    virtual ~Interface() = default;
};

class Dice : public Interface
{
public:
    Dice(unsigned max, unsigned seed) : max(max), dstr(1, max), reng(seed) {}

    unsigned roll() override
    {
        return dstr(reng);
    }

private:
    unsigned max;
    std::uniform_int_distribution<unsigned> dstr;
    std::default_random_engine reng;
};

class ThreeDicePool : public Interface
{
public:
    explicit ThreeDicePool(Interface &d) : d(d) {};

    unsigned roll() override
    {
        unsigned a = d.roll();
        unsigned b = d.roll();
        unsigned c = d.roll();
        return a+b+c;
    }

private:
    Interface &d;
};

double expected_value(Interface &d, unsigned number_of_rolls = 1)
{
    auto accum = 0llu;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
        accum += d.roll();
    return static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}

void part1(){
    Dice d1 = Dice(10, 6767); //5.5
    Dice d2 = Dice(20, 3478843); //10.5
    Dice d3 = Dice(30, 696969); 
    ThreeDicePool triple = ThreeDicePool (d3); // 3*15.5 = 46.5
    Dice d4 = Dice(30, 6969);
    ThreeDicePool triple_2 = ThreeDicePool (d4);
    ThreeDicePool nine = ThreeDicePool(static_cast<Interface&>(triple_2)); // 9*15.5 = 139.5
    unsigned long N = 1000000;
    cout<<expected_value(d1, N)<<", "<<expected_value(d2, N)<<", "<<expected_value(triple, N)<<", "<<expected_value(nine, N)<<", "<<endl;
}

class PenaltyDice : virtual public Interface {
    private:
    Interface& d;
    public:
    explicit PenaltyDice (Interface& d): d(d) {};
    unsigned roll() override{
        unsigned a = d.roll();
        unsigned b = d.roll();
        return min(a,b);
    }
};

class BonusDice : virtual public Interface {
    private:
    Interface& d;
    public:
    explicit BonusDice (Interface& d): d(d) {};
    unsigned roll() override{
        unsigned a = d.roll();
        unsigned b = d.roll();
        return max(a,b);
    }
};

double value_probability(unsigned value, Interface &d, unsigned number_of_rolls = 1){
    auto count = 0llu;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt){
        auto x = d.roll();
        if (x==value){
            ++count;
        }
    }
    return static_cast<double>(count) / static_cast<double>(number_of_rolls);
}

void part2(){
    Dice d_simple = Dice(100, 4782748);
    Dice d_1_pen = Dice(100, 634827);
    Dice d_1_bon = Dice(100, 2353245);
    Dice d_1_triple = Dice(6, 634553);
    BonusDice bonus = BonusDice(d_1_bon);
    PenaltyDice pen = PenaltyDice(d_1_pen);
    ThreeDicePool triple = ThreeDicePool (d_1_triple);
    ofstream simple_2("raw/2_simple.txt");
    ofstream pen_2("raw/2_pen.txt");
    ofstream bonus_2("raw/2_bonus.txt");
    ofstream triple_2("raw/2_triple.txt");
    unsigned long N = 1000000;
    unsigned long M = 1000000;
        for (int i =0; i<100; ++i){
            simple_2<<value_probability(i+1, d_simple, N)<<endl;
        }
        for (int i =0; i<100; ++i){
            pen_2<<value_probability(i+1, pen, N)<<endl;
        }
        for (int i =0; i<100; ++i){
            bonus_2<<value_probability(i+1, bonus, N)<<endl;
        }
        for (int i =0; i<18; ++i){
            triple_2<<value_probability(i+1, triple, M)<<endl;
        }   
        simple_2.close();
        pen_2.close();
        bonus_2.close();
        triple_2.close();
}

class DoubleDice : public PenaltyDice, public BonusDice{
    public:
    explicit DoubleDice(Interface& d): PenaltyDice(d), BonusDice(d) {};
    unsigned roll() override{
        return PenaltyDice::roll()+BonusDice::roll();
    }
};

class DoubleDicePool : public Interface
{
public:
    explicit DoubleDicePool(Interface &d) : p(d), b(d) {};

    unsigned roll() override
    {
        return p.roll() + b.roll();
    }

private:
    PenaltyDice p;
    BonusDice b;
};

void part3(){
    Dice d = Dice(100, 742865);
    DoubleDice dd = DoubleDice(d);
    DoubleDicePool ddp = DoubleDicePool(d);
    cout<<expected_value(dd, 10000000)<<endl;
    cout<<expected_value(ddp, 10000000)<<endl;
    ofstream double_3("raw/3_double.txt");
    unsigned long N = 1000000;
    for (int i =0; i<200; ++i){
            double_3<<value_probability(i+1, dd, N)<<endl;
        }
    double_3.close();
    ofstream double_3p("raw/3_doublep.txt");
    for (int i =0; i<200; ++i){
            double_3p<<value_probability(i+1, ddp, N)<<endl;
        }
    double_3p.close();
}

int main(){
    part1();
    part2();
    part3();
    return 0;
}