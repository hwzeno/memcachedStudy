#include <event2/event.h>
#include <signal.h>
#include <iostream>

using namespace std;

int main(){
    if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        return 1;
    //配置上下文
    event_config *conf = event_config_new();

    //显示支持的网络模式
    const char **methods = event_get_supported_methods();
    for(int i = 0; methods[i] != NULL; i++){
        cout << methods[i] << endl;
    }

    //设置特征
    //event_config_require_features(conf, EV_FEATURE_ET|EV_FEATURE_FDS);设置EV_FEATURE_FDS后其它设置无效
    //event_config_require_features(conf, EV_FEATURE_FDS);//这个设置epoll不支持

    //设置网络模型为select
    event_config_avoid_method(conf, "epoll");
    event_config_avoid_method(conf, "poll");

    //初始化配置libevent上下文
    event_base *base = event_base_new_with_config(conf);
    event_config_free(conf);

    if(!base){
        cerr << "event_base_new_with_config failed!" << endl;
        base = event_base_new();
        if(!base)
            cerr << "event_base_new failed " << endl;
        return 0;
    }else{

        //获取当前网络模型
        cout << "network method: " << event_base_get_method(base) << endl;//默认epoll

       //确认特征是否生效
        int f = event_base_get_features(base);
        if(f&EV_FEATURE_ET)
            cout << "EV_FEATURE_ET events are supported" << endl;
        else
            cout << "EV_FEATURE_ET events are not supported" << endl;

        if(f&EV_FEATURE_O1)
            cout << "EV_FEATURE_O1 events are supported" << endl;
        else
            cout << "EV_FEATURE_O1 events are not supported" << endl;

        if(f&EV_FEATURE_FDS)
            cout << "EV_FEATURE_FDS events are supported" << endl;
        else
            cout << "EV_FEATURE_FDS events are not supported" << endl;

        if(f&EV_FEATURE_EARLY_CLOSE)
            cout << "EV_FEATURE_EARLY_CLOSE events are supported" << endl;
        else
            cout << "EV_FEATURE_EARLY_CLOSE events are not supported" << endl;
    }


    return 0;
}


