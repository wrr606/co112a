#include<iostream>
#include<unordered_map>
#include<algorithm>
#include<fstream>
#include<bitset>
#include<vector>

using namespace std;

#define all(x) x.begin(),x.end()

//創建 A 指令 hash table
unordered_map<string,string> A_instructions={
    {"SCREEN",bitset<15>(16384).to_string()},{"KBD",bitset<15>(24576).to_string()}
};

//創建 C 指令 hash table
unordered_map<string,string> C_comp={
    {"0",   "0101010"}, {"1",   "0111111"}, {"-1",  "0111010"},
    {"D",   "0001100"}, {"A",   "0110000"}, {"!D",  "0001101"},
    {"!A",  "0110001"}, {"-D",  "0001111"}, {"-A",  "0110011"},
    {"D+1", "0011111"}, {"A+1", "0110111"}, {"D-1", "0001110"},
    {"A-1", "0110010"}, {"D+A", "0000010"}, {"D-A", "0010011"},
    {"A-D", "0000111"}, {"D&A", "0000000"}, {"D|A", "0010101"},
    {"M",   "1110000"}, {"!M",  "1110001"}, {"-M",  "1110011"},
    {"M+1", "1110111"}, {"M-1", "1110010"}, {"D+M", "1000010"},
    {"D-M", "1010011"}, {"M-D", "1000111"}, {"D&M", "1000000"},
    {"M+D", "1000010"}, {"D|M", "1010101"}
};
unordered_map<string,string> C_dest {
    {"", "000"}, {"M", "001"}, {"D", "010"}, {"MD", "011"},
    {"A","100"}, {"AM","101"}, {"AD","110"}, {"AMD","111"}
};
unordered_map<string,string> C_jump {
    {"",   "000"}, {"JGT","001"}, {"JEQ","010"}, {"JGE","011"},
    {"JLT","100"}, {"JNE","101"}, {"JLE","110"}, {"JMP","111"}
};

//A 指令處理
int top=16;//未存在變數存放位置
string A_ins_assembler(const string &line){
    //@後的名字
    string name=line.substr(1);

    //確認 name 是變數名字還是數字
    bool is_number=1;
    for(const auto &i:name){
        if(!('0'<=i&&i<='9')){
            is_number=0;
            break;
        }
    }
    if(is_number)
        return "0"+bitset<15>(stoi(name)).to_string();

    //使用 hash table 找尋是否存在
    if(A_instructions.count(name)){//存在
        return "0"+A_instructions[name];
    }
    else{//不存在
        A_instructions[name]=bitset<15>(top++).to_string();
        return "0"+A_instructions[name];
    }
}

string C_ins_assembler(string line){
    //找 = 和 ;
    auto equal=line.find('='),semicolon=line.find(';');

    string comp="",dest="",jump="";
    if(semicolon!=line.npos){
        jump=line.substr(semicolon+1);
        line.erase(semicolon);
    }
    if(equal!=line.npos){
        comp=line.substr(equal+1);
        line.erase(equal);
        dest=line;
    }
    else{
        comp=line;
    }
    return "111"+C_comp[comp]+C_dest[dest]+C_jump[jump];
}

//處理 lable symbol
void lable_symbol(const string &name,const int &idx){
    //使用 hash table 找尋是否存在
    if(!A_instructions.count(name)){//不存在
        A_instructions[name]=bitset<15>(idx).to_string();
    }
}

int main(){
    //將 A 指令剩下的設定完
    for(int i=0;i<=15;i++){
        A_instructions["R"+to_string(i)]=bitset<15>(i).to_string();
    }

    //輸入檔名
    cout<<"Please enter the filename to assemble:\n";
    string filename;
    getline(cin,filename);
    ifstream input(filename);

    //找不到檔案報錯
    if(!input.is_open()){
        cout<<"File not found.";
        return 1;
    }

    // 逐行讀取檔案內容
    string line;
    filename.erase(filename.rfind('.'));

    //將程式稍微處理後裝到 vector 中
    vector<string> lines;
    int cnt=0;
    while(getline(input,line)){
        //偵測註解
        auto annotation_check=line.find("//");
        if(annotation_check!=line.npos){
            line.erase(annotation_check);
        }

        //刪除所有空格
        line.erase(remove(all(line),' '),line.end());

        //將程式裝到 vector 中
        if(line[0]=='@'||line.find('=')!=line.npos||line.find(';')!=line.npos)
            lines.push_back(line);

        //處理 lable symbol 部分
        if(line[0]=='('&&line.back()==')'){
            lable_symbol(line.substr(1,line.size()-2),lines.size());
        }
    }
    input.close();

    //組譯主迴圈
    ofstream output(filename+".hack");
    for(auto &line:lines){
        //判斷是 A 還是 C 指令
        if(line[0]=='@')
            line=A_ins_assembler(line);
        else if(line.find('=')!=line.npos||line.find(';')!=line.npos)
            line=C_ins_assembler(line);
        else
            continue;

        //將組譯後的結果寫入 hack 檔中
        output<<line<<"\n";
    }
    output.close();

    return 0;
}