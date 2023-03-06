import requests
import time
import os

sleep_time = 10 #间隔测速时间

# 设置Clash的API地址和密码
clash_api_url = 'http://127.0.0.1:49995/proxies' ##此处填写自己的端口
headers = {'Authorization': 'Bearer {secret}'} ##此处填写自己的secret,在配置文件中,如果配置文件没有的话直接点clash的clash Core,在网址里有,也有端口号


def now_node():  # 获取当前节点延迟
    now = ""
    delay = 9999
    response = requests.get(clash_api_url, headers=headers)
    proxies = response.json()['proxies']
    ##获取当前全局节点名称
    for proxy in proxies:
        delay_response = requests.get(
            clash_api_url + '/' + proxy, headers=headers)
        data = delay_response.json()['type']
        if data == 'Selector' and delay_response.json()['name'] == 'GLOBAL': 
            now = delay_response.json()['now']
            
    ##获取当前全局节点延迟
    delay_response = requests.get(
        clash_api_url + '/' + now + '/delay?timeout=1000' + '&url=https://www.gstatic.com/Fgenerate_204',##测速网站
        headers=headers)
    if delay_response.status_code == 200:
        delay_json_data = delay_response.json()
        delay = delay_json_data['delay']
    else:
        delay = 9999
    return delay


def delay_test():  # 节点延迟测试
    response = requests.get(clash_api_url, headers=headers)
    proxies = response.json()['proxies']
    min_delay = ""
    min = 9999

    for proxy in proxies:
        if proxy.startswith('节点名开头') and proxy.endswith('节点名结尾'):  #此处修改自己的节点名称,只能切换类型为Selector的节点,可在网站里看哪些是selector节点,或者直接在返回的url里查询
            print(proxy)
            delay_response = requests.get(
                clash_api_url + '/' + proxy + '/delay?timeout=1000' + '&url=https://www.gstatic.com/Fgenerate_204',
                headers=headers)
            if delay_response.status_code == 200:
                delay_json_data = delay_response.json()
                delay = delay_json_data['delay']
                print('节点延迟:',delay)
                if delay < min:
                    min = delay
                    min_delay = proxy
            else:
                print("timeout")

    name = {'name': min_delay}
    response = requests.put(clash_api_url + '/GLOBAL', headers=headers, json=name)
    print(response.status_code)
    return min_delay


if __name__ == '__main__':
    now_delay = 9999
    while True:
        now_delay = now_node()
        if now_delay > 1000:  # 当前节点延迟大于1000自动测速,切换节点
            print("节点不稳定,正在测速")
            current_node = delay_test()
            print("连接已切换,当前节点:" + current_node)
        else:
            print("连接正常")
        time.sleep(sleep_time)
