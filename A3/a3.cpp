#include <bits/stdc++.h>

using namespace std;

class ins{
    public:
    string sen;
    string op;
    vector<string> params;

};

typedef unsigned char byte;

using namespace std;


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
            cout << "zero" << "\t\t" << hex << read(0) <<endl<< dec;
            for (int i=1; i < 31; i++){
                cout << "r" << i << "\t\t" << hex << read(i) << endl << dec;
            }
            cout << "sp" << "\t\t" << hex << read(31) << endl << dec;
        }
};

class Memory {
    public:
        byte mem[1048576];

        int read(int num){
            return byteToInt(&mem[num]);
        }

        void set(int data, int num){
            intToByte(data, &mem[num]);
        }
};

bool add(arrayOfRegister* reg, int x1, int x2, int x3){
    bool err = false;
    if(x1>0 && x1<32 && x2>=0 && x2<32 && x3>=0 && x3<32) {
        //cout<<"in"<<endl;
        reg->set(x1, reg->read(x2) + reg->read(x3));
    }
    else {
        cout<<"Incorrect register values in add instruction"<<endl;
        return true;
    }
    return err;
}

bool sub(arrayOfRegister* reg, int x1, int x2, int x3){
    bool err = false;
    if(x1>0 && x1<32 && x2>=0 && x2<32 && x3>=0 && x3<32) {
        //cout<<"in"<<endl;
        reg->set(x1, reg->read(x2) - reg->read(x3));
    }
    else {
        cout<<"Incorrect register values in sub instruction"<<endl;
        return true;
    }
    return err;
}

bool mul(arrayOfRegister* reg, int x1, int x2, int x3){
    bool err = false;
    if(x1>0 && x1<32 && x2>=0 && x2<32 && x3>=0 && x3<32) {
        //cout<<"in"<<endl;
        reg->set(x1, reg->read(x2) * reg->read(x3));
    }
    else {
        cout<<"Incorrect register values in mul instruction"<<endl;
        return true;
    }
    return err;
}

bool addi(arrayOfRegister* reg, int x1, int x2, int x3){
    bool err = false;
    if(x1>0 && x1<32 && x2>=0 && x2<32) {
        reg->set(x1, reg->read(x2) + x3);
    }
    else {
        cout<<"Incorrect register values in addi instruction"<<endl;
        return true;
    }
    return err;
}

int beq(arrayOfRegister* reg, int x1, int x2, int x3){
    int err = 0;
    if(x1>0 && x1<32 && x2>=0 && x2<32) {
        if (reg->read(x1) == reg->read(x2)) return x3;
        else return 1;
    }
    else {
        cout<<"Incorrect register values in beq instruction"<<endl;
        return 0;
    }
    return err;
}

int bne(arrayOfRegister* reg, int x1, int x2, int x3){
    int err = 0;
    if(x1>0 && x1<32 && x2>=0 && x2<32) {
        if (reg->read(x1) != reg->read(x2)) return x3;
        else return 1;
    }
    else {
        cout<<"Incorrect register values in bne instruction"<<endl;
        return 0;
    }
    return err;
}

bool slt(arrayOfRegister* reg, int x1, int x2, int x3){
    bool err = false;
    if(x1>0 && x1<32 && x2>=0 && x2<32 && x3>=0 && x3<32) {
        if (reg->read(x2) < reg->read(x3)){
        reg->set(x1, 1);
        }
    else{
        reg->set(x2, 0);
        }
    }
    else {
        cout<<"Incorrect register values in slt instruction"<<endl;
        return true;
    }
    return err;
}

bool lw(arrayOfRegister* reg,Memory* m, int x1, int x2, int x3){
    bool err = false;
    if(x1>0 && x1<32 && x3>=0 && x3<32) {
        int loc = (reg->read(x3)) + x2;
        if ((loc % 4) != 0){
            cout<<"Incorrect address: address not multiple of 4!"<<endl;
            return true;
        }
        else{
            reg->set(x1, m->read(loc));
        }
    }
    else {
        cout<<"Incorrect register values in lw instruction"<<endl;
        return true;
    }
    return err;
}

bool sw(arrayOfRegister* reg,Memory* m, int x1, int x2, int x3){
    bool err = false;
    if(x1>0 && x1<31 && x3>=0 && x3<32) {
        int loc = (reg->read(x1));
        if ((x2 % 4) != 0){
            cout<<"Incorrect address: address not multiple of 4!"<<endl;
            return true;
        }
        else{
            m->set(loc, x2+reg->read(x3));
        }
    }
    else {
        cout<<"Incorrect register values in lw instruction"<<endl;
        return true;
    }
    return err;
}

bool check(string s){
    char k = s[0];
    char m = s[1];
    bool t = (k != '$');
    bool z = (m != 'r');
    if (t) return true;
    else{
        if(z) return true;
        else return false;
    }
}

int process(arrayOfRegister* regs, Memory* m,string opr,vector<string> args, int pc){
    int err = pc;
    if(opr == "add"){
        string x1,x2,x3;

        if(args.size()!= 3){
            cout<<"Incorrect arguments in add instruction"<<endl;
            return 0;
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;
            if(x1=="$zero") x1="000";
            else if(x1=="$sp") x1="0031";
            else{
                if(check(x1)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }

            if(x2=="$zero") x2="000";
            else if(x2=="$sp") x2="0031";
            else{
                if(check(x2)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }
            if(x3=="$zero") x3="000";
            else if(x3=="$sp") x3="0031";
            else{
                if(check(x3)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }

            int a = stoi(x1.substr(2));
            int b = stoi(x2.substr(2));
            int c = stoi(x3.substr(2));

            if(add(regs,a,b,c)) return 0;
        }
        
    }
    else if(opr=="sub"){
        string x1,x2,x3;

        if(args.size()!= 3){
            cout<<"Incorrect arguments in sub instruction"<<endl;
            return 0;
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;
            if(x1=="$zero") x1="000";
            else if(x1=="$sp") x1="0031";
            else{
                if(check(x1)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }

            if(x2=="$zero") x2="000";
            else if(x2=="$sp") x2="0031";
            else{
                if(check(x2)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }
            if(x3=="$zero") x3="000";
            else if(x3=="$sp") x3="0031";
            else{
                if(check(x3)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }

            int a = stoi(x1.substr(2));
            int b = stoi(x2.substr(2));
            int c = stoi(x3.substr(2));

            if(sub(regs,a,b,c)) return 0;
        }
    }
    else if(opr=="mul"){
        string x1,x2,x3;

        if(args.size()!= 3){
            cout<<"Incorrect arguments in mul instruction"<<endl;
            return 0;
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;
            if(x1=="$zero") x1="000";
            else if(x1=="$sp") x1="0031";
            else{
                if(check(x1)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }

            if(x2=="$zero") x2="000";
            else if(x2=="$sp") x2="0031";
            else{
                if(check(x2)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }
            if(x3=="$zero") x3="000";
            else if(x3=="$sp") x3="0031";
            else{
                if(check(x3)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }

            int a = stoi(x1.substr(2));
            int b = stoi(x2.substr(2));
            int c = stoi(x3.substr(2));

            if(mul(regs,a,b,c)) return 0;
        }
    }
    else if(opr=="addi"){
        string x1,x2,x3;

        if(args.size()!= 3){
            cout<<"Incorrect arguments in addi instruction"<<endl;
            return 0;
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;
            if(x1=="$zero") x1="00";
            else if(x1=="$sp") x1="031";
            else{
                if(check(x1)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }

            if(x2=="$zero") x2="000";
            else if(x2=="$sp") x2="0031";
            else{
                if(check(x2)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }

            int a = stoi(x1.substr(2));
            int b = stoi(x2.substr(2));
            int c = stoi(x3);

            if(addi(regs,a,b,c)) return 0;
        }
    }
    else if(opr=="beq"){
        string x1,x2,x3;

        if(args.size()!= 3){
            cout<<"Incorrect arguments in beq instruction"<<endl;
            return 0;
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it; 

            if(x1=="$zero") x1="000";
            else if(x1=="$sp") x1="0031";
            else{
                if(check(x1)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }

            if(x2=="$zero") x2="000";
            else if(x2=="$sp") x2="0031";
            else{
                if(check(x2)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }

            int a = stoi(x1.substr(2));
            int b = stoi(x2.substr(2));
            int c = stoi(x3);

            int k = beq(regs,a,b,c);

            if(k==0) return 0;
            else if(k==1) return pc;
            else{
                if(k%4!=0){
                cout<<"Invalid location"<<endl;
                return 0;
                }
                else return k+1;
            }

        }
    }
    else if(opr=="bne"){
        string x1,x2,x3;

        if(args.size()!= 3){
            cout<<"Incorrect arguments in bne instruction"<<endl;
            return 0;
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it; 

            if(x1=="$zero") x1="000";
            else if(x1=="$sp") x1="0031";
            else{
                if(check(x1)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }

            if(x2=="$zero") x2="000";
            else if(x2=="$sp") x2="0031";
            else{
                if(check(x2)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }

            int a = stoi(x1.substr(2));
            int b = stoi(x2.substr(2));
            int c = stoi(x3);

            int k = bne(regs,a,b,c);

            if(k==0) return 0;
            else if(k==1) return pc;
            else{
                if(k%4!=0){
                cout<<"Invalid location"<<endl;
                return 0;
                }
                else return k+1;
            }

        }
    }
    else if(opr=="slt"){
        string x1,x2,x3;

        if(args.size()!= 3){
            cout<<"Incorrect arguments in slt instruction"<<endl;
            return 0;
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;
            if(x1=="$zero") x1="000";
            else if(x1=="$sp") x1="0031";
            else{
                if(check(x1)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }

            if(x2=="$zero") x2="000";
            else if(x2=="$sp") x2="0031";
            else{
                if(check(x2)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }

            if(x3=="$zero") x3="000";
            else if(x3=="$sp") x3="0031";
            else{
                if(check(x3)){
                    cout<<"Invalid register entry\n";
                }
            }

            int a = stoi(x1.substr(2));
            int b = stoi(x2.substr(2));
            int c = stoi(x3.substr(2));

            if(slt(regs,a,b,c)) return 0;
        }
    }
    else if(opr=="lw"){
        string x1,x2,x3;

        if(args.size()!= 3 && args.size()!=2){
            cout<<"Incorrect arguments in lw instruction"<<endl;
            return 0;
        }
       
        else{
            if(args.size()==3){
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;
            if(x1=="$zero") x1="000";
            else if(x1=="$sp") x1="0031";
            else{
                if(check(x1)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }

            if(x3=="($zero)") x3="0000";
            if(x3=="($sp)") x3="0031";

            int a = stoi(x1.substr(2));
            int b = stoi(x2.substr(0));
            int c = stoi(x3.substr(3));
            
            if(lw(regs,m,a,b,c)) return 0;
            }
            else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; 
            if(x1=="$zero") x1="000";
            else if(x1=="$sp") x1="0031";
            else{
                if(check(x1)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }

            if(x2=="($zero)") x2="0000";
            if(x2=="($sp)") x2="00031";

            int a = stoi(x1.substr(2));
            int b = 0;
            int c = stoi(x2.substr(3));
            
            if(lw(regs,m,a,b,c)) return 0;
            }
        }
    }
    else if(opr=="sw"){
        string x1,x2,x3;

        if(args.size()!= 3 && args.size()!=2){
            cout<<"Incorrect arguments in sw instruction"<<endl;
            return 0;
        }
       
        else{
            if(args.size()==3){
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;
            if(x1=="$zero") x1="000";
            else if(x1=="$sp") x1="0031";
            else{
                if(check(x1)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }

            if(x3=="($zero)") x3="000";
            if(x3=="($sp)") x3="0031";

            int a = stoi(x1.substr(2));
            int b = stoi(x2.substr(0));
            int c = stoi(x3.substr(3));
            
            if(sw(regs,m,a,b,c)) return 0;
            }
            else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; 
            if(x1=="$zero") x1="000";
            else if(x1=="$sp") x1="0031";
            else{
                if(check(x1)){
                    cout<<"Invalid register entry\n";
                    return 0;
                }
            }

            if(x2=="($zero)") x2="0000";
            if(x2=="($sp)") x2="00031";

            int a = stoi(x1.substr(2));
            int b = 0;
            int c = stoi(x2.substr(3));
            
            if(sw(regs,m,a,b,c)) return 0;
            }
        }
    }
    else if(opr=="j"){
        string x;

        if(args.size()!= 1){
            cout<<"Incorrect arguments in jump instruction"<<endl;
            return 0;
        }
       
        else{
            auto it = args.begin();
            x = *it; 
            int a = stoi(x);
            if(a%4!=0){
                cout<<"Invalid jump location"<<endl;
                return 0;   
            }
            else return a+1;
        }
        
    }
    else{
        cout<<"Wrong instruction"<<endl;
        err = 0;
    }
    return err;

}

void execute(arrayOfRegister* regs, Memory* m, map<int,ins*> instructions){
    cout<<"Beginning Execution"<<endl;
    int pc;
    vector<int> pcs;
    map<int,int> ccc;
    for(auto x : instructions){
        pcs.push_back(x.first);
        ccc.insert(make_pair(x.first,0));
    }
    for(int i = 0; i < pcs.size(); i++) {
        string opr = instructions.find(pcs[i])->second->op;
        vector<string> args = instructions.find(pcs[i])->second->params;
    
        int k = process(regs,m,opr,args,pcs[i]);
        if(k==0) {
            cout<<"At Instruction: "+instructions.find(pcs[i])->second->sen+"\n";
            return;
            }
        cout<<"-------------- Register Values after "+ instructions.find(pcs[i])->second->sen+" ---------------"<<endl;
        regs->printall();
        ccc.find(pcs[i])->second++;

        if(find(pcs.begin(),pcs.end(),k)!=pcs.end()) {
            int j = find(pcs.begin(),pcs.end(),k) - pcs.begin() ;
            if (i != j) i=j-1;
        }
        else{
            cout<<"pc out of bounds!"<<endl;
            return;
        }
        
    }
    cout<<"--------------------Relevant Statistics-----------------------"<<endl;
    int tc = 0;
    for(auto x : ccc) {
        cout<<"Instruction: "<<instructions.find(x.first)->second->sen<<"\n      count:\t"<<x.second<<endl;
        tc=tc+x.second;
    }
    cout<<"---------------------------------------------------------------\n";
    cout<<"No of clock cycles: "<< tc <<endl;


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
    if (argc!=2){       // unexpected no. of parameters
        cout << "Please run the program using this command format \"./a.exe filename\"\n";
    return 1;
    }

    if (file_notexist(argv[1])){   // checks if file doesn't exist, prints error message if true
        cout << "Please put the input file in the directory"<<endl;
    return 1;
    }
    string text;
    ifstream infile;
    infile.open(argv[1]);

    int count = 1;

    map<int,ins*> instructions;
    arrayOfRegister* regs = new arrayOfRegister();
    regs->set(31,1048576);
    Memory* m = new Memory();

    while (getline (infile, text)) {
        ins* in = new ins();
        in->sen = text;
        string word; 
        stringstream iss(text); 
        iss >> word;

        if(word==""){
            continue;
        }
        in->op = word;
        while (iss >> word){
            stringstream ss(word);
            string substr;
            while (getline(ss, substr, ',')) {
                in->params.push_back(substr);
            }
            
        }

        instructions.insert(make_pair(count,in));  
        m->set(count,count-1);  
        count = count+4;

        if(count>1048576){
            cout<<"Out of Memory!"<<endl;
            break;
        }

    }

    infile.close();
   /* for testing code 
    regs->set(2,43);
    regs->set(3,15);
    m->set(232,100);*/
    //cout<<15<<endl;
    execute(regs,m,instructions);

    return 0;
}

