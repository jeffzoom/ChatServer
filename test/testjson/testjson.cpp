#include "json.hpp"
using json = nlohmann::json;

#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

void test1() {

    // {"id":[1,2,3,4,5],"msg":{"liu shuo":"hello china",
    // "zhang san":"hello world"},"name":"zhang san"}
    json js;
    // 添加数组
    js["id"] = {1,2,3,4,5};
    // 添加key-value
    js["name"] = "zhang san";
    // 添加对象
    js["msg"]["zhang san"] = "hello world";
    js["msg"]["liu shuo"] = "hello china";
    // 上面等同于下面这句一次性添加数组对象
    js["msg"] = {{"zhang san", "hello world"}, {"liu shuo", "hello china"}};

    string sendBuf = js.dump();

    cout << sendBuf << endl;
    cout << sendBuf.c_str() << endl;
    // const char *c_str();
    // 这是为了与c语言兼容，在c语言中没有string类型，
    // 故必须通过string类对象的成员函数c_str()把string 对象转换成c中的字符串样式
    // 这样就可以发送了
    cout << js << endl;

}

string test2() {
    // 一个函数只能创建一个"json js2;"，不然cout << js2 << endl;会无法输出，
    // 但是cout << sendBuf2.c_str() << endl; 可以输出
    json js2;
    // 直接序列化一个vector容器
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(5);
    js2["list"] = vec;
    // 直接序列化一个map容器
    map<int, string> m;
    m.insert({1, "黄山"});
    m.insert({2, "华山"});
    m.insert({3, "泰山"});
    js2["path"] = m;
    cout << js2 << endl;

    string sendBuf2 = js2.dump();
    cout << sendBuf2.c_str() << endl;
    return js2.dump();
}

string test3() {
    json js;
    // {"id":[1,2,3,4,5],"msg":{"liu shuo":"hello china",
    // "zhang san":"hello world"},"name":"zhang san"}
    js["id"] = {1,2,3,4,5};
    
    js["name"] = "zhang san";
    
    js["msg"]["zhang san"] = "hello world";
    js["msg"]["liu shuo"] = "hello china";
    // 上面等同于下面这句一次性添加数组对象
    js["msg"] = {{"zhang san", "hello world"}, {"liu shuo", "hello china"}};

    string sendBuf = js.dump();
    return sendBuf;

}

int main() {
    // test1();
    // test2();

    cout << "----------------反序列化数据----------------" << endl;
    string recvBuf = test3();
    // 模拟从网络接收到json字符串，通过json::parse函数把json字符串转成json对象
    json jsbuf = json::parse(recvBuf);
    // 直接取key-value
    cout << "id:" << jsbuf["id"] << endl;
    cout << "name:" << jsbuf["name"] << endl;
    cout << "msg:" << jsbuf["msg"] << endl;

    cout << "----------------反序列化vector容器----------------" << endl;
    string recvBuf2 = test2();
    json jsbuf2 = json::parse(recvBuf2);
    // 直接反序列化vector容器
    vector<int> v = jsbuf2["list"];
    for(int val : v) {
        cout << val << " ";
    } 
    cout << endl;
    // 直接反序列化map容器
    map<int, string> m2 = jsbuf2["path"];
    for(auto p : m2) {
        cout << p.first << " " << p.second << endl;
    } 
    cout << endl;

    return 0;
}