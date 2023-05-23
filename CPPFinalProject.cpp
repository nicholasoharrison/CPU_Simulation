
//DataTypes:
//I wrote my DataTypes class to be the parent class of IntegerType, DoubleType, and StringType.
//Each of these classes has its own differences, similar to int, double, and std::string.
//(Float I considered the same as double, likewise char the same as std::string)
//Within the CPU class, the accumulator variable is a pointer of the DataType class to allow for
//polymorphism with the functions in the CPU class. The "nick" namespace helps to do all the
//operations on the DataType objects by sending those DataTypes to the CPU class to do
//simple operations.
//
//CPU:
//The CPU class does simple operations on DataTypes that are stored in it. Those DataTypes
//are stored in a vector called "memory" holding DataType pointers to allow for polymorphism
//and modification of the values. Each time a DataType or subclass of that is created, that
//DataType is stored in the memory vector, similar to a stack. When operations are needed to
//be done to DataTypes, the CPU class will use its "find" functions to find where the DataType
//is stored in the vector, then gathers the information of the DataType and does whatever
//operation is needed to it, and the outcome is then stored in the accumulator for any of those
//operations.
//
//"nick" namespace:
//The nick namespace provides the syntax of all the operations called in main, which are then
//sent to the CPU class to access the DataTypes in memory. The operations in the CPU class are
//stored in the accumulator, so the operations in the namespace return the accumulator value
//to be used in main.




//Instructions/Syntax of the "new/created" simple programming language---------------------------------------

//nick::printThis(...); -> prints whatever is in ()
//
//DataType a = [value]; -> creates a new DataType object in memory (works for all subclasses as well)
//DataType a([value]); -> identical to DataType a = [value];
//
//nick::newIntegerType();   |
//nick::newStringType();    |   -> prompt the user for a number or string (similar to cin)
//nick::newDoubleType();    |   AND returns that value to be added to a DataType/CPU function
//
//nick::add(a,b); -> adds numeric DataType objects
//nick::subtract(a,b); -> subtracts numeric DataType objects
//nick::multiply(a,b); -> multiplies numeric DataType objects
//nick::divide(a,b); -> divides numeric DataType objects
//nick::concat(a,b); -> concatenates two string DataType objects
//nick::compare(a,b); -> returns a value when comparing a and b (if 1 a>b, if -1 negative b>a, if 0 a=b)

//End of instructions-------------------------------------------------------------------------------------------



#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cstdlib>




//-------------------------------------------------DataType Class-----------------------------------------------
class DataType
{
	public:
		double value; // for the numeric classes
		std::string str; // for the character classes;
		virtual std::string get() {return str;}
		virtual double getNum() {return value;}
		virtual ~DataType(){}
		virtual void operator=(double d){value =d;}
		virtual void operator=(std::string s){str = s;}
		virtual void operator=(int i){ value  = i;}
		virtual void operator=(DataType d)
		{
			value = d.getNum();
			str = d.get();
		}
		//virtual int getMemLoc(){return memoryLocation;}
		
};
//--------------------------------------------------- END DataType Class-----------------------------------------------












//-----------------------------------------------------CPU Class-----------------------------------------------
class CPU
{
    private:
        DataType *accumulator = new DataType(); // should hold results to arithmetic and logical operations
        std::vector<DataType*> memory; // the memory of CPU
        int programCounter;
        std::map<std::string, bool> flags = {{"equals", false},{"less_than", false},{"greater_than", false}};
        
    public:
        CPU()
        {
            programCounter = 0;
            
        }
        ~CPU()
		{
        	delete accumulator;
    	}
        
        void load(int address)
        {
        	accumulator = memory[address];
		}
        
        
        void store(DataType &d)
        {
            memory.push_back(&d);
            programCounter++;
        }
        
        
        void add(int val1, int val2)
        {
            double w = (std::stod(memory[val1]->get()) + std::stod(memory[val2]->get()));
            *accumulator = w; 
            
        }
        
        void subtract(int val1, int val2)
        {
            double w;
			
            w = (std::stod(memory[val1]->get()) - std::stod(memory[val2]->get()));
            *accumulator = w;
            
        }
        
        void multiply(int val1, int val2)
        {
            double w;
			
            w = (std::stod(memory[val1]->get()) * std::stod(memory[val2]->get()));
            *accumulator = w;
            
        }
        
        void divide(int val1, int val2)
        {
            double w;
			
            w = (std::stod(memory[val1]->get()) / std::stod(memory[val2]->get()));
            *accumulator = w;
            
        }
        
        void concatenate(int val1, int val2)
        {
			std::string w = memory[val1]->get() + " " + memory[val2]->get();
        	*accumulator = w;
		}
        
        int findNum(DataType* d)
        {
			for(int i = 0; i<memory.size(); i++)
        	{
        		if(memory.at(i)->getNum() == d->getNum())
        		{
					return i;
				}
			}
			
			return memory.size()-1;
		}
		
		int findStr(DataType* d)
        {
        	
			for(int i = 0; i<memory.size(); i++)
        	{
        		if(memory.at(i)->get() == d->get())
        		{
					return i;
				}
			}
			
			return memory.size()-1;
		}
		
        
        void jump(int address)
        {
            programCounter = address;
        }
        
        
        void compare(int val1, int val2)
        {
        	if(memory[val1]->getNum() - memory[val2]->getNum()>0)
        	{
        		*accumulator = 1;
			}
			else if(memory[val1]->getNum() - memory[val2]->getNum()<0)
			{
				*accumulator = -1;
			}
			else
			{
				*accumulator = 0; //if they are equal
			}
        	
		}
        
		void outputNum()
		{
			std::cout<<accumulator->getNum();
		}
		
		double getAccNum()
		{
			return accumulator->getNum();
		}
		
		std::string getAccStr()
		{
			return accumulator->get();
		}
		
		void outputStr()
		{
			std::cout<<accumulator->get();
		}
		
			
};
//---------------------------------------------END CPU Class------------------------------------------------------









//---------------------------------------------GLOBAL CPU object-----------------------------------------------------
CPU cpu;                                                       
//-------------------------------------------------------------------------------------------------------------------








//------------------------------------------------- namespace nick -------------------------------------------------
namespace nick
{
	template<class T>
	void printThis(T t)
	{
		std::cout<<t;
	}
	
	template<class T>
	double add(T t1, T t2)
	{
		cpu.add(cpu.findNum(&t1), cpu.findNum(&t2));
		return cpu.getAccNum();
	}
	
	template<class T>
	double subtract(T t1, T t2)
	{
		cpu.subtract(cpu.findNum(&t1), cpu.findNum(&t2));
		return cpu.getAccNum();
	}
	
	template<class T>
	double multiply(T t1, T t2)
	{
		cpu.multiply(cpu.findNum(&t1), cpu.findNum(&t2));
		return cpu.getAccNum();
	}
	
	template<class T>
	double divide(T t1, T t2)
	{
		cpu.divide(cpu.findNum(&t1), cpu.findNum(&t2));
		return cpu.getAccNum();
	}
	
	template<class T>
	std::string concat(T t1, T t2)
	{
		cpu.concatenate(cpu.findStr(&t1), cpu.findStr(&t2));
		//std::cout<<cpu.findStr(&t2);
		return cpu.getAccStr();
	}
	
	int newIntegerType()
	{
		int temp;
		std::cout<<"\nEnter an integer: ";
		std::cin>>temp;
		return temp;	
	}
	
	double newDoubleType()
	{
		double temp;
		std::cout<<"\nEnter a double: ";
		std::cin>>temp;
		return temp;	
	}
	
	std::string newStringType()
	{
		std::string trash;
		std::getline(std::cin, trash);
		std::string temp;
		std::cout<<"\nEnter a string: ";
		std::getline(std::cin, temp);
		return temp;	
	}
	
	
	template<class T>
	int compare(T t1, T t2)
	{
		cpu.compare(cpu.findNum(&t1), cpu.findNum(&t2));
		return cpu.getAccNum();
	}
	
	
	
}
//---------------------------------------------------END namespace nick-------------------------------------------------










//----------------------------------------------------IntegerType Class------------------------------------------------
class IntegerType : public DataType
{
	friend std::istream& operator>>(std::istream& i, IntegerType &inte);
	private:
		int value;
	public:
		IntegerType(int num = 0)
		{
			value = num;
			cpu.store(*this); 
		}
		std::string get() {return std::to_string(value);}
		double getNum() {return value;}
		void operator=(double num)
		{
			value = num;
		}
		void operator=(int i)
		{
			value = i;
		}
		int operator+(IntegerType i)
		{
			return this->value + i.value;
		}
};
//------------------------------------------------END IntegerType Class------------------------------------------------









//--------------------------------------------DoubleType Class--------------------------------------------------------
class DoubleType : public DataType
{
	friend std::istream& operator>>(std::istream& i, DoubleType &doub);
	private:
		double value;
	public:
		DoubleType(double num = 0)
		{
			value = num;
			cpu.store(*this);
		}
		std::string get() {return std::to_string(value);}
		double getNum() {return value;}
		void operator=(double num)
		{
			value = num;
		}
		void operator=(int num)
		{
			value = num;
		}
		double operator+(DoubleType d)
		{
			return this->value + d.value;
		}
		
};
//---------------------------------------------END DoubleType Class-------------------------------------------------------








//------------------------------------------------StringType Class-------------------------------------------------------
class StringType : public DataType
{
	friend std::istream& operator>>(std::istream& i, StringType &str);
	private:
		std::string value;
	public:
		StringType(std::string str = "") 
		{
			value = str;
			cpu.store(*this);
		}
		virtual std::string get() {return value;}
		void operator=(std::string str)
		{
			value = str;
		}
		void operator=(char chr)
		{
			value = std::to_string(chr);
		}
		std::string operator+(StringType s)
		{
			return (this->value + s.value);
		}
};
//-----------------------------------------END StringType Class-------------------------------------------------------







//------------------------------------------istream operators---------------------------------------------------------
	std::istream& operator>>(std::istream& i, StringType &str)
	{
		i>>str.value;
		return i;
	}
	std::istream& operator>>(std::istream& i, IntegerType &inte)
	{
		i>>inte.value;
		return i;
	}
	std::istream& operator>>(std::istream& i, DoubleType &doub)
	{
		i>>doub.value;
		return i;
	}
//---------------------------------------END istream operators---------------------------------------------------------







//---------------------------------------------  MAIN  ----------------------------------------------------------------
int main()
{
	
	DoubleType a = nick::newDoubleType();
	//a = 10;
	DoubleType b(4);
	nick::printThis(a.get());
	nick::printThis(" ");
	nick::printThis(b.get());
	nick::printThis(" ");
	
	
	
	nick::printThis("\nAdd:");	
	nick::printThis(nick::add(a,b));
	nick::printThis("   Subtract:");
	nick::printThis(nick::subtract(a,b));
	nick::printThis("   Multiply:");
	nick::printThis(nick::multiply(a,b));
	nick::printThis("   Divide:");
	nick::printThis(nick::divide(a,b));
	nick::printThis("   Divide backwards:");
	DoubleType c = (nick::divide(b,a));
	nick::printThis(c.get());

	StringType cpp(nick::newStringType());
	StringType fun("is fun");
	
	nick::printThis("\n");
	nick::printThis(nick::concat(cpp, fun));
	
	nick::printThis("\n\na=");
	nick::printThis(a.get());
	nick::printThis(" b=");
	nick::printThis(b.get());
	nick::printThis("\na is greater than b: ");
	if(nick::compare(a,b) == 1)
	{
		nick::printThis("TRUE");
	}
	else if(nick::compare(a,b) == -1)
	{
		nick::printThis("FALSE");
	}
	else
	{
		nick::printThis("EQUAL");
	}
	
	
	
	
	
	

	

	
	
	
	return 0;
}
//--------------------------------------------- END MAIN  -------------------------------------------------------




