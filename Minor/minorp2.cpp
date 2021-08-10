#include <bits/stdc++.h>
using namespace std;

int INS_COUNT;
int ROW_ACCESS_DELAY = 10;
int COL_ACCESS_DELAY = 2;
int CC = 0;
int ROW_BUFFER_UPDATE = 0;

int PREV = -1;
int CURR;

class ins{
    public:
    string sen;
    string op;
    vector<string> params;
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
        throw "Invalid integer entry\n";
    }
    
    if ( *end == '\0' )
    {
        return (int) i;
    }
    else
    {
        cout << "Invalid integer entry\n";
        throw "Invalid integer entry\n";
        
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
                int i = stoi(&str[2]);
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
                
                int i = stoi(&str[2]);
                
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
                    int i = stoi(&str[1]);
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
                cout <<  i << "\t\t" << hex << read(i) << endl << dec;
            }
            // cout << "sp" << "\t\t" << hex << read(31) << endl << dec;
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

        void print_nonzero_mem(){
            for(int i = 4*INS_COUNT; i < 1048576 ; i = i + 4){
                if( read(i)!= 0) cout<<i<<"-"<<i+3<<": "<<read(i)<<endl;
            }
        }
};

map<int,ins*> instructions;
arrayOfRegister* regs = new arrayOfRegister();
Memory* m = new Memory();

bool add(arrayOfRegister* reg, int x1, int x2, int x3, int cc){
    bool err = false;
    if(x1>0 && x1<32 && x2>=0 && x2<32 && x3>=0 && x3<32) {
        int x = reg->read(x2) + reg->read(x3);
        reg->set(x1, x);
        cout<< "cycle " << cc << ":\tadd:\t$" << x1 << " = " << x <<endl;
    }
    else {
        cout<<"Incorrect register values in add instruction"<<endl;
        return true;
    }
    return err;
}

bool addi(arrayOfRegister* reg, int x1, int x2, int x3, int cc){
    
    bool err = false;
    if(x1>0 && x1<32 && x2>=0 && x2<32) {
        int x = reg->read(x2) + x3;
        reg->set(x1, x);
        cout<< "cycle " << cc << ":\taddi:\t$" << x1 << " = " << x << endl;
        return false;
    }
    else {
        cout<<"Incorrect register values in addi instruction"<<endl;
        return true;
    }

}

int parallel_pro(int cycle, int pc, int pre, int a1, int a2, int a3){

    //cout<<"pc:"<<pc<<endl;
    //cout<<"cycle:"<<cycle<<endl;
    int r = pre-1;
    int j = 1;
    int k = pc + 4;

    string org_op = instructions.find(k-4)->second->op;
    //vector<string> org_args = instructions.find(k-4)->second->params;

    while(j <= cycle && k < 4*INS_COUNT){
        string op = instructions.find(k)->second->op;
        vector<string> args = instructions.find(k)->second->params;
        if(op=="lw" || op=="sw") break;
        else{
            if(op == "add"){
                
                string x1,x2,x3;
                if(args.size()!= 3){
                    cout<<"Incorrect arguments in add instruction"<<endl;
                    cout<<"Instruction: "+instructions.find(k)->second->sen+"\n";
                    return -1;
                }
                else{
                    auto it = args.begin();
                    x1 = *it; it++;
                    x2 = *it; it++;
                    x3 = *it;
                    int a = modifyReg(x1);
                    int b = modifyReg(x2);
                    int c = modifyReg(x3);

                    if(org_op == "lw"){
                        if(a==a1 || b==a1 || c==a1) break;
                    }
                    if(org_op == "sw"){
                        if(a==a1) break;
                    }

                    if(add(regs,a,b,c,r+j)) return -1;
                }
            }
            else if(op=="addi"){
        
                string x1,x2,x3;

                if(args.size()!= 3){    
                    cout<<"Incorrect arguments in addi instruction"<<endl;
                    cout<<"Instruction: "+instructions.find(k)->second->sen+"\n";
                    return -1;
                }
       
                else{
                    auto it = args.begin();
                    x1 = *it; it++;
                    x2 = *it; it++;
                    x3 = *it;


                    int a = modifyReg(x1);
                    int b = modifyReg(x2);
                    int c = stoi(x3);
                    
                    if(org_op == "lw"){
                        if(a==a1 || b==a1 || c==a1) break;
                    }
                    if(org_op == "sw"){
                        if(a==a1) break;
                    }

                    if(addi(regs,a,b,c,r+j)==1) return -1;
                }
            }
            else{
                cout<<"Wrong instruction: "<<op<<endl;
                cout<<"Instruction: "+instructions.find(k)->second->sen+"\n";
                return -1;
            }
        }
        k+=4;
        j++;

    }

    return k;
}

int lw(arrayOfRegister* reg,Memory* m, int x1, int x2, int x3, int pc){
    
    if(x1>0 && x1<32 && x3>=0 && x3<32) {
        int loc = (reg->read(x3)) + x2;
        if ((loc % 4) != 0){
            cout<<"Incorrect address: address not multiple of 4!"<<endl;
            cout<<"Instruction: "+instructions.find(pc)->second->sen+"\n";
            return -1;
        }
        else if( loc <= 4*INS_COUNT-4 ){
            cout<<"Incorrect address: cannot access Instruction Memory address"<<endl;
            cout<<"Instruction: "+instructions.find(pc)->second->sen+"\n";
            return -1;
        }
        else{
            CC++;
            cout<< "cycle " << CC << ":\tlw:\t" << "DRAM request issued" << endl;
            int pre = CC + 1;
            string yy;
            int yyk = 0;
            CURR = loc/1024;

            if( PREV==-1 ){ 
                CC = CC+ ROW_ACCESS_DELAY + COL_ACCESS_DELAY ;
                yy = "Row Buffer is initialized";
                ROW_BUFFER_UPDATE++;
                yyk = parallel_pro(CC - pre + 1,pc,pre,x1,x2,x3);
            }
            else{
                if(PREV == CURR){ 
                    CC = CC + COL_ACCESS_DELAY;
                    yy = "No change in Row Buffer";
                    yyk = parallel_pro(CC - pre + 1,pc,pre,x1,x2,x3);
                }
                else {
                    CC = CC+ 2*ROW_ACCESS_DELAY + COL_ACCESS_DELAY;
                    yy = "Row Buffer is updated";
                    ROW_BUFFER_UPDATE++;
                    yyk = parallel_pro(CC - pre + 1,pc,pre,x1,x2,x3);
                }
            }
            PREV=CURR;
            reg->set(x1, m->read(loc));
            //cout<<yyk<<endl;
            if(yyk != -1) cout << "cycle " << pre << "-" << CC << ":\tlw:\t" << "$" << x1 << " = " <<  m->read(loc) <<"; " << yy << endl;
            
            return yyk;
        }
    }
    else {
        cout<<"Incorrect register values in lw instruction"<<endl;
        cout<<"Instruction: "+instructions.find(pc)->second->sen+"\n";
        return -1;
    }
    return 0;
}

int sw(arrayOfRegister* reg,Memory* m, int x1, int x2, int x3, int pc){
    bool err = false;
    if(x1>0 && x1<31 && x3>=0 && x3<32) {
        int loc = (reg->read(x1));
        x2 = x2+reg->read(x3);
        if ((x2 % 4) != 0){
            cout<<"Incorrect address: address not multiple of 4!"<<endl;
            cout<<"Instruction: "+instructions.find(pc)->second->sen+"\n";
            return -1;
        }
        else if( x2 <= 4*INS_COUNT-4 ){
            cout<<"Incorrect address: cannot access Instruction Memory address"<<endl;
            cout<<"Instruction: "+instructions.find(pc)->second->sen+"\n";
            return -1;
        }
        else{
            CC++;
            cout<< "cycle " << CC << ":\tsw:\t" << "DRAM request issued" << endl;
            int pre = CC+1;
            string yy;
            int yyk=0;
            CURR = x2/1024;

            if( PREV == -1 ){ 
                CC = CC+ ROW_ACCESS_DELAY + COL_ACCESS_DELAY ;
                yy = "Row Buffer is initialized";
                ROW_BUFFER_UPDATE++;
                ROW_BUFFER_UPDATE++;
                yyk = parallel_pro(CC - pre + 1,pc,pre,x1,x2,x3);
            }
            else{
                if(PREV == CURR){ 
                    CC = CC + COL_ACCESS_DELAY;
                    yy = "No change in Row Buffer";
                    ROW_BUFFER_UPDATE++;
                    yyk = parallel_pro(CC - pre + 1,pc,pre,x1,x2,x3);
                }
                else {
                    CC = CC+ 2*ROW_ACCESS_DELAY + COL_ACCESS_DELAY;
                    yy = "Row Buffer is updated";
                    ROW_BUFFER_UPDATE++;
                    ROW_BUFFER_UPDATE++;
                    yyk = parallel_pro(CC - pre + 1,pc,pre,x1,x2,x3);
                }
            }
            PREV=CURR;
            m->set(loc, x2);
            //cout<<yyk<<endl;
            if(yyk!= -1) cout<< "cycle " << pre << "-" << CC << ":\tsw:\t" << "Memory address " << x2 << "-" << x2+3 << " = " << loc << "; " << yy << endl;
            return yyk;
        }
    }
    else {
        cout<<"Incorrect register values in sw instruction"<<endl;
        cout<<"Instruction: "+instructions.find(pc)->second->sen+"\n";
        return -1;
    }
    return 0;
}


int process(string opr,vector<string> args, int pc){

    int err = pc;

    if(opr == "add"){

        CC++;

        string x1,x2,x3;

        if(args.size()!= 3){
            cout<<"Incorrect arguments in add instruction"<<endl;
            cout<<"Instruction: "+instructions.find(pc)->second->sen+"\n";
            return -1;
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;


            int a = modifyReg(x1);
            int b = modifyReg(x2);
            int c = modifyReg(x3);
            
            if(add(regs,a,b,c,CC)) return -1;
        }
        
    }
    
    else if(opr=="addi"){

        CC++;
        //cout << "in addi" << endl;
        string x1,x2,x3;

        if(args.size()!= 3){
            cout<<"Incorrect arguments in addi instruction"<<endl;
            cout<<"Instruction: "+instructions.find(pc)->second->sen+"\n";
            return -1;
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;


            int a = modifyReg(x1);
            int b = modifyReg(x2);
            int c = stoi(x3);
            //cout<<addi(regs,a,b,c)<<endl;
            if(addi(regs,a,b,c,CC)==1) return -1;
        }
    }
    
    else if(opr=="lw"){
        string x1,x2,x3;

        if(args.size()!= 3){
            cout<<"Incorrect arguments in lw instruction"<<endl;
            cout<<"Instruction: "+instructions.find(pc)->second->sen+"\n";
            return -1;
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;

            string x4;
            if (x3.substr(0,1) != "(" || x3.substr(x3.size()-1) != ")"){
                cout << "Invalid memory location\n";
                throw "invalid memory location\n";
            }
            else{
                x4 = trim(x3.substr(1,x3.size()-2));
            }

            int a = modifyReg(x1);
            int b = stoi(x2);
            int c = modifyReg(x4);
            
            int res = lw(regs,m,a,b,c,pc);
            return res;

            
        }
    }
    else if(opr=="sw"){
        string x1,x2,x3;

        if(args.size()!= 3 ){
            cout<<"Incorrect arguments in sw instruction"<<endl;
            cout<<"Instruction: "+instructions.find(pc)->second->sen+"\n";
            return -1;
        }
       
        else{
            auto it = args.begin();
            x1 = *it; it++;
            x2 = *it; it++;
            x3 = *it;

            string x4;
            if (x3.substr(0,1) != "(" || x3.substr(x3.size()-1) != ")"){
                cout << "Invalid memory location\n";
                throw "invalid memory location\n";
            }
            else{
                x4 = trim(x3.substr(1,x3.size()-2));
            }

            int a = modifyReg(x1);
            int b = stoi(x2);
            int c = modifyReg(x4);
            
            int res = sw(regs,m,a,b,c,pc);
            return res;


            }


            
    }
    
    else{
        cout<<"Wrong instruction: "<<opr<<endl;
        cout<<"Instruction: "+instructions.find(pc)->second->sen+"\n";
        return -1;
    }

    return err;

}

void execute(){

    cout<<"Execution Details:"<<endl;

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
    
        int k = process(opr,args,pcs[i]);

        if(k==-1) {
            //cout<<"At Instruction: "+instructions.find(pcs[i])->second->sen+"\n";
            return;
        }

        //cout<<"Register Values after Instruction: "+ instructions.find(pcs[i])->second->sen <<endl;
        //regs->printall();
        
        ccc.find(pcs[i])->second++;

        if(find(pcs.begin(),pcs.end(),k)!=pcs.end()) {
            int j = find(pcs.begin(),pcs.end(),k) - pcs.begin() ;
            if (i != j) i=j-1;
        }
        else{
            //cout<<"pc out of bounds!"<<endl;
            break;
        }
        
    }
    cout<<"---------------------------------------------------------------\n";
    cout<<"Final Memory content after execution:\n";
    m->print_nonzero_mem();
    cout<<"---------------------------------------------------------------\n";
    cout<<"Final Register File content after execution:\n";
    regs->printall();
    cout<<"---------------------------------------------------------------\n";
    cout<<"Relevant Statistics:"<<endl;
    cout<<"No of clock cycles: "<< CC<<endl;
    cout<<"No of times Row Buffer is updated: " << ROW_BUFFER_UPDATE <<endl;
    /*cout<<"--------------------------\n";
    int tc = 0;
    for(auto x : ccc) {
        cout<<"Instruction: "<<instructions.find(x.first)->second->sen<<"\n      count:\t"<<x.second<<endl;
        tc=tc+x.second;
    }
    */
    


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
        cout << "Please run the program using this command format \"./part2 filename\"\n";
    return 1;
    }

    if (file_notexist(argv[1])){   // checks if file doesn't exist, prints error message if true
        cout << "Please put the input file in the directory"<<endl;
    return 1;
    }
    string text;
    ifstream infile;
    infile.open(argv[1]);

    int count = 0;
    regs->set(29,1048576);
    ROW_ACCESS_DELAY = stoi(argv[2]);
    COL_ACCESS_DELAY = stoi(argv[3]);
    

    while (getline (infile, text)) {

        ins* in = new ins();
        in->sen = text;
        text = trim(text);
        string word; 
        stringstream iss(text); 
        iss >> word;

        if(word==""){
            continue;
        }

        // cout << &word[word.size()-1] << endl;
        word = trim(word);
        if (word.substr(word.size()-1)==":"){
            iss >> word;

            if(word==text){
                continue;
            }
        }
        in->op = word;
        string w = iss.str();
        string substr;
        while (getline(iss, substr, ',')){
            substr = trim(substr);
            stringstream ss(substr);
            if ((substr.find("(")) != string::npos){
                string temp;
                getline(ss, temp, '(');
                int l = temp.size();
                temp = trim(temp);
                if (temp==""){
                    in->params.push_back("0");
                }
                else{
                    in->params.push_back(temp);
                    string temp1 = ss.str().substr(l);
                    if (temp1.substr(temp1.size()-1) != ")"){
                        cout << "non-closed bracket\n";
                        throw "non-closed bracket\n";
                    }
                    else{
                        in->params.push_back(temp1);
                    }
                }
                
            }
            else{
                in->params.push_back(substr);
            }
            
            
            
        }

        instructions.insert(make_pair(count,in));  
        m->set(count,count-1);  
        count = count+4;

        if(count>=1048576){
            cout<<"Out of Memory!"<<endl;
            break;
        }

    }

    INS_COUNT = count/4;

    infile.close();

    execute();

    return 0;
}

