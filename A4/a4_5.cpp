#include <bits/stdc++.h>
using namespace std;

int INS_COUNT;
int ROW_ACCESS_DELAY = 10;
int COL_ACCESS_DELAY = 2;
int CC = 0;
int ROW_BUFFER_UPDATE = 0;

int PREV = -1;
int CURR;
bool CHANGED = false; //defined in assignment 4

map<string, int> labels;

map<string, int> Register_map {
    	{"$zero",0},{"$1",1},{"$2",2},{"$3",3},{"$4",4},{"$5",5},{"$6",6},{"$7",7},{"$8",8}, {"$9",9},{"$10",10},{"$11",11},{"$12",12},{"$13",13},{"$14",14},{"$15",15},{"$16",16},{"$17",17},{"$18",18}, {"$19",19},{"$20",20},{"$21",21},{"$22",22},{"$23",23},{"$24",24},{"$25",25},{"$26",26},{"$27",27},{"$28",28}, {"$29",29}, {"$30",30},{"$31",31},{"$r0",0},{"$at",1},{"$v0",2},{"$v1",3},{"$a0",4},{"$a1",5},{"$a2",6},{"$a3",7},{"$t0",8}, {"$t1",9},{"$t2",10},{"$t3",11},{"$t4",12},{"$t5",13},{"$t6",14},{"$t7",15},{"$s0",16},{"$s1",17},{"$s2",18}, {"$s3",19},{"$s4",20},{"$s5",21},{"$s6",22},{"$s7",23},{"$t8",24},{"$t9",25},{"$k0",26},{"$k1",27},{"$gp",28}, {"$sp",29}, {"$s8",30},{"$ra",31}
    }; 

map<int, string> map_int_register {
    {0, "$zero"},{1, "$at"},{2, "$v0"},{3, "$v1"},{4,"$a0"},{5,"$a1",},{6,"$a2"},{7,"$a3"},{8,"$t0"}, {9,"$t1"},{10,"$t2"},{11,"$t3"},{12,"$t4"},{13,"$t5"},{14,"$t6"},{15,"$t7"},{16,"$s0",},{17,"$s1"},{18,"$s2"}, {19,"$s3"},{20,"$s4"},{21,"$s5"},{22,"$s6"},{23,"$s7"},{24,"$t8"},{25,"$t9"},{26,"$k0"},{27,"$k0"},{28,"$gp"}, {29,"$sp"}, {30,"$s8"},{31,"$ra"}
}; //defined in assignment 4

class ins{
    public:
    string sen;
    string op;
    vector<string> params;
    int l_addr;
};

typedef unsigned char byte;

inline std::string ltrim(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

inline std::string rtrim(std::string s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

inline std::string trim(std::string s) {
    return ltrim(rtrim(s));
}

string removeSpaces(string str)  
{ 
    str.erase(remove(str.begin(), str.end(), ' '), str.end()); 
    return str; 
}

int stringToInt(string str){
    char *end;
    long i = strtol( str.c_str(), &end, 10 );
    if (end == str.c_str())
    {
        cout << "Invalid integer entry\n";
        throw runtime_error("Invalid integer entry\n");
    }
    
    if ( *end == '\0' )
    {
        return (int) i;
    }
    else
    {
        cout << "Invalid integer entry\n";
        throw runtime_error("Invalid integer entry\n");
        
    }
}

int modifyReg(string str){
        if (str.substr(0,1) != "$"){
            cout << "Invalid register entry\n";
            throw "Invalid integer entry\n";
        }
        else{
            if (str.find("0")<str.size()-1){
                cout << "Invalid register entry\n";
                throw "Invalid integer entry\n";
            }
            if(str=="$zero") return 0;
            else if(str=="$sp") return 29;
            else if(str=="$at") return 1;
            else if(str=="$v0") return 2;
            else if(str=="$v1") return 3;
            else if(str=="$a0") return 4;
            else if(str=="$a1") return 5;
            else if(str=="$a2") return 6;
            else if(str=="$a3") return 7;
            else if(str=="$t8") return 24;
            else if(str=="$t9") return 25;
            else if (str.substr(1,1) == "t"){
                int i = stringToInt(&str[2]);
                if (i <= 7 && i >= 0){
                    return 8 + i;
                }
                else{
                    cout << "Invalid register entry\n";
                    throw "Invalid register entry\n";
                }
            }
            else if(str=="$s8") return 30;
            else if (str.substr(1,1) == "s"){
                
                int i = stringToInt(&str[2]);
                
                if (i <= 7 && i >= 0){
                    return 16 + i;
                }
                else{
                    cout << "Invalid register entry\n";
                    throw "Invalid register entry\n";
                }
                
            }
            else if(str=="$k0") return 26;
            else if(str=="$k1") return 27;
            else if(str=="$gp") return 28;
            // else if(str=="$r0") return 0;
            else if(str=="$ra") return 31;
            else{
                // if (str.substr(1,1) != "r"){
                //     cout << "Invalid register entry\n";
                //     throw "Invalid integer entry\n";
                    
                // }
                // else{
                    int i = stringToInt(&str[1]);
                    return i;
            }
                
        } 
    }
    

int byteToInt(byte* byte) {

    int n = 0;

    n = n + (byte[0] & 0x000000ff);
    n = n + ((byte[1] & 0x000000ff) << 8);
    n = n + ((byte[2] & 0x000000ff) << 16);
    n = n + ((byte[3] & 0x000000ff) << 24);


    return n;
}

void intToByte(int n, byte* result) {

     result[0] = n & 0x000000ff;
     result[1] = (n & 0x0000ff00) >> 8;
     result[2] = (n & 0x00ff0000) >> 16;
     result[3] = (n & 0xff000000) >> 24; 
}

class registers {
    public:
        int data;

        void set(int data1){
            data = data1;
        }

        int read(){
            return data;
        }
};

class arrayOfRegister {
    public:
        registers reg[32];
        int read(int num){
            return reg[num].read();
        }

        void set(int num, int data){
            reg[num].set(data);
        }

        void printall(){
            cout << "Register\t" << "Data\n";
            // cout << "zero" << "\t\t" << hex << read(0) <<endl<< dec;
            for (int i=0; i < 32; i++){
                cout << map_int_register[i]<< "\t\t" << hex << read(i) << endl << dec;
            }
            // cout << "sp" << "\t\t" << hex << read(31) << endl << dec;
        }
};

class Memory {
    public:
        byte mem[1024][1024];
        byte rowbuffer[1024];
        int rownum = 0;
        int opr;
        int count;
        int regd;
        int addr;
        bool inprocess = false;
        bool nonblock = true;
        int delayrow;
        int delaycol;
        int rowbufferupd;

        int read(int num){
            int row = num/1024;
            int col = num%1024;
            if (row == rownum){
                return byteToInt(&rowbuffer[col]);
            }
            else{
                copy(begin(rowbuffer), end(rowbuffer), begin(mem[rownum]));
                copy(begin(mem[row]), end(mem[row]), begin(rowbuffer));
                rownum = row;
                return byteToInt(&rowbuffer[col]);
            }
        }

        void set(int data, int num){
            int row = num/1024;
            int col = num%1024;
            if (row == rownum){
                intToByte(data, &rowbuffer[col]);
            }
            else{
                copy(begin(rowbuffer), end(rowbuffer), begin(mem[rownum]));
                copy(begin(mem[row]), end(mem[row]), begin(rowbuffer));
                rownum = row;
                intToByte(data, &rowbuffer[col]);
            }
        }

        void printall(){
            for (int i = (4*INS_COUNT)/1024; i<1024; i++){
                int j;
                if(i==0) j=(4*INS_COUNT)%1024;
                else j=0;
                for (; j < 1024; j=j+4){
                    int data = byteToInt(&mem[i][j]);
                    if (data != 0){
                        int num = 1024*i + j;
                        cout << num <<"-"<< num + 3<<"\t:\t" <<data<<endl;
                    }
                }
            }
        }

        void flush(){
            copy(begin(rowbuffer), end(rowbuffer), begin(mem[rownum]));
        }
};

bool add(arrayOfRegister* reg, int x1, int x2, int x3, int l_addr){
    bool err = false;
    CC++;
    if(x1>0 && x1<32 && x2>=0 && x2<32 && x3>=0 && x3<32) {
        int x = reg->read(x2) + reg->read(x3);
        reg->set(x1, x);
        cout<< "cycle " << CC << ":\t" << map_int_register[x1] << " = " << x << "; " << "Instruction addr:\t" << l_addr<<endl;
    }
    else {
        cout<<"Incorrect register values in add instruction"<<endl;
        return true;
    }
    return err;
}

bool addi(arrayOfRegister* reg, int x1, int x2, int x3, int l_addr){
    CC++;
    bool err = false;
    if(x1>0 && x1<32 && x2>=0 && x2<32) {
        int x = reg->read(x2) + x3;
        reg->set(x1, x);
        cout<< "cycle " << CC << ":\t" << map_int_register[x1] << " = " << x << "; " << "Instruction addr:\t" << l_addr<<endl;
        return false;
    }
    else {
        cout<<"Incorrect register values in addi instruction"<<endl;
        return true;
    }

}

bool sub(arrayOfRegister* reg, int x1, int x2, int x3, int l_addr){
    bool err = false;
    CC++;
    if(x1>0 && x1<32 && x2>=0 && x2<32 && x3>=0 && x3<32) {
        int x = reg->read(x2) - reg->read(x3);
        reg->set(x1, x);
        cout<< "cycle " << CC << ":\t" << map_int_register[x1] << " = " << x << "; " << "Instruction addr:\t" << l_addr<<endl;
        
    }
    else {
        cout<<"Incorrect register values in sub instruction"<<endl;
        return true;
    }
    return err;
}

bool mul(arrayOfRegister* reg, int x1, int x2, int x3, int l_addr){
    CC++;
    bool err = false;
    if(x1>0 && x1<32 && x2>=0 && x2<32 && x3>=0 && x3<32) {
        int x = reg->read(x2) * reg->read(x3);
        reg->set(x1, x);
        cout<< "cycle " << CC << ":\t" << map_int_register[x1] << " = " << x << "; " << "Instruction addr:\t" << l_addr<<endl;
    }
    else {
        cout<<"Incorrect register values in mul instruction"<<endl;
        return true;
    }
    return err;
}

bool beq(arrayOfRegister* reg, int x1, int x2, string x3, int l_addr){
    CC++;
    int err = 0;
    if(x1>0 && x1<32 && x2>=0 && x2<32) {
        if (reg->read(x1) == reg->read(x2)){
            cout<< "cycle " << CC << ":\t" << "jump to " << x3 << "; " << "Instruction addr:\t" << l_addr<<endl;
            return true;
        } 
        else {
            cout<< "cycle " << CC << ":\t" << "not jumped to " << x3 << "; " << "Instruction addr:\t" << l_addr<<endl;
            return false;
        }
    }
    else {
        cout<<"Incorrect register values in beq instruction"<<endl;
        throw runtime_error("Incorrect register values in beq instruction\n");
    }
    return err;
}

bool bne(arrayOfRegister* reg, int x1, int x2, string x3, int l_addr){
    CC++;
    int err = 0;
    if(x1>0 && x1<32 && x2>=0 && x2<32) {
        if (reg->read(x1) != reg->read(x2)){
            cout<< "cycle " << CC << ":\t" << "jump to " << x3 << "; " << "Instruction addr:\t" << l_addr<<endl;
            return true;
        } 
        else {
            cout<< "cycle " << CC << ":\t" << "not jumped to " << x3 << "; " << "Instruction addr:\t" << l_addr<<endl;
            return false;
        }
    }
    else {
        cout<<"Incorrect register values in bne instruction"<<endl;
        throw runtime_error("Incorrect register values in beq instruction\n");
    }
    return err;
}

bool slt(arrayOfRegister* reg, int x1, int x2, int x3, int l_addr){
    CC++;
    bool err = false;
    int x;
    if(x1>0 && x1<32 && x2>=0 && x2<32 && x3>=0 && x3<32) {
        if (reg->read(x2) < reg->read(x3)){
        reg->set(x1, 1);
        x = 1;
        }
    else{
        reg->set(x1, 0);
        x=0;
        }
    }
    else {
        cout<<"Incorrect register values in slt instruction"<<endl;
        return true;
    }
    cout<< "cycle " << CC << ":\t" << map_int_register[x1] << " = " << x <<"; Instruction addr:\t" << l_addr<<endl;
    return err;
}
//all functions are written for assignment 4
void exe_lw(int r, int add, arrayOfRegister* reg,Memory* m, int l_addr){
    //bool err = false;
    if(r>0 && r<32) {
        int loc = add;
        if ((loc % 4) != 0){
            throw runtime_error("Incorrect address: address not multiple of 4!\n");
        }
        else if( loc <= 4*INS_COUNT-4 ){
            throw runtime_error("Incorrect address: cannot access Instruction Memory address\n");
        }
        else{
            
            int pre = CC + 1;
            string yy;
            CURR = loc/1024;

            if( PREV==-1 ){ 
                CC = CC+ ROW_ACCESS_DELAY + COL_ACCESS_DELAY ;
                yy = "Row Buffer is initialized";
                ROW_BUFFER_UPDATE++;
                CHANGED = false;
            }
            else{
                if(PREV == CURR){ 
                    CC = CC + COL_ACCESS_DELAY;
                    yy = "No change in Row Buffer";
                }
                else {
                    
                    if (CHANGED){
                        CC = CC+ 2*ROW_ACCESS_DELAY + COL_ACCESS_DELAY;
                    }
                    else{
                        CC = CC+ ROW_ACCESS_DELAY + COL_ACCESS_DELAY;
                    }
                    yy = "Row Buffer is updated";
                    ROW_BUFFER_UPDATE++;
                    CHANGED = false;
                }
            }
            PREV=CURR;
            reg->set(r, m->read(loc));
            cout << "cycle " << pre << "-" << CC << ":\t"  << map_int_register[r] << " = " <<  m->read(loc) <<"; " << yy << "; Instruction addr:\t" << l_addr<<endl;
        }
    }
    else {
        throw runtime_error("Incorrect register values in lw instruction\n");
    }
}

void exe_sw(int data, int add, arrayOfRegister* reg,Memory* m, int l_addr){
    // if(r>=0 && r<32) {
        int loc = data;
        int x2 = add;
        if ((x2 % 4) != 0){
            throw runtime_error("Incorrect address: address not multiple of 4!\n");
        }
        else if( x2 <= 4*INS_COUNT-4 ){
            throw runtime_error("Incorrect address: cannot access Instruction Memory address\n");
        }
        else{
            int pre = CC+1;
            string yy;
            CURR = x2/1024;

            if( PREV == -1 ){ 
                CC = CC+ ROW_ACCESS_DELAY + COL_ACCESS_DELAY ;
                yy = "Row Buffer is initialized";
                ROW_BUFFER_UPDATE++;
                ROW_BUFFER_UPDATE++;
            }
            else{
                if(PREV == CURR){ 
                    CC = CC + COL_ACCESS_DELAY;
                    yy = "No change in Row Buffer";
                    ROW_BUFFER_UPDATE++;
                }
                else {
                    if (CHANGED){
                        CC = CC+ 2*ROW_ACCESS_DELAY + COL_ACCESS_DELAY;
                    }
                    else{
                        CC = CC+ ROW_ACCESS_DELAY + COL_ACCESS_DELAY;
                    }
                    yy = "Row Buffer is updated";
                    ROW_BUFFER_UPDATE++;
                    ROW_BUFFER_UPDATE++;
                    
                }
            }
            CHANGED = true;
            PREV=CURR;
            m->set(loc, x2);
            cout<< "cycle " << pre << "-" << CC << ":\t" << "Memory address " << x2 << "-" << x2+3 << " = " << loc << "; " << yy << "; Instruction addr:\t" << l_addr<<endl;
        }
}



class dramdata
{
    public:
    bool lw;
    int reg;
    int add;
    int l_addr;
    int data;
};

void execute_all_que(map<int,vector<dramdata*>> q, arrayOfRegister* regs, Memory* m){
    for(auto x : q){
        vector<dramdata*> v = x.second;
        for(auto in : v){
            if(in->lw) exe_lw(in->reg,in->add,regs,m, in->l_addr);
            else exe_sw(in->data,in->add,regs,m, in->l_addr);
        }
    }
}

void execute_n_que(map<int,vector<dramdata*>> q,int n, arrayOfRegister* regs, Memory* m, map<int,int>* dep_reg){
    
        vector<dramdata*> v = q[n];
        map<int, int> dep_reg1 = *dep_reg;
        for(auto in : v){
            if(in->lw) {
                exe_lw(in->reg,in->add,regs,m, in->l_addr);
                (*dep_reg).erase(in->reg);

            }
            else exe_sw(in->data,in->add,regs,m, in->l_addr);
        }
}

int para_exe(arrayOfRegister* regs, Memory* m, map<int,ins*> instructions, int j, map<int,int>* dep_reg1, map<int,vector<dramdata*>>* que1){

    int pc = j;
    int cycle;
    int i;
    if(PREV==-1) cycle=ROW_ACCESS_DELAY+COL_ACCESS_DELAY;
    else cycle = COL_ACCESS_DELAY;

    vector<int> pcs;

    map<int,vector<dramdata*>> que = *que1;
    map<int,int> dep_reg = *dep_reg1;


    for(auto x : instructions){
        pcs.push_back(x.first);
    }
    for(i = j; i < pcs.size() && i<j+cycle; i++) {
        
        string opr = instructions.find(pcs[i])->second->op;
        vector<string> args = instructions.find(pcs[i])->second->params;
        int l_addr = instructions.find(pcs[i])->second->l_addr;
        string x1,x2,x3;
        pc = pcs[i] + 4;
        
        if(opr=="lw" || opr=="sw") return i;
           
        else{

        if(opr == "add"){
        string x1,x2,x3;

        if(args.size()!= 3){
            throw "Incorrect arguments in add instruction\n";
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;

            int a = modifyReg(x1);
            int b = modifyReg(x2);
            int c = modifyReg(x3);

            if (dep_reg.count(c)||dep_reg.count(b)||dep_reg.count(a)) return i;

            else add(regs,a,b,c,l_addr);
        }
        
        }
        else if(opr=="sub"){
        string x1,x2,x3;

        if(args.size()!= 3){
            throw "Incorrect arguments in sub instruction\n";
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;

            int a = modifyReg(x1);
            int b = modifyReg(x2);
            int c = modifyReg(x3);

            bool analyze_count;
            if (dep_reg.count(c)||dep_reg.count(b)||dep_reg.count(a)) return i;
            else sub(regs,a,b,c, l_addr);
            
        }
        }
        else if(opr=="mul"){
        string x1,x2,x3;

        if(args.size()!= 3){
            throw "Incorrect arguments in sub instruction\n";
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;

            int a = modifyReg(x1);
            int b = modifyReg(x2);
            int c = modifyReg(x3);
            if (dep_reg.count(c)||dep_reg.count(b)||dep_reg.count(a)) return i;
                     
            else mul(regs,a,b,c,l_addr);
            
        }
        }
        else if(opr=="addi"){
        string x1,x2,x3;

        if(args.size()!= 3){
            throw runtime_error("Incorrect arguments in addi instruction\n");
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;


            int a = modifyReg(x1);
            int b = modifyReg(x2);
            int c = stringToInt(x3);

            
            if (dep_reg.count(b)||dep_reg.count(a)) return i;
            else addi(regs,a,b,c,l_addr);
        }
        }
        
        else if(opr=="beq"){
        string x1,x2,x3;

        if(args.size()!= 3){
            throw runtime_error("Incorrect arguments in beq instruction\n");
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it; 

            int a = modifyReg(x1);
            int b = modifyReg(x2);

            if (dep_reg.count(a)||dep_reg.count(b)) return i;

            bool k = beq(regs,a,b, x3, l_addr);

            if(!k) pc = pc;
            else{
                if (labels.count(x3)!=0){
                pc = labels[x3]+1;
            }
            else{
                throw runtime_error("label doesn't exist\n");
            }
            }

        }
    }

    else if(opr=="bne"){
        string x1,x2,x3;

        if(args.size()!= 3){
            throw runtime_error("Incorrect arguments in bne instruction\n");
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it; 



            int a = modifyReg(x1);
            int b = modifyReg(x2);
            
            if (dep_reg.count(a)||dep_reg.count(b)) return i;

            bool k = bne(regs,a,b, x3, l_addr);

            if(!k) pc = pc;
            else{
                if (labels.count(x3)!=0){
                pc = labels[x3]+1;
            }
            else{
                throw runtime_error("label doesn't exist\n");
            }
            }

        }
    }
    else if(opr=="slt"){
        string x1,x2,x3;

        if(args.size()!= 3){
            throw runtime_error("Incorrect arguments in slt instruction\n");
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;



            int a = modifyReg(x1);
            int b = modifyReg(x2);
            int c = modifyReg(x3);

            
            if (dep_reg.count(c)||dep_reg.count(b)||dep_reg.count(a)) return i;
            
            (slt(regs,a,b,c, l_addr));
        }
    }
        else if(opr=="j"){
        string x;

        if(args.size()!= 1){
            throw runtime_error("Incorrect arguments in jump instruction\n");
        }
       
        else{
            CC++;
            auto it = args.begin();
            x = *it; 
            if (labels.count(x)!=0){
                pc = labels[x]+1;
                cout<< "cycle " << CC << ":\t" << "jump to " << x << "; " << "Instruction addr:\t" << l_addr<<endl;
                
            }
            else{
                throw runtime_error("label doesn't exist\n");
            }
            
        }
        
        }
        else{
            throw runtime_error("Wrong Instruction\n");
        }
        }
        
        if(find(pcs.begin(),pcs.end(),pc)!=pcs.end()) {
            int k = find(pcs.begin(),pcs.end(),pc) - pcs.begin() ;
            /*if (i != j)*/ i=k-1;
            
        }
        else{
            //cout<<"pc out of bounds!"<<endl;
            return pcs.size()+1;
            // return;
        }

    }
    return i;
}

int exe_lw_para(int r, int add, arrayOfRegister* reg,Memory* m, int l_addr, map<int,ins*> instructions, int j, map<int,int>* dep_reg1, map<int,vector<dramdata*>>* que1 ){
    //bool err = false;
    int cc1 = CC;
    map<int,vector<dramdata*>> que = *que1;
    map<int,int> dep_reg = *dep_reg1;
    if(r>0 && r<32) {
        int loc = add;
        if ((loc % 4) != 0){
            throw runtime_error("Incorrect address: address not multiple of 4!\n");
        }
        else if( loc <= 4*INS_COUNT-4 ){
            throw runtime_error("Incorrect address: cannot access Instruction Memory address\n");
        }
        else{
            
            int pre = CC + 1;
            string yy;
            CURR = loc/1024;
            (* dep_reg1)[r] = add/1024;
            int k = para_exe(reg, m, instructions, j, dep_reg1, que1);
            (* dep_reg1).erase(r);

            if( PREV==-1 ){ 
                CC = cc1 + ROW_ACCESS_DELAY + COL_ACCESS_DELAY ;
                yy = "Row Buffer is initialized";
                ROW_BUFFER_UPDATE++;
                CHANGED = false;
            }
            else{
                if(PREV == CURR){ 
                    CC = cc1 + COL_ACCESS_DELAY;
                    yy = "No change in Row Buffer";
                }
                else {
                    
                    if (CHANGED){
                        CC = cc1 + 2*ROW_ACCESS_DELAY + COL_ACCESS_DELAY;
                    }
                    else{
                        CC = cc1 + ROW_ACCESS_DELAY + COL_ACCESS_DELAY;
                    }
                    yy = "Row Buffer is updated";
                    ROW_BUFFER_UPDATE++;
                    CHANGED = false;
                }
            }
            
            PREV=CURR;
            reg->set(r, m->read(loc));
            cout << "cycle " << pre << "-" << CC << ":\t"  << map_int_register[r] << " = " <<  m->read(loc) <<"; " << yy << "; Instruction addr:\t" << l_addr<<endl;
            return k;
        }
    }
    else {
        throw runtime_error("Incorrect register values in lw instruction\n");
        return 0;
    }
}

int exe_sw_para(int data, int add, arrayOfRegister* reg,Memory* m, int l_addr, map<int,ins*> instructions, int j, map<int,int>* dep_reg1, map<int,vector<dramdata*>>* que1){
    // if(r>=0 && r<32) {
        int cc1 = CC;
        map<int,vector<dramdata*>> que = *que1;
        map<int,int> dep_reg = *dep_reg1;
        int loc = data;
        int x2 = add;
        if ((x2 % 4) != 0){
            throw runtime_error("Incorrect address: address not multiple of 4!\n");
        }
        else if( x2 <= 4*INS_COUNT-4 ){
            throw runtime_error("Incorrect address: cannot access Instruction Memory address\n");
        }
        else{
            int pre = CC+1;
            string yy;
            CURR = x2/1024;
            int k = para_exe(reg, m, instructions, j, dep_reg1, que1);
            if( PREV == -1 ){ 
                CC = cc1 + ROW_ACCESS_DELAY + COL_ACCESS_DELAY ;
                yy = "Row Buffer is initialized";
                ROW_BUFFER_UPDATE++;
                ROW_BUFFER_UPDATE++;
            }
            else{
                if(PREV == CURR){ 
                    CC = cc1 + COL_ACCESS_DELAY;
                    yy = "No change in Row Buffer";
                    ROW_BUFFER_UPDATE++;
                }
                else {
                    if (CHANGED){
                        CC = cc1 + 2*ROW_ACCESS_DELAY + COL_ACCESS_DELAY;
                    }
                    else{
                        CC = cc1 + ROW_ACCESS_DELAY + COL_ACCESS_DELAY;
                    }
                    yy = "Row Buffer is updated";
                    ROW_BUFFER_UPDATE++;
                    ROW_BUFFER_UPDATE++;
                    
                }
            }
            
            CHANGED = true;
            PREV=CURR;
            m->set(loc, x2);
            cout<< "cycle " << pre << "-" << CC << ":\t" << "Memory address " << x2 << "-" << x2+3 << " = " << loc << "; " << yy << "; Instruction addr:\t" << l_addr<<endl;
            return k;
        }
    // }
    // else {
    //     throw runtime_error("Incorrect register values in sw instruction\n");
    // }
}

void execute(arrayOfRegister* regs, Memory* m, map<int,ins*> instructions){

    cout<<"Beginning Execution"<<endl;
    int pc;
    int cycle = 1;
    vector<int> pcs;
    map<int,int> ccc;
    int curr = 0;
    map<int,vector<dramdata*>> que;
    map<int,int> dep_reg;


    for(auto x : instructions){
        pcs.push_back(x.first);
        ccc.insert(make_pair(x.first,0));
    }
    for(int i = 0; i < pcs.size(); i++) {
        
        string opr = instructions.find(pcs[i])->second->op;
        vector<string> args = instructions.find(pcs[i])->second->params;
        int l_addr = instructions.find(pcs[i])->second->l_addr;
        string x1,x2,x3;
        pc = pcs[i] + 4;
        
        if(opr=="lw"){
            
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;

            string x4;
            if (x3.substr(0,1) != "(" || x3.substr(x3.size()-1) != ")"){
                //cout << "Invalid memory location\n";
                throw runtime_error("invalid memory location\n");
            }
            else{
                x4 = trim(x3.substr(1,x3.size()-2));
            }

            int a = modifyReg(x1);
            int b = stringToInt(x2);
            int c = modifyReg(x4);
            if(c<0 || c>31) throw runtime_error("Incorrect register values in lw instruction\n");
            
            CC++;
            cout<< "cycle " << CC << ":\t" << "DRAM request issued for lw instruction with address: "<< l_addr << endl;
            //cout << "DRAM request issued for lw" << endl;
            bool analyze_count = false;
            if (dep_reg.count(c)){
                CC++;
                analyze_count = true;
                cout<< "cycle " << CC << ":\t" << "DRAM request is dependent instruction, queue is executed" << endl;
            }
            if (dep_reg.count(a)){
                CC++;
                analyze_count = true;
                cout<< "cycle " << CC << ":\t" << "Reduntant lw instructions in queue terminated" << endl;
            }
            
            if(dep_reg.count(c)){
                int x = dep_reg[c];
                //empty all ins in this queue
                execute_n_que(que,x,regs,m, &dep_reg);
                que[x] = {};
                curr = x;
            }
            if(dep_reg.count(a)){
                int x = dep_reg[a];
                //terminate the instruction corresponding to x
                for(auto it = que[x].begin() ; it < que[x].end() ; it++){
                    if((* it) -> reg == a && (* it)->lw){
                        que[x].erase(it);
                        dep_reg.erase(a);
                    }
                }
            }
            int addr = (regs->read(c)) + b;

            if(addr/1024==curr || PREV == -1){
                //execute
                int k = exe_lw_para(a,addr,regs,m, l_addr, instructions, i+1, &dep_reg, &que);
                curr = addr/1024;
                if(k<pcs.size()) pc=pcs[k];
                else pc = pcs[pcs.size()-1]+4;
                //cout << m->nonblock << endl;
            }
            else{
                if(!analyze_count){
                    
                    analyze_count = true;
                    cout<< "\t\tDRAM request requires new buffer row, added to queue" << endl;
                }
                dep_reg[a] = addr/1024;
                dramdata* d = new dramdata();
                d->lw=true;
                d->reg = a;
                d->add = addr;
                d->l_addr = l_addr;
                que[addr/1024].push_back(d);
            }
            
            
        }
        else if(opr=="sw"){

            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;

            string x4;
            if (x3.substr(0,1) != "(" || x3.substr(x3.size()-1) != ")"){
                //cout << "Invalid memory location\n";
                throw runtime_error("invalid memory location\n");
            }
            else{
                x4 = trim(x3.substr(1,x3.size()-2));
            }

            int a = modifyReg(x1);
            int b = stringToInt(x2);
            int c = modifyReg(x4);
            if(c<0 || c>31) throw runtime_error("Incorrect register values in lw instruction\n");
            

            //cout << "DRAM request issued for sw" << endl;
            CC++;
            cout<< "cycle " << CC << ":\t" << "DRAM request issued for sw instruction with address: "<< l_addr << endl;
            bool analyze_count = false;
            if (dep_reg.count(c)||dep_reg.count(a)){
                CC++;
                analyze_count = true;
                cout<< "cycle " << CC << ":\t" << "DRAM request is dependent instruction, queue is executed" << endl;
            }

            if(dep_reg.count(c)){
                int x = dep_reg[c];
                //empty all ins in this queue
                execute_n_que(que,x,regs,m,&dep_reg);
                que[x] = {};
                curr = x;
            }
            if(dep_reg.count(a)){
                int x = dep_reg[a];
                //empty all ins
                execute_n_que(que,x,regs,m,&dep_reg);
                que[x] = {};
                curr = x;
            }
            int addr = (regs->read(c)) + b;
            if(addr/1024==curr || PREV==-1){
                //execute
                if(a>0 && a<32) {
                    int k = exe_sw_para(regs->read(a),addr,regs,m, l_addr, instructions, i+1, &dep_reg, &que);
                    curr = addr/1024;
                    if(k<pcs.size()) pc=pcs[k];
                    else pc = pcs[pcs.size()-1]+4;
                }
                else{
                    throw runtime_error("Incorrect register values in sw instruction\n");
                }
                //m->nonblock=true;
            }
            else{
                if(!analyze_count){
                    
                    analyze_count = true;
                    cout << "\t\tDRAM request requires new buffer row, added to queue" << endl;
                }
                dramdata* d = new dramdata();
                d->reg = a;
                if(a>0 && a<32) {
                    d->data = regs->read(a);
                }
                else{
                    throw runtime_error("Incorrect register values in sw instruction\n");
                }
                
                d->add = addr;
                d->lw = false;
                d->l_addr = l_addr;
                que[addr/1024].push_back(d);
            }
        }
        else{

        if(opr == "add"){
        string x1,x2,x3;

        if(args.size()!= 3){
            throw "Incorrect arguments in add instruction\n";
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;

            int a = modifyReg(x1);
            int b = modifyReg(x2);
            int c = modifyReg(x3);

            bool analyze_count;
            if (dep_reg.count(c)||dep_reg.count(b)){
                CC++;
                analyze_count = true;
                cout<< "cycle " << CC << ":\t" << "Instruction request is dependent instruction, queue is executed" << endl;
            }
            
            if(dep_reg.count(b)){
                int x = dep_reg[b];
                //empty all ins in this queue
                execute_n_que(que,x,regs,m,&dep_reg);
                que[x] = {};
                curr = x;
            }
            if(dep_reg.count(c)){
                int x = dep_reg[c];
                //empty all ins in this queue
                execute_n_que(que,x,regs,m,&dep_reg);
                que[x] = {};
                curr = x;
            }
            if(dep_reg.count(a)){
                int x = dep_reg[a];
                //terminate the instruction corresponding to x
                CC++;
                analyze_count = true;
                cout<< "cycle " << CC << ":\t" << "Reduntant lw instructions in queue terminated" << endl;
                for(auto it = que[x].begin() ; it < que[x].end() ; it++){
                    if((* it) -> reg == a && (* it)->lw){
                        que[x].erase(it);
                        dep_reg.erase(a);
                    }
                }
            }
            
            add(regs,a,b,c,l_addr);
        }
        
        }
        else if(opr=="sub"){
        string x1,x2,x3;

        if(args.size()!= 3){
            throw "Incorrect arguments in sub instruction\n";
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;

            int a = modifyReg(x1);
            int b = modifyReg(x2);
            int c = modifyReg(x3);

            bool analyze_count;
            if (dep_reg.count(c)||dep_reg.count(b)){
                CC++;
                analyze_count = true;
                cout<< "cycle " << CC << ":\t" << "Instruction request is dependent instruction, queue is executed" << endl;
            }
        
            if(dep_reg.count(b)){
                int x = dep_reg[b];
                //empty all ins in this queue
                execute_n_que(que,x,regs,m,&dep_reg);
                que[x] = {};
                curr = x;
            }
            if(dep_reg.count(c)){
                int x = dep_reg[c];
                //empty all ins in this queue
                execute_n_que(que,x,regs,m,&dep_reg);
                que[x] = {};
                curr = x;
            }
            if(dep_reg.count(a)){
                int x = dep_reg[a];
                CC++;
                analyze_count = true;
                cout<< "cycle " << CC << ":\t" << "Reduntant lw instructions in queue terminated" << endl;
                //terminate the instruction corresponding to x
                for(auto it = que[x].begin() ; it < que[x].end() ; it++){
                    if((* it) -> reg == a && (* it)->lw){
                        que[x].erase(it);
                        dep_reg.erase(a);
                    }
                }
            }
            
            sub(regs,a,b,c, l_addr);
            

        }
        }
        else if(opr=="mul"){
        string x1,x2,x3;

        if(args.size()!= 3){
            throw "Incorrect arguments in sub instruction\n";
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;

            int a = modifyReg(x1);
            int b = modifyReg(x2);
            int c = modifyReg(x3);

            bool analyze_count;
            if (dep_reg.count(c)||dep_reg.count(b)){
                CC++;
                analyze_count = true;
                cout<< "cycle " << CC << ":\t" << "Instruction request is dependent instruction, queue is executed" << endl;
            }
           
            if(dep_reg.count(b)){
                int x = dep_reg[b];
                //empty all ins in this queue
                execute_n_que(que,x,regs,m,&dep_reg);
                que[x] = {};
                curr = x;
            }
            if(dep_reg.count(c)){
                int x = dep_reg[c];
                //empty all ins in this queue
                execute_n_que(que,x,regs,m,&dep_reg);
                que[x] = {};
                curr = x;
            }
            if(dep_reg.count(a)){
                int x = dep_reg[a];
                CC++;
                analyze_count = true;
                cout<< "cycle " << CC << ":\t" << "Reduntant lw instructions in queue terminated" << endl;
                //terminate the instruction corresponding to x
                for(auto it = que[x].begin() ; it < que[x].end() ; it++){
                    if((* it) -> reg == a && (* it)->lw){
                        que[x].erase(it);
                        dep_reg.erase(a);
                    }
                }
            }
            
            mul(regs,a,b,c,l_addr);
            

        }
        }
        else if(opr=="addi"){
        string x1,x2,x3;

        if(args.size()!= 3){
            throw "Incorrect arguments in addi instruction\n";
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;


            int a = modifyReg(x1);
            int b = modifyReg(x2);
            int c = stringToInt(x3);

            bool analyze_count;
            if (dep_reg.count(b)){
                CC++;
                analyze_count = true;
                cout<< "cycle " << CC << ":\t" << "Instruction request is dependent instruction, queue is executed" << endl;
            }
            
            if(dep_reg.count(b)){
                int x = dep_reg[b];
                //empty all ins in this queue
                execute_n_que(que,x,regs,m,&dep_reg);
                que[x] = {};
                curr = x;
            }
            if(dep_reg.count(a)){
                int x = dep_reg[a];
                CC++;
                analyze_count = true;
                cout<< "cycle " << CC << ":\t" << "Reduntant lw instructions in queue terminated" << endl;
                //terminate the instruction corresponding to x
                for(auto it = que[x].begin() ; it < que[x].end() ; it++){
                    if((* it) -> reg == a && (* it)->lw){
                        que[x].erase(it);
                        dep_reg.erase(a);
                    }
                }
            }
            
            addi(regs,a,b,c,l_addr);
        }
        }
        
        else if(opr=="beq"){
        string x1,x2,x3;

        if(args.size()!= 3){
            throw "Incorrect arguments in beq instruction\n";
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it; 

            int a = modifyReg(x1);
            int b = modifyReg(x2);

            bool analyze_count;
            if (dep_reg.count(a)||dep_reg.count(b)){
                CC++;
                analyze_count = true;
                cout<< "cycle " << CC << ":\t" << "Instruction request is dependent instruction, queue is executed" << endl;
            }

            if(dep_reg.count(b)){
                int x = dep_reg[b];
                //empty all ins in this queue
                execute_n_que(que,x,regs,m,&dep_reg);
                que[x] = {};
                curr = x;
            }
            if(dep_reg.count(a)){
                int x = dep_reg[a];
                //terminate the instruction corresponding to x
                execute_n_que(que,x,regs,m,&dep_reg);
                que[x] = {};
                curr = x;
            }

            bool k = beq(regs,a,b, x3, l_addr);

            // if(k) return 0;
            if(!k) pc = pc;
            else{
                if (labels.count(x3)!=0){
                pc = labels[x3]+1;
            }
            else{
                throw runtime_error("label doesn't exist\n");
            }
            }

        }
    }

    else if(opr=="bne"){
        string x1,x2,x3;

        if(args.size()!= 3){
            throw "Incorrect arguments in bne instruction\n";
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it; 



            int a = modifyReg(x1);
            int b = modifyReg(x2);

            bool analyze_count;
            if (dep_reg.count(a)||dep_reg.count(b)){
                CC++;
                analyze_count = true;
                cout<< "cycle " << CC << ":\t" << "Instruction request is dependent instruction, queue is executed" << endl;
            }

            if(dep_reg.count(b)){
                int x = dep_reg[b];
                //empty all ins in this queue
                execute_n_que(que,x,regs,m,&dep_reg);
                que[x] = {};
                curr = x;
            }
            if(dep_reg.count(a)){
                int x = dep_reg[a];
                //terminate the instruction corresponding to x
                execute_n_que(que,x,regs,m,&dep_reg);
                que[x] = {};
                curr = x;
            }

            bool k = bne(regs,a,b, x3, l_addr);

            // if(k) return 0;
            if(!k) pc = pc;
            else{
                if (labels.count(x3)!=0){
                pc = labels[x3]+1;
            }
            else{
                throw runtime_error("label doesn't exist\n");
            }
            }

        }
    }
    else if(opr=="slt"){
        string x1,x2,x3;

        if(args.size()!= 3){
            throw "Incorrect arguments in slt instruction\n";
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;



            int a = modifyReg(x1);
            int b = modifyReg(x2);
            int c = modifyReg(x3);

            bool analyze_count;
            if (dep_reg.count(c)||dep_reg.count(b)){
                CC++;
                analyze_count = true;
                cout<< "cycle " << CC << ":\t" << "Instruction request is dependent instruction, queue is executed" << endl;
            }
            

            if(dep_reg.count(b)){
                int x = dep_reg[b];
                //empty all ins in this queue
                execute_n_que(que,x,regs,m,&dep_reg);
                que[x] = {};
                curr = x;
            }
            if(dep_reg.count(c)){
                int x = dep_reg[b];
                //empty all ins in this queue
                execute_n_que(que,x,regs,m,&dep_reg);
                que[x] = {};
                curr = x;
            }
            if(dep_reg.count(a)){
                int x = dep_reg[a];
                CC++;
                analyze_count = true;
                cout<< "cycle " << CC << ":\t" << "Reduntant lw instructions in queue terminated" << endl;
                //terminate the instruction corresponding to x
                for(auto it = que[x].begin() ; it < que[x].end() ; it++){
                    if((* it) -> reg == a && (* it)->lw){
                        que[x].erase(it);
                        dep_reg.erase(a);
                    }
                }
            }

            (slt(regs,a,b,c, l_addr));
        }
    }
        else if(opr=="j"){
        string x;

        if(args.size()!= 1){
            throw runtime_error("Incorrect arguments in jump instruction\n");
        }
       
        else{
            CC++;
            auto it = args.begin();
            x = *it; 
            if (labels.count(x)!=0){
                pc = labels[x]+1;
                cout<< "cycle " << CC << ":\t" << "jump to " << x << "; " << "Instruction addr:\t" << l_addr<<endl;
                
            }
            else{
                throw runtime_error("label doesn't exist\n");
            }
            
        }
        
        }
        else{
            throw runtime_error("Wrong Instruction\n");
        }
        }
        
        ccc.find(pcs[i])->second++;

        if(find(pcs.begin(),pcs.end(),pc)!=pcs.end()) {
            int j = find(pcs.begin(),pcs.end(),pc) - pcs.begin() ;
            /*if (i != j)*/ i=j-1;
        }
        else{
            //cout<<"pc out of bounds!"<<endl;
            break;
            // return;
        }
        
    }

    execute_all_que(que,regs,m);
    dep_reg.clear();
    m->flush(); 
    /*if (CHANGED){
        CC++;
        cout<< "cycle " << CC << ":\t" << "DRAM flush request issued" << endl;
        int pre = CC + 1;
        CC = CC + ROW_ACCESS_DELAY;
        cout<< "cycle " << pre << "-" << CC << ":\t" << "DRAM rowbuffer flushed into memory" << endl;
        m->flush();    
    }*/
    
        
    cout<<"---------------------------------------------------------------\n";
    cout<<"Final Memory content after execution:\n";
    m->printall();
    cout<<"---------------------------------------------------------------\n";
    cout<<"Final Register File content after execution:\n";
    regs->printall();
    cout<<"---------------------------------------------------------------\n";
    cout<<"Relevant Statistics:"<<endl;
    cout<<"No of clock cycles: "<< CC<<endl;
    cout<<"No of times Row Buffer is updated: " << ROW_BUFFER_UPDATE <<endl;

}

inline bool file_notexist (const std::string& name) {  // a function used to check a file with a given name exists in the working directory or not
  if (FILE *file = fopen(name.c_str(), "r")) {        // returns true if file doesn't exist
      fclose(file);                                   // returns false if a file with the given name exists
      return false;
    } else {
        return true;
    }   
}



int main(int argc, char const *argv[]){

    if (argc!=4){       // unexpected no. of parameters
        cout << "Please run the program using this command format \"./part1 filename ROW_ACCESS_DELAY COL_ACCESS_DELAY\"\n";
    return 1;
    }

    if (file_notexist(argv[1])){   // checks if file doesn't exist, prints error message if true
        cout << "Please put the input file in the directory"<<endl;
    return 1;
    }
    string text;
    ifstream infile;
    infile.open(argv[1]);
    ROW_ACCESS_DELAY = stringToInt(argv[2]);
    COL_ACCESS_DELAY = stringToInt(argv[3]);
    int COUNT = 1;

    map<int,ins*> instructions;
    arrayOfRegister* regs = new arrayOfRegister();
    regs->set(29,1048576);
    Memory* m = new Memory();

    while (getline (infile, text)) {
        ins* in = new ins();
        
        text = trim(text);
        in->sen = text;
        string temp3 = text;
        if ((temp3.find("#")) != string::npos){
            stringstream s1(temp3);
            string temp4;
            getline(s1, temp4, '#');
            text = trim(temp4);
        }
        string word; 
        stringstream iss(text); 
        iss >> word;

        if(word==""){
            continue;
        }
        // cout << &word[word.size()-1] << endl;
        word = trim(word);
        if (word.substr(word.size()-1)==":"){
            labels[word.substr(0,word.size()-1)] = COUNT - 1;
            // cout << "here" << endl;
            // cout << word << endl;
            if(word==text){
                continue;
            }
            iss >> word;
        }
        in->op = word;
        in->l_addr = COUNT - 1;
        string w = iss.str();
        string substr;
        while (getline(iss, substr, ',')){
            substr = trim(substr);
            stringstream ss(substr);
            // cout << substr.find("(") << endl;
            if ((substr.find("(")) != string::npos){
                string temp;
                getline(ss, temp, '(');
                int l = temp.size();
                temp = trim(temp);
                // cout << temp << "test" << endl;
                if (temp==""){
                    temp = "0";
                }
                
                    in->params.push_back(temp);
                    string temp1 = ss.str().substr(l);
                    if (temp1.substr(temp1.size()-1) != ")"){
                        cout << "non-closed bracket\n";
                        throw runtime_error("non-closed bracket\n");
                    }
                    else{
                        in->params.push_back(temp1);
                    }
                
                
            }
            else{
                in->params.push_back(substr);
            }
            
            
            
        }

        instructions.insert(make_pair(COUNT,in));  
        m->set(COUNT,COUNT-1);  
        COUNT = COUNT+4;

        if(COUNT>1048572){
            cout<<"Out of Memory!"<<endl;
            break;
        }

    }

    INS_COUNT = COUNT/4;

    infile.close();

    m->flush();

    execute(regs,m,instructions);

    return 0;
}



